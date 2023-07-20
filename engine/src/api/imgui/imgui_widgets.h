#pragma once

namespace PhysiXal {

	class ImGuiWidgets
	{
	public:
	    // Dear ImGUI widgets
        void PerformanceStats();
		void AssetManager(); // #### TEMPORARY ####
		void VertexData();
	private:
		// #### TEMPORARY ####
		inline static bool m_FileSelected;
		
		// #### TEMPORARY ####
		inline static std::string m_SelectedModelFile;
		inline static std::string m_SelectedVertexShaderFile;
		inline static std::string m_SelectedFragmentShaderFile;
	};
}