
#include "GLtfLoader.h"

#define CGLTF_IMPLEMENTATION
#include "../3rd/cgltf.h"

#include "../scene/MeshPart.h"

using namespace gameplay;

class GltfLoaderImp {
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
				if (old != attr->data) {
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

		return model;
	}

	Node* loadNode(cgltf_node* cnode) {
		Node* node = Node::create(cnode->name);
		
		Matrix m;
		cgltf_node_transform_local(cnode, m.m);
		node->setMatrix(m);

		if (cnode->mesh) {
			Model* m = loadMesh(cnode->mesh);
			if (m) {
				node->addComponent(m);
			}
		}

		return node;
	}

	Scene *loadScene(cgltf_data* data) {
		if (!data->scenes_count) {
			return NULL;
		}
		cgltf_scene *cscene = data->scenes;
		Scene* scene = Scene::create(cscene->name);

		for (int i = 0; i < cscene->nodes_count; ++i) {
			cgltf_node* cnode = cscene->nodes[i];
			Node* node = loadNode(cnode);
			scene->addNode(node);
		}

		return scene;
	}
};

Scene* GltfLoader::load(const std::string& file) {
	GltfLoaderImp imp;
	return imp.load(file.c_str());
}