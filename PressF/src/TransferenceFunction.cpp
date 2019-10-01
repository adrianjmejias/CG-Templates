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
	case SDL_MOUSEBUTTONDOWN:
		if (!ignoreMouse)
		{
			OnClickDown(x, y);
		}

		break;
	case SDL_MOUSEBUTTONUP:
		if (!ignoreMouse) 
		{
			OnClickUp(x, y);
		}

		break;
	case SDL_MOUSEMOTION:
		if (!ignoreMouse) { OnClickMove(x, y); }
	case SDL_KEYDOWN:

		auto keyPressed = e.key.keysym.scancode;

		switch (keyPressed)
		{
			case SDL_Scancode::SDL_SCANCODE_LEFT:
				if (indexEditing > 0)
				{
					indexEditing--;
				}
			break;
			case SDL_Scancode::SDL_SCANCODE_RIGHT:
				this->indexEditing = -1;
				if (indexEditing + 1 < points.size())
				{
					indexEditing++;
				}
				break;
			case SDL_Scancode::SDL_SCANCODE_N:
				points.push_back(TransferencePoint());

				int last = points.size() - 1;
				//std::swap(points[last], points[last-1]);
				UpdateTransfer();
				break;
		}
	}
}


TransferenceFunction::TransferenceFunction(COMP_PARAMS) COMP_INIT
{
	points.emplace_back();
	points.emplace_back();
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
	//clicked = true;

	//auto[leftMost, indexClicked] = GetClickedPoint(x, y);


	//if (indexClicked >= 0)
	//{

	//}
	//else
	//{

	//}


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
