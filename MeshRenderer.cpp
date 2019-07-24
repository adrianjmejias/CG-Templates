#include "MeshRenderer.h"



MeshRenderer::MeshRenderer(GameObject & go, Transform & t, Mesh * m) : Component(go, t), mesh(m)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
}

void MeshRenderer::HandleEvent(const SDL_Event & e)
{
}
