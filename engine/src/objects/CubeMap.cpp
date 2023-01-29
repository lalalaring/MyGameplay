#include "CubeMap.h"
#include "scene/Mesh.h"
#include "material/Material.h"
#include "material/Texture.h"
#include "material/MaterialParameter.h"
#include "scene/Node.h"
#include "render/Renderer.h"

using namespace gameplay;

gameplay::CubeMap::CubeMap() : _mesh(NULL), _material(NULL), texture(NULL), followCamera(true)
{
}

gameplay::CubeMap::~CubeMap()
{
    SAFE_RELEASE(_mesh);
    SAFE_RELEASE(_material);
    SAFE_RELEASE(texture);
}

bool gameplay::CubeMap::load(std::vector<std::string>& faces, bool isSkyBox)
{
    _mesh = Mesh::createSimpleCube();
    Material* material = Material::create("res/shaders/cubemap.vert", "res/shaders/cubemap.frag", isSkyBox?"SKY_BOX":NULL);
    const char* cfaces[6];
    cfaces[0] = faces[0].c_str();
    cfaces[1] = faces[1].c_str();
    cfaces[2] = faces[2].c_str();
    cfaces[3] = faces[3].c_str();
    cfaces[4] = faces[4].c_str();
    cfaces[5] = faces[5].c_str();
    Texture* sampler = Texture::loadCubeMap(cfaces);
    if (!sampler) return false;
    material->getParameter("u_diffuseTexture")->setSampler(sampler);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthFunction(StateBlock::DEPTH_LEQUAL);


    //Material* material = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag");
    //Texture* sampler = material->getParameter("u_diffuseTexture")->setValue("res/image/crate.png", true);

    this->texture = sampler;
    this->_material = material;
    followCamera = isSkyBox;
    return true;
}

unsigned int gameplay::CubeMap::draw(RenderView* view)
{
    if (followCamera) {
        auto position = view->camera->getNode()->getTranslationWorld();
        this->getNode()->setTranslation(position);
    }

    if (!_mesh) return 0;

    Renderer::cur()->renderMesh(_mesh, _material, 1, NULL, view, _node);

    return 1;
}
