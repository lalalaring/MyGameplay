#ifndef COMPONENT_H
#define COMPONENT_H

#include "base/Serializable.h"

namespace gameplay
{
class Node;

class Component
{
public:
    Component();
    virtual ~Component();
    /**
     * Sets the node associated with this camera.
     */
    virtual void setNode(Node* node) {}
};

}
#endif // COMPONENT_H
