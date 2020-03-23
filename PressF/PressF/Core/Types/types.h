#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/extend.hpp>
#include <functional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define STL_BYTE_SIZE(cont, type) (cont.size() * sizeof(type))

#include "Base/Base.h"
#include "Extensions/Range.h"
#include "Extensions/Clock.h"

namespace PF
{
	struct ImGuiDraw
	{
		virtual void ImGuiRender() = 0;
	};
	struct Serializable
	{
		virtual json Serialize() = 0;
	};
}

namespace Serialization
{
	json Serialize(Vec3 vec);
}

template<typename ObjType>
using Owns = std::unique_ptr<ObjType>;

template<typename ObjType>
using Shares = std::shared_ptr<ObjType>;

template<typename ObjType>
using Ref = ObjType*;
