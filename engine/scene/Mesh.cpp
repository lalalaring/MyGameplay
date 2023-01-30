#include "base/Base.h"
#include "Mesh.h"
#include "MeshPart.h"
#include "material/ShaderProgram.h"
#include "Model.h"
#include "material/Material.h"
#include "scene/Renderer.h"

namespace gameplay
{

Mesh::Mesh(const VertexFormat& vertexFormat) 
    : _vertexFormat(vertexFormat), _vertexCount(0), _vertexBuffer(0), _primitiveType(TRIANGLES), 
      _dynamic(false), _vertexData(0), _vertexDataDirty(false), _vertexAttributeArray(NULL)
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

Mesh* Mesh::createPlane() {
    float vertices[] = {
        -1, 0, -1, 0, 1, 0,
        -1, 0, 1, 0, 1, 0,
        1, 0, -1, 0, 1, 0,
        1, 0, 1, 0, 1, 0,
    };

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
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

Mesh* Mesh::createQuad3D(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
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

Mesh* Mesh::createCube(float size)
{
    float a = size * 0.5f;
    float vertices[] =
    {
        -a, -a,  a,    0.0,  0.0,  1.0,   0.0, 0.0,
         a, -a,  a,    0.0,  0.0,  1.0,   1.0, 0.0,
        -a,  a,  a,    0.0,  0.0,  1.0,   0.0, 1.0,
         a,  a,  a,    0.0,  0.0,  1.0,   1.0, 1.0,
        -a,  a,  a,    0.0,  1.0,  0.0,   0.0, 0.0,
         a,  a,  a,    0.0,  1.0,  0.0,   1.0, 0.0,
        -a,  a, -a,    0.0,  1.0,  0.0,   0.0, 1.0,
         a,  a, -a,    0.0,  1.0,  0.0,   1.0, 1.0,
        -a,  a, -a,    0.0,  0.0, -1.0,   0.0, 0.0,
         a,  a, -a,    0.0,  0.0, -1.0,   1.0, 0.0,
        -a, -a, -a,    0.0,  0.0, -1.0,   0.0, 1.0,
         a, -a, -a,    0.0,  0.0, -1.0,   1.0, 1.0,
        -a, -a, -a,    0.0, -1.0,  0.0,   0.0, 0.0,
         a, -a, -a,    0.0, -1.0,  0.0,   1.0, 0.0,
        -a, -a,  a,    0.0, -1.0,  0.0,   0.0, 1.0,
         a, -a,  a,    0.0, -1.0,  0.0,   1.0, 1.0,
         a, -a,  a,    1.0,  0.0,  0.0,   0.0, 0.0,
         a, -a, -a,    1.0,  0.0,  0.0,   1.0, 0.0,
         a,  a,  a,    1.0,  0.0,  0.0,   0.0, 1.0,
         a,  a, -a,    1.0,  0.0,  0.0,   1.0, 1.0,
        -a, -a, -a,   -1.0,  0.0,  0.0,   0.0, 0.0,
        -a, -a,  a,   -1.0,  0.0,  0.0,   1.0, 0.0,
        -a,  a, -a,   -1.0,  0.0,  0.0,   0.0, 1.0,
        -a,  a,  a,   -1.0,  0.0,  0.0,   1.0, 1.0
    };
    short indices[] =
    {
        0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 18, 17, 19, 20, 21, 22, 22, 21, 23
    };
    unsigned int vertexCount = 24;
    unsigned int indexCount = 36;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setVertexData(vertices, 0, vertexCount);
    MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, false);
    meshPart->setIndexData(indices, 0, indexCount);
    return mesh;
}

Mesh* Mesh::createSimpleCube()
{
    float vertices[] =
    {
        -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0,
      1.0, -1.0, -1.0, 1.0, -1.0,

      -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0,
      1.0, 1.0, -1.0, -1.0, 1.0,

      1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      -1.0, 1.0, -1.0, -1.0,

      -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,
      1.0, -1.0, -1.0, 1.0,

      -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
      1.0, -1.0, 1.0, -1.0,

      -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0,
      -1.0, 1.0, 1.0, -1.0, 1.0,
    };

    unsigned int vertexCount = 36;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 1), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setVertexData(vertices, 0, vertexCount);
    mesh->_primitiveType = TRIANGLES;
    return mesh;
}

Mesh* Mesh::createSpherical(int subdivision) {
    int width_segments = subdivision;
    int height_segments = subdivision;

    float phi_start = 0, phi_length = MATH_PI * 2, theta_start = 0, theta_length = MATH_PI;
    float theta_end = min(theta_start + theta_length, MATH_PI);
    float radius = 1.0;

    float *_vertices = new float[((width_segments + 1) * (height_segments + 1) * 6)];
    int i = 0;
    std::vector<uint16_t> indices;

    int index = 0;
    std::vector<std::vector<int>> grid;

    // generate vertices, normals and uvs
    for (int iy = 0; iy <= height_segments; iy++) {
        std::vector<int> vertices_row;
        float v = iy / (double)height_segments;
        // special case for the poles
        int uOffset = 0;
        if (iy == 0 && theta_start == 0) {
            uOffset = 0.5 / width_segments;
        }
        else if (iy == height_segments && theta_end == MATH_PI) {
            uOffset = -0.5 / width_segments;
        }

        for (int ix = 0; ix <= width_segments; ix++) {
            float u = ix / (double)width_segments;
            // vertex
            float x = -radius * cos(phi_start + u * phi_length) * sin(theta_start + v * theta_length);
            float y = radius * cos(theta_start + v * theta_length);
            float z = radius * sin(phi_start + u * phi_length) * sin(theta_start + v * theta_length);

            _vertices[i++] = x;
            _vertices[i++] = y;
            _vertices[i++] = z;

            // normal
            _vertices[i++] = x;
            _vertices[i++] = y;
            _vertices[i++] = z;

            // uv
            //uvs.push( u + uOffset, 1 - v );
            vertices_row.push_back(index++);
        }

        grid.push_back(vertices_row);
    }

    // indices
    for (int iy = 0; iy < height_segments; iy++) {
        for (int ix = 0; ix < width_segments; ix++) {
            int a = grid[iy][ix + 1];
            int b = grid[iy][ix];
            int c = grid[iy + 1][ix];
            int d = grid[iy + 1][ix + 1];
            if (iy != 0 || theta_start > 0) {
                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(d);
            }
            if (iy != height_segments - 1 || theta_end < MATH_PI) {
                indices.push_back(b);
                indices.push_back(c);
                indices.push_back(d);
            }
        }
    }

    unsigned int vertexCount = (width_segments + 1) * (height_segments + 1);
    unsigned int indexCount = indices.size();
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        //VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setVertexData(_vertices, 0, vertexCount);
    MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, false);
    meshPart->setIndexData(indices.data(), 0, indexCount);

    SAFE_DELETE_ARRAY(_vertices);
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
    if (_boundingSphere.radius == 0) {
        ((Mesh*)this)->computeBounds();
    }
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


void Mesh::computeBounds()
{
    // If we have a Model with a MeshSkin associated with it,
    // compute the bounds from the skin - otherwise compute
    // it from the local mesh data.
    /*if (model && model->getSkin())
    {
        model->getSkin()->computeBounds();
        return;
    }*/

    //LOG(2, "Computing bounds for mesh: %s\n", getId().c_str());

    _boundingBox.min.x = _boundingBox.min.y = _boundingBox.min.z = FLT_MAX;
    _boundingBox.max.x = _boundingBox.max.y = _boundingBox.max.z = -FLT_MAX;
    _boundingSphere.center.x = _boundingSphere.center.y = _boundingSphere.center.z = 0.0f;
    _boundingSphere.radius = 0.0f;


    int positionOffset = 0;
    for (int i = 0; i < _vertexFormat.getElementCount(); ++i) {
        if (_vertexFormat.getElement(i).usage == VertexFormat::POSITION) {
            break;
        }
        positionOffset += _vertexFormat.getElement(i).size*4;
    }

    for (int i = 0; i < _vertexCount; ++i)
    {
        float *p = (float*)((char*)_vertexData + (i * getVertexSize()) + positionOffset);
        float x = p[0];
        float y = p[1];
        float z = p[2];

        // Update min/max for this vertex
        if (x < _boundingBox.min.x)
            _boundingBox.min.x = x;
        if (y < _boundingBox.min.y)
            _boundingBox.min.y = y;
        if (z < _boundingBox.min.z)
            _boundingBox.min.z = z;
        if (x > _boundingBox.max.x)
            _boundingBox.max.x = x;
        if (y > _boundingBox.max.y)
            _boundingBox.max.y = y;
        if (z > _boundingBox.max.z)
            _boundingBox.max.z = z;
    }

    // Compute center point
    _boundingSphere.center = _boundingBox.getCenter();

    // Compute radius by looping through all points again and finding the max
    // distance between the center point and each vertex position
    for (int i = 0; i < _vertexCount; ++i)
    {
        float* p = (float*)((char*)_vertexData + (i * getVertexSize()) + positionOffset);
        float x = p[0];
        float y = p[1];
        float z = p[2];
    
        float d = _boundingSphere.center.distanceSquared(Vector3(x, y, z));
        if (d > _boundingSphere.radius)
        {
            _boundingSphere.radius = d;
        }
    }

    // Convert squared distance to distance for radius
    _boundingSphere.radius = sqrt(_boundingSphere.radius);
}
}
