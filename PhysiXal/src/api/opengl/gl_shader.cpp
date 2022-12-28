#include "px_pch.h"
#include "api/opengl/gl_shader.h"

namespace PhysiXal {

#ifdef PX_PLATFORM_LINUX

    static std::string ReadFileAsString(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize((size_t)in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            PX_CORE_ERROR("Could not open file '{0}'", filepath);
        }
        
        return result;
    }
    
    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    GLuint OpenGLShader::CompileShader(GLenum type, const std::string& source)
    {
        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);
            
            PX_CORE_ERROR("{0}", infoLog.data());
            PX_CORE_ASSERT(false, "Shader compilation failure!");
        }
        
        return shader;
    }

    OpenGLShader* OpenGLShader::FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        OpenGLShader* shader = new OpenGLShader();
        shader->LoadFromGLSLTextFiles(vertexShaderPath, fragmentShaderPath);
        return shader;
    }

    void OpenGLShader::LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        std::string vertexSource = ReadFileAsString(vertexShaderPath);
        std::string fragmentSource = ReadFileAsString(fragmentShaderPath);

        GLuint program = glCreateProgram();
        int glShaderIDIndex = 0;

        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        glAttachShader(program, vertexShader);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            PX_CORE_ERROR("{0}", infoLog.data());
            PX_CORE_ASSERT(false, "Shader link failure!");
        }
        
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        m_RendererID = program;
    }
#endif
}