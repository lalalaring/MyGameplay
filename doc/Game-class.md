The `gameplay::Game` class is the base class for all your games created with the gameplay framework. You are required to extend this class using C++ and to override the core game and lifecycle event methods initialize, finalize, update, and render. This is where you?ll write your code to load the game assets and apply game logic and rendering code. Under the hood, the game class will receive events and act as an abstraction between the running game and the underlying platform layer that is running the game loop and reacting to operating systems.

There are four methods you must implement to get started in writing your own game:

```c++
#include "gameplay.h"
using namespace gameplay;

class MyGame : public Game
{
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);
};
```

The `Game::initialize()` and `Game::finalize()` methods are called when the game starts up and shuts down, respectively. They are the methods to which you’ll add code to load your game assets and cleanup when the game has ended. The `Game::update()` and `Game::render()` methods are called once per frame from the game loop implemented in the `gameplay::Platform` for each operating system. This allows you to separate the code between handling updates to your game’s state and rendering your game’s visuals. You can use a variety of built-in classes to help with the game rendering.

## Accessing the game instance

The `gameplay::Game` class can be accessed from anywhere in your game code. It implements a singleton design pattern. Call the static method `Game::getInstance()` to gain access to the instance of your game from any code.

## Graphics and audio devices

After your game has started, the underlying graphics and audio devices will automatically initialize. This happens prior to the `Game::initialize()` method being called and readies any classes that use OpenGL or OpenAL. The graphics devices of your Game will be set up with a default 32-bit color frame buffer, a 24-bit depth buffer, and an 8-bit stencil buffer ready for your use. These are the active graphics hardware buffers, which are rendered into from your rendering code.

For more advanced usage, you can apply alternative frame buffers using the `gameplay::FrameBuffer` class. Immediately after the `Game::render()` method, the frame buffer is swapped/presented to the physical display for the user to see. You can invoke the `Game::clear()` method to clear the buffers through any of the methods. You can also call `Game::renderOnce()` from code, such as from the `Game::initialize()` method, to callback onto a method that will be called only once and then swapped/presented to the display. This is useful for presenting ad-hoc updates to the screen during initialization for rendering, such as loading screens.

## Game sub-system controllers

The `gameplay::Game` class also manages game sub-system controllers, such as audio, animation and physics controllers, and provides access to them directly using getter methods. These classes act as controlling interfaces for managing and playing audio and animations that are active in the game, as well as updates to dynamics in the physics systems. These controllers are hosted by the `gameplay::Game` class and react on lifecycle events being handled in the game.

## Game time and state

Once the instance of a `gameplay::Game` class has started, the game starts a running time. You can call the `Game::getGameTime()` to determine how long a game has been running. You can also call `Game::getAbsoluteTime()` to determine the absolute time that has elapsed since the first `Game::run()` call. This includes any paused time too. You can call the `Game::pause()` method and the game will be put into the `Game::PAUSED` state. If the user on the platform puts the game into the background, the game time is also paused. If the user puts the game back into the foreground, the game will invoke `Game::play()` and the game will resume. At any time in the game you can determine the game state by calling `Game::getState()`. The game state can be `UNINITIALIZED`, `RUNNING` or `PAUSED`.
