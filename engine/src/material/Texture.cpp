#include "base/Base.h"
#include "render/Renderer.h"
#include "material/Image.h"
#include "Texture.h"
#include "base/FileSystem.h"
#include "render/CompressedTexture.h"

namespace gameplay
{

static std::vector<Texture*> __textureCache;

Texture::Texture() : _handle(0), _format(UNKNOWN), _type((Texture::Type)0), _width(0), _height(0), _mipmapped(false), _cached(false), _compressed(false),
    _wrapS(Texture::REPEAT), _wrapT(Texture::REPEAT), _wrapR(Texture::REPEAT), _minFilter(Texture::NEAREST), _magFilter(Texture::LINEAR), _data(NULL), _generateMipmaps(false)
{
}

Texture::~Texture()
{
    Renderer::cur()->deleteTexture(this);

    // Remove ourself from the texture cache.
    if (_cached)
    {
        std::vector<Texture*>::iterator itr = std::find(__textureCache.begin(), __textureCache.end(), this);
        if (itr != __textureCache.end())
        {
            __textureCache.erase(itr);
        }
    }
}

bool Texture::load(const char* path) {
    Image* image = Image::create(path);
    if (!image) {
        return false;
    }
    switch (image->getFormat())
    {
    case Image::RGB:
        _format = Texture::RGB;
        break;
    case Image::RGBA:
        _format = Texture::RGBA;
        break;
    default:
        GP_ERROR("Unsupported image format (%d).", image->getFormat());
        return false;
    }
    _width = image->getWidth();
    _height = image->getHeight();
    _data = image->getData();
    
    return true;
}

Texture* Texture::create(const char* path, bool generateMipmaps)
{
    GP_ASSERT( path );

    // Search texture cache first.
    for (size_t i = 0, count = __textureCache.size(); i < count; ++i)
    {
        Texture* t = __textureCache[i];
        GP_ASSERT( t );
        if (t->_path == path)
        {
            // If 'generateMipmaps' is true, call Texture::generateMipamps() to force the
            // texture to generate its mipmap chain if it hasn't already done so.
            /*
            if (generateMipmaps)
            {
                t->_mipmapped = true;
            }
            */

            // Found a match.
            t->addRef();

            return t;
        }
    }

    Texture* texture = NULL;

    // Filter loading based on file extension.
    const char* ext = strrchr(FileSystem::resolvePath(path), '.');
    if (ext)
    {
        switch (strlen(ext))
        {
        case 4:
            if ((tolower(ext[1]) == 'p' && tolower(ext[2]) == 'n' && tolower(ext[3]) == 'g') ||
                (tolower(ext[1]) == 'j' && tolower(ext[2]) == 'p' && tolower(ext[3]) == 'g')
                )
            {
                Image* image = Image::create(path);
                if (image)
                    texture = create(image, generateMipmaps);
                SAFE_RELEASE(image);
            }
            else if (tolower(ext[1]) == 'p' && tolower(ext[2]) == 'v' && tolower(ext[3]) == 'r')
            {
                // PowerVR Compressed Texture RGBA.
                texture = CompressedTexture::createCompressedPVRTC(path);
            }
            else if (tolower(ext[1]) == 'd' && tolower(ext[2]) == 'd' && tolower(ext[3]) == 's')
            {
                // DDS file format (DXT/S3TC) compressed textures
                texture = CompressedTexture::createCompressedDDS(path);
            }
            else if (tolower(ext[1]) == 'k' && tolower(ext[2]) == 't' && tolower(ext[3]) == 'x')
            {
                // KTX file format compressed textures
                texture = CompressedTexture::createCompressedDdsKtx(path);
            }
            break;
        }
    }

    if (texture)
    {
        texture->_path = path;
        texture->_cached = true;

        // Add to texture cache.
        __textureCache.push_back(texture);

        return texture;
    }

    GP_ERROR("Failed to load texture from file '%s'.", path);
    return NULL;
}

Texture* Texture::create(Image* image, bool generateMipmaps)
{
    GP_ASSERT( image );

    switch (image->getFormat())
    {
    case Image::RGB:
        return create(Texture::RGB, image->getWidth(), image->getHeight(), image->getData(), generateMipmaps);
    case Image::RGBA:
        return create(Texture::RGBA, image->getWidth(), image->getHeight(), image->getData(), generateMipmaps);
    default:
        GP_ERROR("Unsupported image format (%d).", image->getFormat());
        return NULL;
    }
}

Texture* Texture::create(Format format, unsigned int width, unsigned int height, const unsigned char* data, bool generateMipmaps, Texture::Type type)
{
    // Set initial minification filter based on whether or not mipmaping was enabled.
    Filter minFilter;
    if (format == Texture::DEPTH)
    {
    	minFilter = NEAREST;
    }
    else
    {
    	minFilter = generateMipmaps ? NEAREST_MIPMAP_LINEAR : LINEAR;
    }

    Texture* texture = new Texture();
    texture->_handle = 0;
    texture->_format = format;
    texture->_type = type;
    texture->_width = width;
    texture->_height = height;
    texture->_minFilter = minFilter;
    if (generateMipmaps)
        texture->_mipmapped = true;
    texture->_data = data;
    Renderer::cur()->updateTexture(texture);
    texture->_data = NULL;
    return texture;
}

size_t Texture::getFormatBPP(Texture::Format format)
{
    switch (format)
    {
    case Texture::RGB565:
    case Texture::RGBA4444:
    case Texture::RGBA5551:
        return 2;
    case Texture::RGB888:
        return 3;
    case Texture::RGBA8888:
        return 4;
    case Texture::ALPHA:
        return 1;
    default:
        return 0;
    }
}

Texture* Texture::loadCubeMap(const char* faces[]) {
    unsigned char* data = NULL;
    int width;
    int height;
    Format format;
    for (int i=0; i<6; ++i) {
        const char *url = faces[i];
        Image* image = Image::create(url, false);
        if (!image) return nullptr;

        int bpp = 4;
        switch (image->getFormat())
        {
        case Image::RGB:
            format = Texture::RGB;
            bpp = 3;
            break;
        case Image::RGBA:
            format = Texture::RGBA;
            bpp = 4;
            break;
        default:
            GP_ERROR("Unsupported image format (%d).", image->getFormat());
            return NULL;
        }

        width = image->getWidth();
        height = image->getHeight();
        int imageDataSize = image->getWidth() * image->getHeight() * bpp;
        if (!data) {
            data = (unsigned char*)malloc(imageDataSize * 6);
        }

        memcpy(data+(imageDataSize*i), image->getData(), imageDataSize);

        SAFE_RELEASE(image);
    }

    Texture *texture = new Texture();
    texture->_type = TEXTURE_CUBE;
    texture->_format = format;
    texture->_width = width;
    texture->_height = height;
    texture->_data = data;
    texture->_minFilter = NEAREST;
    texture->_wrapR = CLAMP;
    texture->_wrapS = CLAMP;
    texture->_wrapT = CLAMP;
    Renderer::cur()->updateTexture(texture);
    texture->_data = NULL;
    free(data);
    return texture;
}

void Texture::generateMipmaps() {
    _generateMipmaps = true;
}

void Texture::setData(const unsigned char* data)
{
    // Don't work with any compressed or cached textures
    GP_ASSERT( data );
    this->_data = data;
    Renderer::cur()->updateTexture(this);
    this->_data = NULL;
}

Texture::Format Texture::getFormat() const
{
    return _format;
}

Texture::Type Texture::getType() const
{
    return _type;
}

const char* Texture::getPath() const
{
    return _path.c_str();
}

unsigned int Texture::getWidth() const
{
    return _width;
}

unsigned int Texture::getHeight() const
{
    return _height;
}

TextureHandle Texture::getHandle() const
{
    return _handle;
}

bool Texture::isMipmapped() const
{
    return _mipmapped;
}

bool Texture::isCompressed() const
{
    return _compressed;
}

Serializable* Texture::createObject() {
    return new Texture();
}


std::string Texture::enumToString(const std::string& enumName, int value)
{
    if (enumName.compare("gameplay::Texture::Format") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Format::UNKNOWN) :
                return "UNKNOWN";
            //case static_cast<int>(Format::RGB) :
            //    return "RGB";
            case static_cast<int>(Format::RGB888) :
                return "RGB888";
            case static_cast<int>(Format::RGB565) :
                return "RGB565";
            //case static_cast<int>(Format::RGBA) :
            //    return "RGBA";
            case static_cast<int>(Format::RGBA8888) :
                return "RGBA8888";
            case static_cast<int>(Format::RGBA4444) :
                return "RGBA4444";
            case static_cast<int>(Format::RGBA5551) :
                return "RGBA5551";
            case static_cast<int>(Format::ALPHA) :
                return "ALPHA";
            case static_cast<int>(Format::DEPTH) :
                return "DEPTH";
            default:
                return "RGBA";
        }
    }
    else if (enumName.compare("gameplay::Texture::Filter") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Filter::NEAREST) :
                return "NEAREST";
            case static_cast<int>(Filter::LINEAR) :
                return "LINEAR";
            case static_cast<int>(Filter::NEAREST_MIPMAP_NEAREST) :
                return "NEAREST_MIPMAP_NEAREST";
            case static_cast<int>(Filter::LINEAR_MIPMAP_NEAREST) :
                return "LINEAR_MIPMAP_NEAREST";
            case static_cast<int>(Filter::NEAREST_MIPMAP_LINEAR) :
                return "NEAREST_MIPMAP_LINEAR";
            case static_cast<int>(Filter::LINEAR_MIPMAP_LINEAR) :
                return "LINEAR_MIPMAP_LINEAR";
            default:
                return "NEAREST";
        }
    }
    else if (enumName.compare("gameplay::Texture::Wrap") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Wrap::REPEAT) :
                return "REPEAT";
            case static_cast<int>(Wrap::CLAMP) :
                return "CLAMP";
            default:
                return "REPEAT";
        }
    }
    else if (enumName.compare("gameplay::Texture::Type") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Type::TEXTURE_2D) :
                return "TEXTURE_2D";
            case static_cast<int>(Type::TEXTURE_CUBE) :
                return "TEXTURE_CUBE";
            default:
                return "TEXTURE_2D";
        }
    }
    return "";
}

int Texture::enumParse(const std::string& enumName, const std::string& str)
{
    if (enumName.compare("gameplay::Texture::Format") == 0)
    {
        if (str.compare("UNKNOWN") == 0)
            return static_cast<int>(Format::UNKNOWN);
        else if (str.compare("RGB888") == 0)
            return static_cast<int>(Format::RGB888);
        else if (str.compare("RGB565") == 0)
            return static_cast<int>(Format::RGB565);
        else if (str.compare("RGB") == 0)
            return static_cast<int>(Format::RGB);
        else if (str.compare("RGBA") == 0)
            return static_cast<int>(Format::RGBA);
        else if (str.compare("RGB565") == 0)
            return static_cast<int>(Format::RGBA8888);
        else if (str.compare("RGBA8888") == 0)
            return static_cast<int>(Format::RGBA4444);
        else if (str.compare("RGBA4444") == 0)
            return static_cast<int>(Format::RGBA5551);
        else if (str.compare("RGBA5551") == 0)
            return static_cast<int>(Format::ALPHA);
        else if (str.compare("ALPHA") == 0)
            return static_cast<int>(Format::RGB565);
        else if (str.compare("DEPTH") == 0)
            return static_cast<int>(Format::DEPTH);
    }
    else if (enumName.compare("gameplay::Texture::Filter") == 0)
    {
        if (str.compare("NEAREST") == 0)
            return static_cast<int>(Filter::NEAREST);
        else if (str.compare("LINEAR") == 0)
            return static_cast<int>(Filter::LINEAR);
        else if (str.compare("NEAREST_MIPMAP_NEAREST") == 0)
            return static_cast<int>(Filter::NEAREST_MIPMAP_NEAREST);
        else if (str.compare("LINEAR_MIPMAP_NEAREST") == 0)
            return static_cast<int>(Filter::LINEAR_MIPMAP_NEAREST);
        else if (str.compare("NEAREST_MIPMAP_LINEAR") == 0)
            return static_cast<int>(Filter::NEAREST_MIPMAP_LINEAR);
        else if (str.compare("LINEAR_MIPMAP_LINEAR") == 0)
            return static_cast<int>(Filter::LINEAR_MIPMAP_LINEAR);
    }
    else if (enumName.compare("gameplay::Texture::Wrap") == 0)
    {
        if (str.compare("REPEAT") == 0)
            return static_cast<int>(Wrap::REPEAT);
        else if (str.compare("CLAMP") == 0)
            return static_cast<int>(Wrap::CLAMP);
    }
    else if (enumName.compare("gameplay::Texture::Type") == 0)
    {
        if (str.compare("TEXTURE_2D") == 0)
            return static_cast<int>(Type::TEXTURE_2D);
        else if (str.compare("TEXTURE_CUBE") == 0)
            return static_cast<int>(Type::TEXTURE_CUBE);
    }
    return static_cast<int>(0);
}

/**
 * @see Serializable::getClassName
 */
std::string Texture::getClassName() {
    return "gameplay::Texture";
}

/**
 * @see Serializable::onSerialize
 */
void Texture::onSerialize(Serializer* serializer) {
    serializer->writeString("path", getPath(), "");
    serializer->writeEnum("minFilter", "gameplay::Texture::Filter", static_cast<int>(_minFilter), -1);
    serializer->writeEnum("magFilter", "gameplay::Texture::Filter", static_cast<int>(_magFilter), -1);

    serializer->writeEnum("wrapS", "gameplay::Texture::Wrap", static_cast<int>(_wrapS), REPEAT);
    serializer->writeEnum("wrapT", "gameplay::Texture::Wrap", static_cast<int>(_wrapT), REPEAT);
    serializer->writeEnum("wrapR", "gameplay::Texture::Wrap", static_cast<int>(_wrapR), REPEAT);

    serializer->writeEnum("format", "gameplay::Texture::Format", static_cast<int>(_format), RGBA8888);
    serializer->writeEnum("type", "gameplay::Texture::Type", static_cast<int>(_type), TEXTURE_2D);
    serializer->writeBool("mipmap", _mipmapped, false);
}

/**
 * @see Serializable::onDeserialize
 */
void Texture::onDeserialize(Serializer* serializer) {
    std::string path;
    serializer->readString("path", path, "");

    this->load(path.c_str());
    _minFilter = static_cast<Texture::Filter>(serializer->readEnum("minFilter", "gameplay::Texture::Filter", -1));
    _magFilter = static_cast<Texture::Filter>(serializer->readEnum("magFilter", "gameplay::Texture::Filter", -1));

    _wrapS = static_cast<Texture::Wrap>(serializer->readEnum("wrapS", "gameplay::Texture::Wrap", REPEAT));
    _wrapT = static_cast<Texture::Wrap>(serializer->readEnum("wrapT", "gameplay::Texture::Wrap", REPEAT));
    _wrapR = static_cast<Texture::Wrap>(serializer->readEnum("wrapR", "gameplay::Texture::Wrap", REPEAT));

    _format = static_cast<Texture::Format>(serializer->readEnum("format", "gameplay::Texture::Format", RGBA8888));
    _type = static_cast<Texture::Type>(serializer->readEnum("type", "gameplay::Texture::Type", TEXTURE_2D));
    _mipmapped = serializer->readBool("mipmap", false);
}

void Texture::setWrapMode(Wrap wrapS, Wrap wrapT, Wrap wrapR)
{
    _wrapS = wrapS;
    _wrapT = wrapT;
    _wrapR = wrapR;
}

void Texture::setFilterMode(Filter minificationFilter, Filter magnificationFilter)
{
    _minFilter = minificationFilter;
    _magFilter = magnificationFilter;
}

void Texture::bind()
{
    if (!this->isMipmapped()) {
        if (_minFilter >= NEAREST_MIPMAP_NEAREST && _minFilter <= LINEAR_MIPMAP_LINEAR) {
            GP_ERROR("Unsupported minFilter (%d).", _minFilter);
        }
    }
    Renderer::cur()->bindTextureSampler(this);
}

}
