#include "base/Base.h"
#include "Text.h"
#include "math/Matrix.h"
#include "scene/Scene.h"
#include "platform/Toolkit.h"

namespace gameplay
{
  
Text::Text() :
    _font(NULL), _drawFont(NULL), _text(""), _size(0), _width(0), _height(0), _wrap(true), _rightToLeft(false),
    _align(Text::ALIGN_TOP_LEFT), _clip(Rectangle(0, 0, 0, 0)),
    _opacity(1.0f), _color(Vector4::one())
{
}

Text::~Text()
{
    // _drawFont is a child of _font, so it should never be released
    SAFE_RELEASE(_font);
    _drawFont = NULL;
}
    
Text& Text::operator=(const Text& text)
{
    return *this;
}
    
Text* Text::create(const char* fontPath, const char* str, const Vector4& color, unsigned int size)
{
    GP_ASSERT(fontPath);
    GP_ASSERT(str);

    Font* font = Font::create(fontPath);
    if (size == 0)
    {
        size = font->_size;
    }

    unsigned int widthOut, heightOut;
    font->measureText(str, size, &widthOut, &heightOut);
    Text* text = new Text();
    text->_font = font;
    text->_drawFont = font;
    text->_text = str;
    text->_size = size;
    text->_width = (float)widthOut + 1;
    text->_height = (float)heightOut + 1;
    text->_color = color;

    return text;
}
    
Text* Text::create(Properties* properties)
{
    // Check if the Properties is valid and has a valid namespace.
    if (!properties || strcmp(properties->getNamespace(), "text") != 0)
    {
        GP_ERROR("Properties object must be non-null and have namespace equal to 'text'.");
        return NULL;
    }

    // Get font path.
    const char* fontPath = properties->getString("font");
    if (fontPath == NULL || strlen(fontPath) == 0)
    {
        GP_ERROR("Text is missing required font file path.");
        return NULL;
    }

    // Get text
    const char* text = properties->getString("text");
    if (text == NULL || strlen(text) == 0)
    {
        GP_ERROR("Text is missing required 'text' value.");
        return NULL;
    }

    // Get size
    int size = properties->getInt("size"); // Default return is 0 if a value doesn't exist
    if (size < 0)
    {
        GP_WARN("Text size must be a positive value, with zero being default font size. Using default font size.");
        size = 0;
    }

    // Get text color
    Vector4 color = Vector4::one();
    if (properties->exists("color"))
    {
        switch (properties->getType("color"))
        {
            case Properties::VECTOR3:
                color.w = 1.0f;
                properties->getVector3("color", (Vector3*)&color);
                break;
            case Properties::VECTOR4:
                properties->getVector4("color", &color);
                break;
            case Properties::STRING:
            default:
                properties->getColor("color", &color);
                break;
        }
    }

    // Create
    return Text::create(fontPath, text, color, size);
}
    
void Text::setText(const char* str)
{
    _text = str;
}

const char* Text::getText() const
{
    return _text.c_str();
}

unsigned int Text::getSize() const
{
    return _size;
}

void Text::setWidth(float width)
{
    _width = width;
}

float Text::getWidth() const
{
    return _width;
}

void Text::setHeight(float height)
{
    _height = height;
}

float Text::getHeight() const
{
    return _height;
}

void Text::setWrap(bool wrap)
{
    _wrap = wrap;
}

bool Text::getWrap() const
{
    return _wrap;
}
    
void Text::setRightToLeft(bool rightToLeft)
{
    _rightToLeft = rightToLeft;
}

bool Text::getRightToLeft() const
{
    return _rightToLeft;
}

void Text::setJustify(Justify align)
{
    _align = align;
}

Text::Justify Text::getJustify() const
{
    return _align;
}
    
void Text::setClip(const Rectangle& clip)
{
    _clip = clip;
}

const Rectangle& Text::getClip() const
{
    return _clip;
}

void Text::setOpacity(float opacity)
{
    _opacity = opacity;
}

float Text::getOpacity() const
{
    return _opacity;
}

void Text::setColor(const Vector4& color)
{
    _color = color;
}

const Vector4& Text::getColor() const
{
    return _color;
}
    
Drawable* Text::clone(NodeCloneContext& context)
{
    Text* textClone = new Text();
    textClone->_font = _font;
    _font->addRef();
    textClone->_drawFont = _drawFont;
    textClone->_text = _text;
    textClone->_size = _size;
    textClone->_width = _width;
    textClone->_height = _height;
    textClone->_wrap = _wrap;
    textClone->_rightToLeft = _rightToLeft;
    textClone->_align = _align;
    textClone->_clip = _clip;
    textClone->_opacity = _opacity;
    textClone->_color = _color;
    return textClone;
}

unsigned int Text::draw(RenderView* view)
{
    // Apply scene camera projection and translation offsets
    Rectangle viewport = Toolkit::cur()->getViewport();
    Vector3 position = Vector3::zero();
    
    // Font is always using a offset projection matrix to top-left. So we need to adjust it back to cartesian
    position.x += viewport.width / 2;
    position.y += viewport.height / 2;
    Rectangle clipViewport = _clip;
    if (_node && _node->getScene())
    {
        Camera* activeCamera = _node->getScene()->getActiveCamera();
        if (activeCamera)
        {
            Node* cameraNode = _node->getScene()->getActiveCamera()->getNode();
            if (cameraNode)
            {
                // Camera translation offsets
                position.x -= cameraNode->getTranslationWorld().x;
                position.y += cameraNode->getTranslationWorld().y - getHeight();
            }
        }
        
        // Apply node translation offsets
        Vector3 translation = _node->getTranslationWorld();
        position.x += translation.x;
        position.y -= translation.y;
    
        if (!clipViewport.isEmpty())
        {
            clipViewport.x += position.x;
            clipViewport.y += position.y;
        }
    }
    _drawFont->start();
    _drawFont->drawText(_text.c_str(), position.x, position.y,
                    Vector4(_color.x, _color.y, _color.z, _color.w * _opacity), _size
                    );
    _drawFont->finish();
    return 1;
}
    
int Text::getPropertyId(TargetType type, const char* propertyIdStr)
{
    GP_ASSERT(propertyIdStr);

    if (type == AnimationTarget::TRANSFORM)
    {
        if (strcmp(propertyIdStr, "ANIMATE_OPACITY") == 0)
        {
            return Text::ANIMATE_OPACITY;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_COLOR") == 0)
        {
            return Text::ANIMATE_COLOR;
        }
    }

    return AnimationTarget::getPropertyId(type, propertyIdStr);
}
    
unsigned int Text::getAnimationPropertyComponentCount(int propertyId) const
{
    switch (propertyId)
    {
        case ANIMATE_OPACITY:
            return 1;
        case ANIMATE_COLOR:
            return 4;
        default:
            return -1;
    }
}

void Text::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    GP_ASSERT(value);
    
    switch (propertyId)
    {
        case ANIMATE_OPACITY:
            value->setFloat(0, _opacity);
            break;
        case ANIMATE_COLOR:
            value->setFloat(0, _color.x);
            value->setFloat(1, _color.y);
            value->setFloat(2, _color.z);
            value->setFloat(3, _color.w);
            break;
        default:
            break;
    }
}

void Text::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
    GP_ASSERT(value);
    
    switch(propertyId)
    {
        case ANIMATE_OPACITY:
            setOpacity(Curve::lerp(blendWeight, _opacity, value->getFloat(0)));
            break;
        case ANIMATE_COLOR:
            setColor(Vector4(Curve::lerp(blendWeight, _color.x, value->getFloat(0)),
                             Curve::lerp(blendWeight, _color.x, value->getFloat(1)),
                             Curve::lerp(blendWeight, _color.x, value->getFloat(2)),
                             Curve::lerp(blendWeight, _color.x, value->getFloat(3))));
            break;
        default:
            break;
    }
}



Text::Justify Text::getJustify(const char* justify)
{
    if (!justify)
    {
        return Text::ALIGN_TOP_LEFT;
    }

    if (strcmpnocase(justify, "ALIGN_LEFT") == 0)
    {
        return Text::ALIGN_LEFT;
    }
    else if (strcmpnocase(justify, "ALIGN_HCENTER") == 0)
    {
        return Text::ALIGN_HCENTER;
    }
    else if (strcmpnocase(justify, "ALIGN_RIGHT") == 0)
    {
        return Text::ALIGN_RIGHT;
    }
    else if (strcmpnocase(justify, "ALIGN_TOP") == 0)
    {
        return Text::ALIGN_TOP;
    }
    else if (strcmpnocase(justify, "ALIGN_VCENTER") == 0)
    {
        return Text::ALIGN_VCENTER;
    }
    else if (strcmpnocase(justify, "ALIGN_BOTTOM") == 0)
    {
        return Text::ALIGN_BOTTOM;
    }
    else if (strcmpnocase(justify, "ALIGN_TOP_LEFT") == 0)
    {
        return Text::ALIGN_TOP_LEFT;
    }
    else if (strcmpnocase(justify, "ALIGN_VCENTER_LEFT") == 0)
    {
        return Text::ALIGN_VCENTER_LEFT;
    }
    else if (strcmpnocase(justify, "ALIGN_BOTTOM_LEFT") == 0)
    {
        return Text::ALIGN_BOTTOM_LEFT;
    }
    else if (strcmpnocase(justify, "ALIGN_TOP_HCENTER") == 0)
    {
        return Text::ALIGN_TOP_HCENTER;
    }
    else if (strcmpnocase(justify, "ALIGN_VCENTER_HCENTER") == 0)
    {
        return Text::ALIGN_VCENTER_HCENTER;
    }
    else if (strcmpnocase(justify, "ALIGN_BOTTOM_HCENTER") == 0)
    {
        return Text::ALIGN_BOTTOM_HCENTER;
    }
    else if (strcmpnocase(justify, "ALIGN_TOP_RIGHT") == 0)
    {
        return Text::ALIGN_TOP_RIGHT;
    }
    else if (strcmpnocase(justify, "ALIGN_VCENTER_RIGHT") == 0)
    {
        return Text::ALIGN_VCENTER_RIGHT;
    }
    else if (strcmpnocase(justify, "ALIGN_BOTTOM_RIGHT") == 0)
    {
        return Text::ALIGN_BOTTOM_RIGHT;
    }
    else
    {
        GP_WARN("Invalid alignment string: '%s'. Defaulting to ALIGN_TOP_LEFT.", justify);
    }

    // Default.
    return Text::ALIGN_TOP_LEFT;
}


}
