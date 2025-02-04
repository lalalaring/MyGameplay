#include "base/Base.h"
#include "AudioListener.h"
#include "Scene.h"
#include "MeshSkin.h"
#include "BoneJoint.h"
#include "objects/Terrain.h"
#include "../base/SerializerJson.h"

#define SCENE_NAME ""
#define SCENE_STREAMING false

namespace gameplay
{

// Global list of active scenes
static std::vector<Scene*> __sceneList;

static inline char lowercase(char c)
{
    if (c >= 'A' && c <='Z')
    {
        c |= 0x20;
    }
    return c;
}

// Returns true if 'str' ends with 'suffix'; false otherwise.
static bool endsWith(const char* str, const char* suffix, bool ignoreCase)
{
    if (str == NULL || suffix == NULL)
        return false;
    size_t length = strlen(str);
    size_t suffixLength = strlen(suffix);

    if (suffixLength > length)
    {
        return false;
    }

    size_t offset = length - suffixLength;

    const char* p = str + offset;
    while (*p != '\0' && *suffix != '\0')
    {
        if (ignoreCase)
        {
            if (lowercase(*p) != lowercase(*suffix))
            {
                return false;
            }
        }
        else if (*p != *suffix)
        {
            return false;
        }
        
        ++p;
        ++suffix;
    }
    return true;
}


Scene::Scene()
    : _id(""), _activeCamera(NULL), _rootNode(NULL), _bindAudioListenerToCamera(true),
      _nextItr(NULL), _nextIndex(-1), _nextReset(true), _streaming(false)
{
    _rootNode = Node::create("root");
    __sceneList.push_back(this);
}

Scene::~Scene()
{
    // Unbind our active camera from the audio listener
    if (_activeCamera)
    {
        AudioListener* audioListener = AudioListener::getInstance();
        if (audioListener && (audioListener->getCamera() == _activeCamera))
        {
            audioListener->setCamera(NULL);
        }

        SAFE_RELEASE(_activeCamera);
    }

    // Remove all nodes from the scene
    SAFE_RELEASE(_rootNode);

    // Remove the scene from global list
    std::vector<Scene*>::iterator itr = std::find(__sceneList.begin(), __sceneList.end(), this);
    if (itr != __sceneList.end())
        __sceneList.erase(itr);
}

Scene* Scene::create(const char* id)
{
    Scene* scene = new Scene();
    scene->setId(id);
    return scene;
}

Scene* Scene::load(const char* filePath)
{
    auto rs = SerializerJson::createReader(filePath);
    Scene* scene = dynamic_cast<Scene*>(rs->readObject(NULL));
    rs->close();
    delete rs;
    return scene;
}

Scene* Scene::getScene(const char* id)
{
    if (id == NULL)
        return __sceneList.size() ? __sceneList[0] : NULL;

    for (size_t i = 0, count = __sceneList.size(); i < count; ++i)
    {
        if (__sceneList[i]->_id == id)
            return __sceneList[i];
    }

    return NULL;
}


const char* Scene::getId() const
{
    return _id.c_str();
}

void Scene::setId(const char* id)
{
    _id = id ? id : "";
}

Node* Scene::findNode(const char* id, bool recursive, bool exactMatch) const
{
    GP_ASSERT(id);

    // Search immediate children first.
    for (size_t i=0; i<_rootNode->getChildCount(); ++i) {
        Node *child = _rootNode->getChild(i);
        // Does this child's ID match?
        if ((exactMatch && child->_name == id) || (!exactMatch && child->_name.find(id) == 0))
        {
            return child;
        }
    }

    // Recurse.
    if (recursive)
    {
        for (size_t i=0; i<_rootNode->getChildCount(); ++i) {
            Node *child = _rootNode->getChild(i);
            Node* match = child->findNode(id, true, exactMatch);
            if (match)
            {
                return match;
            }
        }
    }
    return NULL;
}

unsigned int Scene::findNodes(const char* id, std::vector<Node*>& nodes, bool recursive, bool exactMatch) const
{
    GP_ASSERT(id);
    return _rootNode->findNodes(id, nodes, true, exactMatch);
}

void Scene::visitNode(Node* node, const char* visitMethod)
{
//#ifdef GP_SCRIPT
//    ScriptController* sc = ScriptController::cur();
//
//    // Invoke the visit method for this node.
//    bool result;
//    if (!sc->executeFunction<bool>(visitMethod, "<Node>", &result, (void*)node) || !result)
//        return;
//#endif

    // If this node has a model with a mesh skin, visit the joint hierarchy within it
    // since we don't add joint hierarcies directly to the scene. If joints are never
    // visited, it's possible that nodes embedded within the joint hierarchy that contain
    // models will never get visited (and therefore never get drawn).
    Model* model = dynamic_cast<Model*>(node->getDrawable());
    if (model && model->_skin && model->_skin->_rootNode)
    {
        visitNode(model->_skin->_rootNode, visitMethod);
    }

    // Recurse for all children.
    for (size_t i=0; i<node->getChildCount(); ++i) {
        Node *child = node->getChild(i);
        visitNode(child, visitMethod);
    }
}

Node* Scene::addNode(const char* id)
{
    Node* node = Node::create(id);
    GP_ASSERT(node);
    addNode(node);

    // Call release to decrement the ref count to 1 before returning.
    node->release();

    return node;
}

void Scene::addNode(Node* node)
{
    GP_ASSERT(node);

    if (node->_scene == this)
    {
        // The node is already a member of this scene.
        return;
    }

    _rootNode->addChild(node);
    node->_scene = this;

    // If we don't have an active camera set, then check for one and set it.
    if (_activeCamera == NULL)
    {
        Camera* camera = node->getCamera();
        if (camera)
        {
            setActiveCamera(camera);
        }
    }
}

Camera* Scene::getActiveCamera()
{
    return _activeCamera;
}

void Scene::setActiveCamera(Camera* camera)
{
    // Make sure we don't release the camera if the same camera is set twice.
    if (_activeCamera != camera)
    {
        AudioListener* audioListener = AudioListener::getInstance();

        if (_activeCamera)
        {
            // Unbind the active camera from the audio listener
            if (audioListener && (audioListener->getCamera() == _activeCamera))
            {
                audioListener->setCamera(NULL);
            }

            SAFE_RELEASE(_activeCamera);
        }

        _activeCamera = camera;

        if (_activeCamera)
        {
            _activeCamera->addRef();

            if (audioListener && _bindAudioListenerToCamera)
            {
                audioListener->setCamera(_activeCamera);
            }
        }
    }
}

void Scene::bindAudioListenerToCamera(bool bind)
{
    if (_bindAudioListenerToCamera != bind)
    {
        _bindAudioListenerToCamera = bind;

        if (AudioListener::getInstance())
        {
            AudioListener::getInstance()->setCamera(bind ? _activeCamera : NULL);
        }
    }
}

const Vector3& Scene::getAmbientColor() const
{
    return _ambientColor;
}

void Scene::setAmbientColor(float red, float green, float blue)
{
    _ambientColor.set(red, green, blue);
}

void Scene::update(float elapsedTime)
{
    _rootNode->update(elapsedTime);
}

bool Scene::isNodeVisible(Node* node)
{
    if (!node->isEnabled())
        return false;

    if (node->getDrawable() || node->getLight() || node->getCamera())
    {
        return true;
    }
    else
    {
        return node->getBoundingSphere().intersects(_activeCamera->getFrustum());
    }
}

Serializable* Scene::createObject()
{
    return new Scene();
}

std::string Scene::getClassName()
{
    return "gameplay::Scene";
}

void Scene::onSerialize(Serializer* serializer)
{
    serializer->writeString("name", _name.c_str(), SCENE_NAME);
    serializer->writeBool("streaming", _streaming, SCENE_STREAMING);
    serializer->writeObject("root", _rootNode);
    if (this->_activeCamera && getActiveCamera()->getNode()) {
        serializer->writeString("activeCamera", getActiveCamera()->getNode()->getName(), "");
    }
    else {
        serializer->writeString("activeCamera", "", "");
    }
}

void Scene::onDeserialize(Serializer* serializer)
{
    serializer->readString("name", _name, SCENE_NAME);
    _streaming = serializer->readBool("streaming", SCENE_STREAMING);
    Node *node = (Node*)serializer->readObject("root");
    _rootNode = node;
    _rootNode->_scene = this;

    std::string activeCamera;
    serializer->readString("activeCamera", activeCamera, "");
    if (activeCamera.size() > 0) {
        Node* camera = this->findNode(activeCamera.c_str());
        if (camera && camera->getCamera())
            this->setActiveCamera(camera->getCamera());
    }
    //node->addRef();
}

}
