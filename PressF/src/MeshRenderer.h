#pragma once
#include "Component.h"


class Mesh;
class MeshRenderer : public Component
{
public:
	Mesh *mesh;
	MeshRenderer(GameObject& go, Transform& t, Mesh* m);
	~MeshRenderer();

	// Inherited via Component
	virtual void Update() override;
	virtual void HandleEvent(const SDL_Event & e) override;
};

