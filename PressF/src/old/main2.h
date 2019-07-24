#pragma once

#pragma region Includes
#include <OBJ_Loader.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glm/gtc/random.hpp>




#define NKE_COLOR2VEC4(c) (Vec4{c.r,c.g,c.b,c.a})

std::string to_string(const Vec3& v) {
	char c[30];

	std::string s = "(";

	sprintf(c, "%.3f", v[0]);

	s.append(c);
	s.append(", ");

	sprintf(c, "%.3f", v[1]);

	s.append(c);
	s.append(", ");

	sprintf(c, "%.3f", v[2]);

	s.append(c);
	s.append(" )");

	return s;
}

Vec3 nke_Vec3(Vec3 v, const std::string &label, const float min, const float max) {
	//sprintf(buffer, "%.2f, %.2f, %.2f", position[0], position[1], position[2]);
	if (nk_combo_begin_label(ctx, (label + to_string(v)).data(), nk_vec2(200, 200))) {
		nk_layout_row_dynamic(ctx, 25, 1);

		v.x = nk_propertyf(ctx, "X:", min, v.x, max, 1, 0.01f);
		v.y = nk_propertyf(ctx, "Y:", min, v.y, max, 1, 0.01f);
		v.z = nk_propertyf(ctx, "Z:", min, v.z, max, 1, 0.01f);

		nk_combo_end(ctx);
	}

	return v;
}

void nke_Color(Color &c, std::string label) {
	nk_label(ctx, label.c_str(), nk_text_align::NK_TEXT_ALIGN_LEFT);
	if (nk_combo_begin_color(ctx, nk_rgb_cf(c), nk_vec2(200, 400))) {
		nk_layout_row_dynamic(ctx, 120, 1);
		c = nk_color_picker(ctx, c, NK_RGBA);
		nk_layout_row_dynamic(ctx, 25, 2);
		nk_layout_row_dynamic(ctx, 25, 1);

		c.r = nk_propertyf(ctx, "#R:", 0, c.r, 1.0f, 0.01f, 0.005f);
		c.g = nk_propertyf(ctx, "#G:", 0, c.g, 1.0f, 0.01f, 0.005f);
		c.b = nk_propertyf(ctx, "#B:", 0, c.b, 1.0f, 0.01f, 0.005f);
		c.a = nk_propertyf(ctx, "#A:", 0, c.a, 1.0f, 0.01f, 0.005f);
		nk_combo_end(ctx);
	}
}




enum class ClampType {
	REPEAT = GL_REPEAT,
	MIRROR_REPEAT = GL_MIRRORED_REPEAT,
	EDGE = GL_CLAMP_TO_EDGE,
	BORDER = GL_CLAMP_TO_BORDER,
};

enum class TexInterpolation {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
};









	std::string map_Ka;
	std::string map_Kd;
	std::string map_Ks;
	std::string map_Ns;
	std::string map_d;
	std::string map_bump;



#pragma endregion Shading

std::vector<GameObject*> objects;
std::vector<Mesh*> meshes;

class Component {
	friend class GameObject;
protected:
	int enabled = true;
public:
	std::string name;
	int openUI = false;
	Transform &transform;
	GameObject &gameobject;


	bool Enabled();
	Component(GameObject &o, Transform &t, const std::string n) : transform(t), gameobject(o)
	{
		name = n;
	}

	virtual void UI() = 0;
	virtual void Update() = 0;
	virtual void HandleEvent(const SDL_Event &e) = 0;
};

class GameObject {
	int _id;
	friend class SystemRenderer;
	std::list<Component*> components;
	int enabled = true;
public:


	// esto puede llegar a ser orden N y debe ser seteado como un evento asi como lo es Dirty
	bool IsEnabled() {
		return enabled;// && (transform.parent) ? transform.parent->gameobject.IsEnabled() : true;
	}
	int openUI = false;
	Transform transform;
	std::string name;
	GameObject(const std::string &n) : transform(*this) {
		_id = GO_ID++;
		name = std::to_string(_id).append(n);
	}
	void Update() {
		if (IsEnabled())
			for each (auto comp in components)
			{
				PF_ASSERT(comp && "COMPONENT IS NULL");

				//PF_INFO("Object {0}", name);
				if (comp->Enabled()) {
					transform.TryGetClean();
					comp->Update();
				}
			}
	}

	void HandleEvent(const SDL_Event &e) {
		for each (auto comp in components)
		{
			PF_ASSERT(comp && "COMPONENT IS NULL");
			if (comp->Enabled()) {
				//PF_INFO("HANDLE EVENT ,{0}", this->name);
				comp->HandleEvent(e);
			}
		}
	}
	void UI() {
		if (nk_tree_push_hashed(ctx, NK_TREE_TAB, name.data(), static_cast<nk_collapse_states>(openUI), name.data(), 5, __LINE__)) {
			// height, width, row
			const int val = 6000;
			//nk_layout_row_static(ctx, 18, 100, 1);
			nk_checkbox_label(ctx, "Enabled", &enabled);
			transform.SetPosition(nke_Vec3(transform.GetPosition(), "Position", -val, val));
			transform.SetRotation(nke_Vec3(transform.GetRotation(), "Rotation", -val, val));
			transform.SetScale(nke_Vec3(transform.GetScale(), "Scale", -val, val));
			for each (auto comp in components)
			{
				PF_ASSERT(comp && "component is null");

				if (nk_tree_push_hashed(ctx, NK_TREE_TAB, comp->name.data(), static_cast<nk_collapse_states>(comp->openUI), "hashin\0", 7, __LINE__))
				{
					nk_checkbox_label(ctx, "Enabled", &comp->enabled);

					comp->UI();

					nk_tree_pop(ctx);
				}
			}

			if (transform.children.size() > 0) {
				nk_label(ctx, "Children", nk_text_align::NK_TEXT_ALIGN_LEFT);
			}

			for each (Transform* child in transform.children)
			{
				child->gameobject.UI();
			}

			nk_tree_pop(ctx);
		}
	}

	template <typename TT, typename ...Args>
	TT& AddComponent(Args&&... params) {
		TT* comp = new TT(*this, transform, std::forward<Args>(params)...);
		components.push_back(comp);
		return *comp;
	}
	template <typename TT>
	TT& GetComponent() {
		for each (object comp in component)
		{
			if (TT* t = dynamic_cast<TT*>(comp)) {
				return t;
			}
		}
		return nullptr;
	}
};


#pragma region Components


#pragma region Light


#pragma endregion  Components








