#ifndef MESH_H
#define MESH_H
#include "PressF/pch.h"
#include "MaterialSystem/ShaderProgram.h"
#include "Texture.h"
#include "MaterialSystem/Material.h"


namespace PF
{
    class MeshRenderer;

#define PF_NORMAL 0
#define PF_BLOOM 1
#define	PF_TRANSPARENT 2

    enum class MeshRenderMode
    {
        Normal,
        Shiny,
        Transparent,
        None
    };

    const int PF_RENDER_MASKS_SIZE = 3;
    struct RenderMask : public std::bitset<PF_RENDER_MASKS_SIZE>
    {
        void SetMode(MeshRenderMode renderMode);
    };
    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
    };




    class GPUMesh 
    {
    public:
        /*  Mesh Data  */
        unsigned int nIndices{0};
        Ref<Material> mat{ nullptr };
        std::string name;
        /*  Functions  */
        // constructor


        void Bind();

        void Render();

        void ImGui()
        {

        }
        VertexArrayObject VAO;
        VertexBufferObject VBO;
        VertexBufferObject EBO;
        //unsigned int VAO, EBO, VBO;
        /*  Functions    */
        // initializes all the buffer objects/arrays
        void SetupMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<PF::Texture>& textures);



    };

    struct Model
    {
        std::vector<Owns<GPUMesh>> meshes;

        void ImGui()
        {

        }
    };
}

#endif