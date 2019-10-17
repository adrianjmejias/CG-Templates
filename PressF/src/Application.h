#pragma once

#include "core.h"

#include "Camera.h"
#include "OGLCamera.h"
#include "Light.h"

#define ADD_MAP(NAME)\
		if (!mat.NAME.empty() && (texturesLoaded.find(mat.NAME) == texturesLoaded.end())) {\
			texturesLoaded[mat.NAME] = Texture::TextureFromFile(mat.NAME.c_str(), basePath);\
		}\



#define ADD_MAP_TO_MATERIAL(NAME)\
	if (!mesh.mat.NAME.empty()) {\
		mesh.mat.s##NAME = texturesLoaded.at(mesh.mat.NAME);\
	}\


struct VolumeRush
{
	GLuint quadVAO = 0, quadVBO= 0, cubeVAO =0, cubeVBO=0, volume=0;
	GLuint nQuad=0, nCube=0;
	int axis = 0;
	float timePassed = 0;
	

	void Render(float deltaTime, const Mat4 &view, const Mat4 &projection, ShaderProgram* shaderQuad)
	{
		
		shaderQuad->Use();
		glBindVertexArray(quadVAO);
		{

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_3D, volume);

			shaderQuad->SetUniform("volume", 0);
			shaderQuad->SetUniform("view", view);
			shaderQuad->SetUniform("projection", projection);
			shaderQuad->SetUniform("deltaTime", timePassed * (2.f/10));
			shaderQuad->SetUniform("axis", axis);

			glDrawArrays(GL_TRIANGLES, 0, 4);
		}
		
		glBindVertexArray(0);
		timePassed += deltaTime;
		if (timePassed > 1)
		{
			timePassed = 0;

			axis = (axis + 1) % 3;
		}
	}


	void Setup(GLuint volId) 
	{

		volume = volId;

		{
			float quadVertices[] = {
				// positions        	// texture Coords
				-1.0f,1.f,0.0f,
				0.0f,
				1.0f,
				-1.0f,-1.0f,0.0f,
				0.0f,
				0.0f,
				1.0f, 1.f,0.0f,
				1.0f,
				1.0f,
				1.0f,-1.0f,	0.0f,
				1.0f,
				0.0f,
			};

			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
			glBindVertexArray(0);

			nQuad = 4;
		}
	}
	~VolumeRush()
	{
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &cubeVBO);
		glDeleteBuffers(1, &quadVBO);
	}
};


class Application
{
public:
	//Every object created must have a new id
	//unsigned int GLOBAL_ID; //replaced with extern
	SDL_Window *win;
	SDL_GLContext glContext;
	int win_width = 705;
	int win_heigth = 705;
	int mouse_lastPosX;
	int mouse_lastPosY;
	int mouse_deltaX;
	int mouse_deltaY;
	//static double deltaTime;

	unsigned long long NOW, LAST;

	float IOR_BG = 1;
	float heightScale = 2.f;
	float minLayers = 8;
	float maxLayers = 200;

	bool running;
	bool captureMouse = false;
	bool show_demo_window = true;
	bool show_another_window = true;
	bool KeyPressed[256];
	Vec4 bgColor{ 0.2f,0.2f,0.2f, 1 };

	std::vector<ShaderProgram*> shaders;
	std::vector<Light*> LIGHTS;

	std::vector<GameObject *> rootNodes;
	std::map<std::string, Shader*> shadersLoaded;
	std::map<std::string, Texture*> texturesLoaded;

	VolumeRush vol;
	std::unique_ptr<OGLCamera> camera;
	FlyingController * fc;
	ShaderProgram* rayCastShader = nullptr;


	unsigned int volumeId;
public:

	//static double DeltaTime() { return deltaTime; }


	void Steal(Component *);
	void LoopEvents();
	void Setup(const std::vector<std::string>&, const std::vector<std::tuple<std::string, std::string>>&);
	void SetupScene();
	void SetupShaders(const std::vector<std::tuple<std::string, std::string>>& shaderPaths);

	void LoopMain();
	void LoopUI();
	void LoopUpdate();
	void LoopRender();

	Application();
	~Application();
};
