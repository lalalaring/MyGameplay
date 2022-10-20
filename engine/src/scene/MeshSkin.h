#ifndef MESHSKIN_H_
#define MESHSKIN_H_

#include "math/Matrix.h"
#include "scene/Transform.h"

namespace gameplay
{

class Bundle;
class Model;
class Node;
class Joint;

/**
 * Defines the skin for a mesh.
 *
 * A skin is used to support skinning otherwise known as 
 * vertex blending. This allows for a Model's mesh to support
 * a skeleton on joints that will influence the vertex position
 * and which the joints can be animated.
 */
class MeshSkin : public Transform::Listener, public Ref
{
    friend class Bundle;
    friend class Model;
    friend class Joint;
    friend class Node;
    friend class Scene;

public:

    /**
     * Returns the bind shape matrix.
     * 
     * @return The bind shape matrix.
     */
    const Matrix& getBindShape() const;

    /**
     * Sets the bind shape of this skin.
     * 
     * @param matrix An array of 16 floats.
     */
    void setBindShape(const float* matrix);

    /**
     * Returns the number of joints in this MeshSkin.
     */
    unsigned int getJointCount() const;

    /**
     * Returns the joint at the given index.
     * 
     * @param index The index.
     * 
     * @return The joint.
     */
    Joint* getJoint(unsigned int index) const;

    /**
     * Returns the joint with the given ID.
     * 
     * @param id The ID of the joint to search for.
     * 
     * @return The joint, or NULL if not found.
     */
    Joint* getJoint(const char* id) const;

    /**
     * Returns the root most joint for this MeshSkin.
     *
     * @return The root joint.
     */
    Joint* getRootJoint() const;

    /**
     * Sets the root joint for this MeshSkin.
     *
     * The specified Joint must belong to the joint list for this MeshSkin.
     *
     * @param joint The root joint.
     */
    void setRootJoint(Joint* joint);

    /**
     * Returns the index of the specified joint in this MeshSkin.
     *
     * @return The index of the joint in this MeshSkin, or -1 if the joint does not belong to this MeshSkin.
     */
    int getJointIndex(Joint* joint) const;

    /**
     * Returns the pointer to the Vector4 array for the purpose of binding to a shader.
     * 
     * @return The pointer to the matrix palette.
     */
    Vector4* getMatrixPalette() const;

    /**
     * Returns the number of elements in the matrix palette array.
     * Each element is a Vector4* that represents a row.
     * Each matrix palette is represented by 3 rows of Vector4.
     * 
     * @return The matrix palette size.
     */
    unsigned int getMatrixPaletteSize() const;

    /**
     * Returns our parent Model.
     */
    Model* getModel() const;

    /**
     * Handles transform change events for joints.
     */
    void transformChanged(Transform* transform, long cookie);

    void write(Stream* file);
    static MeshSkin* read(Stream* file);

    void setName(const std::string &name) {
        this->name = name;
    }
    const std::string &getName() {
        return name;
    }

//private:

    /**
     * Constructor.
     */
    MeshSkin();

    /**
     * Constructor.
     */
    MeshSkin(const MeshSkin&);

    /**
     * Destructor.
     */
    ~MeshSkin();
    
    /**
     * Hidden copy assignment operator.
     */
    MeshSkin& operator=(const MeshSkin&);

    /**
     * Clones the MeshSkin and the joints that it references.
     * 
     * @param context The clone context.
     * 
     * @return The newly created MeshSkin.
     */
    MeshSkin* clone(NodeCloneContext &context) const;

    /**
     * Sets the number of joints that can be stored in this skin.
     * This method allocates the necessary memory.
     * 
     * @param jointCount The new size of the joint vector.
     */
    void setJointCount(unsigned int jointCount);

    /**
     * Sets the joint at the given index and increments the ref count.
     * 
     * @param joint The joint to be set.
     * @param index The index in the joints vector.
     */
    void setJoint(Joint* joint, unsigned int index);

    /**
     * Sets the root node of this mesh skin.
     * 
     * @param node The node to set as the root node, may be NULL.
     */
    void setRootNode(Node* node);

    /**
     * Clears the list of joints and releases each joint.
     */
    void clearJoints();
private:
    Matrix _bindShape;
    std::vector<Joint*> _joints;

    //根骨，唯一的用处是计算node的boundBox。
    Joint* _rootJoint;

    //std::vector<std::string> _jointsIdRef;
    //Matrix* _jointsBindPose;
    std::string name;
    
    // Pointer to the root node of the mesh skin.
    // The purpose is so that the joint hierarchy doesn't need to be in the scene.
    // If the joints are not in the scene then something has to hold a reference to it.
    // _rootJoint所在树的root。Node.findNode会通过Skin找到这里来。
    // 如果_rootNode不为空，则它不能在当前场景中，否则便利的时候会死循环。
    Node* _rootNode;

    // Pointer to the array of palette matrices.
    // This array is passed to the vertex shader as a uniform.
    // Each 4x3 row-wise matrix is represented as 3 Vector4's.
    // The number of Vector4's is (_joints.size() * 3).
    Vector4* _matrixPalette;
    Model* _model;
};

}

#endif
