
#include <iostream>
#include "gameplay.h"

using namespace gameplay;

/**
 * Creates a triangle mesh with vertex colors.
 */
static Mesh* createTriangleMesh()
{
    // Calculate the vertices of the equilateral triangle.
    float a = 0.5f;     // length of the side
    Vector2 p1(0.0f, a / sqrtf(3.0f));
    Vector2 p2(-a / 2.0f, -a / (2.0f * sqrtf(3.0f)));
    Vector2 p3(a / 2.0f, -a / (2.0f * sqrtf(3.0f)));

    // Create 3 vertices. Each vertex has position (x, y, z) and color (red, green, blue)
    float vertices[] =
    {
        p1.x, p1.y, 0.0f,     1.0f, 0.0f, 0.0f,
        p2.x, p2.y, 0.0f,     0.0f, 1.0f, 0.0f,
        p3.x, p3.y, 0.0f,     0.0f, 0.0f, 1.0f,
    };
    unsigned int vertexCount = 3;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::TRIANGLES);
    mesh->setVertexData(vertices, 0, vertexCount);
    return mesh;
}

class TriangleApp : public Game {
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
        Mesh* mesh = createTriangleMesh();

        // Create a model for the triangle mesh. A model is an instance of a Mesh that can be drawn with a specified material.
        Model *_model = Model::create(mesh);
        SAFE_RELEASE(mesh);

        // Create a material from the built-in "colored-unlit" vertex and fragment shaders.
        // This sample doesn't use lighting so the unlit shader is used.
        // This sample uses vertex color so VERTEX_COLOR is defined. Look at the shader source files to see the supported defines.
        _model->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "VERTEX_COLOR");

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

TriangleApp instance;

//int main()
//{
//    std::cout << "Hello World!\n";
//}
