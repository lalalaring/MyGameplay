
#include <iostream>
#include "gameplay.h"

using namespace gameplay;

class MainApp : public Game {
    Scene* _scene;
    Camera* _camera;

    void initialize() {

        // Create a new empty scene.
        _scene = Scene::create();

        // Create the camera.
        Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1.0f, 100000.0f);
        Node* cameraNode = _scene->addNode("camera");

        // Attach the camera to a node. This determines the position of the camera.
        cameraNode->setCamera(camera);

        // Make this the active camera of the scene.
        //_scene->setActiveCamera(camera);
        _camera = camera;

        // Move the camera to look at the origin.
        //Matrix matrix;
        //Matrix::createLookAt(Vector3(0, 0, -1000.0), Vector3::zero(), Vector3::unitY(), &matrix);
        //cameraNode->setMatrix(matrix);
        cameraNode->translate(0, 1, 500.0);

        // Create the line model.
        float position[] = { 0,0,0, 45, 15,0, 100, 0, 0, 150, -100, 0, 200, 100, 0 };
        Line* line = new Line();
        line->setPositions(position, 5);
        
        Node* modelNode = _scene->addNode("model");
        modelNode->setDrawable(line);
    }

    void finalize() {
        SAFE_RELEASE(_camera);
        SAFE_RELEASE(_scene);
    }

    void update(float elapsedTime)
    {
        _scene->update(elapsedTime);
    }

    void render(float elapsedTime)
    {
        Rectangle viewport = getViewport();
        getRenderPipline()->render(_scene, _camera, &viewport);
    }

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            _camera->getNode()->rotateZ(1);
            break;
        case Touch::TOUCH_RELEASE:
            break;
        case Touch::TOUCH_MOVE:
            break;
        };
    }

};

MainApp instance;

int main() {
    return Platform::run();
}
