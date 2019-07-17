#pragma once
#include <vector>
#include <queue>
#include <functional>
#include <glad/glad.h> 
//#include "../internal/API.hpp"
#include "../components/MeshRenderer.hpp"
//#include "../components/MeshRenderer.hpp"
#include "../components/Light.hpp"
#include "../components/Camera.hpp"


//extern Camera *camera;

class API;
//static const std::function<bool(Camera*, Camera*)> cmp = [](Camera* left, Camera* right) { return left->power > right->power; };
class SystemRenderer
{
	std::vector<MeshRenderer*> renderers;
	std::vector<Light*> lights;

//, decltype(cmp)

	std::queue < Camera*, std::vector<Camera*> > cameras;
	unsigned int uniformBuffer;


	glm::mat4 projection;
	glm::mat4 view;

public:
	API *c_api;

	SystemRenderer() : projection(glm::mat4(1)), view(glm::mat4(1)) {
	}

	bool Init() {

		glEnable(GL_BLEND);


		return true;
	}

	~SystemRenderer() {
	}

	void AddRenderer(MeshRenderer *renderer) {
		renderers.push_back(renderer);
	}
	void AddLight(Light *light) {
		lights.push_back(light);
	}

	void AddCamera(Camera *cam) {
		cameras.push(cam);
	}

	Vec3_t	ambient;

	void Update() {

		Camera* actCam = nullptr;

		if (cameras.empty()) {
			std::cout << "No Camera detected" << std::endl;
			return;
		}
		else
		{
			actCam = cameras.front();
		}


		if (actCam->IsDirty()) 
		{
		}
		view = actCam->GetTransform().GetAccumulated();
		projection = actCam->projection;


		for (auto renderer : renderers) {

			Mesh &mesh = *renderer->mesh;
			ShaderProgram &shader = *mesh.materials[0]->shader;
			Material &mat = *mesh.materials[0].get();

			GLCall(glBindVertexArray(mesh.VAO));
			shader.use();

			shader.setMat4("transform", renderer->GetTransform().GetAccumulated());
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);

			shader.setVec3("cameraPos", actCam->Position);
			
			shader.setFloat("shininess", mat.shiny);
			shader.setFloat("roughness", mat.rough);
			shader.setFloat("covariance", mat.covariance);
			shader.setFloat("reflectance", mat.reflectance);
			shader.setVec3("kD", mat.diffuse);
			shader.setVec3("kS", mat.specular);
			shader.setVec3("kA", mat.color);


			const int maxPointLights = 3;
			const int maxDirectionals = 3;
			const int maxSpotlights = 3;

			int actPointLights = 0;
			int actDirectionals = 0;
			int actSpotlights = 0;

			for (auto l : lights) {
				std::string name;
				char buff[10];
				_itoa_s(actPointLights, buff, 10);
				
				if (!l->isOn) continue;
				
				switch (l->type)
				{
				case LightType::DIRECTIONAL :
					name = "directionals[" + std::string(buff) + "]";
					shader.setVec3(name+".direction", l->direction);
					shader.setBool(name + ".isOn", l->isOn);

					name.append(".color");
					shader.setVec3(name + ".ambient", l->color);
					shader.setVec3(name + ".diffuse", l->diffuse);
					shader.setVec3(name + ".specular", l->specular);

					actDirectionals++;
					break;
				case LightType::POINT:

					
					name = "points[" + std::string(buff) + "]";

					shader.setVec3(name + ".position", l->GetTransform().GetPosition());
					shader.setVec3(name + ".attenuation", l->attenuation);
					shader.setBool(name + ".isOn", l->isOn);

					name.append(".color");

					shader.setVec3(name + ".ambient", l->color);
					shader.setVec3(name + ".diffuse", l->diffuse);
					shader.setVec3(name +  ".specular", l->specular);


					actPointLights++;
					break;
				case LightType::SPOTLIGHT:

					name = "spots[" + std::string(buff) + "]";
					shader.setBool(name + ".isOn", l->isOn);
					shader.setVec3(name + ".position", l->GetTransform().GetPosition());
					shader.setVec3(name + ".direction", l->direction);
					shader.setVec3(name + ".attenuation", l->attenuation);
					shader.setFloat(name + ".innerAngle", glm::radians( l->innerAngle));
					shader.setFloat(name + ".outerAngle", glm::radians(l->outterAngle));

					name.append(".color");

					shader.setVec3(name + ".ambient", l->color);
					shader.setVec3(name + ".diffuse", l->diffuse);
					shader.setVec3(name + ".specular", l->specular);
					actSpotlights++;
					break;

				default:
					break;
				}
			}

			glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(mesh.vertex.size()) * 3);
			//GLCall(glDrawElements(GL_TRIANGLES, mesh.face.size()*3, GL_UNSIGNED_INT, 0));
			GLCall(glBindVertexArray(0));
		}
	}


	static void RegisterGroup(std::vector<Mesh*> &meshes, std::shared_ptr<Material> defaultMat) {

		for (auto mesh : meshes) {
			if (mesh->materials.size() < 1) {
				mesh->materials.push_back(defaultMat);
			}
			else {
				mesh->materials[0]->shader = defaultMat->shader;
			}


			GLCall(glGenVertexArrays(1, &mesh->VAO)); // FIRST BIND VAOOOOO SIEMPRE EEEEN E E E E E) 

			GLCall(glGenBuffers(1, &mesh->VBO));
			GLCall(glGenBuffers(1, &mesh->VBO_NORM_VERTEX));
			GLCall(glGenBuffers(1, &mesh->VBO_UV));
			GLCall(glGenBuffers(1, &mesh->EBO));


			GLCall(glBindVertexArray(mesh->VAO));
			{
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO));
				{
					const unsigned int layout = 0;
					const unsigned int sizeVertex = 3;
					const unsigned int stride = sizeVertex * sizeof(float);
					
					GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3_t) * mesh->vertex.size(), &mesh->vertex[0], GL_STATIC_DRAW));
					GLCall(glVertexAttribPointer(layout, sizeVertex, GL_FLOAT, GL_FALSE, 0, (void*)0));
					GLCall(glEnableVertexAttribArray(layout));


				}

				if (mesh->hasUVs) {
					GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_UV));
					{
						const unsigned int layout = 1;
						const unsigned int sizeVertex = 3;
						const unsigned int stride = sizeVertex * sizeof(float);

						GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3_t) * mesh->uv.size(), &mesh->uv[0], GL_STATIC_DRAW));
						GLCall(glVertexAttribPointer(layout, sizeVertex, GL_FLOAT, GL_FALSE, 0, (void*)0));
						GLCall(glEnableVertexAttribArray(layout));


					}
				}

				GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_NORM_VERTEX));
				{
					const unsigned int layout = 2;
					const unsigned int sizeVertex = 3;
					const unsigned int stride = sizeVertex * sizeof(float);

					GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3_t) * mesh->vertexNormal.size(), &mesh->vertexNormal[0], GL_STATIC_DRAW));
					GLCall(glVertexAttribPointer(layout, sizeVertex, GL_FLOAT, GL_TRUE, 0, (void*)0));
					GLCall(glEnableVertexAttribArray(layout));
				}

	/*			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
				{
					GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_t) * mesh->face.size(), mesh->face.data(), GL_STATIC_DRAW));
				}*/



				//shader.setMat4("view", entity->transform->GetAccumulated());
				//shader.setMat4("projection", entity->transform->GetAccumulated());


				GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

				// layoutposition 0
				// 3 size of vertex type
				// bytes to next vertex

				//GLCall(glBindTexture(GL_TEXTURE_2D, mesh->materials[0]->m));
			}
			GLCall(glBindVertexArray(0));

		}

	}
	static std::vector<std::shared_ptr<Material>>ExtractMaterials(std::vector<Mesh*> meshes) {
		std::vector<std::shared_ptr<Material>> materials;

		for (auto mesh : meshes) {
			for (auto material : mesh->materials) {
				materials.push_back(material);
			}
		}
		return materials;
	}
};
