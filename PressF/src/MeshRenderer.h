#pragma once
#include "Component.h"


class Mesh;
class MeshRenderer : public Component
{
public:
	Mesh *mesh;
	MeshRenderer(COMP_PARAMS, Mesh *);
	~MeshRenderer();

	// Inherited via Component
	virtual void Update() override;
	virtual void HandleEvent(const SDL_Event & e) override;
};

