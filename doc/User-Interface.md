The gameplay framework provides a set of user interface (UI) controls that can be used to create menus and HUDs. To add UI elements to your game, you'll create a form from a properties file and call update() and draw() on it. Other than adding listeners to buttons and modifying controls programmatically, there's not much code to write in your game. Most of the work of defining a form's look and feel happens in the .form and .theme files, as well as the texture atlas used by the theme.

## Creating a form

To create a form, pass a properties file to `Form::create()` to instantiate a form. The top-most namespace in the file must be named 'form'. The following properties are available for forms:

```
form <formID>
{
    // Form properties.
    theme                  = <Path to .theme file>
    layout                 = <Layout::Type constant>
    style                  = <styleID>
    position               = <x, y>
    alignment              = <Control::Alignment constant>
    size                   = <width, height>
    autoWidth              = <bool>
    autoHeight             = <bool>
    width                  = <width>
    height                 = <height>

    // All the Controls within this Form.
    container { }
    label { }
    textBox { }
    button { }
    checkBox { }
    radioButton { }
    slider { }
}
```

**Form property information:**

The following is a list of form properties, and their purpose:

<dl>
<dt>theme</dt>
  <dd>See "Creating a theme" below.</dd>
<dt>layout</dt>
  <dd>Member of Layout::Type enum.</dd>
<dt>style</dt>
  <dd>A style from the theme.</dd>
<dt>position</dt>
  <dd>The on-screen position, in pixels.</dd>
<dt>alignment</dt>
  <dd>Align the form's position within the bounds of the screen. Note the position property will be ignored if the alignment property has been set.</dd>
<dt>size</dt>
  <dd>The size of the form, in pixels.</dd>
<dt>autoWidth</dt>
  <dd>Use of this property will result in a form with a width spanning the entire display. Note the width property will be ignored if autoWidth has been set.</dd>
<dt>autoHeight</dt>
  <dd>Use of this property will result in a form with a height spanning the entire display. The height property will be ignored if autoHeight has been set.</dd>
<dt>width</dt>
  <dd>Can be used in place of size.</dd>
<dt>height</dt>
  <dd>Can be used in place of size.</dd>
</dl>

A style determines the look of a control and is defined in the theme file, detailed below. Position and size attributes are determined for controls using the same properties as listed above for forms. Controls can be aligned within their parent container by using the alignment property. Setting autoWidth or autoHeight to true will result in a control the width or height of its parent container. You can add controls to the form by placing namespaces within it. The available controls are:

<dl>
<dt>Container</dt>
  <dd>A container has all the same available properties as a form, except for 'theme'. You can add more controls within a container to group them together, and/or to apply a different layout type to a group of controls.</dd>
<dt>Label</dt>
  <dd>A simple text label. Available properties: 'style', 'position', 'alignment', 'size', 'autoWidth', 'autoHeight', and 'text'.</dd>
<dt>TextBox</dt>
  <dd>Editable text label. A TextBox control has the same available properties as label.</dd>
<dt>Button</dt>
  <dd>A button. A button control has the same available properties as label.</dd>
<dt>CheckBox</dt>
  <dd>A button that toggles between 'checked' and 'unchecked' states when tapped or clicked. A CheckBox has the same available properties as label, plus 'checked' for its starting state.</dd>
<dt>RadioButton</dt>
  <dd>RadioButton has the same available properties as CheckBox, with an additional property, 'group'. Only one radio button in a given group can be selected at a time.</dd>
<dt>Slider</dt>
  <dd>A marker that can slide along a track between its end-caps. A slider makes use of the following properties:</dd>
</dl>

```
slider <controlID>
{
    style = <styleID>      // A style from the theme.
    position = <x, y>      // Position of the control on-screen, in pixels.
    size = <width, height> // The size of the control, in pixels.
    min = <float>          // The value of the left- / bottom-most point on the slider.
    max = <float>          // The value of the right- / top-most point on the slider.
    value = <float>        // The default position of the marker.
    step = <float>         // If greater than 0, force the marker to snap to discrete
                           // multiples of 'step'.
    text = <string>        // Text to display above, below or alongside the slider
                           // (depending on the style).
}
```

## Creating a theme

A theme contains the information a form needs to determine the look of its controls. A theme has one property, 'texture', which points to a texture atlas containing the images used by the theme. Cursor images, skins, and lists of images used by controls are defined in their own namespaces. The rest of the theme consists of style namespaces. A style describes the border, margin, and padding of a control, what images, skins, and cursors are associated with a control, and font properties to apply to a control's text.

```
theme <themeID>
{
    texture = <Path to texture>
    cursor <cursorID>
    {
        region = <x, y, width, height>
        color = <#ffffffff>
    }
    imageList <imageID>
    {
        image checked
        {
            region = <x, y, width, height>
        }
        image unchecked
        {
            region = <x, y, width, height>
            color = <#fffffffff>
        }
        color = <#fffffffff>
    }
    skin <skinID>
    {
        border
        {
            top = <int>
            bottom = <int>
            left = <int>
            right = <int>
        }
        region = <x, y, width, height>
        color = <#ffffffff>
    }
    style <styleID>
    {
        margin
        {
            top = <int>
            bottom = <int>
            left = <int>
            right = <int>
        }
        padding
        {
            top = <int>
            bottom = <int>
            left = <int>
            right = <int>
        }
        stateNormal
        {
            skin = <skinID>
            imageList = <imageID>
            cursor = <cursorID>
            font = <Path to font>
            fontSize = <int>
            textColor = <#ffffffff>
            textAlignment = <Control::Alignmentconstant>
            rightToLeft = <bool>
            opacity = <float>
        }
        stateFocus
        {
            skin = <skinID>
            ...
        }
        stateActive
        {
            skin = <skinID>
            ...
        }
        stateDisabled
        {
            skin = <skinID>
            ...
        }
    }
}
```

### Theme property information

The following is a list of theme properties, and their sub-properties if applicable.

<dl>
<dt>texture</dt>
  <dd>The path to the texture atlas used by this theme.</dd>
<dt>cursor</dt>
  <dd>Describes a single image, to be used as a cursor.</dd>
</dl>

### Sub-properties of cursor

<dl>
<dt>region</dt>
  <dd>Region within the texture, in pixels.</dd>
<dt>color</dt>
  <dd>Blend color to apply to this cursor.</dd>
<dt>imageList</dt>
  <dd>A collection of images used by controls.</dd>
<dt>image</dt>
  <dd>A single image within the list.</dd>
<dt>region</dt>
  <dd>Region within the texture, in pixels.</dd>
<dt>color</dt>
  <dd>Optionally override image-list blend color.</dd>
<dt>color</dt>
  <dd>Default blend color for images that don't specify their own.</dd>
</dl>

<dl>
<dt>skin</dt>
  <dd>Defines the border and background of a control.</dd>
</dl>


### Sub-properties of skin

<dl>
<dt>border</dt>
  <dd>The corners and edges of the given region will be used as border sprites.</dd>
<dt>top</dt>
  <dd>Height of top border, top corners.</dd>
<dt>bottom</dt>
  <dd>Height of bottom border, bottom corners.</dd>
<dt>left</dt>
  <dd>Width of left border, left corners.</dd>
<dt>right</dt>
  <dd>Width of right border, right corners.</dd>
<dt>region</dt>
  <dd>Total container region including the entire border. A region within the texture, in pixels.</dd>
<dt>color</dt>
  <dd>The blend color to apply to this skin.</dd>
</dl>

**style:** A combination of theme attributes that can be applied to any control.

### Sub-properties of style

<dl>
<dt>margin</dt>
  <dd>Layouts may make use of a style's margin to put space between adjacent controls.</dd>
<dt>top</dt>
  <dd>Empty space above a control.</dd>
<dt>bottom</dt>
  <dd>Empty space below a control.</dd>
<dt>left</dt>
  <dd>Empty space left of a control.</dd>
<dt>right</dt>
  <dd>Empty space right of a control.</dd>
<dt>padding</dt>
  <dd>The space between a control's border and its content.</dd>
<dt>top</dt>
  <dd>Empty space between the top border and content.</dd>
<dt>bottom</dt>
  <dd>Empty space between the top border and content.</dd>
<dt>left</dt>
  <dd>Empty space between the left border and content.</dd>
<dt>right</dt>
  <dd>Empty space between the right border and content.</dd>
<dt>stateNormal</dt>
  <dd>Properties used when a control is in the normal state.</dd>
<dt>skin</dt>
  <dd>Skin to use for border and background sprites.</dd>
<dt>imageList</dt>
  <dd>Images to use for this state.</dd>
<dt>cursor</dt>
  <dd>Cursor to use when the mouse is over this control.</dd>
<dt>font</dt>
  <dd>Font to use for rendering text.</dd>
<dt>fontSize</dt>
  <dd>Size of text.</dd>
<dt>textColor</dt>
  <dd>Color of text.</dd>
<dt>textAlignment</dt>
  <dd>Align text within the control's content area.</dd>
<dt>rightToLeft</dt>
  <dd>Whether to draw text from right to left.</dd>
<dt>opacity</dt>
  <dd>Opacity to apply to all text/border/icon colors.</dd>
<dt>stateFocus</dt>
  <dd>Properties used when a control is in focus.</dd>

Same properties as stateNormal. Unspecified properties will inherit from stateNormal.</dd>
<dt>stateActive</dt>
  <dd>Properties used when a control is active. This is when a touch/mouse is down within the control. If not specified, the normal overlay will be used.

Same properties as stateNormal. Unspecified properties will inherit from stateNormal.</dd>
<dt>stateDisabled</dt>
  <dd>Properties used when a control is disabled. If not specified, the normal overlay will be used.

Same properties as stateNormal. Unspecified properties will inherit from stateNormal.</dd>
</dl>

The top-level property of a theme is the path to its texture atlas. This is a single image containing all the sprites used by the theme. The skin, cursor, and imageList namespaces within a theme file refer to regions of this image to use to represent their various states.

**Skin:** A rectangular area representing a border and background. A container namespace will specify the rectangular region and blend color as well as the border sizes. From this, the region will be divided into nine areas: four corners, four borders, and the center background area. Note that the top and bottom borders will be stretched horizontally; the left and right borders will stretch vertically; and the center of the container will stretch in both directions. The corners will never be stretched. It's perfectly valid to set any border size to 0.

<dl>
<dt>Cursor</dt>
  <dd>A single rectangular area representing a mouse cursor.
<dt>ImageList</dt>
  <dd>A collection of images used by controls. Images for multiple control types can be combined into one list. Controls use the following images:
<dt>CheckBox</dt>
  <dd>'checked', 'unchecked'
<dt>RadioButton</dt>
  <dd>'selected, 'unselected'
<dt>Slider</dt>
  <dd>'minCap', 'maxCap', 'track', 'marker'
<dt>TextBox</dt>
  <dd>'textCaret'
</dl>

Note that you may specify separate image lists for each state in a style.

## Adding a form to your game

Once you have a form, theme, and texture atlas, only a small amount of code is required in order to actually display your UI within your game. There are two options for displaying forms: two-dimensionally, where the form is drawn directly to the display and three-dimensionally, where the form is assigned to a node within the game's scene and displayed on a quad.

The 2-D case is simple. To initialize a form, pass the path to your .form file to `Form::create()` to be returned a pointer to your form. Now, simply call `Form::update()` on the form during your game's update() method, and call `Form::draw()` on the form during render() . See the section below on event handling to learn how to react to player input from within a form.

The 3-D method of drawing forms is somewhat more advanced. For starters, you'll need a scene with at least one node in it. Call `Node::setForm()` to attach the form to the node. This call will also generate a rectangular model with the dimensions of the form. Scale, rotate, and translate the node as necessary. Now, calling `Form::draw()` on the form will render the contents of the form into a framebuffer and use that framebuffer to texture the form's model.

## Event handling within forms

Controls will trigger events when the user interacts with them. You can handle these events by setting listeners on individual controls.

All controls can trigger the mouse / touch events PRESS , RELEASE , and CLICK . Sliders, check boxes, and radio buttons can also trigger a VALUE_CHANGED event so that you can update your game as a slider is moving or when a radio button becomes unselected. Finally, text boxes trigger a TEXT_CHANGED event any time a character is entered or deleted (but not when the cursor is moved within the text box). Use TEXT_CHANGED along with the getLastKeyPress() method on a TextBox to do things like accepting a player name when the return key is pressed.

To retrieve a control from your form, call `Form::getControl()` with the ID of the control you're looking for. Cast this to the correct control pointer type and then call addListener() on it. This method takes an object that implements `Control::Listener` as well as an int representing the events to listen for. You can bitwise-OR together event types. For example, the following code listens for PRESS and RELEASE events on a button:

```c++
Button* myButton = static_cast<Button*>(myForm->getControl("myButton");
myButton->addListener(this, Control::Listener::PRESS | Control::Listener::RELEASE);
```

In this example, the game itself implements `Control::Listener`. This is easy to do as there's only one method a listener needs to implement, which should look something like this:

```c++
void MyGame::controlEvent(Control* control, EventType evt)
{
    switch(evt)
    {
    case Control::Listener::PRESS:
        if (strcmp("myButton", control->getID()) == 0)
        {
            // Do something.
        }
        break;
    case Control::Listener::RELEASE:
        if (strcmp("myButton", control->getID()) == 0)
        {
            // Do something else.
        }
        break;
    }
}
```

Note that getControl() is also a method on Container. If multiple controls share the same ID but are children of separate containers, the parent container can be retrieved first and then `Container::getControl()` called to retrieve the specific control needed.

