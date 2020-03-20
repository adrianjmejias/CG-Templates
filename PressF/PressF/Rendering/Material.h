#pragma once
#include "PressF/pch.h"
#include "../ObjLoader/OBJ_Loader.h"
//#include "../AssetsManagement/Asset.h"
#include "Texture.h"
#include "ShaderParameters.h"

namespace PF
{

	struct Material : Asset
	{
	public:
		std::vector<Shares<ShaderParameter>> parameters;
		Shares<ShaderProgram> shader;
		 
		//Shares<ShaderParameter> operator[](const std::string& idx)
		//{
		//	return parameters[idx];
		//}


		Material();

		//template <typename TT, typename ...Args>
		//ShaderParameter(const std::string& _name, Args&&... params) : name(_name), value(TT(std::forward(params)))
		//{
		//}

		template <typename TT, typename ...Args >
		void AddParameter(const std::string& name, Args&&... params)
		{
			parameters.emplace_back(ShaderParameter::Create<TT>(name, std::forward<Args>(params)...));
		}

		virtual void Bind()
		{
			//ShaderParameter* p = new ShaderInt();

			shader->Bind();
			for (auto& sp : parameters)
			{
				sp->Bind(shader);
			}
		}
		//Material(const objl::Material& m);

		//Ref<Texture> operator[](int idx)
		//{

		//	if (idx < 0 || idx >= 6) {
		//		__debugbreak();
		//	}
		//		return *static_cast<Ref<Texture>>(std::make_sharedthis + offsetof(Material ,map_Ka) + idx * sizeof(Ref<Texture>));
		//}

		Material& operator = (objl::Material &m)
		{
			//AddParameter<ShaderVec3>("kA", m.Ka);
			//AddParameter<ShaderVec3>("kd", m.Kd);
			//AddParameter<ShaderVec3>("kS", m.Ks);
			//AddParameter<ShaderInt>("mode", 0);

			name = m.name;
			//AddParameter<ShaderVec3>("nS", m.Ns);
			//AddParameter<ShaderVec3>("nI", m.Ni);
			//AddParameter<ShaderVec3>("d", m.d);
			//d = m.d;
			//illum = m.illum;
			//map_Ka = m.map_Ka;
			//map_Kd = m.map_Kd;
			//map_Ks = m.map_Ks;
			//map_Ns = m.map_Ns;
			//map_d = m.map_d;
			//map_bump = m.map_bump;
			//AddParameter()


			//name = std::move(m.name);
			return *this;
		}
	};
}
