#include "base/Base.h"
#include "scene/Node.h"
#include "audio/AudioSource.h"
#include "Scene.h"
#include "Joint.h"
#include "physics/PhysicsRigidBody.h"
#include "physics/PhysicsVehicle.h"
#include "physics/PhysicsVehicleWheel.h"
#include "physics/PhysicsGhostObject.h"
#include "physics/PhysicsCharacter.h"
#include "Terrain.h"
#include "platform/Game.h"
#include "Drawable.h"
#include "ui/Form.h"
#include "base/Ref.h"

#define SCENEOBJECT_NAME ""
#define SCENEOBJECT_STATIC true
#define SCENEOBJECT_ENABLED true
#define SCENEOBJECT_POSITION Vector3::zero()
#define SCENEOBJECT_EULER_ANGLES Vector3::zero()
#define SCENEOBJECT_SCALE Vector3::one()

// Node dirty flags
#define NODE_DIRTY_WORLD 1
#define NODE_DIRTY_BOUNDS 2
#define NODE_DIRTY_HIERARCHY 4
#define NODE_DIRTY_ALL (NODE_DIRTY_WORLD | NODE_DIRTY_BOUNDS | NODE_DIRTY_HIERARCHY)

namespace gameplay
{

Node::Node(const char* id)
    : _scene(NULL), _parent(NULL), _enabled(true), _tags(NULL),
    _userObject(NULL),
      _dirtyBits(NODE_DIRTY_ALL), _static(false)
{
    GP_REGISTER_SCRIPT_EVENTS();
    if (id)
    {
        _name = id;
    }
}

Node::~Node()
{
    if (getDrawable())
        getDrawable()->setNode(NULL);
    //if (getAudioSource())
    //    getAudioSource()->setNode(NULL);

    SAFE_RELEASE(_userObject);
    SAFE_DELETE(_tags);
    //setAgent(NULL);

    for (Component *com : _components) {
        com->setNode(NULL);
        Ref *ref = dynamic_cast<Ref*>(com);
        if (ref) {
            SAFE_RELEASE(ref);
        } else {
            SAFE_DELETE(com);
        }
    }
    _components.clear();


    for (size_t i=0; i<_children.size(); ++i) {
        Node *n = _children[i];
        n->release();
    }
    _children.clear();
}

Node* Node::create(const char* id)
{
    return new Node(id);
}

const char* Node::getTypeName() const
{
    return "Node";
}

const char* Node::getName() const
{
    return _name.c_str();
}

void Node::setName(const char* id)
{
    if (id)
    {
        _name = id;
    }
}

Node::Type Node::getType() const
{
    return Node::NODE;
}

void Node::addChild(Node* child)
{
    GP_ASSERT(child);

    if (child->_parent == this)
    {
        // This node is already present in our hierarchy
        return;
    }
    child->addRef();

    // If the item belongs to another hierarchy, remove it first.
    if (child->_parent)
    {
        child->_parent->removeChild(child);
    }

    // Add child to the end of the list.
    child->_parent = this;
    _children.push_back(child);

    setBoundsDirty();

    if (_dirtyBits & NODE_DIRTY_HIERARCHY)
    {
        hierarchyChanged();
    }
}

void Node::removeChild(Node* child)
{
    if (child == NULL || child->_parent != this)
    {
        // The child is not in our hierarchy.
        return;
    }
    // Call remove on the child.
    for (size_t i=0; i<_children.size(); ++i) {
        if (_children[i] == child) {
            _children.erase(_children.begin()+i);
        }
    }
    SAFE_RELEASE(child);
}

void Node::removeAllChildren()
{
    _dirtyBits &= ~NODE_DIRTY_HIERARCHY;
    for (size_t i=0; i<_children.size(); ++i) {
        Node *n = _children[i];
        n->release();
    }
    _children.clear();
    _dirtyBits |= NODE_DIRTY_HIERARCHY;
    hierarchyChanged();
}

void Node::remove()
{
    // Update our parent.
    Node* parent = _parent;
    if (parent)
    {
        parent->removeChild(this);
    }
    _parent = NULL;

    if (parent && parent->_dirtyBits & NODE_DIRTY_HIERARCHY)
    {
        parent->hierarchyChanged();
    }
}

Node* Node::getChild(int i) const
{
    return _children[i];
}

Node* Node::getParent() const
{
    return _parent;
}

unsigned int Node::getChildCount() const
{
    return _children.size();
}

Node* Node::getRootNode() const
{
    Node* n = const_cast<Node*>(this);
    while (n->getParent())
    {
        n = n->getParent();
    }
    return n;
}

Node* Node::findNode(const char* id, bool recursive, bool exactMatch) const
{
    return findNode(id, recursive, exactMatch, false);
}

Node* Node::findNode(const char* id, bool recursive, bool exactMatch, bool skipSkin) const
{
    GP_ASSERT(id);

    // If not skipSkin hierarchy, try searching the skin hierarchy
    if (!skipSkin)
    {
        // If the drawable is a model with a mesh skin, search the skin's hierarchy as well.
        Node* rootNode = NULL;
        Model* model = dynamic_cast<Model*>(getDrawable());
        if (model)
        {
            if (model->getSkin() != NULL && (rootNode = model->getSkin()->_rootNode) != NULL)
            {
                if ((exactMatch && rootNode->_name == id) || (!exactMatch && rootNode->_name.find(id) == 0))
                    return rootNode;

                Node* match = rootNode->findNode(id, true, exactMatch, true);
                if (match)
                {
                    return match;
                }
            }
        }
    }
    // Search immediate children first.
    for (size_t i=0; i<_children.size(); ++i) {
        Node *child = _children[i];
        // Does this child's ID match?
        if ((exactMatch && child->_name == id) || (!exactMatch && child->_name.find(id) == 0))
        {
            return child;
        }
    }
    // Recurse.
    if (recursive)
    {
        for (size_t i=0; i<_children.size(); ++i) {
            Node *child = _children[i];
            Node* match = child->findNode(id, true, exactMatch, skipSkin);
            if (match)
            {
                return match;
            }
        }
    }
    return NULL;
}

unsigned int Node::findNodes(const char* id, std::vector<Node*>& nodes, bool recursive, bool exactMatch) const
{
    return findNodes(id, nodes, recursive, exactMatch, false);
}

unsigned int Node::findNodes(const char* id, std::vector<Node*>& nodes, bool recursive, bool exactMatch, bool skipSkin) const
{
    GP_ASSERT(id);

    // If the drawable is a model with a mesh skin, search the skin's hierarchy as well.
    unsigned int count = 0;

    if (!skipSkin)
    {
        Node* rootNode = NULL;
        Model* model = dynamic_cast<Model*>(getDrawable());
        if (model)
        {
            if (model->getSkin() != NULL && (rootNode = model->getSkin()->_rootNode) != NULL)
            {
                if ((exactMatch && rootNode->_name == id) || (!exactMatch && rootNode->_name.find(id) == 0))
                {
                    nodes.push_back(rootNode);
                    ++count;
                }
                count += rootNode->findNodes(id, nodes, recursive, exactMatch, true);
            }
        }
    }

    // Search immediate children first.
    for (size_t i=0; i<_children.size(); ++i) {
        Node *child = _children[i];
        // Does this child's ID match?
        if ((exactMatch && child->_name == id) || (!exactMatch && child->_name.find(id) == 0))
        {
            nodes.push_back(child);
            ++count;
        }
    }
    // Recurse.
    if (recursive)
    {
        for (size_t i=0; i<_children.size(); ++i) {
            Node *child = _children[i];
            count += child->findNodes(id, nodes, recursive, exactMatch, skipSkin);
        }
    }

    return count;
}

void Node::getAllDrawable(std::vector<Drawable*> &list) {
    auto d = getDrawable();
    if (d) list.push_back(d);

    // Recurse.
    for (size_t i=0; i<_children.size(); ++i) {
        Node *child = _children[i];
        child->getAllDrawable(list);
    }
}

Scene* Node::getScene() const
{
    if (_scene)
        return _scene;

    // Search our parent for the scene
    if (_parent)
    {
        Scene* scene = _parent->getScene();
        if (scene)
            return scene;
    }
    return NULL;
}

bool Node::hasTag(const char* name) const
{
    GP_ASSERT(name);
    return (_tags ? _tags->find(name) != _tags->end() : false);
}

const char* Node::getTag(const char* name) const
{
    GP_ASSERT(name);

    if (!_tags)
        return NULL;

    std::map<std::string, std::string>::const_iterator itr = _tags->find(name);
    return (itr == _tags->end() ? NULL : itr->second.c_str());
}

void Node::setTag(const char* name, const char* value)
{
    GP_ASSERT(name);

    if (value == NULL)
    {
        // Removing tag
        if (_tags)
        {
            _tags->erase(name);
            if (_tags->size() == 0)
            {
                SAFE_DELETE(_tags);
            }
        }
    }
    else
    {
        // Setting tag
        if (_tags == NULL)
        {
            _tags = new std::map<std::string, std::string>();
        }
        (*_tags)[name] = value;
    }
}

void Node::setEnabled(bool enabled)
{
    if (_enabled != enabled)
    {
        if (getCollisionObject())
        {
            getCollisionObject()->setEnabled(enabled);
        }
        _enabled = enabled;
    }
}

bool Node::isEnabled() const
{
    return _enabled;
}

bool Node::isEnabledInHierarchy() const
{
    if (!_enabled)
       return false;

   Node* node = _parent;
   while (node)
   {
       if (!node->_enabled)
       {
           return false;
       }
       node = node->_parent;
   }
   return true;
}

void Node::update(float elapsedTime)
{
    for (size_t i=0; i<_children.size(); ++i) {
        Node *child = _children[i];
        if (child->isEnabled())
        {
            child->update(elapsedTime);
        }
    }

    for (size_t i = 0; i < _components.size(); i++)
    {
        Drawable *d = dynamic_cast<Drawable*>(_components[i]);
        d->update(elapsedTime);
    }

    fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(Node, update), dynamic_cast<void*>(this), elapsedTime);
}

bool Node::isStatic() const
{
    auto _collisionObject = getCollisionObject();
    return (_collisionObject && _collisionObject->isStatic());
}

const Matrix& Node::getWorldMatrix() const
{
    if (_dirtyBits & NODE_DIRTY_WORLD)
    {
        // Clear our dirty flag immediately to prevent this block from being entered if our
        // parent calls our getWorldMatrix() method as a result of the following calculations.
        _dirtyBits &= ~NODE_DIRTY_WORLD;

        if (!isStatic())
        {
            // If we have a parent, multiply our parent world transform by our local
            // transform to obtain our final resolved world transform.
            Node* parent = getParent();
            auto _collisionObject = getCollisionObject();
            if (parent && (!_collisionObject || _collisionObject->isKinematic()))
            {
                Matrix::multiply(parent->getWorldMatrix(), getMatrix(), &_world);
            }
            else
            {
                _world = getMatrix();
            }

            // Our world matrix was just updated, so call getWorldMatrix() on all child nodes
            // to force their resolved world matrices to be updated.
            for (size_t i=0; i<_children.size(); ++i) {
                Node *child = _children[i];
                child->getWorldMatrix();
            }
        }
    }
    return _world;
}

const Matrix& Node::getWorldViewMatrix() const
{
    static Matrix worldView;
    Matrix::multiply(getViewMatrix(), getWorldMatrix(), &worldView);
    return worldView;
}

const Matrix& Node::getInverseTransposeWorldViewMatrix() const
{
    static Matrix invTransWorldView;
    Matrix::multiply(getViewMatrix(), getWorldMatrix(), &invTransWorldView);
    invTransWorldView.invert();
    invTransWorldView.transpose();
    return invTransWorldView;
}

const Matrix& Node::getInverseTransposeWorldMatrix() const
{
    static Matrix invTransWorld;
    invTransWorld = getWorldMatrix();
    invTransWorld.invert();
    invTransWorld.transpose();
    return invTransWorld;
}

const Matrix& Node::getViewMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getViewMatrix();
    }
    else
    {
        return Matrix::identity();
    }
}

const Matrix& Node::getInverseViewMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getInverseViewMatrix();
    }
    else
    {
        return Matrix::identity();
    }
}

const Matrix& Node::getProjectionMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getProjectionMatrix();
    }
    else
    {
        return Matrix::identity();
    }
}

const Matrix& Node::getViewProjectionMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getViewProjectionMatrix();
    }
    else
    {
        return Matrix::identity();
    }
}

const Matrix& Node::getInverseViewProjectionMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getInverseViewProjectionMatrix();
    }
    return Matrix::identity();
}

const Matrix& Node::getWorldViewProjectionMatrix() const
{
    // Always re-calculate worldViewProjection matrix since it's extremely difficult
    // to track whether the camera has changed (it may frequently change every frame).
    static Matrix worldViewProj;
    Matrix::multiply(getViewProjectionMatrix(), getWorldMatrix(), &worldViewProj);
    return worldViewProj;
}

Vector3 Node::getTranslationWorld() const
{
    Vector3 translation;
    getWorldMatrix().getTranslation(&translation);
    return translation;
}

Vector3 Node::getTranslationView() const
{
    Vector3 translation;
    getWorldMatrix().getTranslation(&translation);
    getViewMatrix().transformPoint(&translation);
    return translation;
}

Vector3 Node::getForwardVectorWorld() const
{
    Vector3 vector;
    getWorldMatrix().getForwardVector(&vector);
    return vector;
}

Vector3 Node::getForwardVectorView() const
{
    Vector3 vector;
    getWorldMatrix().getForwardVector(&vector);
    getViewMatrix().transformVector(&vector);
    return vector;
}

Vector3 Node::getRightVectorWorld() const
{
    Vector3 vector;
    getWorldMatrix().getRightVector(&vector);
    return vector;
}

Vector3 Node::getUpVectorWorld() const
{
    Vector3 vector;
    getWorldMatrix().getUpVector(&vector);
    return vector;
}

Vector3 Node::getActiveCameraTranslationWorld() const
{
    Scene* scene = getScene();
    if (scene)
    {
        Camera* camera = scene->getActiveCamera();
        if (camera)
        {
            Node* cameraNode = camera->getNode();
            if (cameraNode)
            {
                return cameraNode->getTranslationWorld();
            }
        }
    }
    return Vector3::zero();
}

Vector3 Node::getActiveCameraTranslationView() const
{
    Scene* scene = getScene();
    if (scene)
    {
        Camera* camera = scene->getActiveCamera();
        if (camera)
        {
            Node* cameraNode = camera->getNode();
            if (cameraNode)
            {
                return cameraNode->getTranslationView();
            }
        }
    }
    return Vector3::zero();
}

void Node::hierarchyChanged()
{
    // When our hierarchy changes our world transform is affected, so we must dirty it.
    _dirtyBits |= NODE_DIRTY_HIERARCHY;
    transformChanged();
}

void Node::transformChanged()
{
    // Our local transform was changed, so mark our world matrices dirty.
    _dirtyBits |= NODE_DIRTY_WORLD | NODE_DIRTY_BOUNDS;

    // Notify our children that their transform has also changed (since transforms are inherited).
    for (size_t i=0; i<_children.size(); ++i) {
        Node *n = _children[i];
        if (Transform::isTransformChangedSuspended())
        {
            // If the DIRTY_NOTIFY bit is not set
            if (!n->isDirty(Transform::DIRTY_NOTIFY))
            {
                n->transformChanged();
                suspendTransformChange(n);
            }
        }
        else
        {
            n->transformChanged();
        }
    }
    Transform::transformChanged();
}

void Node::setBoundsDirty()
{
    // Mark ourself and our parent nodes as dirty
    _dirtyBits |= NODE_DIRTY_BOUNDS;

    // Mark our parent bounds as dirty as well
    if (_parent)
        _parent->setBoundsDirty();
}

Animation* Node::getAnimation(const char* id) const
{
    Animation* animation = ((AnimationTarget*)this)->getAnimation(id);
    if (animation)
        return animation;
    
    // See if this node has a model, then drill down.
    Model* model = dynamic_cast<Model*>(getDrawable());
    if (model)
    {
        // Check to see if there's any animations with the ID on the joints.
        MeshSkin* skin = model->getSkin();
        if (skin)
        {
            Node* rootNode = skin->_rootNode;
            if (rootNode)
            {
                animation = rootNode->getAnimation(id);
                if (animation)
                    return animation;
            }
        }

        // Check to see if any of the model's material parameter's has an animation
        // with the given ID.
        Material* material = model->getMaterial();
        if (material)
        {
            // How to access material parameters? hidden on the Material::MaterialParamBinding.
            std::vector<MaterialParameter*>::iterator itr = material->_parameters.begin();
            for (; itr != material->_parameters.end(); itr++)
            {
                GP_ASSERT(*itr);
                animation = ((MaterialParameter*)(*itr))->getAnimation(id);
                if (animation)
                    return animation;
            }
        }
    }

    // look through form for animations.
    Form* form = dynamic_cast<Form*>(getDrawable());
    if (form)
    {
        animation = form->getAnimation(id);
        if (animation)
            return animation;
    }

    // Look through this node's children for an animation with the specified ID.
    for (size_t i=0; i<_children.size(); ++i) {
        Node *child = _children[i];
        animation = child->getAnimation(id);
        if (animation)
            return animation;
    }
    
    return NULL;
}

Camera* Node::getCamera() const
{
    return getComponent<Camera>();
}

void Node::setCamera(Camera* camera)
{
    addComponent<Camera>(camera);
}

Light* Node::getLight() const
{
    return getComponent<Light>();
}

void Node::setLight(Light* light)
{
    addComponent<Light>(light);
}

Drawable* Node::getDrawable() const
{
    return getComponent<Drawable>();
}

void Node::setDrawable(Drawable* drawable)
{
    addComponent<Drawable>(drawable);
}

PhysicsCollisionObject* Node::getCollisionObject() const {
    return getComponent<PhysicsCollisionObject>();
}

const BoundingSphere& Node::getBoundingSphere() const
{
    if (_dirtyBits & NODE_DIRTY_BOUNDS)
    {
        _dirtyBits &= ~NODE_DIRTY_BOUNDS;

        const Matrix& worldMatrix = getWorldMatrix();

        // Start with our local bounding sphere
        // TODO: Incorporate bounds from entities other than mesh (i.e. particleemitters, audiosource, etc)
        auto _drawable = getDrawable();
        auto _light = getLight();
        auto _camera = getCamera();
        bool empty = true;
        Terrain* terrain = dynamic_cast<Terrain*>(_drawable);
        if (terrain)
        {
            _bounds.set(terrain->getBoundingBox());
            empty = false;
        }
        Model* model = dynamic_cast<Model*>(_drawable);
        if (model && model->getMesh())
        {
            if (empty)
            {
                _bounds.set(model->getMesh()->getBoundingSphere());
                empty = false;
            }
            else
            {
                _bounds.merge(model->getMesh()->getBoundingSphere());
            }
        }
        if (_light)
        {
            switch (_light->getLightType())
            {
            case Light::POINT:
                if (empty)
                {
                    _bounds.set(Vector3::zero(), _light->getRange());
                    empty = false;
                }
                else
                {
                    _bounds.merge(BoundingSphere(Vector3::zero(), _light->getRange()));
                }
                break;
            case Light::SPOT:
                // TODO: Implement spot light bounds
                break;
            }
        }
        if (empty)
        {
            // Empty bounding sphere, set the world translation with zero radius
            worldMatrix.getTranslation(&_bounds.center);
            _bounds.radius = 0;
        }

        // Transform the sphere (if not empty) into world space.
        if (!empty)
        {
            bool applyWorldTransform = true;
            if (model && model->getSkin())
            {
                // Special case: If the root joint of our mesh skin is parented by any nodes, 
                // multiply the world matrix of the root joint's parent by this node's
                // world matrix. This computes a final world matrix used for transforming this
                // node's bounding volume. This allows us to store a much smaller bounding
                // volume approximation than would otherwise be possible for skinned meshes,
                // since joint parent nodes that are not in the matrix palette do not need to
                // be considered as directly transforming vertices on the GPU (they can instead
                // be applied directly to the bounding volume transformation below).
                GP_ASSERT(model->getSkin()->getRootJoint());
                Node* jointParent = model->getSkin()->getRootJoint()->getParent();
                if (jointParent)
                {
                    // TODO: Should we protect against the case where joints are nested directly
                    // in the node hierachy of the model (this is normally not the case)?
                    Matrix boundsMatrix;
                    Matrix::multiply(getWorldMatrix(), jointParent->getWorldMatrix(), &boundsMatrix);
                    _bounds.transform(boundsMatrix);
                    applyWorldTransform = false;
                }
            }
            if (applyWorldTransform)
            {
                _bounds.transform(getWorldMatrix());
            }
        }

        // Merge this world-space bounding sphere with our childrens' bounding volumes.
        for (size_t i=0; i<_children.size(); ++i) {
            Node *n = _children[i];
            const BoundingSphere& childSphere = n->getBoundingSphere();
            if (!childSphere.isEmpty())
            {
                if (empty)
                {
                    _bounds.set(childSphere);
                    empty = false;
                }
                else
                {
                    _bounds.merge(childSphere);
                }
            }
        }
    }

    return _bounds;
}

Node* Node::clone() const
{
    NodeCloneContext context;
    return cloneRecursive(context);
}

Node* Node::cloneSingleNode(NodeCloneContext &context) const
{
    Node* copy = Node::create(getName());
    context.registerClonedNode(this, copy);
    cloneInto(copy, context);
    return copy;
}

Node* Node::cloneRecursive(NodeCloneContext &context) const
{
    Node* copy = cloneSingleNode(context);
    GP_ASSERT(copy);

    // Add child nodes
    for (size_t i=0; i<_children.size(); ++i) {
        Node *child = _children[i];
        Node* childCopy = child->cloneRecursive(context);
        GP_ASSERT(childCopy);
        copy->addChild(childCopy);
        childCopy->release();
    }

    return copy;
}

void Node::cloneInto(Node* node, NodeCloneContext& context) const
{
    GP_ASSERT(node);

    Transform::cloneInto(node, context);

    if (Drawable* drawable = getDrawable())
    {
        Drawable* clone = drawable->clone(context);
        node->setDrawable(clone);
        Ref* ref = dynamic_cast<Ref*>(clone);
        if (ref)
            ref->release();
    }
    if (Camera* camera = getCamera())
    {
        Camera* clone = camera->clone(context);
        node->setCamera(clone);
        Ref* ref = dynamic_cast<Ref*>(clone);
        if (ref)
            ref->release();
    }
    if (Light* light = getLight())
    {
        Light* clone = light->clone(context);
        node->setLight(clone);
        Ref* ref = dynamic_cast<Ref*>(clone);
        if (ref)
            ref->release();
    }
    /*if (AudioSource* audio = getAudioSource())
    {
        AudioSource* clone = audio->clone(context);
        node->setAudioSource(clone);
        Ref* ref = dynamic_cast<Ref*>(clone);
        if (ref)
            ref->release();
    }*/
    if (_tags)
    {
        node->_tags = new std::map<std::string, std::string>(_tags->begin(), _tags->end());
    }

    node->_world = _world;
    node->_bounds = _bounds;

    // TODO: Clone the rest of the node data.
}

void Node::_addComponent(Component *comp) {
    if (comp) _components.push_back(comp);
}

Ref* Node::getUserObject() const
{
    return _userObject;
}

void Node::setUserObject(Ref* obj)
{
    _userObject = obj;
}


Serializable* Node::createObject()
{
    return new Node();
}

std::string Node::getClassName()
{
    return "gameplay::Node";
}

void Node::onSerialize(Serializer* serializer)
{
    serializer->writeString("name", _name.c_str(), SCENEOBJECT_NAME);
    serializer->writeBool("enabled", isEnabled(), SCENEOBJECT_ENABLED);
    serializer->writeBool("static", isStatic(), SCENEOBJECT_STATIC);
    serializer->writeVector("position", getTranslation(), SCENEOBJECT_POSITION);
    serializer->writeVector("eulerAngles", getEulerAngles(), SCENEOBJECT_EULER_ANGLES);
    serializer->writeVector("scale", getScale(), SCENEOBJECT_SCALE);
    if (getChildCount() > 0)
    {
        serializer->writeList("children", getChildCount());
        for (size_t i=0; i<_children.size(); ++i) {
            Node *child = _children[i];
            serializer->writeObject(nullptr, child);
        }
        serializer->finishColloction();
    }
    if (_components.size() > 0)
    {
        serializer->writeList("components", _components.size());
        for (auto component : _components)
        {
            Serializable *s = dynamic_cast<Serializable*>(component);
            if (s) {
                serializer->writeObject(nullptr, s);
            }
        }
        serializer->finishColloction();
    }

    auto co = getCollisionObject();
    if (co) {
        serializer->writeString("collisionObject", co->getName().c_str(), "");
    }
    else {
        serializer->writeString("collisionObject", "", "");
    }
}

void Node::onDeserialize(Serializer* serializer)
{
    serializer->readString("name", _name, SCENEOBJECT_NAME);
    _enabled = serializer->readBool("enabled", SCENEOBJECT_STATIC);
    _static = serializer->readBool("static", SCENEOBJECT_STATIC);
    Vector3 position = serializer->readVector("position", SCENEOBJECT_POSITION);
    this->setTranslation(position);
    Vector3 _eulerAngles = serializer->readVector("eulerAngles", SCENEOBJECT_EULER_ANGLES);
    Quaternion rotation;
    Quaternion::createFromEuler(_eulerAngles.x, _eulerAngles.y, _eulerAngles.z, &rotation);
    this->setRotation(rotation);
    Vector3 scale = serializer->readVector("scale", SCENEOBJECT_SCALE);
    this->setScale(scale);

    size_t childCount = serializer->readList("children");
    if (childCount > 0)
    {
        for (size_t i = 0; i < childCount; i++) {
            auto ptr = serializer->readObject(nullptr);
            //ptr->addRef();
            this->addChild(dynamic_cast<Node*>(ptr));
        }
    }
    serializer->finishColloction();
    size_t componentCount =  serializer->readList("components");
    if (componentCount > 0)
    {
        _components.resize(componentCount);
        for (size_t i = 0; i < _components.size(); i++)
        {
            auto ptr = serializer->readObject(nullptr);
            //ptr->addRef();
            _components[i] = dynamic_cast<Component*>(ptr);
            _components[i]->setNode(this);
        }
    }
    serializer->finishColloction();

    std::string collisionObject;
    serializer->readString("collisionObject", collisionObject, SCENEOBJECT_NAME);
    PhysicsCollisionObject::load(collisionObject, this);
}


NodeCloneContext::NodeCloneContext()
{
}

NodeCloneContext::~NodeCloneContext()
{
}

Animation* NodeCloneContext::findClonedAnimation(const Animation* animation)
{
    GP_ASSERT(animation);

    std::map<const Animation*, Animation*>::iterator it = _clonedAnimations.find(animation);
    return it != _clonedAnimations.end() ? it->second : NULL;
}

void NodeCloneContext::registerClonedAnimation(const Animation* original, Animation* clone)
{
    GP_ASSERT(original);
    GP_ASSERT(clone);

    _clonedAnimations[original] = clone;
}

Node* NodeCloneContext::findClonedNode(const Node* node)
{
    GP_ASSERT(node);

    std::map<const Node*, Node*>::iterator it = _clonedNodes.find(node);
    return it != _clonedNodes.end() ? it->second : NULL;
}

void NodeCloneContext::registerClonedNode(const Node* original, Node* clone)
{
    GP_ASSERT(original);
    GP_ASSERT(clone);

    _clonedNodes[original] = clone;
}

}
