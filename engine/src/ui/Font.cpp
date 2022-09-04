#include "base/Base.h"
#include "Font.h"
#include "ui/Text.h"
#include "platform/Game.h"
#include "base/FileSystem.h"
#include "material/Material.h"

extern "C" {
#include "../3rd/utf8.h"
}
// Default font shaders
#define FONT_VSH "res/shaders/font.vert"
#define FONT_FSH "res/shaders/font.frag"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../3rd/stb_image_write.h"


size_t utf8decode(char const* str, int len, wchar_t* des, size_t n, int* illegal) {
    if (n == 0)
        return 0;

    char* s = (char*)str;
    size_t i = 0;
    wchar_t uc = 0;
    int r_illegal_all = 0, r_illegal;

    while ((uc = getu8c(&s, &r_illegal)))
    {
        if (len != -1 && s >= str + len) break;

        if (i < (n - 1))
        {
            des[i++] = uc;
            r_illegal_all += r_illegal;
        }
        else
        {
            break;
        }
    }

    des[i] = 0;
    if (illegal)
    {
        *illegal = r_illegal_all + r_illegal;
    }

    return i;
}


namespace gameplay
{

static std::vector<Font*> __fontCache;

static ShaderProgram* __fontEffect = NULL;


struct Font::FontTexture {
    SpriteBatch* batch;
    Texture* texture;
    int indexId;

    int lastX;
    int curY;
    int maxY;
    bool isFull;

    std::vector<Glyph> glyphs;
    unsigned char* data;


    FontTexture() : batch(NULL), texture(NULL), indexId(0), lastX(0), curY(0), maxY(0), isFull(false) {}
};

Font::Font() :
    _style(PLAIN), _size(30), _spacing(0.0f), textureWidth(256), textureHeight(256), _isStarted(false)
{
    shaderProgram = ShaderProgram::createFromFile(FONT_VSH, FONT_FSH);
}

Font::~Font()
{
    // Remove this Font from the font cache.
    std::vector<Font*>::iterator itr = std::find(__fontCache.begin(), __fontCache.end(), this);
    if (itr != __fontCache.end())
    {
        __fontCache.erase(itr);
    }

    for (size_t i = 0, count = fontTextures.size(); i < count; ++i)
    {
        fontTextures[i]->texture->release();
        delete fontTextures[i]->batch;
        delete fontTextures[i];
    }
    fontTextures.clear();

    for (size_t i = 0, count = fontFaces.size(); i < count; ++i)
    {
        delete fontFaces[i];
    }
    fontFaces.clear();

    shaderProgram->release();
    shaderProgram = NULL;
}

Font* Font::create(const char* path)
{
    GP_ASSERT(path);

    // Search the font cache for a font with the given path and ID.
    for (size_t i = 0, count = __fontCache.size(); i < count; ++i)
    {
        Font* f = __fontCache[i];
        GP_ASSERT(f);
        if (f->_path == path)
        {
            // Found a match.
            f->addRef();
            return f;
        }
    }

    Font* font = new Font();
    FontFace* face = new FontFace();
    face->load(path);
    font->fontFaces.push_back(face);

    return font;
}


bool Font::isCharacterSupported(int character) const
{
    //TODO
    return true;
}

void Font::start()
{
    // no-op : fonts now are lazily started on the first draw call
    _isStarted = true;
}

void Font::lazyStart()
{
    for (size_t i = 0, count = fontTextures.size(); i < count; ++i) {
        SpriteBatch* _batch = fontTextures[i]->batch;
        if (_batch->isStarted())
            return; // already started

        // Update the projection matrix for our batch to match the current viewport
        const Rectangle& vp = Game::getInstance()->getViewport();
        if (!vp.isEmpty())
        {
            Game* game = Game::getInstance();
            Matrix projectionMatrix;
            Matrix::createOrthographicOffCenter(vp.x, vp.width, vp.height, vp.y, 0, 1, &projectionMatrix);
            _batch->setProjectionMatrix(projectionMatrix);
        }

        _batch->start();
    }
}

void Font::finish()
{
    for (size_t i = 0, count = fontTextures.size(); i < count; ++i) {
        SpriteBatch* _batch = fontTextures[i]->batch;
        // Finish any font batches that have been started
        if (_batch->isStarted())
            _batch->finish();
    }
    _isStarted = false;
}

void Font::setProjectionMatrix(const Matrix& matrix) {
    for (size_t i = 0, count = fontTextures.size(); i < count; ++i) {
        SpriteBatch* _batch = fontTextures[i]->batch;
        _batch->setProjectionMatrix(matrix);
    }
}

bool Font::isStarted() const {
    return _isStarted;
}


bool Font::drawChar(int c, FontInfo& fontInfo, Glyph& glyph, int x, int y, const Vector4& color, int previous) {
    uint64_t key = ((uint64_t)c << 32) | (((uint64_t)fontInfo.size) << 8) | (fontInfo.bold);


    FontTexture* fontTexture = NULL;
    auto itr = glyphCache.find(key);
    if (itr != glyphCache.end()) {
        glyph = itr->second;
        fontTexture = fontTextures[glyph.texture];
    }
    else {
        //render char to image
        if (!fontFaces.at(0)->renderChar(c, fontInfo, glyph)) {
            return false;
        }

        //find free space
        for (int i = 0; i < fontTextures.size(); ++i) {
            FontTexture* ft = fontTextures[0];
            if (ft->isFull) continue;
            if (ft->lastX + glyph.imgW + 1 > textureWidth) {
                ft->lastX = 0;
                ft->curY = ft->maxY;
            }
            if (ft->curY + glyph.imgH + 1 < textureHeight) {
                fontTexture = ft;
                break;
            }
            else {
                ft->isFull = true;
                //char name[256];
                //snprintf(name, 256, "fontTexture_%d.png", i);
                //stbi_write_png(name, textureWidth, textureHeight, 1, ft->data, textureWidth * 1);
            }
        }

        //new texture
        if (fontTexture == NULL) {
            fontTexture = new FontTexture();
            unsigned char* data = (unsigned char*)calloc(1, textureWidth * textureHeight);
            fontTexture->texture = Texture::create(Texture::Format::ALPHA, textureWidth, textureHeight, data);
            fontTexture->data = data;
            fontTexture->batch = SpriteBatch::create(fontTexture->texture, shaderProgram);
            fontTexture->indexId = fontTextures.size();
            fontTextures.push_back(fontTexture);
        }

        //copy sub image
        glyph.imgX = fontTexture->lastX + 1;
        glyph.imgY = fontTexture->curY + 1;
        for (int i = 0; i < glyph.imgH; ++i) {
            int dstRow = (glyph.imgY + i) * textureWidth;
            int srcRow = i * glyph.imgW;
            for (int j = 0; j < glyph.imgW; ++j) {    
                int dstX = (glyph.imgX + j);
                fontTexture->data[dstRow + dstX] = glyph.imgData[srcRow + j];
            }
        }
        //stbi_write_png("fontTextureChar.png", glyph.imgW, glyph.imgH, 1, glyph.imgData, glyph.imgW * 1);
        free(glyph.imgData);
        glyph.imgData = NULL;

        if (fontTexture->maxY < glyph.imgY + glyph.imgH) {
            fontTexture->maxY = glyph.imgY + glyph.imgH;
        }
        fontTexture->lastX = glyph.imgX + glyph.imgW;

        //upload image data
        fontTexture->texture->setData(fontTexture->data);
        fontTexture->glyphs.push_back(glyph);
        glyph.texture = fontTexture->indexId;
        
        glyphCache[key] = glyph;

        //char name[256];
        //snprintf(name, 256, "fontTexture_%p.png", this);
        //stbi_write_png(name, textureWidth, textureHeight, 1, fontTexture->data, textureWidth * 1);
    }

    
    Texture* texture = fontTexture->texture;
    SpriteBatch* _batch = fontTexture->batch;

    if (previous > 0 && previous < 128 && c < 128) {
        float kerning = fontFaces.at(0)->getKerning(fontInfo, previous, c);
        x += kerning;
    }

    _batch->draw(x + (int)(glyph.metrics.horiBearingX), y - (int)(glyph.metrics.horiBearingY - fontInfo.size),
        glyph.imgW, glyph.imgH,
        glyph.imgX / (float)textureWidth,
        (glyph.imgY / (float)textureHeight),
        (glyph.imgX + glyph.imgW) / (float)textureWidth,
        ((glyph.imgY + glyph.imgH) / (float)textureHeight),
        color);

    return true;
}


void Font::drawText(const char* text, int x, int y, const Vector4& color, unsigned int size, int len)
{
    GP_ASSERT(_size);
    GP_ASSERT(text);

    if (size == 0)
    {
        size = _size;
    }

    wchar_t utext[1024];
    int utextSize = utf8decode(text, len, utext, 1024, NULL);
    
    lazyStart();

    int spacing = (int)(size * _spacing);


    int xPos = x, yPos = y;

    FontInfo fontInfo;
    fontInfo.bold = 0;
    fontInfo.size = size;
    GlyphMetrics metrics;
    fontFaces.at(0)->merics(0, fontInfo, metrics);

    wchar_t previous = 0;
    for (size_t i = 0; i < utextSize; i++)
    {
        uint32_t c = utext[i];

        // Draw this character.
        switch (c)
        {
        case ' ':
            xPos += size/2;
            break;
        case '\r':
        case '\n':
            yPos += size;
            xPos = x;
            break;
        case '\t':
            xPos += size * 2;
            break;
        default: {
            Glyph glyph;
            if (drawChar(c, fontInfo, glyph, xPos, yPos, color, previous)) {
                xPos += floor(glyph.metrics.horiAdvance + spacing);
            }
            else {
                xPos += size;
            }
        }
        }

        previous = c;
    }
}

void Font::measureText(const char* text, unsigned int size, unsigned int* width, unsigned int* height, int len)
{
    GP_ASSERT(_size);
    GP_ASSERT(text);
    GP_ASSERT(width);
    GP_ASSERT(height);

    if (size == 0)
    {
        size = _size;
    }

    wchar_t utext[1024];
    int utextSize = utf8decode(text, len, utext, 1024, NULL);

    if (utextSize == 0)
    {
        *width = 0;
        *height = 0;
        return;
    }

    int spacing = (int)(size * _spacing);

    int xPos = 0, yPos = 0;

    FontInfo fontInfo;
    fontInfo.bold = 0;
    fontInfo.size = size;
    GlyphMetrics metrics;
    fontFaces.at(0)->merics(0, fontInfo, metrics);

    int maxW = 0;
    for (size_t i = 0; i < utextSize; i++)
    {
        uint32_t c = utext[i];

        // Draw this character.
        switch (c)
        {
        case ' ':
            xPos += size/2;
            break;
        case '\r':
        case '\n':
            yPos += metrics.height;
            xPos = 0;
            break;
        case '\t':
            xPos += size * 2;
            break;
        default: {
            GlyphMetrics m;
            if (fontFaces.at(0)->merics(c, fontInfo, m)) {
                xPos += floor(m.horiAdvance + spacing);
            }
            else {
                xPos += size;
            }
        }
        }

        if (maxW < xPos) {
            maxW = xPos;
        }
    }
    *width = maxW;
    *height = metrics.height + yPos;
}


float Font::getCharacterSpacing() const
{
    return _spacing;
}

void Font::setCharacterSpacing(float spacing)
{
    _spacing = spacing;
}


}
