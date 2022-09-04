#include "base/Base.h"
#include "Mesh.h"
#include "MeshPart.h"
#include "material/ShaderProgram.h"
#include "Model.h"
#include "material/Material.h"
#include "render/Renderer.h"

namespace gameplay
{

Mesh::Mesh(const VertexFormat& vertexFormat) 
    : _vertexFormat(vertexFormat), _vertexCount(0), _vertexBuffer(0), _primitiveType(TRIANGLES), 
      _dynamic(false), _vertexData(0), _vertexDataDirty(false)
{
}

Mesh::~Mesh()
{
    if (_vertexData) {
        free(_vertexData);
        _vertexData = NULL;
    }
    Renderer::cur()->deleteMesh(this);
    for (unsigned int i = 0; i < _parts.size(); ++i)
    {
        SAFE_DELETE(_parts[i]);
    }
    _parts.clear();
}

Mesh* Mesh::createMesh(const VertexFormat& vertexFormat, unsigned int vertexCount, bool dynamic)
{
    Mesh* mesh = new Mesh(vertexFormat);
    mesh->_vertexCount = vertexCount;
    mesh->_vertexBuffer = 0;
    mesh->_dynamic = dynamic;

    return mesh;
}


Mesh* Mesh::createQuad(float x, float y, float width, float height, float s1, float t1, float s2, float t2)
{
    float x2 = x + width;
    float y2 = y + height;

    float vertices[] =
    {
        x, y2, 0,   0, 0, 1,    s1, t2,
        x, y, 0,    0, 0, 1,    s1, t1,
        x2, y2, 0,  0, 0, 1,    s2, t2,
        x2, y, 0,   0, 0, 1,    s2, t1,
    };

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), 4, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }

    mesh->_primitiveType = TRIANGLE_STRIP;
    mesh->setVertexData(vertices, 0, 4);

    return mesh;
}

Mesh* Mesh::createQuadFullscreen()
{
    float x = -1.0f;
    float y = -1.0f;
    float x2 = 1.0f;
    float y2 = 1.0f;

    float vertices[] =
    {
        x, y2,   0, 1,
        x, y,    0, 0,
        x2, y2,  1, 1,
        x2, y,   1, 0
    };

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 2),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), 4, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }

    mesh->_primitiveType = TRIANGLE_STRIP;
    mesh->setVertexData(vertices, 0, 4);

    return mesh;
}

Mesh* Mesh::createQuad(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
{
    // Calculate the normal vector of the plane.
    Vector3 v1, v2, n;
    Vector3::subtract(p2, p1, &v1);
    Vector3::subtract(p3, p2, &v2);
    Vector3::cross(v1, v2, &n);
    n.normalize();

    float vertices[] =
    {
        p1.x, p1.y, p1.z, n.x, n.y, n.z, 0, 1,
        p2.x, p2.y, p2.z, n.x, n.y, n.z, 0, 0,
        p3.x, p3.y, p3.z, n.x, n.y, n.z, 1, 1,
        p4.x, p4.y, p4.z, n.x, n.y, n.z, 1, 0
    };

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };

    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), 4, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }

    mesh->_primitiveType = TRIANGLE_STRIP;
    mesh->setVertexData(vertices, 0, 4);

    return mesh;
}

Mesh* Mesh::createLines(Vector3* points, unsigned int pointCount)
{
    GP_ASSERT(points);
    GP_ASSERT(pointCount);

    float* vertices = new float[pointCount*3];
    memcpy(vertices, points, pointCount*3*sizeof(float));

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 1), pointCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        SAFE_DELETE_ARRAY(vertices);
        return NULL;
    }

    mesh->_primitiveType = LINE_STRIP;
    mesh->setVertexData(vertices, 0, pointCount);

    SAFE_DELETE_ARRAY(vertices);
    return mesh;
}

Mesh* Mesh::createBoundingBox(const BoundingBox& box)
{
    Vector3 corners[8];
    box.getCorners(corners);

    float vertices[] =
    {
        corners[7].x, corners[7].y, corners[7].z,
        corners[6].x, corners[6].y, corners[6].z,
        corners[1].x, corners[1].y, corners[1].z,
        corners[0].x, corners[0].y, corners[0].z,
        corners[7].x, corners[7].y, corners[7].z,
        corners[4].x, corners[4].y, corners[4].z,
        corners[3].x, corners[3].y, corners[3].z, 
        corners[0].x, corners[0].y, corners[0].z,
        corners[0].x, corners[0].y, corners[0].z,
        corners[1].x, corners[1].y, corners[1].z,
        corners[2].x, corners[2].y, corners[2].z,
        corners[3].x, corners[3].y, corners[3].z, 
        corners[4].x, corners[4].y, corners[4].z,
        corners[5].x, corners[5].y, corners[5].z, 
        corners[2].x, corners[2].y, corners[2].z,
        corners[1].x, corners[1].y, corners[1].z,
        corners[6].x, corners[6].y, corners[6].z,
        corners[5].x, corners[5].y, corners[5].z
    };

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 1), 18, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }

    mesh->_primitiveType = LINE_STRIP;
    mesh->setVertexData(vertices, 0, 18);

    return mesh;
}

const char* Mesh::getUrl() const
{
    return _url.c_str();
}

const VertexFormat& Mesh::getVertexFormat() const
{
    return _vertexFormat;
}

unsigned int Mesh::getVertexCount() const
{
    return _vertexCount;
}

unsigned int Mesh::getVertexSize() const
{
    return _vertexFormat.getVertexSize();
}

VertexBufferHandle Mesh::getVertexBuffer() const
{
    return _vertexBuffer;
}

bool Mesh::isDynamic() const
{
    return _dynamic;
}

Mesh::PrimitiveType Mesh::getPrimitiveType() const
{
    return _primitiveType;
}

void Mesh::setPrimitiveType(PrimitiveType type)
{
    _primitiveType = type;
}
/*
void* Mesh::mapVertexBuffer()
{
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );

    return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

bool Mesh::unmapVertexBuffer()
{
    return glUnmapBuffer(GL_ARRAY_BUFFER);
}
*/
void Mesh::setVertexData(const void* vertexData, unsigned int vertexStart, unsigned int vertexCount, bool copy)
{
    //if (this->_vertexData) free(this->_vertexData);
    //this->_vertexData = (void*)vertexData;
    //Renderer::cur()->updateMesh(this, vertexStart, vertexCount);

    if (vertexCount == 0) vertexCount = _vertexCount;

    GP_ASSERT(_vertexCount >= vertexStart + vertexCount);
    if (copy) {
        int bufSize = _vertexCount * getVertexSize();
        if (!this->_vertexData) {
            this->_vertexData = malloc(bufSize);
        }
        char* dst = ((char*)this->_vertexData) + (vertexStart * getVertexSize());
        memcpy(dst, vertexData, vertexCount * getVertexSize());
        Renderer::cur()->updateMesh(this, 0, vertexCount);
    }
    else {
        free(this->_vertexData);
        this->_vertexData = (void*)vertexData;
        Renderer::cur()->updateMesh(this, 0, _vertexCount);
    }
}

MeshPart* Mesh::addPart(PrimitiveType primitiveType, IndexFormat indexFormat, unsigned int indexCount, bool dynamic)
{
    MeshPart* part = MeshPart::create(this, _parts.size(), primitiveType, indexFormat, indexCount, dynamic);
    if (part)
    {
        _parts.push_back(part);
    }

    return part;
}

unsigned int Mesh::getPartCount() const
{
    return _parts.size();
}

MeshPart* Mesh::getPart(unsigned int index)
{
    GP_ASSERT(index < _parts.size());
    return _parts[index];
}

const BoundingBox& Mesh::getBoundingBox() const
{
    return _boundingBox;
}

void Mesh::setBoundingBox(const BoundingBox& box)
{
    _boundingBox = box;
}

const BoundingSphere& Mesh::getBoundingSphere() const
{
    return _boundingSphere;
}

void Mesh::setBoundingSphere(const BoundingSphere& sphere)
{
    _boundingSphere = sphere;
}

void Mesh::write(Stream* file) {
    
    // vertex formats
    file->writeUInt8((unsigned int)_vertexFormat.getElementCount());
    for (int i=0; i<_vertexFormat.getElementCount(); ++i)
    {
        const VertexFormat::Element& element = _vertexFormat.getElement(i);
        file->writeUInt8(element.usage);
        file->writeUInt8(element.size);
        file->writeStr(element.name);
    }

    //file->writeStr(_url);
    file->writeUInt8(_primitiveType);
    file->writeUInt8(_dynamic);

    // vertices
    file->writeUInt32(_vertexCount);
    file->write((const char*)_vertexData, _vertexFormat.getVertexSize() * _vertexCount);


    // Write bounds
    file->writeFloat(_boundingBox.min.x);
    file->writeFloat(_boundingBox.min.y);
    file->writeFloat(_boundingBox.min.z);

    file->writeFloat(_boundingBox.max.x);
    file->writeFloat(_boundingBox.max.y);
    file->writeFloat(_boundingBox.max.z);

    file->writeFloat(_boundingSphere.center.x);
    file->writeFloat(_boundingSphere.center.y);
    file->writeFloat(_boundingSphere.center.z);
    file->writeFloat(_boundingSphere.radius);

    // parts
    file->writeInt16(_parts.size());
    for (int i=0; i<_parts.size(); ++i) {
        MeshPart *p = _parts[i];
        p->write(file);
    }
}

Mesh* Mesh::read(Stream* file) {
    int elemCount = file->readUInt8();
    std::vector<VertexFormat::Element> elems;
    elems.resize(elemCount);
    for (int i = 0; i < elemCount; ++i) {
        elems[i].usage = (VertexFormat::Usage)file->readUInt8();
        elems[i].size = file->readUInt8();
        elems[i].name = file->readStr();
    }

    VertexFormat format(&elems[0], elemCount);

    Mesh* mesh = new Mesh(format);
    //mesh->_url = file->readStr();
    mesh->_primitiveType = (Mesh::PrimitiveType)file->readUInt8();
    mesh->_dynamic = file->readUInt8();

    mesh->_vertexCount = file->readUInt32();
    int bufSize = mesh->_vertexFormat.getVertexSize() * mesh->_vertexCount;
    void *vertexData = malloc(bufSize);
    file->read((char*)vertexData, bufSize);
    mesh->setVertexData(vertexData, 0, mesh->_vertexCount, false);
    //mesh->_vertexDataDirty = true;

    mesh->_boundingBox.min.x = file->readFloat();
    mesh->_boundingBox.min.y = file->readFloat();
    mesh->_boundingBox.min.z = file->readFloat();
    mesh->_boundingBox.max.x = file->readFloat();
    mesh->_boundingBox.max.y = file->readFloat();
    mesh->_boundingBox.max.z = file->readFloat();

    mesh->_boundingSphere.center.x = file->readFloat();
    mesh->_boundingSphere.center.y = file->readFloat();
    mesh->_boundingSphere.center.z = file->readFloat();
    mesh->_boundingSphere.radius = file->readFloat();

    int _partCount = file->readInt16();
    mesh->_parts.reserve(_partCount);
    for (int i=0; i<_partCount; ++i) {
        MeshPart *p = MeshPart::read(file);
        mesh->_parts.push_back(p);
    }
    return mesh;
}


}
