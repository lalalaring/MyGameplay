
#include <iostream>
#include "gameplay.h"

using namespace gameplay;

class MainApp : public Game {
    Scene* _scene;
    Camera* _camera;
    CubeMap* _cubemap;
    Model* _model;
    bool _isSkybox;

    void initialize() {

        _isSkybox = true;

        // Create a new empty scene.
        _scene = Scene::create();

        // Create the camera.
        Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1.0f, 1000.0f);
        Node* cameraNode = _scene->addNode("camera");

        // Attach the camera to a node. This determines the position of the camera.
        cameraNode->setCamera(camera);

        // Make this the active camera of the scene.
        //_scene->setActiveCamera(camera);
        _camera = camera;

        // Move the camera to look at the origin.
        cameraNode->translate(0, 1, 5);
        cameraNode->rotateX(MATH_DEG_TO_RAD(-20));



        // Create the skybox
        std::vector<std::string> faces = {
            "res/skybox/right.jpg",
            "res/skybox/left.jpg",
            "res/skybox/top.jpg",
            "res/skybox/bottom.jpg",
            "res/skybox/front.jpg",
            "res/skybox/back.jpg"
        };
        CubeMap* cube = new CubeMap();
        cube->load(faces, _isSkybox);
        Node* modelNode = _scene->addNode("skybox");
        modelNode->setDrawable(cube);
        _cubemap = cube;



        // Create test model
        Mesh* mesh = Mesh::createSpherical();
        //Mesh* mesh = Mesh::createCube();
        _model = Model::create(mesh);
        SAFE_RELEASE(mesh);
        Material* material = _model->setMaterial("res/shaders/enviro.vert", "res/shaders/enviro.frag");
        material->getParameter("u_albedo")->setVector3(Vector3(1.0, 1.0, 1.0));
        material->getParameter("u_roughness")->setFloat(0.2);
        material->getParameter("u_metallic")->setFloat(0.8);
        material->getParameter("u_skybox")->setSampler(cube->getTexture());

        Node* modelNode2 = _scene->addNode("model");
        modelNode2->setDrawable(_model);
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
            //if (_isSkybox) {
            _camera->getNode()->rotateY(0.3);
            //}
            //else {
            //    _cubemap->getNode()->rotateX(0.3);
            //}
            //_model->getNode()->rotateX(0.3);
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