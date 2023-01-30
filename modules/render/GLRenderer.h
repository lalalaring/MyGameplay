#ifndef GLRENDERER_H_
#define GLRENDERER_H_

#include "base/Base.h"
#include "scene/Renderer.h"

namespace gameplay
{
class FrameBuffer;

class GLRenderer : public Renderer {
	ShaderProgram* __currentEffect = NULL;
public:
	GLRenderer();
    ~GLRenderer();

	void clear(ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil);
	void setViewport(int x, int y, int w, int h);

	void updateMesh(Mesh* mesh, unsigned int vertexStart, unsigned int vertexCount);
	void renderMesh(Mesh* mesh, Material* _material,
								unsigned int _partCount,
								Material** _partMaterials, RenderView* view, Node* node);
	void deleteMesh(Mesh* mesh);

	void renderMeshBatch(MeshBatch* mbatch, RenderView* view, Node* node);
	void deleteMeshBatch(MeshBatch* mesh);
	void updateMeshPart(MeshPart* part, unsigned int indexStart, unsigned int indexCount);

	
	void updateState(StateBlock* state, int force = 1);

	void updateTexture(Texture* texture);
	void deleteTexture(Texture* texture);
	void bindTextureSampler(Texture* texture);

	ShaderProgram* createProgram(ProgramSrc* src);
	void deleteProgram(ShaderProgram*effect);
	void bindProgram(ShaderProgram* effect);
	void bindUniform(MaterialParameter* value, Uniform* uniform, ShaderProgram* effect);

	void bindVertexAttributeObj(VertexAttributeBinding* vertextAttribute);
	void unbindVertexAttributeObj(VertexAttributeBinding* vertextAttribute);
	void deleteVertexAttributeObj(VertexAttributeBinding* vertextAttribute);

    FrameBuffer* createFrameBuffer(const char* id, unsigned int width, unsigned int height, Texture::Format format = Texture::RGBA);
    FrameBuffer* bindDefaultFrameBuffer(unsigned int type = 0x8D40/*GL_FRAMEBUFFER*/);
    FrameBuffer* getCurrentFrameBuffer();
    FrameBuffer* getFrameBuffer(const char* id);
private:

	void enableDepthWrite();
	void deleteMeshPart(MeshPart* part);

};

}
#endif
