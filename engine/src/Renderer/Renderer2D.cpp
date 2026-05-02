#include "Engine/Renderer/Renderer2D.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/VertexArray.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"

#include <glad/glad.h>

#include <array>
#include <memory>

namespace Engine
{
    struct QuadVertex
    {
        float position[3];
        float color[4];
        float texCoord[2];
        float texIndex;
    };

    struct Renderer2DData
    {
        static constexpr unsigned int MaxQuads = 10000;
        static constexpr unsigned int MaxVertices = MaxQuads * 4;
        static constexpr unsigned int MaxIndices = MaxQuads * 6;
        static constexpr unsigned int MaxTextureSlots = 16;

        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ibo = 0;
        // std::unique_ptr<VertexArray> quadVertexArray;
        // std::shared_ptr<VertexBuffer> quadVertexBuffer;
        // std::shared_ptr<IndexBuffer> quadIndexBuffer;

        unsigned int quadIndexCount = 0;

        QuadVertex* quadVertexBufferBase = nullptr;
        QuadVertex* quadVertexBufferPtr = nullptr;

        std::unique_ptr<Shader> shader;
        std::unique_ptr<Texture2D> whiteTexture;

        std::array<const Texture2D*, MaxTextureSlots> textureSlots{};
        unsigned int textureSlotIndex = 1;

        std::array<float, 12> quadVertexPositions = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };
    };

    static Renderer2DData s_data;

    static const char* vertexShaderSource = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        layout(location = 2) in vec2 a_TexCoord;
        layout(location = 3) in float a_TexIndex;

        uniform mat4 u_ViewProjection;

        out vec4 v_Color;
        out vec2 v_TexCoord;
        out float v_TexIndex;

        void main()
        {
            v_Color = a_Color;
            v_TexCoord = a_TexCoord;
            v_TexIndex = a_TexIndex;
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
        }
    )";

    static const char* fragmentShaderSource = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec4 v_Color;
        in vec2 v_TexCoord;
        in float v_TexIndex;

        uniform sampler2D u_Textures[16];

        void main()
        {
            int index = int(v_TexIndex);
            vec4 texColor = vec4(1.0);

            switch (index)
            {
                case 0:  texColor = texture(u_Textures[0],  v_TexCoord); break;
                case 1:  texColor = texture(u_Textures[1],  v_TexCoord); break;
                case 2:  texColor = texture(u_Textures[2],  v_TexCoord); break;
                case 3:  texColor = texture(u_Textures[3],  v_TexCoord); break;
                case 4:  texColor = texture(u_Textures[4],  v_TexCoord); break;
                case 5:  texColor = texture(u_Textures[5],  v_TexCoord); break;
                case 6:  texColor = texture(u_Textures[6],  v_TexCoord); break;
                case 7:  texColor = texture(u_Textures[7],  v_TexCoord); break;
                case 8:  texColor = texture(u_Textures[8],  v_TexCoord); break;
                case 9:  texColor = texture(u_Textures[9],  v_TexCoord); break;
                case 10: texColor = texture(u_Textures[10], v_TexCoord); break;
                case 11: texColor = texture(u_Textures[11], v_TexCoord); break;
                case 12: texColor = texture(u_Textures[12], v_TexCoord); break;
                case 13: texColor = texture(u_Textures[13], v_TexCoord); break;
                case 14: texColor = texture(u_Textures[14], v_TexCoord); break;
                case 15: texColor = texture(u_Textures[15], v_TexCoord); break;
            }

            color = texColor * v_Color;
        }
    )";

    void Renderer2D::init()
    {
        // s_data.quadVertexArray = std::make_unique<VertexArray>();
        // s_data.quadVertexBuffer = std::make_unique<VertexBuffer>(
        //     Renderer2DData::MaxVertices * sizeof(QuadVertex)
        // );

        // VertexAttribute attributes[] = {
        //     {0, ShaderDataType::Float3, false }, // position
        //     {1, ShaderDataType::Float4, false }, // color
        //     {2, ShaderDataType::Float2, false }, // texCoord
        //     {3, ShaderDataType::Float, false } // texIndex
        // };

        // s_data.quadVertexArray->addVertexBuffer(
        //     s_data.quadVertexBuffer,
        //     attributes,
        //     4,
        //     sizeof(QuadVertex)
        // );

        glGenVertexArrays(1, &s_data.vao);
        glBindVertexArray(s_data.vao);

        glGenBuffers(1, &s_data.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, s_data.vbo);
        glBufferData(GL_ARRAY_BUFFER, Renderer2DData::MaxVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<const void*>(offsetof(QuadVertex, position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<const void*>(offsetof(QuadVertex, color)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<const void*>(offsetof(QuadVertex, texCoord)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<const void*>(offsetof(QuadVertex, texIndex)));

        auto* indices = new unsigned int[Renderer2DData::MaxIndices];

        unsigned int offset = 0;
        for (unsigned int i = 0; i < Renderer2DData::MaxIndices; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        glGenBuffers(1, &s_data.ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data.ibo);        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Renderer2DData::MaxIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        delete[] indices;

        s_data.quadVertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices];

        s_data.whiteTexture = std::make_unique<Texture2D>(1, 1);

        unsigned int white = 0xffffffff;
        glBindTexture(GL_TEXTURE_2D, s_data.whiteTexture->rendererId());

        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,
            0,
            1,
            1,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            &white
        );
        s_data.shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);

        s_data.shader->bind();

        int samplers[16];
        for (int i = 0; i < 16; i++)
            samplers[i] = i;

        glUniform1iv(
            glGetUniformLocation(s_data.shader->rendererId(), "u_Textures"),
            16,
            samplers
        );        

        s_data.textureSlots[0] = s_data.whiteTexture.get();
    }

    void Renderer2D::shutdown()
    {
        s_data.shader.reset();
        s_data.whiteTexture.reset();

        delete[] s_data.quadVertexBufferBase;
        s_data.quadVertexBufferBase = nullptr;
        s_data.quadVertexBufferPtr = nullptr;

        glDeleteVertexArrays(1, &s_data.vao);
        glDeleteBuffers(1, &s_data.vbo);
        glDeleteBuffers(1, &s_data.ibo);

        s_data.vao = 0;
        s_data.vbo = 0;
        s_data.ibo = 0;
    }

    void Renderer2D::beginScene(const OrthographicCamera& camera)
    {
        s_data.shader->bind();
        s_data.shader->setMat4("u_ViewProjection", camera.viewProjectionMatrix().data());

        startBatch();
    }

    void Renderer2D::endScene()
    {
        const auto dataSize =
            reinterpret_cast<unsigned char*>(s_data.quadVertexBufferPtr) -
            reinterpret_cast<unsigned char*>(s_data.quadVertexBufferBase);

        glBindBuffer(GL_ARRAY_BUFFER, s_data.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_data.quadVertexBufferBase);

        flush();
    }

    void Renderer2D::startBatch()
    {
        s_data.quadIndexCount = 0;
        s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
        s_data.textureSlotIndex = 1;
    }

    void Renderer2D::nextBatch()
    {
        endScene();
        startBatch();
    }

    void Renderer2D::flush()
    {
        if (s_data.quadIndexCount == 0)
            return;

        for (unsigned int i = 0; i < s_data.textureSlotIndex; i++)
            s_data.textureSlots[i]->bind(i);

        glBindVertexArray(s_data.vao);
        glDrawElements(GL_TRIANGLES, s_data.quadIndexCount, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer2D::drawQuad(
        float x,
        float y,
        float width,
        float height,
        float r,
        float g,
        float b,
        float a
    )
    {
        drawTexturedQuad(x, y, width, height, *s_data.whiteTexture);

        QuadVertex* vertex = s_data.quadVertexBufferPtr - 4;

        for (int i = 0; i < 4; i++)
        {
            vertex[i].color[0] = r;
            vertex[i].color[1] = g;
            vertex[i].color[2] = b;
            vertex[i].color[3] = a;
        }
    }

    void Renderer2D::drawTexturedQuad(
        float x,
        float y,
        float width,
        float height,
        const Texture2D& texture
    )
    {
        if (s_data.quadIndexCount >= Renderer2DData::MaxIndices)
            nextBatch();

        float textureIndex = 0.0f;

        for (unsigned int i = 1; i < s_data.textureSlotIndex; i++)
        {
            if (s_data.textureSlots[i]->rendererId() == texture.rendererId())
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = static_cast<float>(s_data.textureSlotIndex);
            s_data.textureSlots[s_data.textureSlotIndex] = &texture;
            s_data.textureSlotIndex++;
        }

        constexpr float texCoords[8] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
        };

        for (int i = 0; i < 4; i++)
        {
            const float localX = s_data.quadVertexPositions[i * 3 + 0];
            const float localY = s_data.quadVertexPositions[i * 3 + 1];

            s_data.quadVertexBufferPtr->position[0] = x + localX * width;
            s_data.quadVertexBufferPtr->position[1] = y + localY * height;
            s_data.quadVertexBufferPtr->position[2] = 0.0f;

            s_data.quadVertexBufferPtr->color[0] = 1.0f;
            s_data.quadVertexBufferPtr->color[1] = 1.0f;
            s_data.quadVertexBufferPtr->color[2] = 1.0f;
            s_data.quadVertexBufferPtr->color[3] = 1.0f;

            s_data.quadVertexBufferPtr->texCoord[0] = texCoords[i * 2 + 0];
            s_data.quadVertexBufferPtr->texCoord[1] = texCoords[i * 2 + 1];

            s_data.quadVertexBufferPtr->texIndex = textureIndex;

            s_data.quadVertexBufferPtr++;
        }

        s_data.quadIndexCount += 6;
    }
}