#pragma once

namespace PhysiXal {

	class ImGuiWidgets
	{
	public:
	    // Dear ImGUI widgets
        void PerformanceStats();
		void AssetManager();
		void VertexData();
	private:
		inline static bool m_FileSelected;
		
		inline static std::string m_SelectedModelFile;
		inline static std::string m_SelectedVertexShaderFile;
		inline static std::string m_SelectedFragmentShaderFile;
		inline static std::string m_SelectedTextureFile;
	};
}