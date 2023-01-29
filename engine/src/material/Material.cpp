#include "base/Base.h"
#include "Material.h"
#include "base/FileSystem.h"
#include "ShaderProgram.h"
#include "base/Properties.h"
#include "scene/Node.h"
#include "MaterialParameter.h"

namespace gameplay
{
extern void loadRenderState(Material* renderState, Properties* properties);

Material::Material() :
    _shaderProgram(NULL), _nextPass(NULL)
{
}

Material::~Material()
{
    // Destroy all the material parameters
    for (size_t i = 0, count = _parameters.size(); i < count; ++i)
    {
        SAFE_RELEASE(_parameters[i]);
    }

    SAFE_RELEASE(_shaderProgram);
    //SAFE_RELEASE(_vertexAttributeBinding);
    SAFE_RELEASE(_nextPass);
}

Material* Material::create(const char* url)
{
    return create(url, (PassCallback)NULL, NULL);
}

std::vector<Material*> Material::createAll(const char* url)
{
    std::vector<Material*> res;

    Properties* properties = Properties::create(url);
    if (properties == NULL)
    {
        GP_WARN("Failed to create material from file: %s", url);
        return res;
    }

    for (Properties *p = properties->getNextNamespace(); p != NULL; p = properties->getNextNamespace()) {
        Material* material = create(p, NULL, NULL);
        res.push_back(material);
    }
    SAFE_DELETE(properties);
    return res;
}

Material* Material::create(const char* url, PassCallback callback, void* cookie)
{
    // Load the material properties from file.
    Properties* properties = Properties::create(url);
    if (properties == NULL)
    {
        GP_WARN("Failed to create material from file: %s", url);
        return NULL;
    }

    Material* material = create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace(), callback, cookie);
    SAFE_DELETE(properties);

    return material;
}

Material* Material::create(Properties* materialProperties)
{
    return create(materialProperties, (PassCallback)NULL, NULL);
}

bool Material::initialize(RenderView* view)
{
    if (_shaderProgram) return true;

    // Attempt to create/load the effect.
    _shaderProgram = ShaderProgram::createFromFile(vertexShaderPath.c_str(), fragmentShaderPath.c_str(), shaderDefines.c_str());
    if (_shaderProgram == NULL)
    {
        GP_WARN("Failed to create effect for pass. vertexShader = %s, fragmentShader = %s, defines = %s", 
            vertexShaderPath.c_str(), fragmentShaderPath.c_str(), shaderDefines.c_str());
        return false;
    }

    return true;
}

void Material::bindCamera(RenderView* view, Node *node) {
    if (!node) return;
    Uniform *uniform = _shaderProgram->getUniform("u_worldViewProjectionMatrix");
    if (uniform) {
        Matrix worldViewProj;
        Matrix::multiply(view->camera->getViewProjectionMatrix(), node->getWorldMatrix(), &worldViewProj);
        MaterialParameter* param = getParameter("u_worldViewProjectionMatrix");
        param->setMatrix(worldViewProj);
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_worldMatrix");
    if (uniform) {
        MaterialParameter* param = getParameter("u_worldMatrix");
        param->setMatrix(node->getWorldMatrix());
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_viewMatrix");
    if (uniform) {
        MaterialParameter* param = getParameter("u_worldMatrix");
        param->setMatrix(view->camera->getViewMatrix());
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_projectionMatrix");
    if (uniform) {
        MaterialParameter* param = getParameter("u_projectionMatrix");
        param->setMatrix(view->camera->getProjectionMatrix());
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_worldViewMatrix");
    if (uniform) {
        Matrix worldViewProj;
        Matrix::multiply(view->camera->getViewMatrix(), node->getWorldMatrix(), &worldViewProj);
        MaterialParameter* param = getParameter("u_worldViewMatrix");
        param->setMatrix(worldViewProj);
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_viewProjectionMatrix");
    if (uniform) {
        MaterialParameter* param = getParameter("u_viewProjectionMatrix");
        param->setMatrix(view->camera->getViewProjectionMatrix());
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_inverseTransposeWorldMatrix");
    if (uniform) {
        Matrix invTransWorld;
        invTransWorld = node->getWorldMatrix();
        invTransWorld.invert();
        invTransWorld.transpose();
        MaterialParameter* param = getParameter("u_inverseTransposeWorldMatrix");
        param->setMatrix(invTransWorld);
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_inverseTransposeWorldViewMatrix");
    if (uniform) {
        Matrix invTransWorld;
        Matrix::multiply(view->camera->getViewMatrix(), node->getWorldMatrix(), &invTransWorld);
        invTransWorld.invert();
        invTransWorld.transpose();
        MaterialParameter* param = getParameter("u_inverseTransposeWorldViewMatrix");
        param->setMatrix(invTransWorld);
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_normalMatrix");
    if (uniform) {
        Matrix invTransWorld;
        Matrix::multiply(view->camera->getViewMatrix(), node->getWorldMatrix(), &invTransWorld);
        invTransWorld.invert();
        invTransWorld.transpose();
        MaterialParameter* param = getParameter("u_normalMatrix");
        param->setMatrix(invTransWorld);
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_cameraPosition");
    if (uniform) {
        MaterialParameter* param = getParameter("u_cameraPosition");
        param->setVector3(view->camera->getNode()->getTranslationWorld());
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_matrixPalette");
    if (uniform) {
        Model* model = dynamic_cast<Model*>(node->getDrawable());
        if (model)
        {
            MeshSkin* skin = model->getSkin();
            if (skin) {
                MaterialParameter* param = getParameter("u_matrixPalette");
                param->setVector4Array(skin->getMatrixPalette(), skin->getMatrixPaletteSize());
                param->_temporary = true;
            }
        }
    }

    uniform = _shaderProgram->getUniform("u_ambientColor");
    if (uniform) {
        Scene* scene = node->getScene();
        MaterialParameter* param = getParameter("u_ambientColor");
        param->setVector3(view->camera->getNode()->getTranslationWorld());
        param->_temporary = true;
    }

    uniform = _shaderProgram->getUniform("u_viewPort");
    if (uniform) {
        MaterialParameter* param = getParameter("u_viewPort");
        param->setVector4(&view->viewport.x);
        param->_temporary = true;
    }
}

Material* Material::create(Properties* materialProperties, PassCallback callback, void* cookie)
{
    // Check if the Properties is valid and has a valid namespace.
    if (!materialProperties || !(strcmp(materialProperties->getNamespace(), "material") == 0))
    {
        GP_ERROR("Properties object must be non-null and have namespace equal to 'material'.");
        return NULL;
    }

    // Create new material from the file passed in.
    Material* material = new Material();

    material->setName(materialProperties->getId());

    // Load uniform value parameters for this material.
    loadRenderState(material, materialProperties);

    // Fetch shader info required to create the effect of this technique.
    const char* vertexShaderPath = materialProperties->getString("vertexShader");
    GP_ASSERT(vertexShaderPath);
    const char* fragmentShaderPath = materialProperties->getString("fragmentShader");
    GP_ASSERT(fragmentShaderPath);
    const char* passDefines = materialProperties->getString("defines");

    // If a pass callback was specified, call it and add the result to our list of defines
    std::string allDefines = passDefines ? passDefines : "";
    if (callback)
    {
        std::string customDefines = callback(material, cookie);
        if (customDefines.length() > 0)
        {
            if (allDefines.length() > 0)
                allDefines += ';';
            allDefines += customDefines;
        }
    }

    material->vertexShaderPath = vertexShaderPath;
    material->fragmentShaderPath = fragmentShaderPath;
    material->shaderDefines = allDefines;

    return material;
}

Material* Material::create(ShaderProgram* effect)
{
    GP_ASSERT(effect);

    // Create a new material with a single technique and pass for the given effect.
    Material* material = new Material();
    material->_shaderProgram = effect;
    effect->addRef();

    return material;
}

Material* Material::create(const char* vshPath, const char* fshPath, const char* defines)
{
    GP_ASSERT(vshPath);
    GP_ASSERT(fshPath);

    // Create a new material with a single technique and pass for the given effect
    Material* material = new Material();

    material->vertexShaderPath = vshPath;
    material->fragmentShaderPath = fshPath;
    material->shaderDefines = defines == NULL ? "" : defines;

    return material;
}

//void Material::setNodeBinding(Node* node)
//{
//    _node = node;
//
//    //_paramBinding.setNodeBinding(node);
//
//    if (_nextPass) _nextPass->setNodeBinding(node);
//}

//void Material::setParameterAutoBinding(const char* name, MaterialParamBinding::AutoBinding autoBinding) {
//    _paramBinding.setParameterAutoBinding(name, autoBinding);
//}
//
//
//void Material::setParameterAutoBinding(const char* name, const char* autoBinding) {
//    _paramBinding.setParameterAutoBinding(name, autoBinding);
//}

Material* Material::clone(NodeCloneContext &context) const
{
    Material* material = new Material();

    for (std::vector<MaterialParameter*>::const_iterator it = _parameters.begin(); it != _parameters.end(); ++it)
    {
        const MaterialParameter* param = *it;
        GP_ASSERT(param);

        // If this parameter is a method binding auto binding, don't clone it - it will get setup automatically
        // via the cloned auto bindings instead.
        if (param->_methodBinding && param->_methodBinding->_autoBinding)
            continue;

        MaterialParameter* paramCopy = new MaterialParameter(param->getName());
        param->cloneInto(paramCopy);

        material->_parameters.push_back(paramCopy);
    }

    // Clone our state block
    _state.cloneInto(material->getStateBlock());

    //_paramBinding.cloneInto(&material->_paramBinding, context);

    GP_ASSERT(_shaderProgram);
    _shaderProgram->addRef();
    material->_shaderProgram = _shaderProgram;

    if (_nextPass) material->_nextPass = _nextPass->clone(context);
    return material;
}


ShaderProgram* Material::getEffect() const {
    return _shaderProgram;
}

//void Material::setVertexAttributeBinding(VertexAttributeBinding* binding)
//{
//    SAFE_RELEASE(_vertexAttributeBinding);
//
//    if (binding)
//    {
//        _vertexAttributeBinding = binding;
//        binding->addRef();
//    }
//
//    if (_nextPass) _nextPass->setVertexAttributeBinding(binding);
//}
//
//VertexAttributeBinding* Material::getVertexAttributeBinding() const
//{
//    return _vertexAttributeBinding;
//}

void Material::bind(RenderView* view, Node* node)
{
    if (!initialize(view)) {
        return;
    }
    GP_ASSERT(_shaderProgram);

    // Bind our effect.
    _shaderProgram->bind();

    if (view) bindCamera(view, node);

    // Bind our render state
    for (size_t i = 0, count = _parameters.size(); i < count; ++i)
    {
        GP_ASSERT(_parameters[i]);
        _parameters[i]->bind(this->_shaderProgram);
    }
    _state.bind();

    // If we have a vertex attribute binding, bind it
    /*if (_vertexAttributeBinding)
    {
        _vertexAttributeBinding->bind();
    }*/
}

void Material::unbind()
{
    // If we have a vertex attribute binding, unbind it
    /*if (_vertexAttributeBinding)
    {
        _vertexAttributeBinding->unbind();
    }*/
}

Serializable* Material::createObject() {
    return new Material();
}

/**
 * @see Serializable::getClassName
 */
std::string Material::getClassName() {
    return "gameplay::Material";
}

/**
 * @see Serializable::onSerialize
 */
void Material::onSerialize(Serializer* serializer) {
    //serializer->writeString("name", getName().c_str(), "");
    serializer->writeString("vertexShaderPath", vertexShaderPath.c_str(), "");
    serializer->writeString("fragmentShaderPath", fragmentShaderPath.c_str(), "");
    serializer->writeString("shaderDefines", shaderDefines.c_str(), "");

    /*
    std::vector<std::string> params;
    for (int i=0; i<getParameterCount(); ++i) {
        MaterialParameter *p = getParameterByIndex(i);
        params.push_back(p->getName());
    }
    */

    int count;
    for (int i = 0; i < getParameterCount(); ++i) {
        MaterialParameter* p = getParameterByIndex(i);
        if (p->_temporary) continue;
        ++count;
    }

    serializer->writeList("parameters", count);
    for (int i=0; i<getParameterCount(); ++i) {
        MaterialParameter *p = getParameterByIndex(i);
        if (p->_temporary) continue;
        serializer->writeObject(NULL, p);
    }
    serializer->finishColloction();
}

/**
 * @see Serializable::onDeserialize
 */
void Material::onDeserialize(Serializer* serializer) {
    serializer->readString("vertexShaderPath", vertexShaderPath, "");
    serializer->readString("fragmentShaderPath", fragmentShaderPath, "");
    serializer->readString("shaderDefines", shaderDefines, "");


    int size = serializer->readList("parameters");
    for (int i = 0; i < size; ++i) {
        MaterialParameter* p = dynamic_cast<MaterialParameter*>(serializer->readObject(NULL));
        _parameters.push_back(p);
    }
    serializer->finishColloction();
}

void Material::setStateBlock(StateBlock* state)
{
    _state = *state;
}

StateBlock* Material::getStateBlock() const
{
    return (StateBlock*)(&_state);
}

MaterialParameter* Material::getParameter(const char* name) const
{
    GP_ASSERT(name);

    // Search for an existing parameter with this name.
    MaterialParameter* param;
    for (size_t i = 0, count = _parameters.size(); i < count; ++i)
    {
        param = _parameters[i];
        GP_ASSERT(param);
        if (strcmp(param->getName(), name) == 0)
        {
            return param;
        }
    }

    // Create a new parameter and store it in our list.
    param = new MaterialParameter(name);
    _parameters.push_back(param);

    return param;
}

unsigned int Material::getParameterCount() const
{
    return _parameters.size();
}

MaterialParameter* Material::getParameterByIndex(unsigned int index)
{
    return _parameters[index];
}

void Material::addParameter(MaterialParameter* param)
{
    _parameters.push_back(param);
    param->addRef();
}

void Material::removeParameter(const char* name)
{
    for (size_t i = 0, count = _parameters.size(); i < count; ++i)
    {
        MaterialParameter* p = _parameters[i];
        if (p->_name == name)
        {
            _parameters.erase(_parameters.begin() + i);
            SAFE_RELEASE(p);
            break;
        }
    }
}
}
