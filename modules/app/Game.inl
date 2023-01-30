#include "app/Game.h"
#include "Platform.h"

namespace gameplay
{

inline Game::State Game::getState() const
{
    return _state;
}

inline bool Game::isInitialized() const
{
    return _initialized;
}

inline unsigned int Game::getFrameRate() const
{
    return _frameRate;
}

inline unsigned int Game::getWidth() const
{
    return _width;
}

inline unsigned int Game::getHeight() const
{
    return _height;
}

inline float Game::getAspectRatio() const
{
    return (float)_width / (float)_height;
}

inline const Rectangle& Game::getViewport() const
{
    return _viewport;
}

//inline AnimationController* Game::getAnimationController() const
//{
//    return _animationController;
//}
//
//inline AudioController* Game::getAudioController() const
//{
//    return _audioController;
//}
//
//inline PhysicsController* Game::getPhysicsController() const
//{
//    return _physicsController;
//}
//
//inline ScriptController* Game::getScriptController() const
//{
//    return _scriptController;
//}
//inline AIController* Game::getAIController() const
//{
//    return _aiController;
//}
inline bool Game::isMouseCaptured()
{
    return Platform::isMouseCaptured();
}
inline void Game::displayKeyboard(bool display)
{
    Platform::displayKeyboard(display);
}
inline void Game::getArguments(int* argc, char*** argv) const
{
    Platform::getArguments(argc, argv);
}
}
