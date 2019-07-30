#pragma once
#include "types.h"
#include "Logger.h"


class GameObject;
class Transform;
enum class Dirty {
	None,
	Acum,
	Model,
};

class Application;

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
	Mat4 MVP = Mat4(1);
	glm::mat3 normalMatrix;

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
	static Mat4 GetProjection(const Transform & t, bool isPerspective, float aspectRatio);
	static Mat4 GetView(const Transform & t);
	//static Mat4& ApplyRotation(const Vec3 & rotation, Mat4& model);

	friend class Application;
	/*
	Returs true if it was dirty.
	Cleans accumulated matrix
	*/
	bool TryGetClean();
	/*
	we only update the state if we are getting dirtier ;)
	*/
	Transform * SetDirty(Dirty);
};

