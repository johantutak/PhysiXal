#include "px_pch.h"
#include "asset/asset_manager.h"

#include "platform/win/win_utilities.h"
#include "utilities/string_utilities.h"

#include <imgui.h>
#include "api/imgui/imgui_initializers.h"

#include "api/vulkan/vk_initializers.h"
#include "api/vulkan/vk_utilities.h"

#include "ecs/ecs.h"
#include "scene/model_component.h"
#include "scene/transform_component.h"

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Asset Manager
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void AssetManager::Manager()
    {
        // #### TO DO #### Put texture, mesh and shaders(vertex and fragment) into own classes.
        ImGui::Begin("Asset Manager");

        // ###################################
        // #### Texture ######################
        // ###################################

        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Texture:");

        ImGui::SameLine(0, 10);

        ImGui::Text("%s", AssetManager::GetTexturePath().c_str());

        // Texture selection
        ImGui::PushID(1);
        if (ImGui::Button("Select"))
        {
            if (FileManager::SelectFile(s_SelectedTextureFile, TEXT("Texture Files\0*.png;*.jpg;*.bmp\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetTexturePath(s_SelectedTextureFile);
                s_FileSelected = true;

                PX_CORE_INFO("New texture selected");
            }
        }
        ImGui::PopID();

        ImGui::SameLine(0, 10);

        // Apply texture
        ImGui::PushID(2);
        if (ImGui::Button("Apply"))
        {
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
            else
            {
                if (s_SelectedTextureFile.empty())
                {
                    PX_CORE_ERROR("No texture selected");
                }

                PX_CORE_WARN("Texture selection needed to update texture specific image, image view and sampler");
            }
        }
        ImGui::PopID();

        ImGui::Separator();

        // ###################################
        // #### Mesh #########################
        // ###################################

        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Mesh:");

        ImGui::SameLine(0, 10);

        ImGui::Text("%s", AssetManager::GetMeshPath().c_str());

        // Mesh selection
        ImGui::PushID(3);
        if (ImGui::Button("Select"))
        {
            if (FileManager::SelectFile(m_SelectedMeshFile, TEXT("Mesh Files\0*.obj\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetMeshPath(m_SelectedMeshFile);
                s_FileSelected = true;

                PX_CORE_INFO("New mesh selected");
            }
        }
        ImGui::PopID();

        ImGui::SameLine(0, 10);

        // Apply mesh
        ImGui::PushID(4);
        if (ImGui::Button("Apply (Mesh)"))
        {
            if (!m_SelectedMeshFile.empty())
            {
                extern PhysiXal::ECSManager* g_ECSManager; // global ECSManager pointer
                PhysiXal::Entity* entity = g_ECSManager->GetSelectedEntity();
                if (!entity) {
                    // No entity is currently selected, so create one.
                    entity = g_ECSManager->CreateEntity();
                    entity->AddComponent<PhysiXal::TransformComponent>();
                    entity->AddComponent<PhysiXal::ModelComponent>();
                    g_ECSManager->SetSelectedEntity(entity);
                }
                // Update the ModelComponent in the selected entity
                PhysiXal::ModelComponent* model = entity->GetComponent<PhysiXal::ModelComponent>();
                if (model)
                {
                    model->MeshPath = m_SelectedMeshFile;
                    model->VertexShaderPath = AssetManager::GetVertexShaderPath();
                    model->FragmentShaderPath = AssetManager::GetFragmentShaderPath();
                    // (Optional) Trigger your renderer to reload the mesh from model->MeshPath here.
                }
                // Clear the selected mesh file so it doesn't reapply
                m_SelectedMeshFile.clear();
            }
            else
            {
                PX_CORE_ERROR("No mesh selected");
                PX_CORE_WARN("Mesh selection needed to update model component");
            }
        }
        ImGui::PopID();

        ImGui::Separator();

        // ###################################
        // #### Shader #######################
        // ###################################

        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Vertex shader:");

        ImGui::SameLine(0, 10);

        ImGui::Text("%s", AssetManager::GetVertexShaderPath().c_str());

        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Fragment shader:");

        ImGui::SameLine(0, 10);

        ImGui::Text("%s", AssetManager::GetFragmentShaderPath().c_str());

        // Vertex shader selection
        if (ImGui::Button("Select (vertex)"))
        {
            if (FileManager::SelectFile(m_SelectedVertexShaderFile, TEXT("Vertex Shader Files\0*.spv\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetVertexShaderPath(m_SelectedVertexShaderFile);
                s_FileSelected = true;

                PX_CORE_INFO("New Shader (vertex) selected");
            }
        }

        ImGui::SameLine(0, 10);

        // Fragment shader selection
        if (ImGui::Button("Select (fragment)"))
        {
            if (FileManager::SelectFile(m_SelectedFragmentShaderFile, TEXT("Fragment Shader Files\0*.spv\0All Files\0*.*\0")))
            {
                // File selected successfully
                SetFragmentShaderPath(m_SelectedFragmentShaderFile);
                s_FileSelected = true;

                PX_CORE_INFO("New shader (fragment) selected");
            }
        }

        ImGui::SameLine(0, 10);

        // Apply both vertex and fragment shader
        ImGui::PushID(5);
        if (ImGui::Button("Apply (Shaders)"))
        {
            if (!m_SelectedVertexShaderFile.empty() && !m_SelectedFragmentShaderFile.empty())
            {
                // Get the selected entity and update its ModelComponent (if any)
                extern PhysiXal::ECSManager* g_ECSManager;
                PhysiXal::Entity* entity = g_ECSManager->GetSelectedEntity();
                if (entity && entity->HasComponent<PhysiXal::ModelComponent>())
                {
                    PhysiXal::ModelComponent* model = entity->GetComponent<PhysiXal::ModelComponent>();
                    model->VertexShaderPath = m_SelectedVertexShaderFile;
                    model->FragmentShaderPath = m_SelectedFragmentShaderFile;
                }
                m_SelectedVertexShaderFile.clear();
                m_SelectedFragmentShaderFile.clear();
            }
            else
            {
                if (m_SelectedVertexShaderFile.empty())
                    PX_CORE_ERROR("No vertex shader selected");
                if (m_SelectedFragmentShaderFile.empty())
                    PX_CORE_ERROR("No fragment shader selected");
                PX_CORE_WARN("Shader selection needed to update pipeline");
            }
        }
        ImGui::PopID();

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