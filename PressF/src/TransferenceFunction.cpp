#pragma once
#include "types.h"
#include "TransferenceFunction.h"
float distClickPoint = 5.f;

bool TransferencePoint::isOver(int x, int y)
{
	return glm::distance(*reinterpret_cast<Vec3*>(this), Vec3(x, y, this->z)) <= distClickPoint;
}


void TransferenceFunction::Update()
{


}
void TransferenceFunction::HandleEvent(const SDL_Event &e)
{

	int x, y;
	SDL_GetMouseState(&x, &y);
	switch (e.type) {
	case SDL_QUIT:
		break;
	case SDL_MOUSEBUTTONDOWN:
		OnClickDown(x, y);
		break;
	case SDL_MOUSEBUTTONUP:
		OnClickUp(x, y);
		break;
	case SDL_MOUSEMOTION:
		OnClickMove(x, y);
	}
}


TransferenceFunction::TransferenceFunction(COMP_PARAMS) COMP_INIT
{
	glGenTextures(1, &texPreview);
	glBindTexture(GL_TEXTURE_1D, texPreview);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, 256, 0, GL_RGBA, GL_FLOAT, dataPreview);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

std::tuple<int, int> TransferenceFunction::GetClickedPoint(int x, int y)
{
	int leftMost = (points.size() > 0) ? 0 : -1;

	for (int ii = 0; ii < points.size(); ii++)
	{
		TransferencePoint &p = points[ii];
		if (p.isOver(x, y)) {
			return { leftMost, ii };
		}
		else
		{
			leftMost = ii;
		}
	}
	return { leftMost, -1 };
}

void TransferenceFunction::OnClickDown(int x, int y)
{
	clicked = true;

	auto[leftMost, indexClicked] = GetClickedPoint(x, y);


	if (indexClicked >= 0)
	{

	}
	else
	{

	}


}
void TransferenceFunction::OnClickUp(int x, int y)
{
	clicked = false;
}
void TransferenceFunction::OnClickMove(int x, int y)
{
	if (clicked)
	{
		bool clickedSlider = false;



		if (!clickedSlider)
		{

		}


	}
}

void TransferenceFunction::Render(Mesh &bg, Mesh &preview, Mesh &alpha, Mesh &hue, ShaderProgram &shader)
{

}