#include "ShaderProgram.h"


namespace PF
{


	struct ShaderParameterValue
	{
		virtual void Bind(const std::string name, const Shares<ShaderProgram>& shader) = 0;
	};

    struct ShaderParameter
	{
		//ParameterType value;
		std::string name;
		Shares<ShaderParameterValue> value;

		ShaderParameter()
		{
		}

		template <typename TT, typename ...Args>
		static ShaderParameter* Create(const std::string& _name, Args&&... params);

		virtual void Bind(const Shares<ShaderProgram>& s);

	};






//#define ShaderParamBody(Type) 




	struct ShaderInt : Int, ShaderParameterValue
	{
		ShaderInt(Int& val): Int(val){}

		virtual void Bind(const std::string name, const Shares<ShaderProgram>& shader) override
		{
			shader->SetUniform(name, *this);
		}
	};

	struct ShaderVec3 : Vec3, ShaderParameterValue
	{
		ShaderVec3(Vec3& val) : Vec3(val) {}

		virtual void Bind(const std::string name, const Shares<ShaderProgram>& shader) override
		{
			shader->SetUniform(name, *this);
		}
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