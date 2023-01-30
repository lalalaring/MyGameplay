#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "base/Base.h"
#include "base/Ref.h"
#include <mutex>

namespace gameplay
{
class AssetManager
{
public:
    enum ResType {
        rt_none,
        rt_texture,
        rt_materail,
        rt_shaderProgram,
        rt_mesh,
        rt_animation,
        rt_skin,
        rt_count
      };
private:
    std::map<std::string, Ref*> resourceMap[rt_count];
    std::mutex mutex;
    std::string path;
public:
    static AssetManager *getInstance();
    static void releaseInstance();
private:
    AssetManager();
    ~AssetManager();

public:
    void clear();

    template<typename T>
    T *load(const std::string &name, ResType type) {
        return dynamic_cast<T*>(load(name, type));
    }

    Ref *load(const std::string &name, ResType type);

    void remove(const std::string &name, ResType type);

    void save(const std::string &name, Ref *res);
};
}
#endif // ASSETMANAGER_H
