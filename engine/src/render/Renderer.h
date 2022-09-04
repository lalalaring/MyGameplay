#ifndef RENDERER_H_
#define RENDERER_H_

#include "base/Base.h"
#include "scene/Mesh.h"
#include "scene/MeshBatch.h"
#include "material/StateBlock.h"
#include "material/Texture.h"
#include "material/ShaderProgram.h"
#include "material/MaterialParameter.h"
#include "material/VertexAttributeBinding.h"

namespace gameplay
{
class FrameBuffer;
class RenderView;

class Renderer {
public:
	static Renderer* cur();
    void finalize();

public:
    /**
     * Flags used when clearing the active frame buffer targets.
     */
    enum ClearFlags
    {
        CLEAR_COLOR = 0x00004000,
        CLEAR_DEPTH = 0x00000100,
        CLEAR_STENCIL = 0x00000400,
        CLEAR_COLOR_DEPTH = CLEAR_COLOR | CLEAR_DEPTH,
        CLEAR_COLOR_STENCIL = CLEAR_COLOR | CLEAR_STENCIL,
        CLEAR_DEPTH_STENCIL = CLEAR_DEPTH | CLEAR_STENCIL,
        CLEAR_COLOR_DEPTH_STENCIL = CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL
    };

    virtual void clear(ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil) = 0;
    virtual void setViewport(int x, int y, int w, int h) = 0;
    StateBlock stateBlock;
    virtual void updateState(StateBlock *state, int force) = 0;
public:
	virtual void updateMesh(Mesh* mesh, unsigned int vertexStart, unsigned int vertexCount) = 0;
	virtual void renderMesh(Mesh *mesh, Material* _material,
										unsigned int _partCount,
										Material** _partMaterials, RenderView *view, Node *node) = 0;
	virtual void deleteMesh(Mesh* mesh) = 0;

	virtual void renderMeshBatch(MeshBatch* mbatch, RenderView* view, Node* node) = 0;
    virtual void deleteMeshBatch(MeshBatch* mesh) = 0;

	virtual void updateMeshPart(MeshPart* part, unsigned int indexStart, unsigned int indexCount) = 0;
public:
    virtual void updateTexture(Texture* texture) = 0;
    virtual void deleteTexture(Texture* texture) = 0;
    virtual void bindTextureSampler(Texture::Sampler* texture) = 0;


    virtual FrameBuffer* createFrameBuffer(const char* id, unsigned int width, unsigned int height, Texture::Format format = Texture::RGBA) = 0;
    virtual FrameBuffer* bindDefaultFrameBuffer(unsigned int type = 0x8D40/*GL_FRAMEBUFFER*/) = 0;
    virtual FrameBuffer* getCurrentFrameBuffer() = 0;
    virtual FrameBuffer* getFrameBuffer(const char* id) = 0;

public:
    struct ProgramSrc {
        const char* defines;
        const char* vshSource;
        const char* fshSource;
    };

    virtual ShaderProgram* createProgram(ProgramSrc* src) = 0;
    virtual void deleteProgram(ShaderProgram* effect) = 0;
    virtual void bindProgram(ShaderProgram* effect) = 0;
    virtual void bindUniform(MaterialParameter *value, Uniform *uniform, ShaderProgram* effect) = 0;

    //virtual void bindVertexAttributeObj(VertexAttributeBinding *vertextAttribute) = 0;
    //virtual void unbindVertexAttributeObj(VertexAttributeBinding* vertextAttribute) = 0;
    //virtual void deleteVertexAttributeObj(VertexAttributeBinding* vertextAttribute) = 0;
};

}
#endif
