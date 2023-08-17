#pragma once

namespace PhysiXal {

    class AssetManager
    {
    public:
        // Asset Manager
        void Manager();

        static const std::string& GetTexturePath();
        static void SetTexturePath(const std::string& newPath);

        static const std::string& GetMeshPath();
        static void SetMeshPath(const std::string& newPath);

        static const std::string& GetVertexShaderPath();
        static void SetVertexShaderPath(const std::string& newPath);
        static const std::string& GetFragmentShaderPath();
        static void SetFragmentShaderPath(const std::string& newPath);

        static std::string GetTexture() { return s_TexturePath; }
        static std::string GetSelectedTexture() { return s_SelectedTextureFile; }

        static std::string GetMesh() { return s_MeshPath; }
        static std::string GetSelectedMesh() { return m_SelectedMeshFile; }

        static std::string GetVertexShader() { return s_VertexShaderPath; }
        static std::string GetSelectedVertexShader() { return m_SelectedVertexShaderFile; }
        static std::string GetFragmentShader() { return s_FragmentShaderPath; }
        static std::string GetSelectedFragmentShader() { return m_SelectedFragmentShaderFile; }
    private:
        inline static bool s_FileSelected;

        inline static std::string s_SelectedTextureFile;

        inline static std::string m_SelectedMeshFile;

        inline static std::string m_SelectedVertexShaderFile;
        inline static std::string m_SelectedFragmentShaderFile;
        
        inline static std::string s_TexturePath = "../editor/assets/textures/viking_room.png";

        inline static std::string s_MeshPath = "../editor/assets/models/viking_room.obj";

        inline static std::string s_VertexShaderPath = "../editor/assets/shaders/base_vert.spv";
        inline static std::string s_FragmentShaderPath = "../editor/assets/shaders/base_frag.spv";
    };
}