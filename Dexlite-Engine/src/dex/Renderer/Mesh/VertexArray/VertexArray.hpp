#pragma once

#include <vector>
#include "Buffers/VertexBuffer.hpp"
#include "Buffers/IndexBuffer.hpp"

#include "../../../Util/Logging.hpp"

namespace dex
{
    class VertexArray
    {
    public:
        VertexArray()
        {
            glGenVertexArrays(1, &m_ID);
            bind();
        }
        ~VertexArray()
        {
            //DEX_LOG_ERROR("Deleted VAO: {}", m_ID);

            if (m_ID)
                glDeleteVertexArrays(1, &m_ID);
        }

        VertexArray(const VertexArray& other) = delete;
        const VertexArray& operator=(const VertexArray& other) = delete;

        VertexArray(VertexArray&& other) noexcept
            : m_ID(other.m_ID), m_VertexBuffers(std::move(other.m_VertexBuffers)), m_IndexBuffer(std::move(other.m_IndexBuffer))
        {
            m_ID = other.m_ID;

            other.m_ID = 0;
        }
        VertexArray& operator=(VertexArray&& other) noexcept
        {
            if (this != &other)
            {
                m_ID = other.m_ID;
                m_VertexBuffers = std::move(other.m_VertexBuffers);
                m_IndexBuffer = std::move(other.m_IndexBuffer);

                other.m_ID = 0;
            }

            return *this;
        }

        void render()
        {
            bind();
            glDrawElements(GL_TRIANGLES, m_IndexBuffer.getCount(), GL_UNSIGNED_INT, 0);
        }
        void renderAsLines()
        {
            bind();
            glDrawElements(GL_LINES, m_IndexBuffer.getCount(), GL_UNSIGNED_INT, 0);
        }

        void bind()
        {
            glBindVertexArray(m_ID);
        }
        void unbind()
        {
            glBindVertexArray(0);
        }
    public:
        VertexBuffer m_VertexBuffers;
        IndexBuffer m_IndexBuffer;
    private:
        uint32 m_ID = 0;
    };
}
