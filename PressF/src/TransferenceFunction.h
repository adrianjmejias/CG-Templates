#pragma once
#include "types.h"
#include "core.h"


class TransferencePoint : public Vec3
{
public:
	bool isOver(int x, int y);
};



class TransferenceFunction : public Component
{
	bool clicked = true;
	int indexEditing = -1;
	int x, y;

	GLuint texHue{0};
	GLuint texAlpha{0};
	GLuint texPreview{0};
	GLfloat dataPreview[256]{};
	// acá asumamos que siempre está ordenado con respecto a X
	std::vector<TransferencePoint> points;

public:
	virtual void Update();
	virtual void HandleEvent(const SDL_Event &e);

	TransferenceFunction(COMP_PARAMS);

	std::tuple<int, int> GetClickedPoint(int x, int y);
	void OnClickDown(int x, int y);
	void OnClickUp(int x, int y);
	void OnClickMove(int x, int y);
	void Render(Mesh &bg, Mesh &preview, Mesh &alpha, Mesh &hue, ShaderProgram &shader);
		
};