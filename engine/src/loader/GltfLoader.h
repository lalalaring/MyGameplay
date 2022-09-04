#ifndef GLTFLOADER_H_
#define GLTFLOADER_H_

#include "base/Base.h"
#include "../scene/Mesh.h"
#include "../scene/Scene.h"
#include "../scene/Node.h"

namespace gameplay
{
class GltfLoader {
	Scene* load(const std::string &file);
};
}

#endif //GLTFLOADER_H_