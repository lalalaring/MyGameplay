#include "base/Base.h"
#include "Model.h"
#include "MeshPart.h"
#include "Scene.h"
#include "material/Material.h"
#include "scene/Node.h"
#include "render/Renderer.h"
#include "AssetManager.h"

namespace gameplay
{

Model::Model() : Drawable(),
    _mesh(NULL), _material(NULL), _skin(NULL)
{
}

Model::Model(Mesh* mesh) : Drawable(),
    _mesh(mesh), _material(NULL), _skin(NULL)
{
    GP_ASSERT(mesh);
}

Model::~Model()
{
    SAFE_RELEASE(_material);

    for (unsigned int i = 0; i < _partMaterials.size(); ++i)
    {
        SAFE_RELEASE(_partMaterials[i]);
    }
    _partMaterials.clear();

    SAFE_RELEASE(_mesh);
    SAFE_DELETE(_skin);
}

Model* Model::create(Mesh* mesh)
{
    GP_ASSERT(mesh);
    mesh->addRef();
    return new Model(mesh);
}

Mesh* Model::getMesh() const
{
    return _mesh;
}

unsigned int Model::getMeshPartCount() const
{
    GP_ASSERT(_mesh);
    return _mesh->getPartCount();
}

Material* Model::getMaterial(int partIndex)
{
    GP_ASSERT(partIndex == -1 || partIndex >= 0);

    Material* m = NULL;

    if (partIndex < 0)
        return _material;
    if (partIndex >= (int)_partMaterials.size())
        return NULL;

    // Look up explicitly specified part material.
    if (_partMaterials.size() > partIndex)
    {
        m = _partMaterials[partIndex];
    }
    if (m == NULL)
    {
        // Return the shared material.
         m = _material;
    }

    return m;
}

void Model::setMaterial(Material* material, int partIndex)
{
    GP_ASSERT(partIndex == -1 || (partIndex >= 0 && partIndex < (int)getMeshPartCount()));

    Material* oldMaterial = NULL;

    if (partIndex == -1)
    {
        oldMaterial = _material;

        // Set new shared material.
        if (material)
        {
            _material = material;
            _material->addRef();
        }
    }
    else if (partIndex >= 0 && partIndex < (int)getMeshPartCount())
    {
        // Ensure mesh part count is up-to-date.
        validatePartCount();

        // Release existing part material and part binding.
        if (_partMaterials.size() > partIndex)
        {
            oldMaterial = _partMaterials[partIndex];
        }
        else
        {
            // Allocate part arrays for the first time.
            while (_partMaterials.size() < getMeshPartCount())
            {
                _partMaterials.resize(getMeshPartCount());
            }
        }

        // Set new part material.
        if (material)
        {
            _partMaterials[partIndex] = material;
            material->addRef();
        }
    }

    // Release existing material and binding.
    if (oldMaterial)
    {
        oldMaterial->setVertexAttributeBinding(NULL);
        SAFE_RELEASE(oldMaterial);
    }

    if (material)
    {
        // Hookup vertex attribute bindings for all passes in the new material.
        VertexAttributeBinding* b = VertexAttributeBinding::create(_mesh, material->getEffect());
        material->setVertexAttributeBinding(b);
        SAFE_RELEASE(b);

        // Apply node binding for the new material.
        /*if (_node)
        {
            setMaterialNodeBinding(material);
        }*/
    }
}

Material* Model::setMaterial(const char* vshPath, const char* fshPath, const char* defines, int partIndex)
{
    // Try to create a Material with the given parameters.
    Material* material = Material::create(vshPath, fshPath, defines);
    if (material == NULL)
    {
        GP_ERROR("Failed to create material for model.");
        return NULL;
    }

    // Assign the material to us.
    setMaterial(material, partIndex);

    // Release the material since we now have a reference to it.
    material->release();

    return material;
}

Material* Model::setMaterial(const char* materialPath, int partIndex)
{
    // Try to create a Material from the specified material file.
    Material* material = Material::create(materialPath);
    if (material == NULL)
    {
        GP_ERROR("Failed to create material for model.");
        return NULL;
    }

    // Assign the material to us
    setMaterial(material, partIndex);

    // Release the material since we now have a reference to it
    material->release();

    return material;
}

bool Model::hasMaterial(unsigned int partIndex) const
{
    return (partIndex < _partMaterials.size() && _partMaterials[partIndex]);
}

MeshSkin* Model::getSkin() const
{
    return _skin;
}

void Model::setSkin(MeshSkin* skin)
{
    if (_skin != skin)
    {
        // Free the old skin
        SAFE_DELETE(_skin);

        // Assign the new skin
        _skin = skin;
        if (_skin)
            _skin->_model = this;
    }
}

void Model::setNode(Node* node)
{
    Drawable::setNode(node);

    // Re-bind node related material parameters
    //if (node)
    //{
    //    if (_material)
    //    {
    //       setMaterialNodeBinding(_material);
    //    }

    //    for (unsigned int i = 0; i < _partMaterials.size(); ++i)
    //    {
    //        if (_partMaterials[i])
    //        {
    //            setMaterialNodeBinding(_partMaterials[i]);
    //        }
    //    }

    //}
}

unsigned int Model::draw(RenderView* view)
{
    GP_ASSERT(_mesh);

    Renderer::cur()->renderMesh(_mesh, _material, _partMaterials.size(), _partMaterials.data(), view, _node);

    return _mesh->getPartCount();
}

//void Model::setMaterialNodeBinding(Material *material)
//{
//    GP_ASSERT(material);
//
//    if (_node)
//    {
//        material->setNodeBinding(getNode());
//    }
//}

Drawable* Model::clone(NodeCloneContext& context)
{
    Model* model = Model::create(getMesh());
    if (!model)
    {
        GP_ERROR("Failed to clone model.");
        return NULL;
    }

    if (getSkin())
    {
        model->setSkin(getSkin()->clone(context));
    }
    if (getMaterial())
    {
        Material* materialClone = getMaterial()->clone(context);
        if (!materialClone)
        {
            GP_ERROR("Failed to clone material for model.");
            return model;
        }
        model->setMaterial(materialClone);
        materialClone->release();
    }

    GP_ASSERT(getMeshPartCount() == model->getMeshPartCount());
    for (unsigned int i = 0; i < _partMaterials.size(); ++i)
    {
        if (_partMaterials[i])
        {
            Material* materialClone = _partMaterials[i]->clone(context);
            model->setMaterial(materialClone, i);
            materialClone->release();
        }
    }

    return model;
}

void Model::validatePartCount()
{
    GP_ASSERT(_mesh);
    unsigned int partCount = _mesh->getPartCount();

    if (_partMaterials.size() != partCount)
    {
        _partMaterials.resize(partCount);
    }
}

Serializable* Model::createObject() {
    return new Model();
}


std::string Model::getClassName() {
    return "gameplay::Model";
}


void Model::onSerialize(Serializer* serializer) {
    if (_mesh) serializer->writeString("mesh", _mesh->getName().c_str(), "");
    else serializer->writeString("mesh", "", "");
        
    if (_skin) serializer->writeString("skin", _skin->getName().c_str(), "");
    else serializer->writeString("skin", "", "");

    if (_material) serializer->writeString("material", _material->getName().c_str(), "");
    else serializer->writeString("material", "", "");

    serializer->writeList("partMaterials", _partMaterials.size());
    for (int i=0; i<_partMaterials.size(); ++i) {
        if (_partMaterials[i]) serializer->writeString(NULL, _partMaterials[i]->getName().c_str(), NULL);
        else serializer->writeString(NULL, "", NULL);
    }
    serializer->finishColloction();
}

void Model::onDeserialize(Serializer* serializer) {
    std::string mesh;
    serializer->readString("mesh", mesh, "");
    _mesh = AssetManager::getInstance()->load<Mesh>(mesh, AssetManager::rt_mesh);
    if (_mesh) _mesh->addRef();

    std::string skin;
    serializer->readString("skin", skin, "");
    _skin = AssetManager::getInstance()->load<MeshSkin>(skin, AssetManager::rt_skin);
    if (_skin) _skin->addRef();

    std::string material;
    serializer->readString("material", material, "");

    _material = AssetManager::getInstance()->load<Material>(material, AssetManager::rt_materail);
    if (_material) _material->addRef();

    int materialsCount = serializer->readList("partMaterials");
    for (int i=0; i<materialsCount; ++i) {
        std::string material;
        serializer->readString(NULL, material, NULL);
        Material *m = AssetManager::getInstance()->load<Material>(material, AssetManager::rt_materail);
        if (m) this->setMaterial(m, i);
    }
    serializer->finishColloction();
}

}
