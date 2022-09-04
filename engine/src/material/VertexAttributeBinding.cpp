#include "base/Base.h"
#include "render/Renderer.h"
#include "VertexAttributeBinding.h"
#include "scene/Mesh.h"
#include "ShaderProgram.h"


// Graphics (GLSL)
#define VERTEX_ATTRIBUTE_POSITION_NAME              "a_position"
#define VERTEX_ATTRIBUTE_NORMAL_NAME                "a_normal"
#define VERTEX_ATTRIBUTE_COLOR_NAME                 "a_color"
#define VERTEX_ATTRIBUTE_TANGENT_NAME               "a_tangent"
#define VERTEX_ATTRIBUTE_BINORMAL_NAME              "a_binormal"
#define VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME          "a_blendWeights"
#define VERTEX_ATTRIBUTE_BLENDINDICES_NAME          "a_blendIndices"
#define VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME       "a_texCoord"


namespace gameplay
{

static std::vector<VertexAttributeBinding*> __vertexAttributeBindingCache;

VertexAttributeBinding::VertexAttributeBinding() :
    _handle(0), _mesh(NULL), _effect(NULL)
{
}

VertexAttributeBinding::~VertexAttributeBinding()
{
    // Delete from the vertex attribute binding cache.
    std::vector<VertexAttributeBinding*>::iterator itr = std::find(__vertexAttributeBindingCache.begin(), __vertexAttributeBindingCache.end(), this);
    if (itr != __vertexAttributeBindingCache.end())
    {
        __vertexAttributeBindingCache.erase(itr);
    }

    SAFE_RELEASE(_mesh);
    SAFE_RELEASE(_effect);
    //SAFE_DELETE_ARRAY(_attributes);

    Renderer::cur()->deleteVertexAttributeObj(this);
}

VertexAttributeBinding* VertexAttributeBinding::create(Mesh* mesh, ShaderProgram* effect)
{
    GP_ASSERT(mesh);

    // Search for an existing vertex attribute binding that can be used.
    VertexAttributeBinding* b;
    for (size_t i = 0, count = __vertexAttributeBindingCache.size(); i < count; ++i)
    {
        b = __vertexAttributeBindingCache[i];
        GP_ASSERT(b);
        if (b->_mesh == mesh && b->_effect == effect)
        {
            // Found a match!
            b->addRef();
            return b;
        }
    }

    b = create(mesh, mesh->getVertexFormat(), 0, effect);

    // Add the new vertex attribute binding to the cache.
    if (b)
    {
        __vertexAttributeBindingCache.push_back(b);
    }

    return b;
}

VertexAttributeBinding* VertexAttributeBinding::create(const VertexFormat& vertexFormat, void* vertexPointer, ShaderProgram* effect)
{
    return create(NULL, vertexFormat, vertexPointer, effect);
}

VertexAttributeBinding* VertexAttributeBinding::create(Mesh* mesh, const VertexFormat& vertexFormat, void* vertexPointer, ShaderProgram* effect)
{
    GP_ASSERT(effect);

    // Create a new VertexAttributeBinding.
    VertexAttributeBinding* b = new VertexAttributeBinding();

    if (mesh)
    {
        b->_mesh = mesh;
        mesh->addRef();
    }
    
    b->_effect = effect;
    effect->addRef();

    // Call setVertexAttribPointer for each vertex element.
    std::string name;
    size_t offset = 0;
    for (size_t i = 0, count = vertexFormat.getElementCount(); i < count; ++i)
    {
        const VertexFormat::Element& e = vertexFormat.getElement(i);
        gameplay::VertexAttributeLoc attrib = -1;

        if (e.name.size() > 0) {
            attrib = effect->getVertexAttribute(e.name.c_str());
        }

        if (attrib == -1) {

            // Constructor vertex attribute name expected in shader.
            switch (e.usage)
            {
            case VertexFormat::POSITION:
                attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_POSITION_NAME);
                break;
            case VertexFormat::NORMAL:
                attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_NORMAL_NAME);
                break;
            case VertexFormat::COLOR:
                attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_COLOR_NAME);
                break;
            case VertexFormat::TANGENT:
                attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_TANGENT_NAME);
                break;
            case VertexFormat::BINORMAL:
                attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_BINORMAL_NAME);
                break;
            case VertexFormat::BLENDWEIGHTS:
                attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME);
                break;
            case VertexFormat::BLENDINDICES:
                attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_BLENDINDICES_NAME);
                break;
            case VertexFormat::TEXCOORD0:
                if ((attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME)) != -1)
                    break;

            case VertexFormat::TEXCOORD1:
            case VertexFormat::TEXCOORD2:
            case VertexFormat::TEXCOORD3:
            case VertexFormat::TEXCOORD4:
            case VertexFormat::TEXCOORD5:
            case VertexFormat::TEXCOORD6:
            case VertexFormat::TEXCOORD7:
                name = VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME;
                name += '0' + (e.usage - VertexFormat::TEXCOORD0);
                attrib = effect->getVertexAttribute(name.c_str());
                break;
            default:
                // This happens whenever vertex data contains extra information (not an error).
                attrib = -1;
                break;
            }
        }

        if (attrib == -1)
        {
            //GP_WARN("Warning: Vertex element with usage '%s' in mesh '%s' does not correspond to an attribute in effect '%s'.", VertexFormat::toString(e.usage), mesh->getUrl(), effect->getId());
        }
        else
        {
            void* pointer = vertexPointer ? (void*)(((unsigned char*)vertexPointer) + offset) : (void*)offset;

            VertexAttribute attri;
            attri.enabled = true;
            attri.size = e.size;
            attri.type = 0x1406;//GL_FLOAT
            attri.normalized = 0;
            attri.stride = vertexFormat.getVertexSize();
            attri.location = attrib;
            attri.pointer = pointer;

            b->_attributes.push_back(attri);
        }

        offset += e.size * sizeof(float);
    }

    b->bind();
    b->unbind();

    return b;
}

void VertexAttributeBinding::bind()
{
    Renderer::cur()->bindVertexAttributeObj(this);
}

void VertexAttributeBinding::unbind()
{
    Renderer::cur()->unbindVertexAttributeObj(this);
}

}
