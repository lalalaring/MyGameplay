#ifndef COMPRESSEDTEXTURE_H_
#define COMPRESSEDTEXTURE_H_

#include "material/Texture.h"

namespace gameplay
{
    class CompressedTexture {
    public:
        static Texture* createCompressedDdsKtx(const char* path);

        static Texture* createCompressedPVRTC(const char* path);

        static Texture* createCompressedDDS(const char* path);

    };
}

#endif