
#include <iostream>
#include "gameplay.h"

using namespace gameplay;

class MainApp : public Game {
    Scene* _scene;
    Camera* _camera;

    void initialize() {

        GltfLoader loader;
        _scene = loader.load("res/gltf/Triangle.gltf");

        if (_scene->getActiveCamera() == NULL) {
            Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1.0f, 10.0f);
            Node* cameraNode = _scene->addNode("camera");
            cameraNode->setCamera(camera);
            cameraNode->translate(0, 0, 5);
            _camera = camera;
        }
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