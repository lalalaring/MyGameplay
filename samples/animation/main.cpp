
#include <iostream>
#include "gameplay.h"

using namespace gameplay;

class App : public Game {
    Scene* _scene;
    Camera* _camera;

    void initialize() {

        GltfLoader loader;
        _scene = loader.load("res/gltf/RiggedSimple.gltf");

        if (_scene->getActiveCamera() == NULL) {
            Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1.0f, 100.0f);
            Node* cameraNode = _scene->addNode("camera");
            cameraNode->setCamera(camera);
            cameraNode->translate(0, 0, 15);
            _camera = camera;
        }

        Node *node = _scene->getRootNode()->getChild(0)->getChild(1);
        Model* model = dynamic_cast<Model*>(node->getDrawable());

        Material* mat = model->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", 
            "SKINNING; SKINNING_JOINT_COUNT 2", 0);
        mat->getParameter("u_diffuseColor")->setVector4(Vector4(1.0, 0.0, 0.0, 1.0));
        //mat->getParameter("u_ambientColor")->setVector4(Vector4(0.0, 0.0, 0.0, 1.0));
        

        Animation *animation = model->getSkin()->getRootJoint()->getAnimation();
        AnimationClip* clip = animation->getClip();
        clip->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
        clip->play();
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

App instance;

//int main()
//{
//    std::cout << "Hello World!\n";
//}
