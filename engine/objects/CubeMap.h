#ifndef CUBEMAP_H_
#define CUBEMAP_H_

#include "base/Ref.h"
#include "base/Base.h"
#include "scene/Drawable.h"

namespace gameplay
{
class Texture;
class Mesh;
class Material;

class CubeMap : public Ref, public Drawable {
    Mesh* _mesh;
    Material* _material;

    Texture *texture;
    bool followCamera;

public:
    Texture* getTexture() {
        return texture;
    }

    CubeMap();
    ~CubeMap();

    bool load(std::vector<std::string>& faces, bool isSkyBox);

    unsigned int draw(RenderView* view);
};

}

#endif