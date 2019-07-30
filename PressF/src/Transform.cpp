#include "Transform.h"




Vec3 Transform::WorldFront() {
	return Vec3{ 0,0,1 };
}

Vec3 Transform::WorldRight() {
	return Vec3{ 1,0,0 };
}

Vec3 Transform::WorldUp() {
	return Vec3{ 0,1,0 };
}

Vec3 Transform::RotatePoint(Vec3 point, Vec3 rotation) {
	return  RotatePoint(point, GenRotMat(rotation));
}

Vec3 Transform::RotatePoint(Vec3 point, Mat4 rotation) {
	return  rotation * Vec4(point, 1);
}

bool Transform::TryGetClean()
{
	if (dirty == Dirty::None) return false;
	//PF_INFO("Cleaned {0}/ pos/ rot/scale, front, right,up", gameobject.name);
	//std::cout << position << " " << rotation << " " << scale << std::endl;
	//std::cout << front << " " << right << " " << up << std::endl;
	if (dirty == Dirty::Model) {
		rotMat = Transform::GenRotMat(rotation);

		up = RotatePoint(WorldUp(), rotMat);
		right = RotatePoint(WorldRight(), rotMat);
		front = RotatePoint(WorldFront(), rotMat);

		up = glm::normalize(up);
		right = glm::normalize(right);
		front = glm::normalize(front);


		if (parent) {
			const Mat4& parentRot = parent->rotMat;
			up = RotatePoint(up, parentRot);
			right = RotatePoint(right, parentRot);
			front = RotatePoint(front, parentRot);
			
			up = glm::normalize(up);
			right = glm::normalize(right);
			front = glm::normalize(front);
		}

		model = Transform::GenModel(scale, position, rotMat);
		dirty = Dirty::Acum; // IMPORTANTEEEEEE SINO LOS HIJOS NO SE ACTUALIZAN y yo no me actualizo bien
	}

	if (dirty == Dirty::Acum) {
		if (parent == nullptr) {
			acum = model;
		}
		else
		{
			acum = parent->GetAccumulated() * model;
		}

		for (auto child : children) {
			PF_ASSERT(child && "you shouldn't have a null child");
			child->SetDirty(Dirty::Acum);
		}
	}

	dirty = Dirty::None;
	return true;
}

Transform * Transform::SetDirty(Dirty newVal) {
	if (static_cast<int>(dirty) < static_cast<int>(newVal)) {
		dirty = newVal;
	}
	return this;
}

Mat4 Transform::GenModel(const Vec3 & scale, const Vec3 & position, const Vec3 & rotation) {
	return Transform::GenModel(scale, position, GenRotMat(rotation));
}

Mat4 Transform::GenModel(const Vec3 & scale, const Vec3 & position, const Mat4 & rotation) {
	return glm::translate(Mat4(1), position) * rotation * glm::scale(Mat4(1), scale);
}

Mat4 Transform::GenRotMat(const Vec3 & rotation) {
	Mat4 rot = Mat4(1);
	rot = glm::rotate(rot, glm::radians(rotation.x), Vec3(1, 0, 0));
	rot = glm::rotate(rot, glm::radians(rotation.y), Vec3(0, 1, 0));
	rot = glm::rotate(rot, glm::radians(rotation.z), Vec3(0, 0, 1));
	return rot;
}

Mat4 Transform::GetProjection(const Transform & t, bool isPerspective, float aspectRatio) {

	if (isPerspective) {
		return  glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 300.0f);
	}
	else
	{
		return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.f, 300.f);
	}
}

Mat4 Transform::GetView(const Transform & t) {
	return glm::lookAt(t.position, t.position + t.front, t.up);
}

Transform::Transform(GameObject & go) : gameObject(go) {

}

Transform::~Transform()
{
}

Transform & Transform::SetParent(Transform * other) {
	if (!other) {
		// poner como root node porque estoy es quitando el padre
		throw std::exception("Not implemented yet");
		return *this;
	}

	if (parent == other) {
		return *this; // it is done already
	}

	if (other->parent == this) {
		throw std::exception("CICLO INFINITO POR GAFO EN LA JERARQA\n");
	}

	//ponerme a mi de padre
	SetDirty(Dirty::Acum);

	parent = other; // el es mi padre

	other->children.push_back(this);// yo soy su hijo

	return *this;
}

const Mat4 & Transform::GetAccumulated() {
	TryGetClean();
	return acum;
}

const Mat4 & Transform::GetModel() {
	TryGetClean();
	return model;
}

const Vec3 & Transform::GetRotation() { return rotation; }

const Vec3 & Transform::GetScale() { return scale; }

const Vec3 & Transform::GetPosition() { return position; }

Transform & Transform::SetRotation(const Vec3 & val) {
	SetDirty(Dirty::Model);
	rotation = val;
	rotation = glm::mod(val, 360.0f);
	return *this;
}

Transform & Transform::SetScale(const Vec3 & val) { SetDirty(Dirty::Model); scale = val; return *this; }

Transform & Transform::SetPosition(const Vec3 & val) { SetDirty(Dirty::Model); position = val; return *this; }

Transform & Transform::SetRotation(float x, float y, float z) { return SetRotation(Vec3(x, y, z)); }

Transform & Transform::SetScale(float x, float y, float z) { return SetScale(Vec3(x, y, z)); }

Transform & Transform::SetPosition(float x, float y, float z) { return SetPosition(Vec3(x, y, z)); }

Transform & Transform::Translate(const Vec3 & val) { return SetPosition(position + val); }

Transform & Transform::Rotate(const Vec3 & val) { return SetRotation(rotation + val); }

Transform & Transform::Scale(const Vec3 & val) { return SetScale(scale + val); }

Transform & Transform::Translate(float x, float y, float z) { return Translate(Vec3(x, y, z)); }

Transform & Transform::Rotate(float x, float y, float z) { return Rotate(Vec3(x, y, z)); }

Transform & Transform::Scale(float x, float y, float z) { return Scale(Vec3(x, y, z)); }

const Vec3 & Transform::Front() { return front; }

const Vec3 & Transform::Right() { return right; }

const Vec3 & Transform::Up() { return up; }
