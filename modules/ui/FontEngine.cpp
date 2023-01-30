#include "FontEngine.h"

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H
#include FT_STROKER_H
}
//#define PF_FONT_DPI 300

using namespace gameplay;


FontFace::FontFace() : library(NULL), face(NULL) {}
FontFace::~FontFace() {
    FT_Done_Face((FT_Face)face);
    FT_Done_FreeType((FT_Library)library);
    face = NULL;
    library = NULL;
}

bool FontFace::load(const char* fileName) {
    FT_Library  library;   /* handle to library     */
    FT_Face     face;      /* handle to face object */
    FT_Error error;


    error = FT_Init_FreeType(&library);
    if (error) {
        printf("init font error\n");
        return false;
    }

    error = FT_New_Face(library,
        fileName,
        0,
        &face);
    if (error != FT_Err_Ok) {
        printf("open file error:%s\n", fileName);
        return false;
    }

    this->library = library;
    this->face = face;

    if (face->num_faces > 1) {
        for (int i = 1, n = face->num_faces; i < n; ++i) {
            error = FT_New_Face(library,
                fileName,
                0,
                &face);
            if (error != FT_Err_Ok) {
                break;
            }
            size_t len = strlen(face->family_name);
            //SC表示简体中文
            if (len > 2 && face->family_name[len - 2] == 'S' && (face->family_name[len - 1] == 'C')) {

                FT_Done_Face((FT_Face)this->face);
                this->face = face;
                break;
            }
        }
    }

    return true;
}


bool FontFace::merics(Char uniChar, FontInfo& font, GlyphMetrics& m) {
    FT_Face face = (FT_Face)this->face;      /* handle to face object */
    FT_Error error;
    FT_UInt glyph_index;

    if (!face) return false;

    /* set character size */
    error = FT_Set_Pixel_Sizes(face, 0, font.size);

    if (uniChar == 0) {
        m.horiAdvance = face->size->metrics.max_advance / 64.0;
        m.horiBearingY = face->size->metrics.ascender / 64.0;
        m.horiBearingX = 0;
        m.vertAdvance = face->size->metrics.height / 64.0;
        m.vertBearingX = 0;
        m.vertBearingY = 0;
        m.height = face->size->metrics.height / 64.0;
        m.width = face->size->metrics.max_advance / 64.0;
        return true;
    }

    /* retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index(face, uniChar);
    if (glyph_index == 0) return false;

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error) {
        return false;
    }

    m.height = face->glyph->metrics.height / 64.0;
    m.width = face->glyph->metrics.width / 64.0;
    m.horiBearingX = face->glyph->metrics.horiBearingX / 64.0;
    m.horiBearingY = face->glyph->metrics.horiBearingY / 64.0;
    m.horiAdvance = face->glyph->metrics.horiAdvance / 64.0;
    m.vertBearingX = face->glyph->metrics.vertBearingX / 64.0;
    m.vertBearingY = face->glyph->metrics.vertBearingY / 64.0;
    m.vertAdvance = face->glyph->metrics.vertAdvance / 64.0;

    return true;
}

float FontFace::getKerning(FontInfo& font, Char previous, Char current) {
    FT_Face     face = (FT_Face)this->face;      /* handle to face object */
    FT_Bool use_kerning = FT_HAS_KERNING(face);

    if (use_kerning && previous && current) {
        FT_Set_Pixel_Sizes(face, 0, font.size);

        FT_UInt index1 = FT_Get_Char_Index(face, previous);
        FT_UInt index2 = FT_Get_Char_Index(face, current);

        FT_Vector  delta;
        FT_Error error = FT_Get_Kerning(face, index1, index2,
            FT_KERNING_DEFAULT, &delta);
        if (error) {
            return 0;
        }
        return delta.x / 64.0;
    }
    return 0;
}

static void bitmapToImage(FT_Glyph glyph, Glyph& image) {
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
    FT_Bitmap bitmap = bitmap_glyph->bitmap;
    image.imgX = 0;
    image.imgY = 0;
    image.texture = -1;
    image.imgW = bitmap.width;
    image.imgH = bitmap.rows;
    //image.pixelFormat = PixelFormat::GRAY8;
    int size = image.imgW * image.imgH;
    image.imgData = (unsigned char*)malloc(size);
    memcpy(image.imgData, bitmap.buffer, size);
}

bool FontFace::renderChar(Char uniChar, FontInfo& font, Glyph& g) {
    FT_Face face = (FT_Face)this->face;      /* handle to face object */
    FT_Error error;
    FT_UInt glyph_index;
    FT_Glyph glyph;

    if (!face) return false;

    /* use 50pt at 100dpi */
    /* set character size */
    error = FT_Set_Pixel_Sizes(face, 0, font.size);

    /* retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index(face, uniChar);
    if (glyph_index == 0) return false;

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error) return false;

    if (font.bold) {
        FT_Outline_Embolden(&face->glyph->outline, font.bold << 6);
    }

    error = FT_Get_Glyph(face->glyph, &glyph);
    if (error) return false;

    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
    if (error) {
        FT_Done_Glyph(glyph);
        return false;
    }

    g.code = uniChar;
    g.metrics.horiAdvance = face->glyph->advance.x / 64.0f;
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
    g.metrics.width = bitmap_glyph->bitmap.width;
    g.metrics.height = bitmap_glyph->bitmap.rows;
    g.metrics.horiBearingX = bitmap_glyph->left;
    g.metrics.horiBearingY = bitmap_glyph->top;
    bitmapToImage(glyph, g);

    FT_Done_Glyph(glyph);

    /////////////////////////////////////////////////////////////
    /*
    FT_Stroker stroker;
    error = FT_Get_Glyph(face->glyph, &glyph);
    if (error) {
        return false;
    }
    error = FT_Stroker_New((FT_Library)library, &stroker);
    if (error) {
        FT_Done_Glyph(glyph);
        return false;
    }

    FT_Stroker_Set(stroker,
        (int)(font.outline * 64),
        FT_STROKER_LINECAP_ROUND,
        FT_STROKER_LINEJOIN_ROUND,
        0);

    error = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
    if (error) {
        FT_Done_Glyph(glyph);
        FT_Stroker_Done(stroker);
        return false;
    }
    error = FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
    if (error) {
        FT_Done_Glyph(glyph);
        FT_Stroker_Done(stroker);
        return false;
    }

    FT_BitmapGlyph bitmap_glyph2 = (FT_BitmapGlyph)glyph;
    bitmapToImage(glyph, g.outlineImage);

    FT_Done_Glyph(glyph);
    FT_Stroker_Done(stroker);
    */
    return true;
}


