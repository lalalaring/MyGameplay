By creating your game and extending `gameplay::Game`, you'll be able to add all the required handlers of input events. Additionally, there are methods on `gameplay::Game` to poll for the current sensor data. This architecture insulates you, as a developer, from the platform-specific details on handling keyboard, touch and mouse events, and from polling the accelerometer state. The following illustrates overridden methods to handle input events:

```c++
#include "gameplay.h"
using namespace gameplay;
class MyGame : public Game
{
    // Input events
    void keyEvent(Keyboard::KeyEvent evt, int key);
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);
    
    // Input methods
    void displayKeyboard(bool display);
    void registerGesture(Gesture::GestureEvent evt);
    void getAccelerometerValues(float* pitch, float* roll);
    // ...
};
```

## Mouse

`Game::mouseEvent()` is called when a mouse event occurs.

Mouse events that are not consumed will be interpreted as a touch event. You can consume a mouse event by overridding `Game::mouseEvent()` and returning true. This gives you the option to uniquely handle mouse events from touch events. `Game::mouseEvent()` returns false by default.

Note that some mobile devices can use a Bluetooth mouse.

### Mouse Capture

`Game::setMouseCaptured()` can be used to enable mouse capture. While mouse capture is enabled, all mouse move events will then be delivered as deltas instead of absolute positions.

`Game::setCursorVisible()` can be used to hide the mouse cursor.

## Touch

`Game::touchEvent()` is called when a touch event occurs. `x` and `y` are screen coordinates where the top left is `0,0`. Some platforms may allow you to touch outside the bounds of the screen so negative `x` and `y` values are possible.

### Multi-touch

You can enable multi-touch using `Game::setMultiTouch()`. The `Game::touchEvent()` parameter `contactIndex` is used to differentiate the touch contacts. Do not assume that the `contactIndex` values are sequential.

## Keyboard

`Game::keyEvent()` is called when a keyboard event occurs.

### Showing the virtual keyboard

You can call `Game::displayKeyboard()` to show or hide a virtual keyboard for platforms that support it.

## Gestures

Some platforms support gestures. `Game::isGestureSupported()` can be used to determine which gestures are supported.

`Game::registerGesture()` is used to register for a type of gesture. Once a gesture is registered, you will receive callbacks via `Game::gestureSwipeEvent()`, `Game::gesturePinchEvent()` or `Game::gestureTapEvent()`.

## Accelerometer

You can call `Game::getAccelerometerValues()` and pass in pointers to parameters that will be populated with the current sensor values for the accelerometer.

Despite its name, implementations of `Game::getAccelerometerValues()` on the various platforms are
at liberty to utilize data from other sensors as well. Gyro data may be incorporated _in fusion with_ accelerometer data to improve transient response for a better indication of device orientation.
Therefore `Game::getAccelerometerValues()` is to be used as an indicator of device orientation
rather than strictly net acceleration. For games that need to distinguish between acceleration
and device rotation rate, the current plan is to introduce separate functions for each.

## Gamepad

You can listen for gamepad events via `Game::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)`.  This will return you a pointer to a `gameplay::Gamepad` instance that has been connected or disconnected.  You can then poll the gamepad's state each frame within your game's `update()` method.

You can also use a game.config file to define one or more virtual gamepads from a UI form.  These virtual gamepads are typically used on mobile devices when a gamepad is not connected -- instead, a touch screen is used to interact with the form.  The form contains joystick and button controls with data bindings to the appropriate button mappings defined in the Gamepad class.

Ex. within game.config:

```c++
gamepad playerOne
{
    form = res/common/gamepadP1.form
}

gamepad playerTwo
{
    form = res/common/gamepadP2.form
}
```

This will create two gamepads at startup.  A connected event will be triggered for each virtual gamepad when it is created.  Both the Character and Racer samples make use of gamepad events, using virtual gamepads if no physical device is available, so you can take a look at their source code to see how we're currently handling input.

### Gamepads/Controllers supported (and tested)

Gamepads are supported on all platforms except iOS (coming soon)

* **Linux** - XBox360, PlayStation3, Logitech F310/F510/F710
* **Windows** - XBox 360, Logitech F310/F510/F710 
* **MacOSX** - XBox 360, PlayStation3, Logitech F310/F510
* **Android** - XBox360, PlayStation3, Logitech F310/F510/F710, MOGA, SteelSeries
* **iOS** - Coming soon...
