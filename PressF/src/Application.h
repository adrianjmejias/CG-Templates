#pragma once

#include "core.h"

#include "Camera.h"
#include "Light.h"
#include "MeshRenderer.h"




class Application
{
public:
	//Every object created must have a new id
	//unsigned int GLOBAL_ID; //replaced with extern
	SDL_Window *win;
	SDL_GLContext glContext;
	int win_width = 800;
	int win_heigth = 600;

	//static double deltaTime;
	int mouse_lastPosX;
	int mouse_lastPosY;
	int mouse_deltaX;
	int mouse_deltaY;

	unsigned long long NOW, LAST;

	bool running;
	bool captureMouse = true;

	bool KeyPressed[256];
	Vec4 bgColor{ 0.2f,0.2f,0.2f, 1 };

	int actCam = 0;
	ShaderProgram* shaders[30]{ nullptr };
	std::vector<Light*> LIGHTS;
	//std::vector < Camera*> orderedCameras;
	std::vector < Camera*> cameras;
	std::vector<MeshRenderer*> renderers;
	std::vector<Model> models;
	std::vector<GameObject *> rootNodes;
	std::map<std::string, Shader*> shadersLoaded;
	std::map<std::string, Texture*> texturesLoaded;
	CubeMap *cubeMap;
public:



	//static double DeltaTime() { return deltaTime; }
	void GLCreate(objl::Loader & model);
	void Steal(Component *);
	void HandleEvents();
	unsigned int VBO, VAO, EBO;
	void Setup(const std::vector<std::string>&, const std::vector<std::tuple<std::string, std::string>>&);
	void SetupScene();
	void SetupModels(const std::vector<std::string>& objPathsp);
	void SetupShaders(const std::vector<std::tuple<std::string, std::string>>& shaderPaths);

	//void RenderFrame() {

	//}




	ShaderProgram* shaderTri;


	void LoopMain();
	void LoopUI();
	void LoopUpdate();
	void LoopRender();

	void DrawObjects(const Mat4& view, const Mat4& projection, std::vector<const Mesh *> meshes, std::function<bool(const ShaderProgram& shader, const Material& MAT)>  PreReqs);

	void DrawObjects(const Mat4& view, const Mat4& projection, std::vector<const MeshRenderer *> meshes, std::function<bool(const ShaderProgram& shader, const Material& MAT, const MeshRenderer& mesh)>  PreReqs)
	{
		auto meshesToRender = meshes;
		for (const MeshRenderer * const meshRen : meshesToRender) {

			const Mesh * mesh = meshRen->mesh;
			const Material &MAT = mesh->mat;
			const ShaderProgram &shader = *shaders[MAT.illum];

			shader.Use();
			GLCALL(glBindVertexArray(mesh->VAO));


			if (PreReqs(shader, MAT, *meshRen)) {

				const Mat4 &model = meshRen->transform.GetAccumulated();
				SET_UNIFORM(shader, model);
				SET_UNIFORM(shader, view);
				SET_UNIFORM(shader, projection);
				GLCALL(glDrawElements(GL_TRIANGLES, mesh->nElem, GL_UNSIGNED_INT, 0));
			}

		}
		GLCALL(glBindVertexArray(0));

	}


	Application();
	~Application();
};
