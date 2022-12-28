#pragma once

#include <string>

//#### TO DO #### Fix the include to work as it should
#ifdef PX_PLATFORM_LINUX
    #include <../../../PhysiXal/thirdparty/glad/include/glad/glad.h>
#endif

namespace PhysiXal {
    
#ifdef PX_PLATFORM_LINUX

    class OpenGLShader
    {
    public:
        ~OpenGLShader();

        GLuint GetRendererID() { return m_RendererID; }
        
        static OpenGLShader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    private:
        OpenGLShader() = default;
        
        void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        GLuint CompileShader(GLenum type, const std::string& source);
    private:
        GLuint m_RendererID;
    };
#endif
}

