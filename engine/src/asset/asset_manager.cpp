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
        // #### TO DO #### Put texture, mesh and shaders(vertex and fragment) into own classes.
        ImGui::Begin("Asset Manager");
        
        // Texture selection
        if (ImGui::Button("Texture"))
        {
            if (FileManager::SelectFile(s_SelectedTextureFile, TEXT("Texture Files\0*.png;*.jpg;*.bmp\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetTexturePath(s_SelectedTextureFile);
                s_FileSelected = true;

                PX_CORE_INFO("New texture selected");
            }
        }
        ImGui::Text("%s", AssetManager::GetTexturePath().c_str());

        // #### TODO #### Get it to increment instead and do it when i add UUID.
        if (!s_SelectedTextureFile.empty())
        {
            // Create texture image
            m_Texture->CreateTextureImage(VulkanTexture::GetVulkanTexture().textureImage, VulkanTexture::GetVulkanTexture().textureImageMemory, s_SelectedTextureFile);

            // Create texture image views
            m_Texture->CreateTextureImageView(VulkanTexture::GetVulkanTexture().textureImage, VulkanTexture::GetVulkanTexture().textureImageView);

            // Create texture sampler for LOD
            m_Texture->CreateTextureSampler(VulkanTexture::GetVulkanTexture().textureSampler);

            // Destruction of the texture descriptor set
            m_UniformBuffer->DestroyTextureDescriptorSet();

            // Create texture descriptor set
            m_UniformBuffer->CreateTextureDescriptorSet();

            s_SelectedTextureFile.clear();
        }

        ImGui::Separator();

        if (ImGui::Button("Mesh"))
        {
            if (FileManager::SelectFile(m_SelectedMeshFile, TEXT("Mesh Files\0*.obj\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetMeshPath(m_SelectedMeshFile);
                s_FileSelected = true;

                PX_CORE_INFO("New mesh selected");
            }
        }
        ImGui::Text("%s", AssetManager::GetMeshPath().c_str());

        if (!m_SelectedMeshFile.empty())
        {
            // Destruction of the index buffer
            m_Buffer->DestroyIndexBuffer(m_Mesh->GetVulkanMesh().indexBuffer, m_Mesh->GetVulkanMesh().indexBufferMemory);

            // Destruction of the vertex buffer
            m_Buffer->DestroyVertexBuffer(m_Mesh->GetVulkanMesh().vertexBuffer, m_Mesh->GetVulkanMesh().vertexBufferMemory);

            // Load mesh (.obj) to renderer
            m_Mesh->LoadMesh(m_SelectedMeshFile, m_Mesh->GetVulkanMeshNew().vertices, m_Mesh->GetVulkanMeshNew().indices);

            // Create vertex buffer
            m_Buffer->CreateVertexBuffer(m_Mesh->GetVulkanMeshNew().vertexBuffer, m_Mesh->GetVulkanMeshNew().vertexBufferMemory, m_Mesh->GetVulkanMeshNew().vertices);

            // Create index buffer
            m_Buffer->CreateIndexBuffer(m_Mesh->GetVulkanMeshNew().indexBuffer, m_Mesh->GetVulkanMeshNew().indexBufferMemory, m_Mesh->GetVulkanMeshNew().indices);

            m_SelectedMeshFile.clear();
        }

        ImGui::Separator();

        // Vertex shader selection
        if (ImGui::Button("Vertex Shader"))
        {
            if (FileManager::SelectFile(m_SelectedVertexShaderFile, TEXT("Vertex Shader Files\0*.spv\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetVertexShaderPath(m_SelectedVertexShaderFile);
                s_FileSelected = true;

                PX_CORE_INFO("New shader (vertex) selected");
            }
        }
        ImGui::Text("%s", AssetManager::GetVertexShaderPath().c_str());

        // Fragment shader selection
        if (ImGui::Button("Fragment Shader"))
        {
            if (FileManager::SelectFile(m_SelectedFragmentShaderFile, TEXT("Fragment Shader Files\0*.spv\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetFragmentShaderPath(m_SelectedFragmentShaderFile);
                s_FileSelected = true;

                PX_CORE_INFO("New shader (fragment) selected");
            }
        }
        ImGui::Text("%s", AssetManager::GetFragmentShaderPath().c_str());

        if (ImGui::Button("Apply"))
        {
            if (!m_SelectedVertexShaderFile.empty() && !m_SelectedFragmentShaderFile.empty())
            {
                // Destruction of the pipeline
                m_Pipeline->DestroyGraphicsPipeline();

                // Create pipeline
                m_Pipeline->CreateGraphicsPipeline(m_SelectedVertexShaderFile, m_SelectedFragmentShaderFile);

                m_SelectedVertexShaderFile.clear();
                m_SelectedFragmentShaderFile.clear();
            }
            else
            {
                if (m_SelectedVertexShaderFile.empty())
                {
                    PX_CORE_ERROR("No shader (vertex) selected");
                }

                if (m_SelectedFragmentShaderFile.empty())
                {
                    PX_CORE_ERROR("No shader (fragment) selected");
                }

                PX_CORE_WARN("vertex and fragment shader needs selection to update pipeline");
            }
        }

        ImGui::End();

        // #### TO DO #### Bellow is an example of how to add a shader text feature in the future.
        /*
        // Maybe to be used for shader editing
        ImGui::Begin("Shader Editor");
        static char shaderCode[4096]; // Buffer to hold shader code
        ImGui::InputTextMultiline("##shader_code", shaderCode, sizeof(shaderCode), ImVec2(-1.0f, -1.0f));
        if (ImGui::Button("Apply")) {
            // Handle shader compilation and reloading here
        }
        ImGui::End();
        */
    }

    const std::string& AssetManager::GetTexturePath()
    {
        return s_TexturePath;
    }

    void AssetManager::SetTexturePath(const std::string& newPath)
    {
        s_TexturePath = newPath;
    }

    const std::string& AssetManager::GetMeshPath()
    {
        return s_MeshPath;
    }

    void AssetManager::SetMeshPath(const std::string& newPath)
    {
        s_MeshPath = newPath;
    }

    const std::string& AssetManager::GetVertexShaderPath()
    {
        return s_VertexShaderPath;
    }

    void AssetManager::SetVertexShaderPath(const std::string& newPath)
    {
        s_VertexShaderPath = newPath;
    }

    const std::string& AssetManager::GetFragmentShaderPath()
    {
        return s_FragmentShaderPath;
    }

    void AssetManager::SetFragmentShaderPath(const std::string& newPath)
    {
        s_FragmentShaderPath = newPath;
    }
}