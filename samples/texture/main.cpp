
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
        Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1.0f, 10.0f);
        Node* cameraNode = _scene->addNode("camera");

        // Attach the camera to a node. This determines the position of the camera.
        cameraNode->setCamera(camera);

        // Make this the active camera of the scene.
        //_scene->setActiveCamera(camera);
        _camera = camera;

        // Move the camera to look at the origin.
        cameraNode->translate(0, 1, 5);
        cameraNode->rotateX(MATH_DEG_TO_RAD(-11.25f));

        // Create the triangle mesh.
        Mesh* mesh = Mesh::createCube();

        // Create a model for the triangle mesh. A model is an instance of a Mesh that can be drawn with a specified material.
        Model *_model = Model::create(mesh);
        SAFE_RELEASE(mesh);

        // Create a material from the built-in "colored-unlit" vertex and fragment shaders.
        // This sample doesn't use lighting so the unlit shader is used.
        // This sample uses vertex color so VERTEX_COLOR is defined. Look at the shader source files to see the supported defines.
        Material* material = _model->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag");
        Texture* sampler = material->getParameter("u_diffuseTexture")->setValue("res/image/crate.png", true);
        sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
        material->getStateBlock()->setCullFace(true);
        material->getStateBlock()->setDepthTest(true);
        material->getStateBlock()->setDepthWrite(true);

        Node* modelNode = _scene->addNode("model");
        modelNode->setDrawable(_model);
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
