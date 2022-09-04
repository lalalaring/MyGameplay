#include "Base.h"
#include "Activator.h"
#include "platform/Game.h"
#include "scene/Scene.h"
#include "scene/Node.h"
#include "scene/Camera.h"
#include "scene/Light.h"
#include "scene/Model.h"
#include "../material/Material.h"
#include "../material/MaterialParameter.h"
#include "../material/Texture.h"

namespace gameplay
{

static Activator *g_activator;

Activator::Activator()
{
}

Activator::~Activator()
{
}

Activator* Activator::getActivator()
{
    if (!g_activator) {
        g_activator = new Activator();
        g_activator->registerSystemTypes();
    }
    return g_activator;
}

Serializable *Activator::createObject(const std::string& className)
{
    Serializable *object = nullptr;
    std::map<std::string, CreateObjectCallback>::const_iterator itr = _classes.find(className);
    if (itr == _classes.end())
    {
        return nullptr;
    }
    
    CreateObjectCallback createObject = itr->second;
    if (createObject)
    {
        object = createObject();
    }
    return object;
}

std::string Activator::enumToString(const std::string& enumName, int value)
{
    std::map<std::string,std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr != _enums.end())
    {
        EnumToStringCallback enumToString = itr->second.first;
        if (enumToString)
        {
            return enumToString(enumName, value);
        }
    }
    return nullptr;
}

int Activator::enumParse(const std::string& enumName, const std::string& str)
{
    std::map<std::string,std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr != _enums.end())
    {
        EnumParseCallback enumParse = itr->second.second;
        if (enumParse)
        {
            return enumParse(enumName, str);
        }
    }
    return 0;
}

void Activator::registerType(const std::string&  className, CreateObjectCallback createObject)
{
    std::map<std::string,CreateObjectCallback>::const_iterator itr = _classes.find(className);
    if ( itr == _classes.end() )
    {
        _classes[className] = createObject;
    }
    else
    {
        GP_ERROR("className already registered:%s", className.c_str());
    }
}
    
void Activator::registerEnum(const std::string& enumName, EnumToStringCallback toString, EnumParseCallback parse)
{
    std::map<std::string, std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr == _enums.end())
    {
        _enums[enumName] = std::make_pair(toString, parse);
    }
    else
    {
        GP_ERROR("enumName already registered:%s", enumName.c_str());
    }
}

void Activator::registerSystemTypes()
{
    // Register engine types with
    this->registerType("gameplay::Game::Config", Game::Config::createObject);
    this->registerType("gameplay::Scene", Scene::createObject);
    this->registerType("gameplay::Node", Node::createObject);
    this->registerType("gameplay::Camera", Camera::createObject);
    this->registerType("gameplay::Light", Light::createObject);
    this->registerType("gameplay::Model", Model::createObject);
    this->registerType("gameplay::Material", Material::createObject);
    this->registerType("gameplay::Texture", Texture::createObject);
    this->registerType("gameplay::MaterialParameter", MaterialParameter::createObject);

    // Register engine enums
    this->registerEnum("gameplay::Camera::Mode", Camera::enumToString, Camera::enumParse);
    this->registerEnum("gameplay::Light::Type", Light::enumToString, Light::enumParse);
    this->registerEnum("gameplay::Light::Mode", Light::enumToString, Light::enumParse);
    this->registerEnum("gameplay::Light::Shadows", Light::enumToString, Light::enumParse);

    this->registerEnum("gameplay::Texture::Format", Texture::enumToString, Texture::enumParse);
    this->registerEnum("gameplay::Texture::Type", Texture::enumToString, Texture::enumParse);
    this->registerEnum("gameplay::Texture::Wrap", Texture::enumToString, Texture::enumParse);
    this->registerEnum("gameplay::Texture::Filter", Texture::enumToString, Texture::enumParse);

    this->registerEnum("gameplay::MaterialParameter::Type", MaterialParameter::enumToString, MaterialParameter::enumParse);
}

}
