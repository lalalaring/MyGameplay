
#include "GLtfLoader.h"

#define CGLTF_IMPLEMENTATION
#include "../3rd/cgltf.h"

#include "../scene/MeshPart.h"
#include "../scene/Joint.h"

using namespace gameplay;

class GltfLoaderImp {
	std::map<cgltf_node*, Node*> nodeMap;

public:
	Scene * load(const char* file) {
		cgltf_options options = { 0 };
		cgltf_data* data = NULL;
		cgltf_result result = cgltf_parse_file(&options, file, &data);
		if (result == cgltf_result_success)
		{
			cgltf_load_buffers(&options, data, file);

			Scene *scene = loadScene(data);

			cgltf_free(data);

			return scene;
		}
		return NULL;
	}
private:
	Material* loadMaterial(cgltf_material* cmaterial) {
		if (cmaterial->has_pbr_metallic_roughness) {

		}
		return NULL;
	}

	Model* loadMesh(cgltf_mesh *cmesh) {

		bool sharedVertexBuf = true;
		std::map<std::string, cgltf_accessor*> attributeUnique;
		std::vector<VertexFormat::Element> vertexElemets;
		int vertexCount = 0;

		for (int i = 0; i < cmesh->primitives_count; ++i) {
			cgltf_primitive* primitive = cmesh->primitives + i;
			for (int j = 0; j < primitive->attributes_count; ++j) {
				cgltf_attribute* attr = primitive->attributes + j;
				cgltf_accessor* old = attributeUnique[attr->name];
				if (!old) {
					attributeUnique[attr->name] = attr->data;
				}
				else if (old != attr->data) {
					sharedVertexBuf = false;
					goto label1;
				}
				if (vertexCount < attr->data->count) {
					vertexCount = attr->data->count;
				}
			}
		}

	label1:

		if (!sharedVertexBuf) {
			printf("TODO: not support mesh part");
			return NULL;
		}

		std::vector<cgltf_accessor*> accessors;
		for (auto itr = attributeUnique.begin(); itr != attributeUnique.end(); ++itr) {
			cgltf_accessor* accessor = itr->second;
			VertexFormat::Element element;
			element.size = cgltf_num_components(accessor->type);

			std::string name = itr->first;
			if (name == "POSITION") {
				element.usage = VertexFormat::POSITION;
			}
			else if (name == "NORMAL") {
				element.usage = VertexFormat::NORMAL;
			}
			else if (name == "TANGENT") {
				element.usage = VertexFormat::TANGENT;
			}
			else if (name == "TEXCOORD_0") {
				element.usage = VertexFormat::TEXCOORD0;
			}
			else if (name == "TEXCOORD_0") {
				element.usage = VertexFormat::TEXCOORD0;
			}
			else if (name == "TEXCOORD_1") {
				element.usage = VertexFormat::TEXCOORD1;
			}
			else if (name == "TEXCOORD_2") {
				element.usage = VertexFormat::TEXCOORD2;
			}
			else if (name == "TEXCOORD_3") {
				element.usage = VertexFormat::TEXCOORD3;
			}
			else if (name == "TEXCOORD_4") {
				element.usage = VertexFormat::TEXCOORD4;
			}
			else if (name == "TEXCOORD_5") {
				element.usage = VertexFormat::TEXCOORD5;
			}
			else if (name == "TEXCOORD_6") {
				element.usage = VertexFormat::TEXCOORD6;
			}
			else if (name == "TEXCOORD_7") {
				element.usage = VertexFormat::TEXCOORD7;
			}
			else if (name == "COLOR_0") {
				element.usage = VertexFormat::COLOR;
			}
			else if (name == "JOINTS_0") {
				element.usage = VertexFormat::BLENDINDICES;
			}
			else if (name == "WEIGHTS_0") {
				element.usage = VertexFormat::BLENDWEIGHTS;
			}
			else {
				printf("unsupport attribute type: %s\n", name.c_str());
				continue;
			}
			vertexElemets.push_back(element);
			accessors.push_back(accessor);
		}

		VertexFormat format(vertexElemets.data(), vertexElemets.size());
		Mesh* mesh = Mesh::createMesh(format, vertexCount);

		char* data = (char*)malloc(format.getVertexSize() * vertexCount);

		for (int i = 0; i < vertexCount; ++i) {
			int offset = 0;
			for (int j = 0; j < vertexElemets.size(); ++j) {
				cgltf_accessor* accessor = accessors[j];
				float* out = (float*)(data + (format.getVertexSize() * i) + offset);
				cgltf_accessor_read_float(accessor, i, out, cgltf_num_components(accessor->type));
				offset += vertexElemets[j].size*sizeof(float);
			}
		}

		mesh->setVertexData(data, 0, vertexCount, false);


		for (int i = 0; i < cmesh->primitives_count; ++i) {
			cgltf_primitive* primitive = cmesh->primitives + i;
			if (primitive->indices) {
				Mesh::PrimitiveType type;
				switch (primitive->type)
				{
				case cgltf_primitive_type_points:
					type = Mesh::PrimitiveType::POINTS;
					break;
				case cgltf_primitive_type_lines:
					type = Mesh::PrimitiveType::LINES;
					break;
				case cgltf_primitive_type_line_loop:
					type = Mesh::PrimitiveType::LINE_LOOP;
					break;
				case cgltf_primitive_type_line_strip:
					type = Mesh::PrimitiveType::LINE_STRIP;
					break;
				case cgltf_primitive_type_triangles:
					type = Mesh::PrimitiveType::TRIANGLES;
					break;
				case cgltf_primitive_type_triangle_strip:
					type = Mesh::PrimitiveType::TRIANGLE_STRIP;
					break;
				case cgltf_primitive_type_triangle_fan:
					type = Mesh::PrimitiveType::TRIANGLE_FAN;
					break;
				default:
					type = Mesh::PrimitiveType::LINES;
				}
				Mesh::IndexFormat iformat = Mesh::IndexFormat::INDEX32;
				int indexCount = primitive->indices->count;
				MeshPart *part = mesh->addPart(type, iformat, indexCount);

				int32_t* data = (int32_t*)malloc(indexCount * 4);
				for (int j = 0; j < indexCount; ++j) {
					int v = cgltf_accessor_read_index(primitive->indices, j);
					data[j] = v;
				}
				part->setIndexData(data, 0, indexCount, false);
			}
		}
		
		Model* model = Model::create(mesh);

		//model->setMaterial();
		Material *mat = model->setMaterial("res/shaders/min.vert", "res/shaders/min.frag");
		mat->getParameter("u_diffuseColor")->setVector4(Vector4(1.0, 0.0, 0.0, 1.0));

		return model;
	}

	Joint* getJoint(cgltf_node* cnode) {
		auto it = nodeMap.find(cnode);
		if (it != nodeMap.end()) {
			return dynamic_cast<Joint*>(it->second);
		}
		
		return NULL;
	}

	MeshSkin* loadSkin(cgltf_skin* cskin) {
		MeshSkin* skin = new MeshSkin();
		int num_comp = cgltf_num_components(cskin->inverse_bind_matrices->type);
		float* matrix = (float*)malloc(cskin->inverse_bind_matrices->count * num_comp * sizeof(float));
		for (int i = 0; i < cskin->inverse_bind_matrices->count; ++i) {
			float* out = matrix + (num_comp * i);
			cgltf_accessor_read_float(cskin->inverse_bind_matrices, i, out, num_comp);
		}

		skin->setJointCount(cskin->joints_count);
		for (int i = 0; i < cskin->joints_count; ++i) {
			Joint* joint = getJoint(cskin->joints[i]);
			if (!joint) continue;
			Matrix m(matrix+(i*16));
			joint->setInverseBindPose(m);
			skin->setJoint(joint, i);
		}

		Joint* skeleton = getJoint(cskin->skeleton);
		assert(skeleton);
		skeleton->remove();
		skin->setRootJoint(skeleton);
		
		return skin;
	}

	Animation* loadAnimation(cgltf_animation* canimation) {
		Animation* animation = new Animation(canimation->name == NULL ? "" : canimation->name);
		for (int i = 0; i < canimation->channels_count; ++i) {
			cgltf_animation_channel* cchannel = canimation->channels+i;
			cgltf_animation_sampler* csampler = cchannel->sampler;

			AnimationTarget* target = nodeMap[cchannel->target_node];
			if (!target) continue;

			unsigned int propertyId = 0;
			switch (cchannel->target_path)
			{
			case cgltf_animation_path_type_invalid:
				break;
			case cgltf_animation_path_type_translation:
				propertyId = Transform::ANIMATE_TRANSLATE;
				break;
			case cgltf_animation_path_type_rotation:
				propertyId = Transform::ANIMATE_ROTATE;
				break;
			case cgltf_animation_path_type_scale:
				propertyId = Transform::ANIMATE_SCALE;
				break;
			case cgltf_animation_path_type_weights:
				break;
			case cgltf_animation_path_type_max_enum:
				break;
			default:
				break;
			}
			if (propertyId == 0) continue;

			unsigned int keyCount = cchannel->sampler->input->count;
			if (keyCount != cchannel->sampler->output->count) {
				printf("ERROR: keyCount != valueCount\n");
				continue;
			}
			//unsigned int num_comp = cgltf_num_components(cchannel->sampler->input->type);
			//float* fKeyTimes = (float*)malloc(num_comp * sizeof(float));
			unsigned int* keyTimes = (unsigned int*)malloc(keyCount*sizeof(int));
			float minTime = cchannel->sampler->input->min[0];
			float maxTime = cchannel->sampler->input->max[0];
			for (int i = 0; i < keyCount; ++i) {
				float out[16];
				cgltf_accessor_read_float(cchannel->sampler->input, i, out, 1);
				keyTimes[i] = ((out[0] - minTime) / (maxTime-minTime))*1000;
			}

			unsigned int interpolationType = 0;
			switch (cchannel->sampler->interpolation)
			{
			case cgltf_interpolation_type_linear:
				interpolationType = Curve::LINEAR;
				break;
			case cgltf_interpolation_type_step:
				interpolationType = Curve::STEP;
				break;
			case cgltf_interpolation_type_cubic_spline:
				interpolationType = Curve::BSPLINE;
				break;
			case cgltf_interpolation_type_max_enum:
			default:
				break;
			}
			if (interpolationType == 0) break;

			int num_comp = cgltf_num_components(cchannel->sampler->output->type);
			float* keyValues = (float*)malloc(keyCount*num_comp*sizeof(float));
			for (int i = 0; i < keyCount; ++i) {
				float* out = keyValues+(num_comp*i);
				cgltf_accessor_read_float(cchannel->sampler->output, i, out, num_comp);
			}

			animation->createChannel(target, propertyId, keyCount, keyTimes, keyValues, interpolationType);
		}
		return animation;
	}

	Node* loadNode(cgltf_node* cnode) {
		Node* node = NULL;
		auto it = nodeMap.find(cnode);
		if (it != nodeMap.end()) {
			node = it->second;
		}
		else {
			node = Node::create(cnode->name);
		}

		Matrix m;
		cgltf_node_transform_local(cnode, m.m);
		node->setMatrix(m);

		Model* model = NULL;
		if (cnode->mesh) {
			model = loadMesh(cnode->mesh);
			if (model) {
				node->addComponent(model);
			}
		}

		if (cnode->skin) {
			MeshSkin* skin = loadSkin(cnode->skin);
			if (!model) {
				model = new Model();
				node->addComponent(model);
			}
			model->setSkin(skin);
		}

		for (int i = 0; i < cnode->children_count; ++i) {
			Node* child = loadNode(cnode->children[i]);
			node->addChild(child);
		}

		nodeMap[cnode] = node;
		return node;
	}

	Scene *loadScene(cgltf_data* data) {
		if (!data->scenes_count) {
			return NULL;
		}
		cgltf_scene *cscene = data->scenes;
		Scene* scene = Scene::create(cscene->name);

		for (int i = 0; i < data->skins_count; ++i) {
			cgltf_skin* skin = data->skins + i;
			for (int j = 0; j < skin->joints_count; ++j) {
				cgltf_node* joint = skin->joints[j];
				Node* node = Joint::create(joint->name);
				nodeMap[joint] = node;
			}
		}

		for (int i = 0; i < cscene->nodes_count; ++i) {
			cgltf_node* cnode = cscene->nodes[i];
			Node* node = loadNode(cnode);
			scene->addNode(node);
		}

		for (int i = 0; i < data->animations_count; ++i) {
			cgltf_animation* ca = data->animations + i;
			Animation* a = loadAnimation(ca);
			a->release();
		}

		return scene;
	}
};

Scene* GltfLoader::load(const std::string& file) {
	GltfLoaderImp imp;
	return imp.load(file.c_str());
}