#pragma once

#include <string>
#include <string_view>

namespace Engine
{
    class Shader
    {
    public:
        Shader(std::string_view vertexSource, std::string_view fragmentSource);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void bind() const;
        void unbind() const;

        void setInt(const std::string& name, int value);
        void setFloat4(const std::string& name, float x, float y, float z, float w);
        void setMat4(const std::string& name, const float* matrix);
        unsigned int rendererId() const;

    private:
        unsigned int m_rendererId = 0;

        unsigned int compile(unsigned int type, std::string_view source);
    };
}