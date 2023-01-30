#ifndef GAME_H_
#define GAME_H_

#include "platform/Keyboard.h"
#include "platform/Mouse.h"
#include "platform/Touch.h"
#include "platform/Gesture.h"
//#include "Gamepad.h"
#include "audio/AudioController.h"
#include "animation/AnimationController.h"
#include "physics/PhysicsController.h"
#include "ai/AIController.h"
#include "scene/AudioListener.h"
#include "math/Rectangle.h"
#include "math/Vector4.h"
#include "platform/Toolkit.h"
#include "scene/Renderer.h"
#include "base/Serializable.h"
#include "InputListener.h"

#include <queue>

namespace gameplay
{

class ScriptController;
class RenderPipline;

/**
 * Defines the base class your game will extend for game initialization, logic and platform delegates.
 *
 * This represents a running cross-platform game application and provides an abstraction
 * to most typical platform functionality and events.
 *
 * @see http://gameplay3d.github.io/GamePlay/docs/file-formats.html#wiki-Game_Config
 */
class Game: public Toolkit, public InputListener
{
    friend class Platform;
    //friend class Gamepad;
    friend class ShutdownListener;

public:
    /**
     * Defines a splash screen.
     */
    class SplashScreen
    {
    public:
        std::string url;
        float duration;
    };

    /**
     * Game configuration.
     */
    class Config : public Serializable, public Ref
    {
    public:

        /**
         * Constructor
         */
        Config();

        /**
         * Destructor
         */
        ~Config();

        /**
         * @see Serializable::getClassName
         */
        std::string getClassName();

        /**
         * @see Serializable::onSerialize
         */
        void onSerialize(Serializer* serializer);

        /**
         * @see Serializable::onDeserialize
         */
        void onDeserialize(Serializer* serializer);

        /**
         * @see Activator::createObject
         */
        static Serializable *createObject();

        std::string title;
        int width;
        int height;
        bool fullscreen;
        bool vsync;
        size_t multisampling;
        bool validation;
        std::string homePath;
        std::vector<SplashScreen> splashScreens;
        std::string mainScene;
    };
    
    /**
     * The game states.
     */
    enum State
    {
        UNINITIALIZED,
        RUNNING,
        PAUSED
    };

    /**
     * Constructor.
     */
    Game();

    /**
     * Destructor.
     */
    virtual ~Game();

    /**
     * Gets the single instance of the game.
     * 
     * @return The single instance of the game.
     */
    static Game* getInstance();

    /**
     * Gets the total game time (in milliseconds). This is the total accumulated game time (unpaused).
     *
     * You would typically use things in your game that you want to stop when the game is paused.
     * This includes things such as game physics and animation.
     * 
     * @return The total game time (in milliseconds).
     */
    double getGameTime();

    /**
     * Gets the game state.
     *
     * @return The current game state.
     */
    inline State getState() const;

    /**
     * Determines if the game has been initialized.
     *
     * @return true if the game initialization has completed, false otherwise.
     */
    inline bool isInitialized() const;

    /**
     * Returns the game configuration object.
     *
     * This method returns a Properties object containing the contents
     * of the game.config file.
     *
     * @return The game configuration Properties object.
     */
    Properties* getConfig() const;

    /**
     * Called to initialize the game, and begin running the game.
     * 
     * @return Zero for normal termination, or non-zero if an error occurred.
     */
    int run();

    /**
     * Pauses the game after being run.
     */
    void pause();

    /**
     * Resumes the game after being paused.
     */
    void resume();

    /**
     * Exits the game.
     */
    void exit();

    /**
     * Platform frame delegate.
     *
     * This is called every frame from the platform.
     * This in turn calls back on the user implemented game methods: update() then render()
     */
    void frame();

    /**
     * Gets the current frame rate.
     * 
     * @return The current frame rate.
     */
    inline unsigned int getFrameRate() const;

    /**
     * Gets the game window width.
     * 
     * @return The game window width.
     */
    inline unsigned int getWidth() const;

    /**
     * Gets the game window height.
     * 
     * @return The game window height.
     */
    inline unsigned int getHeight() const;
    
    /**
     * Gets the aspect ratio of the window. (width / height)
     * 
     * @return The aspect ratio of the window.
     */
    inline float getAspectRatio() const;

    /**
     * Gets the game current viewport.
     *
     * The default viewport is Rectangle(0, 0, Game::getWidth(), Game::getHeight()).
     */
    inline const Rectangle& getViewport() const;

    /**
     * Sets the game current viewport.
     *
     * The x, y, width and height of the viewport must all be positive.
     *
     * viewport The custom viewport to be set on the game.
     */
    void setViewport(const Rectangle& viewport);


    /**
     * Gets whether mouse input is currently captured.
     *
     * @return is the mouse captured.
     */
    virtual bool isMouseCaptured();


    /**
     * Shows or hides the virtual keyboard (if supported).
     *
     * @param display true when virtual keyboard needs to be displayed; false otherwise.
     */
     virtual void displayKeyboard(bool display);

    
    /**
     * Gets the command line arguments.
     * 
     * @param argc The number of command line arguments.
     * @param argv The array of command line arguments.
     * @script{ignore}
     */
    void getArguments(int* argc, char*** argv) const;

    /**
     * Schedules a time event to be sent to the given TimeListener a given number of game milliseconds from now.
     * Game time stops while the game is paused. A time offset of zero will fire the time event in the next frame.
     * 
     * @param timeOffset The number of game milliseconds in the future to schedule the event to be fired.
     * @param timeListener The TimeListener that will receive the event.
     * @param cookie The cookie data that the time event will contain.
     * @script{ignore}
     */
    void schedule(float timeOffset, TimeListener* timeListener, void* cookie = 0);

    /**
     * Schedules a time event to be sent to the given TimeListener a given number of game milliseconds from now.
     * Game time stops while the game is paused. A time offset of zero will fire the time event in the next frame.
     * 
     * The given script function must take a single floating point number, which is the difference between the
     * current game time and the target time (see TimeListener::timeEvent). The function will be executed
     * in the context of the script envionrment that the schedule function was called from.
     * 
     * @param timeOffset The number of game milliseconds in the future to schedule the event to be fired.
     * @param function The script function that will receive the event.
     */
    void schedule(float timeOffset, const char* function);

    /**
     * Clears all scheduled time events.
     */
    void clearSchedule();

protected:

    /**
     * Initialize callback that is called just before the first frame when the game starts.
     */
    virtual void initialize();

    /**
     * Finalize callback that is called when the game on exits.
     */
    virtual void finalize();

    /**
     * Update callback for handling update routines.
     *
     * Called just before render, once per frame when game is running.
     * Ideal for non-render code and game logic such as input and animation.
     *
     * @param elapsedTime The elapsed game time.
     */
    virtual void update(float elapsedTime);

    /**
     * Render callback for handling rendering routines.
     *
     * Called just after update, once per frame when game is running.
     * Ideal for all rendering code.
     *
     * @param elapsedTime The elapsed game time.
     */
    virtual void render(float elapsedTime);


    RenderPipline* getRenderPipline();

    void setInputListener(InputListener *t) {
        _inputListener = t;
    }

private:

    struct ShutdownListener : public TimeListener
    {
        void timeEvent(long timeDiff, void* cookie);
    };

    /**
     * TimeEvent represents the event that is sent to TimeListeners as a result of calling Game::schedule().
     */
    class TimeEvent
    {
    public:

        TimeEvent(double time, TimeListener* timeListener, void* cookie);
        bool operator<(const TimeEvent& v) const;
        double time;
        TimeListener* listener;
        void* cookie;
    };

    /**
     * Constructor.
     *
     * @param copy The game to copy.
     */
    Game(const Game& copy);

    /**
     * Starts the game.
     */
    bool startup();

    /**
     * Shuts down the game.
     */
    void shutdown();

    /**
     * Fires the time events that were scheduled to be called.
     * 
     * @param frameTime The current game frame time. Used to determine which time events need to be fired.
     */
    void fireTimeEvents(double frameTime);

    /**
     * Loads the game configuration.
     */
    void loadConfig();
public:
    void keyEventInternal(Keyboard::KeyEvent evt, int key);
    void touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    bool mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
    void resizeEventInternal(unsigned int width, unsigned int height);
    void gestureSwipeEventInternal(int x, int y, int direction);
    void gesturePinchEventInternal(int x, int y, float scale);
    void gestureTapEventInternal(int x, int y);
    void gestureLongTapEventInternal(int x, int y, float duration);
    void gestureDragEventInternal(int x, int y);
    void gestureDropEventInternal(int x, int y);
    //void gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad);
private:
    bool _initialized;                          // If game has initialized yet.
    State _state;                               // The game state.
    unsigned int _pausedCount;                  // Number of times pause() has been called.
    static double _pausedTimeLast;              // The last time paused.
    static double _pausedTimeTotal;             // The total time paused.
    double _frameLastFPS;                       // The last time the frame count was updated.
    unsigned int _frameCount;                   // The current frame count.
    unsigned int _frameRate;                    // The current frame rate.
    unsigned int _width;                        // The game's display width.
    unsigned int _height;                       // The game's display height.
    Rectangle _viewport;                        // the games's current viewport.
    Vector4 _clearColor;                        // The clear color value last used for clearing the color buffer.
    float _clearDepth;                          // The clear depth value last used for clearing the depth buffer.
    int _clearStencil;                          // The clear stencil value last used for clearing the stencil buffer.
    Properties* _properties;                    // Game configuration properties object.
    AnimationController* _animationController;  // Controls the scheduling and running of animations.
    AudioController* _audioController;          // Controls audio sources that are playing in the game.
    PhysicsController* _physicsController;      // Controls the simulation of a physics scene and entities.
    AIController* _aiController;                // Controls AI simulation.
    AudioListener* _audioListener;              // The audio listener in 3D space.
    std::priority_queue<TimeEvent, std::vector<TimeEvent>, std::less<TimeEvent> >* _timeEvents;     // Contains the scheduled time events.
    ScriptController* _scriptController;        // Controls the scripting engine.
    ScriptTarget* _scriptTarget;                // Script target for the game
    RenderPipline* _renderPipline;
    InputListener* _inputListener;

    // Note: Do not add STL object member variables on the stack; this will cause false memory leaks to be reported.
    friend class ScreenDisplayer;
};

}

#include "Game.inl"

#endif
