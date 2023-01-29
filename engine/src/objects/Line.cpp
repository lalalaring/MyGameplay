
#include "Line.h"
#include "scene/Mesh.h"
#include "scene/MeshPart.h"
#include "material/Material.h"
#include "material/Texture.h"
#include "material/MaterialParameter.h"
#include "scene/Node.h"
#include "render/Renderer.h"

using namespace gameplay;

gameplay::Line::Line() : _mesh(NULL), _material(NULL)
{
}

gameplay::Line::~Line()
{
    SAFE_RELEASE(_mesh);
    SAFE_RELEASE(_material);
}

unsigned int gameplay::Line::draw(RenderView* view)
{
    if (!_mesh) return 0;
    Renderer::cur()->renderMesh(_mesh, _material, 1, NULL, view, _node);
    return 1;
}

void Line::addVertex(float *npos, int nindex, float *position, int sindex, Vector3 refpoint,
    float side, float endoffset, float begin_distance) {
    int p = nindex;
    int q = sindex;
    npos[p+0] = position[q];
    npos[p+1] = position[q+1];
    npos[p+2] = position[q+2];

    npos[p+3] = refpoint.x;
    npos[p+4] = refpoint.y;
    npos[p+5] = refpoint.z;
    npos[p+6] = side;
    npos[p+7] = endoffset;
    npos[p+8] = begin_distance;

    if (side > 0) {
      npos[p+0] = npos[p+0]+0.1;
    }
}

int Line::addLineSeg(float *npos, int nindex, float *position, int sindex,
    int vertex_size, float begin_distance) {
    int p = nindex;
    int q = sindex;
    int q2  = sindex+3;
    Vector3 start_point = Vector3(position[q], position[q+1], position[q+2]);
    Vector3 end_point = Vector3(position[q2], position[q2+1], position[q2+2]);
    //Vector3 tangent = Vector3(position[q2]-position[q], position[q2+1]-position[q+1], position[q2+2]-position[q+2]);
    float len = start_point.distance(end_point);

    addVertex(npos, p, position, q, end_point, 1, -1, begin_distance);
    p += vertex_size;
    addVertex(npos, p, position, q, end_point, -1, -1, begin_distance);
    p += vertex_size;

    addVertex(npos, p, position, q2, start_point, 1, 1, begin_distance+len);
    p += vertex_size;
    addVertex(npos, p, position, q2, start_point, -1, 1, begin_distance+len);
    p += vertex_size;

    return len;
}


void Line::setPositions(float *position, int point_count) {
    int vertex_size = 9;
    int vertex_pcount = 4;
    float* npos = new float[((point_count-1)*vertex_size*vertex_pcount)];

    int begin_distance = 0;

    for (int i=0; i<point_count-1; ++i) {
      int p = i * vertex_size*vertex_pcount;
      begin_distance += addLineSeg(npos, p, position, i*3, vertex_size, begin_distance);
    }

    short *indices = new short[((point_count-1) * 6)];
    for (int i=0; i<point_count-1; ++i) {
      int p = i*4;
      int k = i*6;
      indices[k++] = p+0;
      indices[k++] = p+1;
      indices[k++] = p+3;

      indices[k++] = p+0;
      indices[k++] = p+3;
      indices[k++] = p+2;
    }

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element("a_refpoint", 3),
        VertexFormat::Element("a_side", 1),
        VertexFormat::Element("a_endoffset", 1),
        VertexFormat::Element("a_distance", 1),
    };

    int vertexCount = (point_count-1)*vertex_pcount;
    int indexCount = ((point_count-1) * 6);
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 5), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return;
    }
    mesh->setVertexData(npos, 0, vertexCount);
    MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, false);
    meshPart->setIndexData(indices, 0, indexCount);


    
    Material* material = Material::create("res/shaders/line.vert", "res/shaders/line.frag");
    material->getParameter("u_lineWidth")->setFloat(20);
    material->getParameter("u_diffuseColor")->setVector4(Vector4(0.5, 1.0, 0.0, 1.0));
    material->getParameter("u_dashSize")->setFloat(20);

    material->getStateBlock()->setCullFace(false);
    
    //Material* material = Material::create("res/shaders/min.vert", "res/shaders/min.frag");
    //material->getParameter("u_diffuseColor")->setVector4(Vector4(0.5, 1.0, 0.0, 1.0));

    this->_material = material;
    this->_mesh = mesh;
}