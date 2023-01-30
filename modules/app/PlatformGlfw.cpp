#ifndef GP_NO_PLATFORM
#ifdef GP_GLFW

#define WIN32_LEAN_AND_MEAN
#define GLEW_STATIC
#include <GL/glew.h>
//#include <GL/wglew.h>
#include <GLFW/glfw3.h>

#include "Platform.h"
#include "app/Game.h"


GLFWwindow* window;
static bool __multiSampling = false;

namespace gameplay
{

extern int strcmpnocase(const char* s1, const char* s2)
{
    return _strcmpi(s1, s2);
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
void character_callback(GLFWwindow* window, unsigned int codepoint)
{
}

static void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_MOVE, x, y, 0))
    {
        gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_MOVE, x, y, 0, true);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_PRESS_LEFT_BUTTON, x, y, 0))
            {
                gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_PRESS, x, y, 0, true);
            }
        }
        else {
            if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_RELEASE_LEFT_BUTTON, x, y, 0))
            {
                gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_RELEASE, x, y, 0, true);
            }
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_PRESS_RIGHT_BUTTON, x, y, 0);
        }
        else {
            gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_RELEASE_RIGHT_BUTTON, x, y, 0);
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_WHEEL, x, y, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    gameplay::Platform::resizeEventInternal(width, height);
}

void Platform::init(const char* title)
{
    //FileSystem::setResourcePath("./");

    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit())
        goto error;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 768, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        goto error;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
    return;
error:

    exit(0);
}


int Platform::enterMessagePump()
{
    Game* _game = Game::getInstance();
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
    glfwDestroyWindow(window);
    glfwTerminate();
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
    if (visible)
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

int Platform::run() {
    Game* game = Game::getInstance();
    GP_ASSERT(game);
    Platform::init("MGP Engine");
    int result = Platform::enterMessagePump();
    Platform::signalShutdown();
    return result;
}

}

#endif //GP_GLFW
#endif //GP_NO_PLATFORM
