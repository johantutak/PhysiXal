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

        static std::string GetTexture() { return s_TexturePath; }
        static std::string GetSelectedTexture() { return s_SelectedTextureFile; }

        static std::string GetMesh() { return s_MeshPath; }
        static std::string GetSelectedMesh() { return m_SelectedMeshFile; }
    private:
        inline static bool s_FileSelected;

        inline static std::string s_SelectedTextureFile;
        inline static std::string m_SelectedMeshFile;
        inline static std::string m_SelectedVertexShaderFile;
        inline static std::string m_SelectedFragmentShaderFile;
        
        inline static std::string s_TexturePath = "../editor/assets/textures/viking_room.png";
        inline static std::string s_MeshPath = "../editor/assets/models/viking_room.obj";
    };
}