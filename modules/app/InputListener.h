#ifndef INPUTLISTENER_H_
#define INPUTLISTENER_H_

namespace gameplay {
class InputListener {
public:
    /**
     * Keyboard callback on keyPress events.
     *
     * @param evt The key event that occurred.
     * @param key If evt is KEY_PRESS or KEY_RELEASE then key is the key code from Keyboard::Key.
     *            If evt is KEY_CHAR then key is the unicode value of the character.
     * 
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    virtual void keyEvent(Keyboard::KeyEvent evt, int key) {}

    /**
     * Touch callback on touch events.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @see Touch::TouchEvent
     */
    virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {}

    /**
     * Mouse callback on mouse events. If the game does not consume the mouse move event or left mouse click event
     * then it is interpreted as a touch event instead.
     *
     * @param evt The mouse event that occurred.
     * @param x The x position of the mouse in pixels. Left edge is zero.
     * @param y The y position of the mouse in pixels. Top edge is zero.
     * @param wheelDelta The number of mouse wheel ticks. Positive is up (forward), negative is down (backward).
     *
     * @return True if the mouse event is consumed or false if it is not consumed.
     *
     * @see Mouse::MouseEvent
     */
    virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta) { return false; }
    
    /**
     * Called when the game window has been resized.
     *
     * This method is called once the game window is created with its initial size
     * and then again any time the game window changes size.
     *
     * @param width The new game window width.
     * @param height The new game window height.
     */
    virtual void resizeEvent(unsigned int width, unsigned int height) {}




    /**
     * Gesture callback on Gesture::SWIPE events.
     *
     * @param x The x-coordinate of the start of the swipe.
     * @param y The y-coordinate of the start of the swipe.
     * @param direction The direction of the swipe
     *
     * @see Gesture::SWIPE_DIRECTION_UP
     * @see Gesture::SWIPE_DIRECTION_DOWN
     * @see Gesture::SWIPE_DIRECTION_LEFT
     * @see Gesture::SWIPE_DIRECTION_RIGHT
     */
    virtual void gestureSwipeEvent(int x, int y, int direction) {}

    /**
     * Gesture callback on Gesture::PINCH events.
     *
     * @param x The centroid x-coordinate of the pinch.
     * @param y The centroid y-coordinate of the pinch.
     * @param scale The scale of the pinch.
     */
    virtual void gesturePinchEvent(int x, int y, float scale) {}

    /**
     * Gesture callback on Gesture::LONG_TAP events.
     *
     * @param x The x-coordinate of the long tap.
     * @param y The y-coordinate of the long tap.
     * @param duration The duration of the long tap in ms.
     */
    virtual void gestureLongTapEvent(int x, int y, float duration) {}

    /**
     * Gesture callback on Gesture::TAP events.
     *
     * @param x The x-coordinate of the tap.
     * @param y The y-coordinate of the tap.
     */
    virtual void gestureTapEvent(int x, int y) {}

    /**
     * Gesture callback on Gesture::DRAG events.
     *
     * @param x The x-coordinate of the start of the drag event.
     * @param y The y-coordinate of the start of the drag event.
     */
    virtual void gestureDragEvent(int x, int y) {}

    /**
     * Gesture callback on Gesture::DROP events.
     *
     * @param x The x-coordinate of the drop event.
     * @param y The y-coordinate of the drop event.
     */
    virtual void gestureDropEvent(int x, int y) {}


    /**
     * Gamepad callback on gamepad events.  Override to receive Gamepad::CONNECTED_EVENT 
     * and Gamepad::DISCONNECTED_EVENT, and store the Gamepad* in order to poll it from update().
     *
     * @param evt The gamepad event that occurred.
     * @param gamepad The gamepad that generated the event.
     */
    //virtual void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);
};
}


#endif