#pragma once
#include "types.h"
#include "Logger.h"

enum class Dirty {
	None,
	Acum,
	Model,
};
class Application;

class Transform {
public:

	Transform();
	Transform::~Transform();

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

	Dirty dirty = Dirty::None;


	Transform * SetParent(Transform *other);
	Mat4& GetAccumulated();
	Mat4& GetModel();
	Vec3 GetRotation();
	Vec3 GetScale();
	Vec3 GetPosition();
	Transform* SetRotation(const Vec3& val);
	Transform* SetScale(const Vec3& val);
	Transform* SetPosition(const Vec3& val);
	Transform* SetRotation(float x, float y, float z);
	Transform* SetScale(float x, float y, float z);
	Transform* SetPosition(float x, float y, float z);
	Transform* Translate(const Vec3& val);
	Transform* Rotate(const Vec3& val);
	Transform* Scale(const Vec3& val);
	Transform* Translate(float x, float y, float z);
	Transform* Rotate(float x, float y, float z);
	Transform* Scale(float x, float y, float z);
	Vec3 Front();
	Vec3 Right();
	Vec3 Up();

	static Vec3 WorldFront();
	static Vec3 WorldRight();
	static Vec3 WorldUp();
	static Vec3 RotatePoint(Vec3 point, Vec3 rotation);
	static Vec3 RotatePoint(Vec3 point, Mat4 rotation);
	static Mat4 GenModel(const Vec3 &scale, const Vec3 &position, const Vec3 &rotation);
	static Mat4 GenModel(const Vec3 &scale, const Vec3 &position, const Mat4 &rotation);
	static Mat4 GenRotMat(const Vec3 &rotation);

private:
	friend class Application;
	/*
	Returs true if it was dirty.
	Cleans accumulated matrix
	*/
	bool TryGetClean();
	/*
	we only update the state if we are getting dirtier ;)
	*/
	Transform * SetDirty(Dirty newVal);
};




