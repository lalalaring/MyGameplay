#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "base/Ref.h"
#include "base/Stream.h"
#include "base/Serializable.h"

namespace gameplay
{

/** Texture handle. */
typedef unsigned int TextureHandle;

class Image;

/**
 * Defines a standard texture.
 */
class Texture : public Ref, public Serializable
{
    friend class Sampler;
    friend class CompressedTexture;
    friend class GLRenderer;
public:

    /**
     * Defines the set of supported texture formats.
     */
    enum Format
    {
        UNKNOWN = 0,
        RGB,
        RGB888 = RGB,
        RGB565,
        RGBA,
        RGBA8888 = RGBA,
        RGBA4444,
        RGBA5551,
        ALPHA,
        DEPTH,
    };

    /**
     * Defines the set of supported texture filters.
     */
    enum Filter
    {
        NEAREST = 0x2600,
        LINEAR = 0x2601,
        NEAREST_MIPMAP_NEAREST = 0x2700,
        LINEAR_MIPMAP_NEAREST = 0x2701,
        NEAREST_MIPMAP_LINEAR = 0x2702,
        LINEAR_MIPMAP_LINEAR = 0x2703
    };

    /**
     * Defines the set of supported texture wrapping modes.
     */
    enum Wrap
    {
        REPEAT = 0x2901,
        CLAMP = 0x812F
    };

    /**
     * Defines the type of Texture in use.
     */
    enum Type
    {
        TEXTURE_2D = 0x0DE1,
        TEXTURE_CUBE = 0x8513
    };

    /**
     * Defines a face of a Texture of Type: cube.
     */
    enum CubeFace
    {
        POSITIVE_X,
        NEGATIVE_X,
        POSITIVE_Y,
        NEGATIVE_Y,
        POSITIVE_Z,
        NEGATIVE_Z
    };
    
    /**
     * Creates a texture from the given image resource.
     *
     * Note that for textures that include mipmap data in the source data (such as most compressed textures),
     * the generateMipmaps flags should NOT be set to true.
     *
     * @param path The image resource path.
     * @param generateMipmaps true to auto-generate a full mipmap chain, false otherwise.
     * 
     * @return The new texture, or NULL if the texture could not be loaded/created.
     * @script{create}
     */
    static Texture* create(const char* path, bool generateMipmaps = false);

    bool load(const char* path);

    /**
     * Creates a texture from the given image.
     *
     * @param image The image containing the texture data.
     * @param generateMipmaps True to generate a full mipmap chain, false otherwise.
     *
     * @return The new texture, or NULL if the image is not of a supported texture format.
     * @script{create}
     */
    static Texture* create(Image* image, bool generateMipmaps = false);

    /**
     * Creates a texture from the given texture data.
     *
     * The data in the texture is expected to be tightly packed (no padding at the end of rows).
     *
     * @param format Format of the texture data.
     * @param width Width of the texture data. If type is TEX_CUBE, then this is the cube face width.
     * @param height Height of the texture data. If type is TEX_CUBE, then this is the cube face height.
     * @param data Raw texture data (expected to be tightly packed). If the type parameter is set 
     *   to TEXTURE_CUBE, then data is expected to be each face stored back contiguously within the
     *   array.
     * @param generateMipmaps True to generate a full mipmap chain, false otherwise.
     * @param type What type of Texture should be created.
     *
     * @return The new texture.
     * @script{create}
     */
    static Texture* create(Format format, unsigned int width, unsigned int height, const unsigned char* data, bool generateMipmaps = false, Type type = TEXTURE_2D);

    static Texture* loadCubeMap(const char* faces[]);

    static size_t getFormatBPP(Texture::Format format);
    /**
     * Creates a texture object to wrap the specified pre-created native texture handle.
     *
     * The specified TextureHandle must represent a valid texture that has been created
     * on the underlying renderer and it should not be referenced by any other Texture
     * object. When the returned Texture object is destroyed, the passed in TextureHandle
     * will also be destroyed.
     *
     * @param handle Native texture handle.
     * @param width The width of the texture represented by 'handle'.
     * @param height The height of the texture represented by 'handle'.
     * @param format Optionally, the format of the texture represented by 'handle'.
     *      If the format cannot be represented by any of the Texture::Format values,
     *      use a value of UNKNOWN.
     *
     * @return The new texture.
     * @script{create}
     */
    //static Texture* create(TextureHandle handle, int width, int height, Format format = UNKNOWN);

    /**
     * Set texture data to replace current texture image.
     * 
     * @param data Raw texture data (expected to be tightly packed). If the type parameter is set 
     *   to TEXTURE_CUBE, then data is expected to be each face stored back contiguously within the
     *   array.
     */
    void setData(const unsigned char* data);

    /**
     * Returns the path that the texture was originally loaded from (if applicable).
     *
     * @return The texture path, or an empty string if the texture was not loaded from file.
     */
    const char* getPath() const;

    /**
     * Gets the format of the texture.
     *
     * @return The texture format.
     */
    Format getFormat() const;

    /**
     * Gets the texture type.
     *
     * @return The texture type.
     */
    Type getType() const;

    /**
     * Gets the texture width.
     *
     * @return The texture width.
     */
    unsigned int getWidth() const;

    /**
     * Gets the texture height.
     *
     * @return The texture height.
     */
    unsigned int getHeight() const;

    /**
     * Generates a full mipmap chain for this texture if it isn't already mipmapped.
     */
    void generateMipmaps();

    /**
     * Determines if this texture currently contains a full mipmap chain.
     *
     * @return True if this texture is currently mipmapped, false otherwise.
     */
    bool isMipmapped() const;

    /**
     * Determines if this texture is a compressed texture.
     */
    bool isCompressed() const;

    /**
     * Returns the texture handle.
     *
     * @return The texture handle.
     */
    TextureHandle getHandle() const;

    /**
     * @see Activator::createObject
     */
    static Serializable* createObject();

    /**
     * @see Activator::enumToString
     */
    static std::string enumToString(const std::string& enumName, int value);

    /**
     * @see Activator::enumParse
     */
    static int enumParse(const std::string& enumName, const std::string& str);

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();

    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer);

    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(Serializer* serializer);

    /**
     * Sets the wrap mode for this sampler.
     *
     * @param wrapS The horizontal wrap mode.
     * @param wrapT The vertical wrap mode.
     * @param wrapR The depth wrap mode.
     */
    void setWrapMode(Wrap wrapS, Wrap wrapT, Wrap wrapR = REPEAT);

    /**
     * Sets the texture filter modes for this sampler.
     *
     * @param minificationFilter The texture minification filter.
     * @param magnificationFilter The texture magnification filter.
     */
    void setFilterMode(Filter minificationFilter, Filter magnificationFilter);

    /**
     * Binds the texture of this sampler to the renderer and applies the sampler state.
     */
    void bind();
private:

    /**
     * Constructor.
     */
    Texture();

    /**
     * Copy constructor.
     */
    Texture(const Texture& copy);

    /**
     * Destructor.
     */
    virtual ~Texture();

    /**
     * Hidden copy assignment operator.
     */
    Texture& operator=(const Texture&);

    std::string _path;
    
    Format _format;
    Type _type;
    unsigned int _width;
    unsigned int _height;
    bool _mipmapped;
    bool _cached;
    bool _compressed;
    Wrap _wrapS;
    Wrap _wrapT;
    Wrap _wrapR;

    Filter _magFilter;

    bool _generateMipmaps;

    //int _internalFormat;
    //unsigned int _texelType;
    //size_t _bpp;
public:
    TextureHandle _handle;
    const unsigned char* _data;
    Filter _minFilter;
};

}

#endif
