#ifndef GP_NO_PLATFORM
#ifdef GP_GLFW

#include "base/Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "platform/Game.h"
#include "Form.h"
#include "math/Vector2.h"
#include "ScriptController.h"
#include <GL/wglew.h>
#include <GLFW/glfw3.h>


GLFWwindow* window;
static bool __multiSampling = false;

namespace gameplay
{

extern void print(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    int sz = vfprintf(stderr, format, argptr);
    if (sz > 0)
    {
        char* buf = new char[sz + 1];
        vsprintf(buf, format, argptr);
        buf[sz] = 0;
        OutputDebugStringA(buf);
        SAFE_DELETE_ARRAY(buf);
    }
    va_end(argptr);
}

extern int strcmpnocase(const char* s1, const char* s2)
{
    return _strcmpi(s1, s2);
}


Platform::Platform(Game* game)
    : _game(game)
{
}

Platform::~Platform()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


Platform* Platform::create(Game* game)
{
    GP_ASSERT(game);

    FileSystem::setResourcePath("./");
    Platform* platform = new Platform(game);

    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit())
        goto error;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        goto error;
    }

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //gladLoadGL(glfwGetProcAddress);
    if (GLEW_OK != glewInit())
    {
        GP_ERROR("Failed to initialize GLEW.");
        glfwTerminate();
        goto error;
    }
    glfwSwapInterval(1);

    return platform;

error:

    exit(0);
    return NULL;
}


int Platform::enterMessagePump()
{
    GP_ASSERT(_game);
    if (_game->getState() != Game::RUNNING)
        _game->run();

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        //glClear(GL_COLOR_BUFFER_BIT);

        _game->frame();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        

        // If we are done, then exit.
        if (_game->getState() == Game::UNINITIALIZED)
            break;
    }
    return 0;
}


void Platform::signalShutdown() 
{
    // nothing to do  
}

bool Platform::canExit()
{
    return true;
}

unsigned int Platform::getDisplayWidth()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return width;
}

unsigned int Platform::getDisplayHeight()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return height;
}

double Platform::getAbsoluteTime()
{
    double time = glfwGetTime();
    return time * 1000;
}

void Platform::setAbsoluteTime(double time)
{
    //__timeAbsolute = time;
    abort();
}

bool Platform::isVsync()
{
    return false;
}

void Platform::setVsync(bool enable)
{
}

void Platform::swapBuffers()
{
    glfwSwapBuffers(window);
}

void Platform::sleep(long ms)
{
    Sleep(ms);
}

void Platform::setMultiSampling(bool enabled)
{
    if (enabled == __multiSampling)
    {
        return;
    }

    if (enabled)
    {
        glEnable(GL_MULTISAMPLE);
    }
    else
    {
        glDisable(GL_MULTISAMPLE);
    }

    __multiSampling = enabled;
}

bool Platform::isMultiSampling()
{
    return __multiSampling;
}

void Platform::setMultiTouch(bool enabled)
{
    // not supported
}

bool Platform::isMultiTouch()
{
    return false;
}

bool Platform::hasAccelerometer()
{
    return false;
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    GP_ASSERT(pitch);
    GP_ASSERT(roll);

    *pitch = 0;
    *roll = 0;
}

void Platform::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    if (accelX)
    {
        *accelX = 0;
    }

    if (accelY)
    {
        *accelY = 0;
    }

    if (accelZ)
    {
        *accelZ = 0;
    }

    if (gyroX)
    {
        *gyroX = 0;
    }

    if (gyroY)
    {
        *gyroY = 0;
    }

    if (gyroZ)
    {
        *gyroZ = 0;
    }
}

void Platform::getArguments(int* argc, char*** argv)
{
    if (argc)
        *argc = __argc;
    if (argv)
        *argv = __argv;
}

bool Platform::hasMouse()
{
    return true;
}

void Platform::setMouseCaptured(bool captured)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool Platform::isMouseCaptured()
{
    //return __mouseCaptured;
    return true;
}

void Platform::setCursorVisible(bool visible)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

bool Platform::isCursorVisible()
{
    //return __cursorVisible;
    return true;
}

void Platform::displayKeyboard(bool display)
{
    // Do nothing.
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
}
    
bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::pollGamepadState(Gamepad* gamepad)
{
}

void Platform::shutdownInternal()
{
    Game::getInstance()->shutdown();
}

}

#endif
#endif
