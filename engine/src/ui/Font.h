#ifndef FONT_H_
#define FONT_H_

#include "objects/SpriteBatch.h"
#include "FontEngine.h"

namespace gameplay
{

/**
 * Defines a font for text rendering.
 */
class Font : public Ref, public BatchableLayer
{
    friend class Bundle;
    friend class Text;
    friend class TextBox;

public:

    /**
     * Defines the set of allowable font styles.
     */
    enum Style
    {
        PLAIN = 0,
        BOLD = 1,
        ITALIC = 2,
        BOLD_ITALIC = 4
    };


    /**
     * Creates a font from the given bundle.
     *
     * If the 'id' parameter is NULL, it is assumed that the Bundle at 'path'
     * contains exactly one Font resource. If the Bundle does not meet this criteria,
     * NULL is returned.
     *
     * If a font for the given path has already been loaded, the existing font will be
     * returned with its reference count increased.
     *
     * @param path The path to a bundle file containing a font resource.
     * @param id An optional ID of the font resource within the bundle (NULL for the first/only resource).
     *
     * @return The specified Font or NULL if there was an error.
     * @script{create}
     */
    static Font* create(const char* path);


    /**
     * Determines if this font supports the specified character code.
     *
     * @param character The character code to check.
     * @return True if this Font supports (can draw) the specified character, false otherwise.
     */
    bool isCharacterSupported(int character) const;

    /**
     * Starts text drawing for this font.
     */
    void start();

    /**
     * Draws the specified text in a solid color, with a scaling factor.
     *
     * @param text The text to draw.
     * @param x The viewport x position to draw text at.
     * @param y The viewport y position to draw text at.
     * @param color The color of text.
     * @param size The size to draw text (0 for default size).
     * @param rightToLeft Whether to draw text from right to left.
     */
    void drawText(const char* text, int x, int y, const Vector4& color, unsigned int size = 0, int len = -1);


    /**
     * Finishes text batching for this font and renders all drawn text.
     */
    void finish();

    virtual void setProjectionMatrix(const Matrix& matrix);
    virtual bool isStarted() const;

    /**
     * Measures a string's width and height without alignment, wrapping or clipping.
     *
     * @param text The text to measure.
     * @param size The font height to scale to.
     * @param widthOut Destination for the text's width.
     * @param heightOut Destination for the text's height.
     */
    void measureText(const char* text, unsigned int size, unsigned int* widthOut, unsigned int* heightOut, int len = -1);


    /**
     * Returns current character spacing for this font in percentage of fonts size.
     *
     * @see setCharacterSpacing(float)
     */
    float getCharacterSpacing() const;

    /**
     * Sets the additional character spacing for this font.
     *
     * Character spacing is the additional amount of space that is inserted between characters. Character spacing is defined
     * as a floating point value that is interpreted as a percentage of size used to draw the font. For example,
     * a value of 0.1 would cause a spacing of 10% of the font size to be inserted between adjacent characters.
     * For a font size of 20, this would equate to 2 pixels of extra space between characters.
     *
     * The default additional character spacing for fonts is 0.0.
     *
     * @param spacing New fixed character spacing, expressed as a percentage of font size.
     */
    void setCharacterSpacing(float spacing);


    int getSize() { return _size; }

private:


    /**
     * Constructor.
     */
    Font();

    /**
     * Constructor.
     */
    Font(const Font& copy);

    /**
     * Destructor.
     */
    ~Font();

    /**
     * Hidden copy assignment operator.
     */
    Font& operator=(const Font&);

    bool drawChar(int c, FontInfo &fontInfo, Glyph &glyph, int x, int y, const Vector4& color, int previous);


    void lazyStart();

    std::string _path;
    Style _style;
    unsigned int _size;
    float _spacing;
    bool _isStarted;

    int textureWidth;
    int textureHeight;

    struct FontTexture;
    std::vector<FontTexture*> fontTextures;
    std::vector<FontFace*> fontFaces;

    std::map<uint64_t, Glyph> glyphCache;

    ShaderProgram* shaderProgram;
};

}

#endif
