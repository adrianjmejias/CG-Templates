#pragma once
#include "PressF/pch.h"
#include "PressF/Core/OpenGL.h"
#include "PressF/AssetsManagement/Asset.h"
namespace PF
{

	enum class IllumModel {
		CUBEMAP = 11,
		NORMAL,
		OCCLUSION_PARALLAX,
		REFLECTION,
		REFRACTION,
		TRANSPARENCY,
		SHADOW_MAPPING,
		COOK,
		BLINN_PHONG,
		DISPLACEMENT,
		NO_SHADER,
	};

	enum class MapType {
		AMBIENT,
		DIFFUSE,
		SPECULAR,
		BUMP,
		CUBEMAP,
		SHINY,
		DISPLACEMENT,
		DECAL,
		REFLECTION,
		DISSOLVE,
	};

    class ShaderProgram : public GPUObject, Asset
    {
    public:
        std::string vPath;
        std::string fPath;

        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        // activate the shader
        // ------------------------------------------------------------------------
        ~ShaderProgram();
        

        static Owns<ShaderProgram> FromFiles(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
        void ReCompile();
        virtual void ImGui();
        void Bind() override;
        // utility uniform functions
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, bool value) const;
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, int value) const;
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, float value) const;
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, const glm::vec2& value) const;
        void SetUniform(const std::string& name, float x, float y) const;
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, const glm::vec3& value) const;
        void SetUniform(const std::string& name, float x, float y, float z) const;
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, const glm::vec4& value) const;
        void SetUniform(const std::string& name, float x, float y, float z, float w);
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, const glm::mat2& mat) const;
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, const glm::mat3& mat) const;
        // ------------------------------------------------------------------------
        void SetUniform(const std::string& name, const glm::mat4& mat) const;

    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void CheckCompileErrors(GLuint shader, std::string type);
    };

}
