#define GLEW_STATIC
#include <GL/glew.h>

#include "GameView.h"
#include <QtWidgets>


using namespace gameplay;

GameView::GameView(QWidget* parent) : QOpenGLWidget(parent),
    _editor(nullptr),
    //_graphics(nullptr),
    _scene(nullptr),
    _wireframe(false)
{
}


GameView::~GameView()
{
    Game::exit();
}

void GameView::setEditor(EditorWindow* editor)
{
    _editor = editor;
}

void GameView::onSceneChanged()
{
    _scene = _editor->getScene();
}

void GameView::initializeGL() {
    initializeOpenGLFunctions();
    if (GLEW_OK != glewInit())
    {
        printf("Failed to initialize GLEW.\n");
        return;
    }

    /* Version */
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *shader = glGetString( GL_SHADING_LANGUAGE_VERSION );

    printf("GL Vendor    : %s\n", vendor);
    printf("GL Renderer  : %s\n", renderer);
    printf("GL Version   : %s\n", version);
    printf("GL Shader    : %s\n", shader);

    this->run();

    Game::resizeEventInternal(this->size().width(), this->size().height());

    GltfLoader loader;
    _scene = loader.load("res/gltf/Triangle.gltf");

    if (_scene->getActiveCamera() == NULL) {
        Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1.0f, 10.0f);
        Node* cameraNode = _scene->addNode("camera");
        cameraNode->setCamera(camera);
        cameraNode->translate(0, 0, 5);
        //_camera = camera;
        _scene->setActiveCamera(camera);
    }
}
void GameView::paintGL() {
    Game::frame();
    //context()->swapBuffers(context()->surface());
    QWidget::update();
}
void GameView::resizeGL(int width, int height) {
    Game::resizeEventInternal(width, height);
}
void GameView::update(float elapsedTime)
{
    _scene->update(elapsedTime);
}

void GameView::render(float elapsedTime)
{
    gameplay::Rectangle viewport = getViewport();
    Camera *_camera = _scene->getActiveCamera();
    getRenderPipline()->render(_scene.get(), _camera, &viewport);
}

void GameView::mousePressEvent(QMouseEvent* evt)
{
    const bool down = true;
    _mousePosition = QPoint(evt->x(), evt->y());
    if (!Game::mouseEventInternal(gameplay::Mouse::MOUSE_PRESS_LEFT_BUTTON, evt->x(), evt->y(), 0))
    {
        Game::touchEventInternal(gameplay::Touch::TOUCH_PRESS, evt->x(), evt->y(), 0);
    }
}

void GameView::mouseReleaseEvent(QMouseEvent* evt)
{
    const bool down = false;
    _mousePosition = QPoint(evt->x(), evt->y());
    if (!Game::mouseEventInternal(gameplay::Mouse::MOUSE_RELEASE_LEFT_BUTTON, evt->x(), evt->y(), 0))
    {
        Game::touchEventInternal(gameplay::Touch::TOUCH_RELEASE, evt->x(), evt->y(), 0);
    }
}

void GameView::mouseMoveEvent(QMouseEvent* evt)
{
      _mousePosition = QPoint(evt->x(), evt->y());
      if (!Game::mouseEventInternal(gameplay::Mouse::MOUSE_MOVE, evt->x(), evt->y(), 0))
      {
          if (evt->button() & Qt::LeftButton)
          {
              // Mouse move events should be interpreted as touch move only if left mouse is held and the game did not consume the mouse event.
              Game::touchEventInternal(gameplay::Touch::TOUCH_MOVE, evt->x(), evt->y(), 0);
          }
      }
}

void GameView::mouseWheelEvent(QWheelEvent* evt)
{
    const float step = evt->pixelDelta().y() / 240.0;
    _mousePosition = QPoint(evt->position().x(), evt->position().y());
    _mouseScroll += step;
    Game::mouseEventInternal(gameplay::Mouse::MOUSE_WHEEL, evt->position().x(), evt->position().y(), evt->pixelDelta().y() / 120);
}

void GameView::keyPressEvent(QKeyEvent* evt)
{
    if(evt->key() == Qt::Key_Escape && !evt->isAutoRepeat())
    {
        if (_editor->isFullscreen())
        {
            _editor->setFullscreen(false);
        }
    }

    //Game::keyEventInternal(gameplay::Keyboard::KEY_PRESS, getKey(wParam, shiftDown ^ capsOn));
}

void GameView::keyReleaseEvent(QKeyEvent* evt)
{
    // TODO: Handler here...
    //Game::keyEventInternal(gameplay::Keyboard::KEY_RELEASE, getKey(wParam, shiftDown ^ capsOn));
}

void GameView::closeEvent(QCloseEvent* evt)
{
    Game::exit();
}


