#include "Engine/Renderer/Shader.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Core/Log.hpp"

#include <glad/glad.h>

namespace Engine
{
    Shader::Shader(std::string_view vertexSource, std::string_view fragmentSource)
    {
        const unsigned int vertexShader = compile(GL_VERTEX_SHADER, vertexSource);
        const unsigned int fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSource);

        m_rendererId = glCreateProgram();

        glAttachShader(m_rendererId, vertexShader);
        glAttachShader(m_rendererId, fragmentShader);
        glLinkProgram(m_rendererId);

        int success = 0;
        glGetProgramiv(m_rendererId, GL_LINK_STATUS, &success);

        if (!success)
        {
            char infoLog[1024];
            glGetProgramInfoLog(m_rendererId, 1024, nullptr, infoLog);
            ENGINE_ERROR(infoLog);
            ENGINE_ASSERT(false, "Shader program link failed");
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_rendererId);
    }

    unsigned int Shader::compile(unsigned int type, std::string_view source)
    {
        unsigned int shader = glCreateShader(type);
        const char* src = source.data();

        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            ENGINE_ERROR(infoLog);
            ENGINE_ASSERT(false, "Shader compile failed");
        }

        return shader;
    }

    void Shader::bind() const
    {
        glUseProgram(m_rendererId);
    }

    void Shader::unbind() const
    {
        glUseProgram(0);
    }

    void Shader::setInt(const std::string& name, int value)
    {
        bind();
        glUniform1i(glGetUniformLocation(m_rendererId, name.c_str()), value);
    }

    void Shader::setFloat4(const std::string& name, float x, float y, float z, float w)
    {
        bind();
        glUniform4f(glGetUniformLocation(m_rendererId, name.c_str()), x, y, z, w);
    }

    void Shader::setMat4(const std::string& name, const float* matrix)
    {
        bind();
        glUniformMatrix4fv(glGetUniformLocation(m_rendererId, name.c_str()), 1, GL_FALSE, matrix);
    }
    
    unsigned int Shader::rendererId() const
    {
        return m_rendererId;
    }    
}