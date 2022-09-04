#ifndef FONTFACE_H_
#define FONTFACE_H_

#include "../scene/SpriteBatch.h"


namespace gameplay {

struct FontInfo {
	char name[128];
	int size;
	int bold;
	int outline;

	FontInfo() : size(30), bold(1), outline(1) { name[0] = 0; }
};

struct GlyphMetrics {
	float  width;
	float  height;

	float  horiBearingX;
	float  horiBearingY;
	float  horiAdvance;

	float  vertBearingX;
	float  vertBearingY;
	float  vertAdvance;
};

struct Glyph {
	GlyphMetrics metrics;
	int code;
	int imgX;
	int imgY;
	int imgW;
	int imgH;
	unsigned char* imgData;
	int texture;
};


class FontFace {
	void* library;
	void* face;
public:
	typedef uint32_t Char;
	FontFace();
	~FontFace();
	bool load(const char* file);
	bool renderChar(Char ch, FontInfo& font, Glyph& glyph);

	float getKerning(FontInfo& font, Char previous, Char current);
	bool merics(Char ch, FontInfo& font, GlyphMetrics& m);
};


}


#endif