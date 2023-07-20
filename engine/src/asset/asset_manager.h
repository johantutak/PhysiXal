#pragma once

namespace PhysiXal {

    class AssetManager
    {
    public:
        // Asset Manager
        void Manager();

        static const std::string& GetTexturePath();
        static void SetTexturePath(const std::string& newPath);

        static std::string GetTexture() { return s_TexturePath; }
        static std::string GetSelectedTexture() { return s_SelectedTextureFile; }
    private:
        inline static bool s_FileSelected;

        inline static std::string s_SelectedTextureFile;
        
        inline static std::string s_TexturePath = "../editor/assets/textures/viking_room.png";
    };
}