#ifndef LINE_H_
#define LINE_H_

#include "base/Ref.h"
#include "base/Base.h"
#include "scene/Drawable.h"

namespace gameplay
{
class Texture;
class Mesh;
class Material;

class Line : public Ref, public Drawable {
    Mesh* _mesh;
    Material* _material;
public:

    Line();
    ~Line();

    unsigned int draw(RenderView* view);

private:
    void addVertex(float *npos, int nindex, float *position, int sindex, Vector3 refpoint,
        float side, float endoffset, float begin_distance);

    int addLineSeg(float *npos, int nindex, float *position, int sindex,
        int vertex_size, float begin_distance);

  public:
    void setPositions(float *position, int point_count);
};

}

#endif