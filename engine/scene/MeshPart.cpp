#include "base/Base.h"
#include "MeshPart.h"
#include "scene/Renderer.h"

namespace gameplay
{

MeshPart::MeshPart() :
    _mesh(NULL), _meshIndex(0), _primitiveType(Mesh::TRIANGLES), _indexCount(0), _indexBuffer(0), _dynamic(false), _indexDataDirty(false), _indexData(NULL)
{
}

MeshPart::~MeshPart()
{
/*    if (_indexBuffer)
    {
        glDeleteBuffers(1, &_indexBuffer);
    }
    */
    if (_indexData) {
        free(_indexData);
        _indexData = NULL;
    }
}

MeshPart* MeshPart::create(Mesh* mesh, unsigned int meshIndex, Mesh::PrimitiveType primitiveType,
    Mesh::IndexFormat indexFormat, unsigned int indexCount, bool dynamic)
{
    MeshPart* part = new MeshPart();
    part->_mesh = mesh;
    part->_meshIndex = meshIndex;
    part->_primitiveType = primitiveType;
    part->_indexFormat = indexFormat;
    part->_indexCount = indexCount;
    part->_indexBuffer = 0;
    part->_dynamic = dynamic;

    return part;
}

unsigned int MeshPart::getMeshIndex() const
{
    return _meshIndex;
}

Mesh::PrimitiveType MeshPart::getPrimitiveType() const
{
    return _primitiveType;
}

unsigned int MeshPart::getIndexCount() const
{
    return _indexCount;
}

Mesh::IndexFormat MeshPart::getIndexFormat() const
{
    return _indexFormat;
}

IndexBufferHandle MeshPart::getIndexBuffer() const
{
    return _indexBuffer;
}
unsigned int MeshPart::getIndexSize() const {
    unsigned int indexSize = 0;
    switch (_indexFormat)
    {
    case Mesh::INDEX8:
        indexSize = 1;
        break;
    case Mesh::INDEX16:
        indexSize = 2;
        break;
    case Mesh::INDEX32:
        indexSize = 4;
        break;
    default:
        GP_ERROR("Unsupported index format (%d).", this->getIndexFormat());
        return 0;
    }
    return indexSize;
}
/*
void* MeshPart::mapIndexBuffer()
{
    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer) );

    return (void*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
}

bool MeshPart::unmapIndexBuffer()
{
    return glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}
*/
void MeshPart::setIndexData(const void* indexData, unsigned int indexStart, unsigned int indexCount, bool copy)
{

    GP_ASSERT(_indexCount >= indexStart + indexCount);
    if (copy) {
        if (!_indexData) _indexData = malloc(getIndexSize() * indexCount);
        memcpy((char*)_indexData + (indexStart * getIndexSize()), indexData, getIndexSize() * indexCount);
        Renderer::cur()->updateMeshPart(this, 0, _indexCount);
    }
    else {
        if (_indexData) free(_indexData);
        _indexData = (void*)indexData;
        Renderer::cur()->updateMeshPart(this, 0, _indexCount);
    }
}

bool MeshPart::isDynamic() const
{
    return _dynamic;
}

void MeshPart::write(Stream* file) {
    file->writeUInt8(_primitiveType);
    file->writeUInt16(_indexFormat);
    file->writeUInt8(_dynamic);


    unsigned int indexSize = getIndexSize();

    file->writeUInt32(_indexCount);
    file->write((const char*)_indexData, indexSize* _indexCount);
}

MeshPart* MeshPart::read(Stream* file) {
    MeshPart* p = new MeshPart();
    p->_primitiveType = (Mesh::PrimitiveType)file->readUInt8();
    p->_indexFormat = (Mesh::IndexFormat)file->readUInt16();
    p->_dynamic = file->readUInt8();

    unsigned int indexSize = p->getIndexSize();
    p->_indexCount = file->readUInt32();
    int dataSize = p->_indexCount * indexSize;
    void *indexData = (void*)malloc(dataSize);
    file->read((char*)indexData, dataSize);

    p->setIndexData(indexData, 0, p->_indexCount, false);
    //p->_indexDataDirty = true;
    return p;
}

}
