#include "Base.h"
#include "SerializerBinary.h"
#include "Activator.h"
#include "Serializer.h"
#include "Stream.h"
#include "FileSystem.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix.h"

namespace gameplay
{

unsigned char SerializerBinary::BIT_NULL = 0x00;
unsigned char SerializerBinary::BIT_VALUE = 0x01;
unsigned char SerializerBinary::BIT_XREF = 0x02;
unsigned char SerializerBinary::BIT_DEFAULT = 0x04;

SerializerBinary::SerializerBinary(Type type,
                                   const std::string& path,
                                   Stream* stream,
                                   uint32_t versionMajor,
                                   uint32_t versionMinor) : 
    Serializer(type, path, stream, versionMajor, versionMinor)
{
}
    
SerializerBinary::~SerializerBinary()
{
}
    
Serializer* SerializerBinary::create(const std::string& path, Stream* stream)
{
    // Read the binary file header info.
    const char magicNumber[9] = GP_ENGINE_MAGIC_NUMBER;
    char header[9];
    if (stream->read(header, sizeof(char), 9) != 9 || memcmp(header, magicNumber, 9) != 0)
        return nullptr;

    // Read the file version.
    unsigned char version[2];
    if (stream->read(version, sizeof(unsigned char), 2) != 2)
    {
        GP_WARN("Failed to read version from binary file: %s", path.c_str());
        return nullptr;
    }
    Serializer* serializer = new SerializerBinary(Type::eReader, path, stream, version[0], version[1]);
    return serializer;
}

Serializer* SerializerBinary::createWriter(const std::string& path)
{
    Stream* stream = FileSystem::open(path.c_str(), FileSystem::StreamMode::WRITE);
    if (stream == nullptr)
        return nullptr;

    const char magicNumber[9] = GP_ENGINE_MAGIC_NUMBER;
    const unsigned char version[2] = { GP_ENGINE_VERSION_MAJOR, GP_ENGINE_VERSION_MINOR};
    // Write out the file identifier and version
    if (stream->write(magicNumber, sizeof(char), 9) != 9)
        GP_WARN("Unable to write binary file identifier.");
    if (stream->write(version, sizeof(unsigned char), 2) != 2)
        GP_WARN("Unable to write binary file version.");

    Serializer* serializer = new SerializerBinary(Type::eWriter, path, stream, version[0], version[1]);

    return serializer;
}
    
void SerializerBinary::close()
{
    if (_stream)
        _stream->close();
}
    
Serializer::Format SerializerBinary::getFormat() const
{
    return Format::eBinary;
}

void SerializerBinary::writeEnum(const char* propertyName, const char* enumName, int value, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(enumName);
    
    writeInt(propertyName, value, defaultValue);
}
    
void SerializerBinary::writeBool(const char* propertyName, bool value, bool defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write(&value, sizeof(bool), 1);
}

void SerializerBinary::writeInt(const char* propertyName, int value, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        _stream->write(&value, sizeof(int), 1);
    }
}

void SerializerBinary::writeFloat(const char* propertyName, float value, float defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        _stream->write(&value, sizeof(float), 1);
    }
}

void SerializerBinary::writeVector(const char* propertyName, const Vector2& value, const Vector2& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        _stream->write(&value.x, sizeof(float), 1);
        _stream->write(&value.y, sizeof(float), 1);
    }
}

void SerializerBinary::writeVector(const char* propertyName, const Vector3& value, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        _stream->write(&value.x, sizeof(float), 1);
        _stream->write(&value.y, sizeof(float), 1);
        _stream->write(&value.z, sizeof(float), 1);
    }
}

void SerializerBinary::writeVector(const char* propertyName, const Vector4& value, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        _stream->write(&value.x, sizeof(float), 1);
        _stream->write(&value.y, sizeof(float), 1);
        _stream->write(&value.z, sizeof(float), 1);
        _stream->write(&value.w, sizeof(float), 1);
    }
}

void SerializerBinary::writeColor(const char* propertyName, const Vector3& value, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        uint32_t color = value.toColor();
        _stream->write(&color, sizeof(uint32_t), 1);
    }
}

void SerializerBinary::writeColor(const char* propertyName, const Vector4& value, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        uint32_t color = value.toColor();
        _stream->write(&color, sizeof(uint32_t), 1);
    }
}

void SerializerBinary::writeMatrix(const char* propertyName, const Matrix& value, const Matrix& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        _stream->write(value.m, sizeof(float), 16);
    }
}

void SerializerBinary::writeString(const char* propertyName, const char* value, const char* defaultValue)
{
    //GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);

    if ((value == defaultValue) || (value && defaultValue && strcmp(value, defaultValue) == 0))
    {
        _stream->write(&BIT_DEFAULT, sizeof(unsigned char), 1);
    }
    else
    {
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
        writeLengthPrefixedString(value);
    }
}

void SerializerBinary::writeMap(const char* propertyName, std::vector<std::string> &keys)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);

    _stream->write(&keys, sizeof(size_t), 1);
    for (int i=0; i<keys.size(); ++i) {
        writeLengthPrefixedString(keys[i].c_str());
    }
}
    
void SerializerBinary::writeObject(const char* propertyName, Serializable* value)
{
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == nullptr)
    {
        _stream->write(&BIT_NULL, sizeof(unsigned char), 1);
        return;
    }
    
    bool writeValue = true;
    if (value && dynamic_cast<Ref*>(value)->getRefCount() > 1)
    {
        _stream->write(&BIT_XREF, sizeof(unsigned char), 1);
        unsigned long xrefAddress = reinterpret_cast<unsigned long>(value);
        _stream->write(&xrefAddress, sizeof(unsigned long), 1);
        
        // Check if already serialized from xref table
        std::map<unsigned long, Serializable*>::const_iterator itr = _xrefs.find(xrefAddress);
        if (itr == _xrefs.end())
        {
            writeValue = true;
            _xrefs[xrefAddress] = value;
        }
        else
        {
            writeValue = false;
        }
    }
    else
    {
        // Write out this is a object value
        _stream->write(&BIT_VALUE, sizeof(unsigned char), 1);
    }
    
    if (writeValue)
    {
        // Write out the objects class
        writeLengthPrefixedString(value->getClassName().c_str());
        
        // Serialize the object properties
        value->onSerialize(this);
    }
}

void SerializerBinary::writeList(const char* propertyName, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write(&count, sizeof(size_t), 1);
}

void SerializerBinary::finishColloction() {

}

void SerializerBinary::writeIntArray(const char* propertyName, const int* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write(&count, sizeof(size_t), 1);
    if (count > 0 && data )
    {
        _stream->write(data, sizeof(int), count);
    }
}

void SerializerBinary::writeFloatArray(const char* propertyName, const float* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write(&count, sizeof(size_t), 1);
    if (count > 0 && data )
    {
        _stream->write(data, sizeof(float), count);
    }
}

void SerializerBinary::writeByteArray(const char* propertyName, const unsigned char* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write(&count, sizeof(size_t), 1);
    if (count > 0 && data )
    {
        _stream->write(data, sizeof(unsigned char), count);
    }
}

int SerializerBinary::readEnum(const char* propertyName, const char* enumName, int defaultValue)
{
    GP_ASSERT(enumName);
    
    return readInt(propertyName, defaultValue);
}

bool SerializerBinary::readBool(const char* propertyName, bool defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    bool value;
    _stream->read(&value, sizeof(bool), 1);
    return value;
}

int SerializerBinary::readInt(const char* propertyName, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        return defaultValue;
    }
    else
    {
        int value;
        _stream->read(&value, sizeof(int), 1);
        return value;
    }
}

float SerializerBinary::readFloat(const char* propertyName, float defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        return defaultValue;
    }
    else
    {
        float value;
        _stream->read(&value, sizeof(float), 1);
        return value;
    }
}
    
Vector2 SerializerBinary::readVector(const char* propertyName, const Vector2& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        return defaultValue;
    }
    else
    {
        Vector2 value;
        _stream->read(&value.x, sizeof(float), 1);
        _stream->read(&value.y, sizeof(float), 1);
        return value;
    }
}

Vector3 SerializerBinary::readVector(const char* propertyName, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        return defaultValue;
    }
    else
    {
        Vector3 value;
        _stream->read(&value.x, sizeof(float), 1);
        _stream->read(&value.y, sizeof(float), 1);
        _stream->read(&value.z, sizeof(float), 1);
        return value;
    }
}

Vector4 SerializerBinary::readVector(const char* propertyName, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        return defaultValue;
    }
    else
    {
        Vector4 value;
        _stream->read(&value.x, sizeof(float), 1);
        _stream->read(&value.y, sizeof(float), 1);
        _stream->read(&value.z, sizeof(float), 1);
        _stream->read(&value.w, sizeof(float), 1);
        return value;
    }
}

Vector3 SerializerBinary::readColor(const char* propertyName, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        return defaultValue;
    }
    else
    {
        uint32_t color;
        _stream->read(&color, sizeof(uint32_t), 1);
        return Vector3::fromColor(color);
    }
}
    
Vector4 SerializerBinary::readColor(const char* propertyName, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        return defaultValue;
    }
    else
    {
        uint32_t color;
        _stream->read(&color, sizeof(uint32_t), 1);
        return Vector4::fromColor(color);
    }
}

Matrix SerializerBinary::readMatrix(const char* propertyName, const Matrix& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        return defaultValue;
    }
    else
    {
        Matrix value;
        _stream->read(value.m, sizeof(float), 16);
        return value;
    }
}

void SerializerBinary::readString(const char* propertyName, std::string& value,  const char* defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    if (bit == BIT_DEFAULT)
    {
        std::strcpy(&value[0], defaultValue);
    }
    else
    {
        readLengthPrefixedString(value);
    }
}

void SerializerBinary::readMap(const char* propertyName, std::vector<std::string> &keys)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    size_t count;
    _stream->read(&count, sizeof(size_t), 1);

    for (int i=0; i<count; ++i) {
        std::string str;
        readLengthPrefixedString(str);
        keys.push_back(str);
    }

}

Serializable* SerializerBinary::readObject(const char* propertyName)
{
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read(&bit, sizeof(unsigned char), 1);
    
    unsigned long xrefAddress = 0L;
    if (bit == BIT_NULL)
    {
        return nullptr;
    }
    else if (bit == BIT_XREF)
    {
        _stream->read(&xrefAddress, sizeof(unsigned long), 1);
        std::map<unsigned long, Serializable*>::const_iterator itr = _xrefs.find(xrefAddress);
        if (itr != _xrefs.end())
        {
            return itr->second;
        }
    }
    
    // The class name for the object being read
    std::string className;
    readLengthPrefixedString(className);

    Serializable* value = (Activator::getActivator()->createObject(className));
    if (value == nullptr)
    {
        GP_ERROR("Failed to deserialize binary class: %s for propertyName:%s", className.c_str(), propertyName);
        return value;
    }
    
    // Deserialize the properties
    value->onDeserialize(this);
    
    if (xrefAddress != 0)
    {
        _xrefs[xrefAddress] = value;
    }
    
    return value;
}

size_t SerializerBinary::readList(const char* propertyName)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    size_t count;
    _stream->read(&count, sizeof(size_t), 1);
    return count;
}

size_t SerializerBinary::readIntArray(const char* propertyName, int** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    size_t count = 0;
    _stream->read(&count, sizeof(size_t), 1);
    int* buffer = nullptr;
    if (count > 0)
    {
        if (*data == nullptr)
        {
            buffer = new int[count];
        }
        else
        {
            buffer = *data;
        }
        _stream->read(buffer, sizeof(int), count);
    }
    *data = buffer;
    
    return count;
}

size_t SerializerBinary::readFloatArray(const char* propertyName, float** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    size_t count = 0;
    _stream->read(&count, sizeof(size_t), 1);
    float* buffer = nullptr;
    if (count > 0)
    {
        if (*data == nullptr)
        {
            buffer = new float[count];
        }
        else
        {
            buffer = *data;
        }
        _stream->read(buffer, sizeof(float), count);
    }
    *data = buffer;
    
    return count;
}

size_t SerializerBinary::readByteArray(const char* propertyName, unsigned char** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    size_t count = 0;
    _stream->read(&count, sizeof(size_t), 1);
    unsigned char* buffer = nullptr;
    if (count > 0)
    {
        if (*data == nullptr)
        {
            buffer = new unsigned char[count];
        }
        else
        {
            buffer = *data;
        }
        _stream->read(buffer, sizeof(unsigned char), count);
    }
    *data = buffer;
    
    return count;
}

void SerializerBinary::writeLengthPrefixedString(const char* str)
{
    size_t length = strlen(str);
    _stream->write(&length, sizeof(size_t), 1);
    if (length > 0)
    {
        _stream->write(str, sizeof(char), length);
    }
}
    
void SerializerBinary::readLengthPrefixedString(std::string& str)
{
    size_t length;
    _stream->read(&length, sizeof(size_t), 1);
    if (length > 0)
    {
        str.resize(length);
        _stream->read(&str[0], sizeof(char), length);
    }
    else
    {
        str.clear();
    }
}

}
