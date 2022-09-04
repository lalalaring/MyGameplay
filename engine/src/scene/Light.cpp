#include "base/Base.h"
#include "Light.h"
#include "scene/Node.h"

#define LIGHT_COLOR Vector3::one()
#define LIGHT_RANGE 10.0f
#define LIGHT_ANGLE 30.0f
#define LIGHT_INTENSITY 1.0f

namespace gameplay
{

Light::Light() : _type(DIRECTIONAL), _node(NULL),
    _lighting(Lighting::eRealtime), _shadows(Shadows::eNone), _color(LIGHT_COLOR),  _intensity(LIGHT_INTENSITY)
{
    _directional = new Directional(LIGHT_COLOR);
}

Light::Light(Light::Type type, const Vector3& color) :
    _type(type), _node(NULL),
    _lighting(Lighting::eRealtime), _shadows(Shadows::eNone), _color(LIGHT_COLOR),  _intensity(LIGHT_INTENSITY)
{
    _directional = new Directional(color);
}

Light::Light(Light::Type type, const Vector3& color, float range) :
    _type(type), _node(NULL),
    _lighting(Lighting::eRealtime), _shadows(Shadows::eNone), _color(LIGHT_COLOR),  _intensity(LIGHT_INTENSITY)
{
    _point = new Point(color, range);
}

Light::Light(Light::Type type, const Vector3& color, float range, float innerAngle, float outerAngle) :
    _type(type), _node(NULL),
    _lighting(Lighting::eRealtime), _shadows(Shadows::eNone), _color(LIGHT_COLOR),  _intensity(LIGHT_INTENSITY)
{
    _spot = new Spot(color, range, innerAngle, outerAngle);
}

Light::~Light()
{
    switch (_type)
    {
    case DIRECTIONAL:
        SAFE_DELETE(_directional);
        break;
    case POINT:
        SAFE_DELETE(_point);
        break;
    case SPOT:
        SAFE_DELETE(_spot);
        break;
    default:
        GP_ERROR("Invalid light type (%d).", _type);
        break;
    }
}

Light* Light::createDirectional(const Vector3& color)
{
    return new Light(DIRECTIONAL, color);
}

Light* Light::createDirectional(float red, float green, float blue)
{
    return new Light(DIRECTIONAL, Vector3(red, green, blue));
}

Light* Light::createPoint(const Vector3& color, float range)
{
    return new Light(POINT, color, range);
}

Light* Light::createPoint(float red, float green, float blue, float range)
{
    return new Light(POINT, Vector3(red, green, blue), range);
}

Light* Light::createSpot(const Vector3& color, float range, float innerAngle, float outerAngle)
{
    return new Light(SPOT, color, range, innerAngle, outerAngle);
}

Light* Light::createSpot(float red, float green, float blue, float range, float innerAngle, float outerAngle)
{
    return new Light(SPOT, Vector3(red, green, blue), range, innerAngle, outerAngle);
}

Light* Light::create(Properties* properties)
{
    GP_ASSERT(properties);

    // Read light type
    std::string typeStr;
    if (properties->exists("type"))
        typeStr = properties->getString("type");
    Light::Type type;
    if (typeStr == "DIRECTIONAL")
    {
        type = Light::DIRECTIONAL;
    }
    else if (typeStr == "POINT")
    {
        type = Light::POINT;
    }
    else if (typeStr == "SPOT")
    {
        type = Light::SPOT;
    }
    else
    {
        GP_ERROR("Invalid 'type' parameter for light definition.");
        return NULL;
    }

    // Read common parameters
    Vector3 color;
    if (!properties->getVector3("color", &color))
    {
        GP_ERROR("Missing valid 'color' parameter for light definition.");
        return NULL;
    }

    // Read light-specific parameters
    Light* light = NULL;
    switch (type)
    {
    case DIRECTIONAL:
        light = createDirectional(color);
        break;
    case POINT:
        {
            float range = properties->getFloat("range");
            if (range == 0.0f)
            {
                GP_ERROR("Missing valid 'range' parameter for point light definition.");
                return NULL;
            }
            light = createPoint(color, range);
        }
        break;
    case SPOT:
            float range = properties->getFloat("range");
            if (range == 0.0f)
            {
                GP_ERROR("Missing valid 'range' parameter for spot light definition.");
                return NULL;
            }
            float innerAngle = properties->getFloat("innerAngle");
            if (innerAngle == 0.0f)
            {
                GP_ERROR("Missing valid 'innerAngle' parameter for spot light definition.");
                return NULL;
            }
            float outerAngle = properties->getFloat("outerAngle");
            if (outerAngle == 0.0f)
            {
                GP_ERROR("Missing valid 'outerAngle' parameter for spot light definition.");
                return NULL;
            }
            light = createSpot(color, range, innerAngle, outerAngle);
        break;
    }

    return light;
}

Light::Type Light::getLightType() const
{
    return _type;
}

Node* Light::getNode() const
{
    return _node;
}

void Light::setNode(Node* node)
{
    // Connect the new node.
    _node = node;
}

const Vector3& Light::getColor() const
{
    switch (_type)
    {
    case DIRECTIONAL:
        GP_ASSERT(_directional);
        return _directional->color;
    case POINT:
        GP_ASSERT(_point);
        return _point->color;
    case SPOT:
        GP_ASSERT(_spot);
        return _spot->color;
    default:
        GP_ERROR("Unsupported light type (%d).", _type);
        return Vector3::zero();

    }
}

void Light::setColor(const Vector3& color)
{
    switch (_type)
    {
    case DIRECTIONAL:
        GP_ASSERT(_directional);
        _directional->color = color;
        break;
    case POINT:
        GP_ASSERT(_point);
        _point->color = color;
        break;
    case SPOT:
        GP_ASSERT(_spot);
        _spot->color = color;
        break;
    default:
        GP_ERROR("Unsupported light type (%d).", _type);
        break;
    }
}

void Light::setColor(float red, float green, float blue)
{
    setColor(Vector3(red, green, blue));
}

float Light::getRange()  const
{
    GP_ASSERT(_type != DIRECTIONAL);

    switch (_type)
    {
    case POINT:
        GP_ASSERT(_point);
        return _point->range;
    case SPOT:
        GP_ASSERT(_spot);
        return _spot->range;
    default:
        GP_ERROR("Unsupported light type (%d).", _type);
        return 0.0f;
    }
}
    
void Light::setRange(float range)
{
    GP_ASSERT(_type != DIRECTIONAL);

    switch (_type)
    {
    case POINT:
        GP_ASSERT(_point);
        _point->range = range;
        _point->rangeInverse = 1.0f / range;
        break;
    case SPOT:
        GP_ASSERT(_spot);
        _spot->range = range;
        _spot->rangeInverse = 1.0f / range;
        break;
    default:
        GP_ERROR("Unsupported light type (%d).", _type);
        break;
    }

    if (_node)
        _node->setBoundsDirty();
}

float Light::getRangeInverse() const
{
    GP_ASSERT(_type != DIRECTIONAL);

    switch (_type)
    {
    case POINT:
        GP_ASSERT(_point);
        return _point->rangeInverse;
    case SPOT:
        GP_ASSERT(_spot);
        return _spot->rangeInverse;
    default:
        GP_ERROR("Unsupported light type (%d).", _type);
        return 0.0f;
    }
}
    
float Light::getInnerAngle()  const
{
    GP_ASSERT(_type == SPOT);

    return _spot->innerAngle;
}

void Light::setInnerAngle(float innerAngle)
{
    GP_ASSERT(_type == SPOT);

    _spot->innerAngle = innerAngle;
    _spot->innerAngleCos = cos(innerAngle);
}
    
float Light::getOuterAngle()  const
{
    GP_ASSERT(_type == SPOT);

    return _spot->outerAngle;
}

void Light::setOuterAngle(float outerAngle)
{
    GP_ASSERT(_type == SPOT);

    _spot->outerAngle = outerAngle;
    _spot->outerAngleCos = cos(outerAngle);

    if (_node)
        _node->setBoundsDirty();
}
    
float Light::getInnerAngleCos()  const
{
    GP_ASSERT(_type == SPOT);

    return _spot->innerAngleCos;
}
    
float Light::getOuterAngleCos()  const
{
    GP_ASSERT(_type == SPOT);

    return _spot->outerAngleCos;
}

Light* Light::clone(NodeCloneContext &context)
{
    Light* lightClone = NULL;
    switch (_type)
    {
    case DIRECTIONAL:
        lightClone = createDirectional(getColor());
        break;
    case POINT:
        lightClone = createPoint(getColor(), getRange());
        break;
    case SPOT:
        lightClone = createSpot(getColor(), getRange(), getInnerAngle(), getOuterAngle());
        break;
    default:
        GP_ERROR("Unsupported light type (%d).", _type);
        return NULL;
    }
    GP_ASSERT(lightClone);

    if (Node* node = context.findClonedNode(getNode()))
    {
        lightClone->setNode(node);
    }
    return lightClone;
}

Light::Directional::Directional(const Vector3& color)
    : color(color)
{
}

Light::Point::Point(const Vector3& color, float range)
    : color(color), range(range)
{
    rangeInverse = 1.0f / range;
}

Light::Spot::Spot(const Vector3& color, float range, float innerAngle, float outerAngle)
    : color(color), range(range), innerAngle(innerAngle), outerAngle(outerAngle)
{
    rangeInverse = 1.0f / range;
    innerAngleCos = cos(innerAngle);
    outerAngleCos = cos(outerAngle);
}



Serializable* Light::createObject()
{
    return new Light();
}

std::string Light::enumToString(const std::string& enumName, int value)
{
    if (enumName.compare("gameplay::Light::Type") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Type::DIRECTIONAL):
                return "Directional";
            case static_cast<int>(Type::POINT):
                return "Point";
            case static_cast<int>(Type::SPOT):
                return "Spot";
            default:
                return "Directional";
        }
    }
    else if(enumName.compare("gameplay::Light::Lighting") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Lighting::eRealtime):
                return "Realtime";
            case static_cast<int>(Lighting::eBaked):
                return "Baked";
            default:
                return "Realtime";
        }
    }
    else if(enumName.compare("gameplay::Light::Shadows") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Shadows::eNone):
                return "None";
            case static_cast<int>(Shadows::eHard):
                return "Hard";
            case static_cast<int>(Shadows::eSoft):
                return "Soft";
            default:
                return "None";
        }
    }
    return "";
}

int Light::enumParse(const std::string& enumName, const std::string& str)
{
    if (enumName.compare("gameplay::Light::Type") == 0)
    {
        if (str.compare("Directional") == 0)
            return static_cast<int>(Type::DIRECTIONAL);
        else if (str.compare("Point") == 0)
            return static_cast<int>(Type::POINT);
        else if (str.compare("Spot") == 0)
            return static_cast<int>(Type::SPOT);
    }
    else if (enumName.compare("gameplay::Light::Lighting") == 0)
    {
        if (str.compare("Realtime") == 0)
            return static_cast<int>(Lighting::eRealtime);
        else if (str.compare("Baked") == 0)
            return static_cast<int>(Lighting::eBaked);
    }
    else if (enumName.compare("gameplay::Light::Shadows") == 0)
    {
        if (str.compare("None") == 0)
            return static_cast<int>(Shadows::eNone);
        else if (str.compare("Hard") == 0)
            return static_cast<int>(Shadows::eHard);
        else if (str.compare("Soft") == 0)
            return static_cast<int>(Shadows::eSoft);
    }
    return static_cast<int>(Shadows::eNone);
}

std::string Light::getClassName()
{
    return "gameplay::Light";
}

void Light::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("type", "gameplay::Light::Type", static_cast<int>(_type), -1);
    serializer->writeColor("color", _color, LIGHT_COLOR);
    serializer->writeFloat("intensity", _intensity, LIGHT_INTENSITY);
    switch (_type)
    {
        case Type::POINT:
        {
            serializer->writeFloat("range", _point->range, LIGHT_RANGE);
            break;
        }

        case Type::SPOT:
        {
            serializer->writeFloat("range", _spot->range, LIGHT_RANGE);
            serializer->writeFloat("angle", _spot->outerAngle, LIGHT_ANGLE);\
            serializer->writeFloat("innerAngle", _spot->innerAngle, LIGHT_ANGLE);\
            break;
        }

        default:
            break;
    }

    serializer->writeEnum("lighting", "gameplay::Light::Lighting", static_cast<int>(_lighting), static_cast<int>(Lighting::eRealtime));
    serializer->writeEnum("shadows", "gameplay::Light::Shadows", static_cast<int>(_shadows), static_cast<int>(Shadows::eNone));
}

void Light::onDeserialize(Serializer* serializer)
{
    _type = static_cast<Light::Type>(serializer->readEnum("type", "gameplay::Light::Type", -1));
    _color = serializer->readColor("color", LIGHT_COLOR);
    _intensity = serializer->readFloat("intensity", LIGHT_INTENSITY);
    switch(_type)
    {
        case Type::POINT:
        {
            _point->range = serializer->readFloat("range", LIGHT_RANGE);
            break;
        }

        case Type::SPOT:
        {
            _spot->range = serializer->readFloat("range", LIGHT_RANGE);
            _spot->outerAngle = serializer->readFloat("angle", LIGHT_ANGLE);
            _spot->outerAngleCos = cos(_spot->outerAngle);
            _spot->innerAngle = serializer->readFloat("innerAngle", LIGHT_ANGLE);
            _spot->innerAngleCos = cos(_spot->innerAngle);
            break;
        }

        default:
            break;
    }
    _lighting = static_cast<Light::Lighting>(serializer->readEnum("lighting", "gameplay::Light::Lighting", static_cast<int>(Lighting::eRealtime)));
    _shadows = static_cast<Light::Shadows>(serializer->readEnum("shadows", "gameplay::Light::Shadows", static_cast<int>(Shadows::eNone)));
}

}
