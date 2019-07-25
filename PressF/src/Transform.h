#pragma once
#include "types.h"
#include "Logger.h"


class GameObject;
class Transform;
enum class Dirty;

enum class Dirty {
	None,
	Acum,
	Model,
};
class Application;
class GameObject;
class Transform {
public:

	Transform(GameObject& go);
	Transform::~Transform();
	GameObject& gameObject;
	Transform *parent = nullptr;
	std::vector<Transform *> children;

	//GameObject &gameobject;
	Vec3 right = Transform::WorldRight();
	Vec3 up = Transform::WorldUp();
	Vec3 front = Transform::WorldFront();
	Vec3 position{ 0, 0, 0 };
	Vec3 rotation{ 0, 0, 0 };
	Vec3 scale{ 1, 1, 1 };
	Mat4 model = Mat4(1);
	Mat4 acum = Mat4(1);
	Mat4 rotMat = Mat4(1);

	Dirty dirty;


	Transform& SetParent(Transform *other);
	const Mat4& GetAccumulated();
	const Mat4& GetModel();
	const Vec3& GetRotation();
	const Vec3& GetScale();
	const Vec3& GetPosition();
	Transform& SetRotation(const Vec3& val);
	Transform& SetScale(const Vec3& val);
	Transform& SetPosition(const Vec3& val);
	Transform& SetRotation(float x, float y, float z);
	Transform& SetScale(float x, float y, float z);
	Transform& SetPosition(float x, float y, float z);
	Transform& Translate(const Vec3& val);
	Transform& Rotate(const Vec3& val);
	Transform& Scale(const Vec3& val);
	Transform& Translate(float x, float y, float z);
	Transform& Rotate(float x, float y, float z);
	Transform& Scale(float x, float y, float z);
	const Vec3& Front();
	const Vec3& Right();
	const Vec3& Up();

	static Vec3 WorldFront();
	static Vec3 WorldRight();
	static Vec3 WorldUp();
	static Vec3 RotatePoint(Vec3 point, Vec3 rotation);
	static Vec3 RotatePoint(Vec3 point, Mat4 rotation);
	static Mat4 GenModel(const Vec3 &scale, const Vec3 &position, const Vec3 &rotation);
	static Mat4 GenModel(const Vec3 &scale, const Vec3 &position, const Mat4 &rotation);
	static Mat4 GenRotMat(const Vec3 &rotation);
	Mat4& ApplyRotation(const Vec3 & rotation, Mat4& model);

private:
	friend class Application;
	/*
	Returs true if it was dirty.
	Cleans accumulated matrix
	*/
	bool TryGetClean();
private:
	/*
	we only update the state if we are getting dirtier ;)
	*/
	Transform * SetDirty(Dirty);
};

#define STB_IMAGE_IMPLEMENTATION   // use of stb functions once and for all
#define STB_IMAGE_STATIC
#include <stb_image.h>

class ShaderProgram;
class CubeMap {
public:
	unsigned int skyboxVAO = 0;
	
	unsigned int skyboxVBO = 0;
	unsigned int textureID = 0;
	ShaderProgram* shader = nullptr;

	int width, height, nrComponents;

	CubeMap(std::vector<std::string> faces) {


		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	void Draw() {

	
	}
};