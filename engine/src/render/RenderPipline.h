#ifndef RENDERPIPLINE_H_
#define RENDERPIPLINE_H_

#include "base/Base.h"
#include "Renderer.h"
#include "scene/Scene.h"
#include "scene/Camera.h"

namespace gameplay {

	class RenderPipline
	{
		Renderer *renderer;
		Scene* _scene;
		std::vector<Node*> _renderQueues[2];
		bool __viewFrustumCulling;
	public:
		RenderPipline(Renderer* renderer);
		Renderer* getRenderer() { return renderer; }
		void render(Scene* scene, Camera *camera, Rectangle *viewport);


		void finalize();

	protected:
		bool buildRenderQueues(Node* node);
		void drawScene(Camera* camera, Rectangle* viewport);
	};

}
#endif