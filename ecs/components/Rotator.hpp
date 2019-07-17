#pragma once

#include "../framework/StartableComponent.hpp"
#include "../internal/API.hpp"

#include <iostream>
class API;
class Rotator : public UpdatableComponent
{
public:
	Vec3_t speed = {0,0,0};
	Rotator(Vec3_t speed = {0,0,0}) : UpdatableComponent() {
		this->speed = speed;
	}
	Rotator(float x, float y, float z) : UpdatableComponent() {
		this->speed = Vec3_t(x, y, z);
	}


	virtual void Update(const API &api) {
		transform->Rotate(speed * (float) api.DeltaTime());
	}
private:

};
