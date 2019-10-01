#pragma once
#include "types.h"
#include "core.h"


class TransferencePoint : public Vec4
{
public:
	Vec4 color;
	bool isOver(int x, int y);

};



class TransferenceFunction : public Component
{
	bool ignoreMouse = true;
	bool clicked = true;
	int indexEditing = 0;
	int x, y;
	float selectedHue{ 0 };
	GLuint texHue{ 0 };
	GLuint texAlpha{ 0 };
	std::vector<Vec4> dataPreview;
	int width;
	// acá asumamos que siempre está ordenado con respecto a X
	std::vector<TransferencePoint> points;
public:
	GLuint texPreview{ 0 };
	virtual void Update();
	virtual void HandleEvent(const SDL_Event &e);

	TransferenceFunction(COMP_PARAMS);

	void Render(ShaderProgram * shaderUI, GLuint quadVAO, int width)
	{
		this->width = width;
		shaderUI->Use();
		glBindVertexArray(quadVAO);
		{
			shaderUI->SetUniform("zIndex", 0);
			shaderUI->SetUniform("model", transform->GetAccumulated());
			shaderUI->SetUniform("tf", 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_1D, texPreview);

			//shaderUI->SetUniform("projection", projection);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
		glBindVertexArray(0);
	}

	void RenderUI() {

		TransferencePoint &p = points[indexEditing];

		ImGui::Begin("Color picker");
		{
			bool change = false;

			ImGui::SliderInt("picked", &indexEditing, 0, points.size() - 1);
			change = change || ImGui::ColorEdit4("hue", &p.color[0], 0);

			if (change)
			{
				UpdateTransfer();
			}
		}
		ImGui::End();
	}


	void UpdateTransfer()
	{

		while (dataPreview.size() < width)
		{
			dataPreview.emplace_back();
		}



		std::vector<int> indexes;


		float space = width;




		for (size_t ii = 0; ii < points.size(); ii++)
		{
			float lerpFactor = float(ii / (points.size() - 1));
			indexes.push_back(space* lerpFactor);
		}

		for (size_t ii = 0; ii < points.size() - 1; ii++)
		{
			int init = indexes[ii];
			int end = indexes[ii + 1];

			for (size_t jj = init, iLerp = 0, eLerp = end - init; jj < end; jj++, iLerp++)
			{
				float lerpFactor = iLerp / float(eLerp);
				dataPreview[jj] =
					points[ii].color * (1-lerpFactor)
					+ (lerpFactor) *points[ii + 1].color;

			}
		}
		UpdateTexture();
	}

	
	void UpdateTexture()
	{

		if (texPreview == 0)
		{
			glDeleteTextures(1, &texPreview);
		}

		glGenTextures(1, &texPreview);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, texPreview);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, this->width, 0, GL_RGBA, GL_FLOAT, dataPreview.data());
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	}
private:
	std::tuple<int, int> GetClickedPoint(int x, int y);
	void OnClickDown(int x, int y);
	void OnClickUp(int x, int y);
	void OnClickMove(int x, int y);
		
};