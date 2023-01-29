#ifndef DRAWABLE_H_
#define DRAWABLE_H_
#include "scene/Component.h"
#include "Camera.h"

namespace gameplay
{

class Node;
class NodeCloneContext;
class Light;


class RenderView {
public:
    bool wireframe = false;
    std::vector<Light*> lights;
    Camera* camera = NULL;
    Rectangle viewport;
};

/**
 * Defines a drawable object that can be attached to a Node.
 */
class Drawable : public Component
{
    friend class Node;

public:

    /**
     * Constructor.
     */
    Drawable();

    /**
     * Destructor.
     */
    virtual ~Drawable();

    /**
     * Called to update the state.
     *
     * @param elapsedTime Elapsed time in milliseconds.
     */
    virtual void update(float elapsedTime) {}

    /**
     * Draws the object.
     *
     * @param wireframe true if you want to request to draw the wireframe only.
     * @return The number of graphics draw calls required to draw the object.
     */

    virtual unsigned int draw(RenderView *view) = 0;

    /**
     * Gets the node this drawable is attached to.
     *
     * @return The node this drawable is attached to.
     */
    Node* getNode() const;

protected:

    /**
     * Clones the drawable and returns a new drawable.
     *
     * @param context The clone context.
     * @return The newly created drawable.
     */
    virtual Drawable* clone(NodeCloneContext& context) { return NULL; }

    /**
     * Sets the node this drawable is attached to.
     *
     * @param node The node this drawable is attached to.
     */
    virtual void setNode(Node* node);

    /**
     * Node this drawable is attached to.
     */
    Node* _node;
};

}

#endif
