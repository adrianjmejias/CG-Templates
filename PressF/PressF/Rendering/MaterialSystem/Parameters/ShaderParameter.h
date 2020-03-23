#pragma once
//#include "PressF/PressF.h"
#include "../ShaderProgram.h"


namespace PF
{

	class ShaderProgram;
	#define SP_IMGUI_RENDER_VARIABLE()\
	virtual void ImGui(const std::string& name)\
	{\
		ImGuiRender(*this, name);\
	}\



	#define SP_BIND_VARIABLE()\
	virtual void Bind(const std::string name, const Ref<ShaderProgram>& shader) override\
	{\
		shader->SetUniform(name, *this);\
	}\


	#define SP_DEFAULT_CONSTRUCTOR(TYPE)\
	Shader##TYPE(const TYPE & val) : TYPE(val) {}\


	#define SP_CLASS_VARIABLE(TYPE)\
	struct Shader##TYPE : TYPE, ShaderParameterValue\
	{\
		SP_DEFAULT_CONSTRUCTOR(TYPE)\
\
		SP_BIND_VARIABLE()\
\
		SP_IMGUI_RENDER_VARIABLE()\
	};\
	





	struct ShaderParameterValue
	{
		virtual void Bind(const std::string name, const Ref<ShaderProgram>& shader) = 0;
		virtual void ImGui(const std::string& name) = 0;
	};

	struct ShaderParameter
	{
		//ParameterType value;
		std::string name;
		Shares<ShaderParameterValue> value;

		ShaderParameter();

		template <typename TT, typename ...Args>
		static ShaderParameter* Create(const std::string& _name, Args&&... params);

		virtual void ImGui();
		virtual void Bind(const Ref<ShaderProgram>& s);
	};

	template<typename TT, typename ...Args>
	inline ShaderParameter* ShaderParameter::Create(const std::string& _name, Args&& ...params)
	{
		auto sp = new ShaderParameter();

		sp->name = _name;
		sp->value.reset(new TT(std::forward<Args>(params)...));

		return sp;
	}


}