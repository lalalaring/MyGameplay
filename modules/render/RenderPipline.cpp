#include "RenderPipline.h"
#include "math/Vector4.h"

using namespace gameplay;

// Render queue indexes (in order of drawing).
enum RenderQueue
{
    QUEUE_OPAQUE = 0,
    QUEUE_TRANSPARENT,
    QUEUE_COUNT
};

RenderPipline::RenderPipline(Renderer* renderer) : renderer(renderer), _scene(NULL) ,__viewFrustumCulling(true){
}

void RenderPipline::render(Scene* scene, Camera* camera, Rectangle* viewport) {
    _scene = scene;
    _camera = camera;

    for (unsigned int i = 0; i < QUEUE_COUNT; ++i)
    {
        std::vector<Node*>& queue = _renderQueues[i];
        queue.clear();
    }

    // Clear the color and depth buffers
    renderer->clear(Renderer::CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    scene->visit(this, &RenderPipline::buildRenderQueues);

    // Draw the scene from our render queues
    drawScene(camera, viewport);
}

bool RenderPipline::buildRenderQueues(Node *node) {
    Drawable* drawable = node->getDrawable();
    if (drawable)
    {
        // Perform view-frustum culling for this node
        if (dynamic_cast<Model*>(drawable)) {
            if (__viewFrustumCulling && !node->getBoundingSphere().intersects(_camera->getFrustum()))
                return true;
        }
        // Determine which render queue to insert the node into
        std::vector<Node*>* queue;
        if (node->hasTag("transparent"))
            queue = &_renderQueues[QUEUE_TRANSPARENT];
        else
            queue = &_renderQueues[QUEUE_OPAQUE];

        queue->push_back(node);
    }
    return true;
}

void RenderPipline::drawScene(Camera* camera, Rectangle* viewport)
{
    RenderView view;
    view.camera = camera;
    view.viewport = *viewport;
    view.wireframe = false;
    // Iterate through each render queue and draw the nodes in them
    for (unsigned int i = 0; i < QUEUE_COUNT; ++i)
    {
        std::vector<Node*>& queue = _renderQueues[i];

        for (size_t j = 0, ncount = queue.size(); j < ncount; ++j)
        {
            queue[j]->getDrawable()->draw(&view);
        }
    }
}

void RenderPipline::finalize() {
    renderer->finalize();
    delete renderer;
    renderer = NULL;
}
