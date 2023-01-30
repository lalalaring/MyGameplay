#ifndef Toolkit_H_
#define Toolkit_H_

#include "math/Rectangle.h"

namespace gameplay
{
    class Properties;
/**
 * Defines a interface to be scheduled and called back at a later time using Game::schedule().
 *
 * @script{ignore}
 */
class TimeListener
{
public:

    /**
     * Callback method that is called when the scheduled event is fired.
     * 
     * @param timeDiff The time difference between the current game time and the target time.
     *                 The time differences will always be non-negative because scheduled events will not fire early.
     * @param cookie The cookie data that was passed when the event was scheduled.
     */
    virtual void timeEvent(long timeDiff, void* cookie) = 0;
};
/**
 * Defines a gesture touch screen event.
 */
class Toolkit
{
protected:
    static Toolkit* g_instance;
public:
    static Toolkit* cur();
    
    /**
     * Gets the game current viewport.
     *
     * The default viewport is Rectangle(0, 0, Game::getWidth(), Game::getHeight()).
     */
    virtual const Rectangle& getViewport() const = 0;

    /**
     * Returns the game configuration object.
     *
     * This method returns a Properties object containing the contents
     * of the game.config file.
     *
     * @return The game configuration Properties object.
     */
    virtual Properties* getConfig() const = 0;


    /**
     * Shows or hides the virtual keyboard (if supported).
     *
     * @param display true when virtual keyboard needs to be displayed; false otherwise.
     */
     virtual void displayKeyboard(bool display) = 0;


     /**
     * Schedules a time event to be sent to the given TimeListener a given number of game milliseconds from now.
     * Game time stops while the game is paused. A time offset of zero will fire the time event in the next frame.
     * 
     * @param timeOffset The number of game milliseconds in the future to schedule the event to be fired.
     * @param timeListener The TimeListener that will receive the event.
     * @param cookie The cookie data that the time event will contain.
     * @script{ignore}
     */
    virtual void schedule(float timeOffset, TimeListener* timeListener, void* cookie = 0) = 0;


    /**
     * Clears all scheduled time events.
     */
    virtual void clearSchedule() = 0;

    /**
     * Gets the command line arguments.
     * 
     * @param argc The number of command line arguments.
     * @param argv The array of command line arguments.
     * @script{ignore}
     */
    virtual void getArguments(int* argc, char*** argv) const = 0;


    /**
     * Gets whether mouse input is currently captured.
     *
     * @return is the mouse captured.
     */
    virtual bool isMouseCaptured() = 0;


    /**
     * Gets the total game time (in milliseconds). This is the total accumulated game time (unpaused).
     *
     * You would typically use things in your game that you want to stop when the game is paused.
     * This includes things such as game physics and animation.
     * 
     * @return The total game time (in milliseconds).
     */
    virtual double getGameTime() = 0;

};

}

#endif
