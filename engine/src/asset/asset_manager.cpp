#include "px_pch.h"
#include "asset/asset_manager.h"

#include "platform/win/win_utilities.h"
#include "utilities/string_utilities.h"

#include <imgui.h>
#include "api/imgui/imgui_initializers.h"

#include "api/vulkan/vk_initializers.h"
#include "api/vulkan/vk_utilities.h"

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Vulkan renderer
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void AssetManager::Manager()
    {
        ImGui::Begin("Asset Manager");
        // Texture selection
        if (ImGui::Button("Texture"))
        {
            if (FileManager::SelectFile(s_SelectedTextureFile, TEXT("Image Files\0*.png;*.jpg;*.bmp\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetTexturePath(s_SelectedTextureFile);
                s_FileSelected = true;
            }
        }
        ImGui::Text("%s", AssetManager::GetTexturePath().c_str());

        ImGui::End();

        // #### TODO #### Get it to increment instead and do it when i add UUID.
        if (!s_SelectedTextureFile.empty())
        {
            PX_CORE_INFO("New texture selected");

            m_Texture->CreateTextureImage(VulkanTexture::GetVulkanTexture().textureImage, VulkanTexture::GetVulkanTexture().textureImageMemory, s_SelectedTextureFile);
            m_Texture->CreateTextureImageView(VulkanTexture::GetVulkanTexture().textureImage, VulkanTexture::GetVulkanTexture().textureImageView);
            m_Texture->CreateTextureSampler(VulkanTexture::GetVulkanTexture().textureSampler);

            m_UniformBuffer->DestroyTextureDescriptorSet();
            m_UniformBuffer->CreateTextureDescriptorSet();

            s_SelectedTextureFile.clear();
        }
    }

    const std::string& AssetManager::GetTexturePath()
    {
        return s_TexturePath;
    }

    void AssetManager::SetTexturePath(const std::string& newPath)
    {
        s_TexturePath = newPath;
    }
}