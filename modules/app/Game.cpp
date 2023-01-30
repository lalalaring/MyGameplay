#include "base/Base.h"
#include "app/Game.h"
#include "Platform.h"
#include "base/FileSystem.h"
#include "render/FrameBuffer.h"
#include "ui/ControlFactory.h"
#include "ui/Theme.h"
#include "ui/Form.h"
#include "render/RenderPipline.h"
#include "script/ScriptController.h"
#include "base/SerializerManager.h"
#include "render/GLRenderer.h"

#define SPLASH_DURATION     2.0f


// Graphics
#define GP_GRAPHICS_WIDTH                           1280
#define GP_GRAPHICS_HEIGHT                          720
#define GP_GRAPHICS_FULLSCREEN                      false
#define GP_GRAPHICS_VSYNC                           true
#define GP_GRAPHICS_MULTISAMPLING                   1
#define GP_GRAPHICS_VALIDATION                      false


/** @script{ignore} */
ALenum __al_error_code = AL_NO_ERROR;

extern gameplay::Renderer* g_rendererInstance;

namespace gameplay
{

static Game* __gameInstance = NULL;
double Game::_pausedTimeLast = 0.0;
double Game::_pausedTimeTotal = 0.0;


void regiseterSerializer() {
    SerializerManager *mgr = SerializerManager::getActivator();
    mgr->registerType("gameplay::Game::Config", Game::Config::createObject);
    mgr->registerType("gameplay::Scene", Scene::createObject);
    mgr->registerType("gameplay::Node", Node::createObject);
    mgr->registerType("gameplay::Camera", Camera::createObject);
    mgr->registerType("gameplay::Light", Light::createObject);
    mgr->registerType("gameplay::Model", Model::createObject);
    mgr->registerType("gameplay::Material", Material::createObject);
    mgr->registerType("gameplay::Texture", Texture::createObject);
    mgr->registerType("gameplay::MaterialParameter", MaterialParameter::createObject);

    // Register engine enums
    mgr->registerEnum("gameplay::Camera::Mode", Camera::enumToString, Camera::enumParse);
    mgr->registerEnum("gameplay::Light::Type", Light::enumToString, Light::enumParse);
    mgr->registerEnum("gameplay::Light::Mode", Light::enumToString, Light::enumParse);
    mgr->registerEnum("gameplay::Light::Shadows", Light::enumToString, Light::enumParse);

    mgr->registerEnum("gameplay::Texture::Format", Texture::enumToString, Texture::enumParse);
    mgr->registerEnum("gameplay::Texture::Type", Texture::enumToString, Texture::enumParse);
    mgr->registerEnum("gameplay::Texture::Wrap", Texture::enumToString, Texture::enumParse);
    mgr->registerEnum("gameplay::Texture::Filter", Texture::enumToString, Texture::enumParse);

    mgr->registerEnum("gameplay::MaterialParameter::Type", MaterialParameter::enumToString, MaterialParameter::enumParse);
}

/**
* @script{ignore}
*/
class GameScriptTarget : public ScriptTarget
{
    friend class Game;

    GP_SCRIPT_EVENTS_START();
    GP_SCRIPT_EVENT(initialize, "");
    GP_SCRIPT_EVENT(finalize, "");
    GP_SCRIPT_EVENT(update, "f");
    GP_SCRIPT_EVENT(render, "f");
    GP_SCRIPT_EVENT(resizeEvent, "ii");
    GP_SCRIPT_EVENT(keyEvent, "[Keyboard::KeyEvent]i");
    GP_SCRIPT_EVENT(touchEvent, "[Touch::TouchEvent]iiui");
    GP_SCRIPT_EVENT(mouseEvent, "[Mouse::MouseEvent]iii");
    GP_SCRIPT_EVENT(gestureSwipeEvent, "iii");
    GP_SCRIPT_EVENT(gesturePinchEvent, "iif");
    GP_SCRIPT_EVENT(gestureTapEvent, "ii");
    GP_SCRIPT_EVENT(gestureLongTapevent, "iif");
    GP_SCRIPT_EVENT(gestureDragEvent, "ii");
    GP_SCRIPT_EVENT(gestureDropEvent, "ii");
    GP_SCRIPT_EVENT(gamepadEvent, "[Gamepad::GamepadEvent]<Gamepad>");
    GP_SCRIPT_EVENTS_END();

public:

    GameScriptTarget()
    {
        GP_REGISTER_SCRIPT_EVENTS();
    }

    const char* getTypeName() const
    {
        return "GameScriptTarget";
    }
};

Game::Game()
    : _initialized(false), _state(UNINITIALIZED), _pausedCount(0),
      _frameLastFPS(0), _frameCount(0), _frameRate(0), _width(0), _height(0),
      _clearDepth(1.0f), _clearStencil(0), _properties(NULL),
      _animationController(NULL), _audioController(NULL),
      _physicsController(NULL), _aiController(NULL), _audioListener(NULL),
      _timeEvents(NULL), _scriptController(NULL), _scriptTarget(NULL), _renderPipline(NULL),
      _inputListener(NULL)
{
    GP_ASSERT(__gameInstance == NULL);

    __gameInstance = this;
    _timeEvents = new std::priority_queue<TimeEvent, std::vector<TimeEvent>, std::less<TimeEvent> >();

    Toolkit::g_instance = this;

    regiseterSerializer();
    setInputListener(this);
}

Game::~Game()
{
    SAFE_DELETE(_scriptTarget);
	SAFE_DELETE(_scriptController);

    // Do not call any virtual functions from the destructor.
    // Finalization is done from outside this class.
    SAFE_DELETE(_timeEvents);
#ifdef GP_USE_MEM_LEAK_DETECTION
    Ref::printLeaks();
    printMemoryLeaks();
#endif

    __gameInstance = NULL;
}

Game* Game::getInstance()
{
    GP_ASSERT(__gameInstance);
    return __gameInstance;
}

void Game::initialize()
{
    // stub
}

void Game::finalize()
{
    // stub
}

void Game::update(float elapsedTime)
{
    // stub
}

void Game::render(float elapsedTime)
{
    // stub
}

double Game::getGameTime()
{
    return System::currentTimeMillis() - _pausedTimeTotal;
}

int Game::run()
{
    if (_state != UNINITIALIZED)
        return -1;

    loadConfig();

    _width = 1024;// Platform::getDisplayWidth();
    _height = 768;// Platform::getDisplayHeight();

    // Start up game systems.
    if (!startup())
    {
        shutdown();
        return -2;
    }

    return 0;
}

bool Game::startup()
{
    if (_state != UNINITIALIZED)
        return false;

    g_rendererInstance = new GLRenderer();
    _renderPipline = new RenderPipline(Renderer::cur());

    setViewport(Rectangle(0.0f, 0.0f, (float)_width, (float)_height));
    //RenderState::initialize();
    //FrameBuffer::initialize();

    _animationController = new AnimationController();
    _animationController->initialize();

    _audioController = new AudioController();
    _audioController->initialize();

    _physicsController = new PhysicsController();
    _physicsController->initialize();

    _aiController = new AIController();
    _aiController->initialize();

    _scriptController = new ScriptController();
    _scriptController->initialize();

    _audioListener = new AudioListener();

    // Load any gamepads, ui or physical.
    //loadGamepads();

    // Set script handler
    if (_properties)
    {
        const char* scriptPath = _properties->getString("script");
        if (scriptPath)
        {
            _scriptTarget = new GameScriptTarget();
            _scriptTarget->addScript(scriptPath);
        }
        else
        {
            // Use the older scripts namespace for loading individual global script callback functions.
            Properties* sns = _properties->getNamespace("scripts", true);
            if (sns)
            {
                _scriptTarget = new GameScriptTarget();

                // Define a macro to simplify defining the following script callback registrations
                #define GP_REG_GAME_SCRIPT_CB(e) if (sns->exists(#e)) _scriptTarget->addScriptCallback(GP_GET_SCRIPT_EVENT(GameScriptTarget, e), sns->getString(#e))

                // Register all supported script callbacks if they are defined
                GP_REG_GAME_SCRIPT_CB(initialize);
                GP_REG_GAME_SCRIPT_CB(finalize);
                GP_REG_GAME_SCRIPT_CB(update);
                GP_REG_GAME_SCRIPT_CB(render);
                GP_REG_GAME_SCRIPT_CB(resizeEvent);
                GP_REG_GAME_SCRIPT_CB(keyEvent);
                GP_REG_GAME_SCRIPT_CB(touchEvent);
                GP_REG_GAME_SCRIPT_CB(mouseEvent);
                GP_REG_GAME_SCRIPT_CB(gestureSwipeEvent);
                GP_REG_GAME_SCRIPT_CB(gesturePinchEvent);
                GP_REG_GAME_SCRIPT_CB(gestureTapEvent);
                GP_REG_GAME_SCRIPT_CB(gestureLongTapevent);
                GP_REG_GAME_SCRIPT_CB(gestureDragEvent);
                GP_REG_GAME_SCRIPT_CB(gestureDropEvent);
                GP_REG_GAME_SCRIPT_CB(gamepadEvent);
            }
        }
    }

    _state = RUNNING;

    return true;
}

void Game::shutdown()
{
    // Call user finalization.
    if (_state != UNINITIALIZED)
    {
        GP_ASSERT(_animationController);
        GP_ASSERT(_audioController);
        GP_ASSERT(_physicsController);
        GP_ASSERT(_aiController);

        //Platform::signalShutdown();

		// Call user finalize
        finalize();

        // Call script finalize
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, finalize));

        // Destroy script target so no more script events are fired
        SAFE_DELETE(_scriptTarget);

		// Shutdown scripting system first so that any objects allocated in script are released before our subsystems are released
		_scriptController->finalize();

        /*unsigned int gamepadCount = Gamepad::getGamepadCount();
        for (unsigned int i = 0; i < gamepadCount; i++)
        {
            Gamepad* gamepad = Gamepad::getGamepad(i, false);
            SAFE_DELETE(gamepad);
        }*/

        _renderPipline->finalize();
        SAFE_DELETE(_renderPipline);

        _animationController->finalize();
        SAFE_DELETE(_animationController);

        _audioController->finalize();
        SAFE_DELETE(_audioController);

        _physicsController->finalize();
        SAFE_DELETE(_physicsController);
        _aiController->finalize();
        SAFE_DELETE(_aiController);
        
        ControlFactory::finalize();

        Theme::finalize();

        // Note: we do not clean up the script controller here
        // because users can call Game::exit() from a script.

        SAFE_DELETE(_audioListener);

        //FrameBuffer::finalize();
        //RenderState::finalize();

        SAFE_DELETE(_properties);

		_state = UNINITIALIZED;
    }
}

void Game::pause()
{
    if (_state == RUNNING)
    {
        GP_ASSERT(_animationController);
        GP_ASSERT(_audioController);
        GP_ASSERT(_physicsController);
        GP_ASSERT(_aiController);
        _state = PAUSED;
        _pausedTimeLast = System::currentTimeMillis();
        _animationController->pause();
        _audioController->pause();
        _physicsController->pause();
        _aiController->pause();
    }

    ++_pausedCount;
}

void Game::resume()
{
    if (_state == PAUSED)
    {
        --_pausedCount;

        if (_pausedCount == 0)
        {
            GP_ASSERT(_animationController);
            GP_ASSERT(_audioController);
            GP_ASSERT(_physicsController);
            GP_ASSERT(_aiController);
            _state = RUNNING;
            _pausedTimeTotal += System::currentTimeMillis() - _pausedTimeLast;
            _animationController->resume();
            _audioController->resume();
            _physicsController->resume();
            _aiController->resume();
        }
    }
}

void Game::exit()
{
    // Only perform a full/clean shutdown if GP_USE_MEM_LEAK_DETECTION is defined.
	// Every modern OS is able to handle reclaiming process memory hundreds of times
	// faster than it would take us to go through every pointer in the engine and
	// release them nicely. For large games, shutdown can end up taking long time,
    // so we'll just call ::exit(0) to force an instant shutdown.

#ifdef GP_USE_MEM_LEAK_DETECTION

    // Schedule a call to shutdown rather than calling it right away.
	// This handles the case of shutting down the script system from
	// within a script function (which can cause errors).
	static ShutdownListener listener;
	schedule(0, &listener);

#else

    // End the process immediately without a full shutdown
    ::exit(0);

#endif
}


void Game::frame()
{
    if (!_initialized)
    {
        // Perform lazy first time initialization
        initialize();
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, initialize));
        _initialized = true;

        // Fire first game resize event
        resizeEventInternal(_width, _height);
    }

	static double lastFrameTime = Game::getGameTime();
	double frameTime = getGameTime();

    // Fire time events to scheduled TimeListeners
    fireTimeEvents(frameTime);

    if (_state == Game::RUNNING)
    {
        GP_ASSERT(_animationController);
        GP_ASSERT(_audioController);
        GP_ASSERT(_physicsController);
        GP_ASSERT(_aiController);

        // Update Time.
        float elapsedTime = (frameTime - lastFrameTime);
        lastFrameTime = frameTime;

        // Update the scheduled and running animations.
        _animationController->update(elapsedTime);

        // Update the physics.
        _physicsController->update(elapsedTime);

        // Update AI.
        _aiController->update(elapsedTime);

        // Update gamepads.
        //Gamepad::updateInternal(elapsedTime);

        // Application Update.
        update(elapsedTime);

        // Update forms.
        Form::updateInternal(elapsedTime);

        // Run script update.
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, update), elapsedTime);

        // Audio Rendering.
        _audioController->update(elapsedTime);

        // Graphics Rendering.
        render(elapsedTime);

        // Run script render.
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, render), elapsedTime);

        // Update FPS.
        ++_frameCount;
        if ((Game::getGameTime() - _frameLastFPS) >= 1000)
        {
            _frameRate = _frameCount;
            _frameCount = 0;
            _frameLastFPS = getGameTime();
        }
    }
	else if (_state == Game::PAUSED)
    {
        // Update gamepads.
        //Gamepad::updateInternal(0);

        // Application Update.
        update(0);

        // Update forms.
        Form::updateInternal(0);

        // Script update.
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, update), 0);

        // Graphics Rendering.
        render(0);

        // Script render.
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, render), 0);
    }
}

RenderPipline* Game::getRenderPipline() {
    return _renderPipline;
}

void Game::setViewport(const Rectangle& viewport)
{
    _viewport = viewport;
    _renderPipline->getRenderer()->setViewport((int)viewport.x, (int)viewport.y, (int)viewport.width, (int)viewport.height);
}

//AudioListener* Game::getAudioListener()
//{
//    if (_audioListener == NULL)
//    {
//        _audioListener = new AudioListener();
//    }
//    return _audioListener;
//}

void Game::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    if (Form::keyEventInternal(evt, key)) {
        return;
    }
    if (_inputListener) _inputListener->keyEvent(evt, key);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, keyEvent), evt, key);
}

void Game::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (Form::touchEventInternal(evt, x, y, contactIndex)) {
        return;
    }
    if (_inputListener) _inputListener->touchEvent(evt, x, y, contactIndex);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, touchEvent), evt, x, y, contactIndex);
}

bool Game::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (Form::mouseEventInternal(evt, x, y, wheelDelta))
        return true;

    if (_inputListener && _inputListener->mouseEvent(evt, x, y, wheelDelta))
        return true;

    if (_scriptTarget)
        return _scriptTarget->fireScriptEvent<bool>(GP_GET_SCRIPT_EVENT(GameScriptTarget, mouseEvent), evt, x, y, wheelDelta);

    return false;
}

void Game::resizeEventInternal(unsigned int width, unsigned int height)
{
    // Update the width and height of the game
    if (_width != width || _height != height)
    {
        _width = width;
        _height = height;
        if (_inputListener) _inputListener->resizeEvent(width, height);
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, resizeEvent), width, height);
    }
    Form::resizeEventInternal(width, height);
}

void Game::gestureSwipeEventInternal(int x, int y, int direction)
{
    if (_inputListener) _inputListener->gestureSwipeEvent(x, y, direction);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureSwipeEvent), x, y, direction);
}

void Game::gesturePinchEventInternal(int x, int y, float scale)
{
    if (_inputListener) _inputListener->gesturePinchEvent(x, y, scale);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gesturePinchEvent), x, y, scale);
}

void Game::gestureTapEventInternal(int x, int y)
{
    if (_inputListener) _inputListener->gestureTapEvent(x, y);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureTapEvent), x, y);
}

void Game::gestureLongTapEventInternal(int x, int y, float duration)
{
    if (_inputListener) _inputListener->gestureLongTapEvent(x, y, duration);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureLongTapevent), x, y, duration);
}

void Game::gestureDragEventInternal(int x, int y)
{
    if (_inputListener) _inputListener->gestureDragEvent(x, y);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureDragEvent), x, y);
}

void Game::gestureDropEventInternal(int x, int y)
{
    if (_inputListener) _inputListener->gestureDropEvent(x, y);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureDropEvent), x, y);
}

//void Game::gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad)
//{
//    if (_inputListener) _inputListener->gamepadEvent(evt, gamepad);
//    if (_scriptTarget)
//        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gamepadEvent), evt, gamepad);
//}

void Game::schedule(float timeOffset, TimeListener* timeListener, void* cookie)
{
    GP_ASSERT(_timeEvents);
    TimeEvent timeEvent(getGameTime() + timeOffset, timeListener, cookie);
    _timeEvents->push(timeEvent);
}

void Game::schedule(float timeOffset, const char* function)
{
    _scriptController->schedule(timeOffset, function);
}

void Game::clearSchedule()
{
    SAFE_DELETE(_timeEvents);
    _timeEvents = new std::priority_queue<TimeEvent, std::vector<TimeEvent>, std::less<TimeEvent> >();
}

void Game::fireTimeEvents(double frameTime)
{
    while (_timeEvents->size() > 0)
    {
        const TimeEvent* timeEvent = &_timeEvents->top();
        if (timeEvent->time > frameTime)
        {
            break;
        }
        if (timeEvent->listener)
        {
            timeEvent->listener->timeEvent(frameTime - timeEvent->time, timeEvent->cookie);
        }
        _timeEvents->pop();
    }
}

Game::TimeEvent::TimeEvent(double time, TimeListener* timeListener, void* cookie)
    : time(time), listener(timeListener), cookie(cookie)
{
}

bool Game::TimeEvent::operator<(const TimeEvent& v) const
{
    // The first element of std::priority_queue is the greatest.
    return time > v.time;
}

Properties* Game::getConfig() const
{
    if (_properties == NULL)
        const_cast<Game*>(this)->loadConfig();

    return _properties;
}

void Game::loadConfig()
{
    if (_properties == NULL)
    {
        // Try to load custom config from file.
        if (FileSystem::fileExists("game.config"))
        {
            _properties = Properties::create("game.config");

            // Load filesystem aliases.
            Properties* aliases = _properties->getNamespace("aliases", true);
            if (aliases)
            {
                FileSystem::loadResourceAliases(aliases);
            }
        }
        else
        {
            // Create an empty config
            _properties = new Properties();
        }
    }
}

void Game::ShutdownListener::timeEvent(long timeDiff, void* cookie)
{
	Game::getInstance()->shutdown();
}


Game::Config::Config() :
    title(""),
    width(GP_GRAPHICS_WIDTH),
    height(GP_GRAPHICS_HEIGHT),
    fullscreen(GP_GRAPHICS_FULLSCREEN),
    vsync(GP_GRAPHICS_VSYNC),
    multisampling(GP_GRAPHICS_MULTISAMPLING),
    validation(GP_GRAPHICS_VALIDATION),
    homePath(GP_ENGINE_HOME_PATH),
    mainScene("main.scene")
{
}

Game::Config::~Config()
{
}

Serializable* Game::Config::createObject()
{
    return new Game::Config();
}

std::string Game::Config::getClassName()
{
    return "gameplay::Game::Config";
}

void Game::Config::onSerialize(Serializer* serializer)
{
    serializer->writeString("title", title.c_str(), "");
    serializer->writeInt("width", width, 0);
    serializer->writeInt("height", height, 0);
    serializer->writeBool("fullscreen", fullscreen, false);
    serializer->writeBool("vsync", vsync, false);
    serializer->writeInt("multisampling", (uint32_t)multisampling, 0);
    serializer->writeBool("validation", validation, false);
    serializer->writeString("homePath", homePath.c_str(), GP_ENGINE_HOME_PATH);
    serializer->writeList("splashScreens", splashScreens.size());
    for (size_t i = 0; i < splashScreens.size(); i++)
    {
        std::string splash = std::string(splashScreens[i].url) + ":" + std::to_string(splashScreens[i].duration);
        serializer->writeString(nullptr, splash.c_str(), "");
    }
    serializer->finishColloction();
    serializer->writeString("mainScene", mainScene.c_str(), "");
}

void Game::Config::onDeserialize(Serializer* serializer)
{
    serializer->readString("title", title, "");
    width = serializer->readInt("width", 0);
    height = serializer->readInt("height", 0);
    fullscreen = serializer->readBool("fullscreen", false);
    vsync = serializer->readBool("vsync", false);
    multisampling = serializer->readInt("multisampling", 0);
    validation = serializer->readBool("validation", false);
    serializer->readString("homePath", homePath, "");
    size_t splashScreensCount = serializer->readList("splashScreens");
    for (size_t i = 0; i < splashScreensCount; i++)
    {
        std::string splash;
        serializer->readString(nullptr, splash, "");
        if (splash.length() > 0)
        {
            SplashScreen splashScreen;
            size_t semiColonIdx = splash.find(':');
            if (semiColonIdx == std::string::npos)
            {
                splashScreen.url = splash;
                splashScreen.duration = SPLASH_DURATION;
            }
            else
            {
                splashScreen.url = splash.substr(0, semiColonIdx);
                std::string durationStr = splash.substr(semiColonIdx + 1, splash.length() - semiColonIdx);
                try
                {
                    splashScreen.duration = std::stof(durationStr);
                }
                catch (...)
                {
                    splashScreen.duration = SPLASH_DURATION;
                }
            }
            splashScreens.push_back(splashScreen);
        }
    }
    serializer->finishColloction();
    serializer->readString("mainScene", mainScene, "");
}

}

