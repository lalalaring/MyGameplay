#include "AssetManager.h"
#include "../base/Stream.h"
#include "../base/FileSystem.h"
#include "Mesh.h"
#include "../material/Material.h"
#include "MeshSkin.h"
#include "../base/SerializerJson.h"
#include "../animation/Animation.h"

using namespace gameplay;

AssetManager *AssetManager_instance = NULL;

AssetManager *AssetManager::getInstance() {
  if (AssetManager_instance == NULL) {
    AssetManager_instance = new (AssetManager);
  }
  return AssetManager_instance;
}

void AssetManager::releaseInstance() {
  if (AssetManager_instance) {
    AssetManager_instance->clear();
    delete AssetManager_instance;
    AssetManager_instance = NULL;
  }
}

AssetManager::AssetManager(): path("res") {

}

AssetManager::~AssetManager() {

}

void AssetManager::clear() {
  std::lock_guard<std::mutex> lock_guard(mutex);
  for (int i=0; i<rt_count; ++i) {
    std::map<std::string, Ref*> &map = resourceMap[i];
    for (auto it = map.begin(); it != map.end(); ++it) {
      if (it->second) {
        it->second->release();
      }
    }
    map.clear();
  }
}

void AssetManager::remove(const std::string &name, ResType type) {
  std::lock_guard<std::mutex> lock_guard(mutex);

  auto it = resourceMap[type].find(name);
  if (it != resourceMap[type].end()) {
      resourceMap[type].erase(it);
  }
}

Ref *AssetManager::load(const std::string &name, ResType type) {
    if (name.size() == 0) return NULL;

    std::lock_guard<std::mutex> lock_guard(mutex);
    auto itr = this->resourceMap[type].find(name);
    if (itr != this->resourceMap[type].end()) {
        return itr->second;
    }
    Ref* res = NULL;
    switch (type) {
        case rt_mesh: {
            std::string file = path + "/" + name + ".mesh";
            Stream *s = FileSystem::open(file.c_str());
            Mesh *mesh = Mesh::read(s);
            mesh->setName(name);
            s->close();
            delete s;
            res = mesh;
            break;
        }
        case rt_skin: {
            std::string file = path + "/" + name + ".skin";
            Stream *s = FileSystem::open(file.c_str());
            MeshSkin *mesh = MeshSkin::read(s);
            mesh->setName(name);
            s->close();
            delete s;
            res = mesh;
            break;
        }
        case rt_materail: {
            std::string file = path + "/" + name + ".material";
            auto stream = SerializerJson::createReader(file);
            Material *m = dynamic_cast<Material*>(stream->readObject(nullptr));
            stream->close();
            delete stream;
            m->setName(name);
            res = m;
            break;
        }
        case rt_animation: {
            std::string file = path + "/" + name + ".anim";
            Stream *s = FileSystem::open(file.c_str());
            Animation *mesh = Animation::read(s);
            mesh->setName(name);
            s->close();
            delete s;
            res = mesh;
            break;
        }
    }
    if (res) {
        res->addRef();
        this->resourceMap[type][name] = res;
    }
    return res;
}

void AssetManager::save(const std::string &name, Ref *res) {
    if (res == NULL) return;

     if (Mesh *mesh = dynamic_cast<Mesh*>(res)) {
        std::string file = path + "/" + name + ".mesh";
        Stream *s = FileSystem::open(file.c_str(), FileSystem::WRITE);
        mesh->write(s);
        s->close();
        delete s;
      }
      else if (MeshSkin *mesh = dynamic_cast<MeshSkin*>(res)) {
        std::string file = path + "/" + name + ".skin";
        Stream *s = FileSystem::open(file.c_str(), FileSystem::WRITE);
        mesh->write(s);
        s->close();
        delete s;
      }
      else if (Material *m = dynamic_cast<Material*>(res)) {
          std::string file = path + "/" + name + ".material";
          auto stream = SerializerJson::createWriter(file);
          stream->writeObject(nullptr, m);
          stream->close();
          delete stream;
      }
      else if (Animation *mesh = dynamic_cast<Animation*>(res)) {
       std::string file = path + "/" + name + ".anim";
       Stream *s = FileSystem::open(file.c_str(), FileSystem::WRITE);
       mesh->write(s);
       s->close();
       delete s;
     }
}

