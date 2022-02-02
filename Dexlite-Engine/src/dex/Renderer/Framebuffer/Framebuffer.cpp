#include "Framebuffer.hpp"

#include "../../Util/Logging.hpp"
#include "../../Core/Engine.hpp"

namespace dex
{
    Framebuffer::Framebuffer(const glm::vec2& size, const std::vector<ColorAttachmentFormat>& formats)
        : m_Size(size), m_RenderBuffer(size)
    {
        for (auto format : formats)
        {
            switch (format)
            {
                case ColorAttachmentFormat::RED_INTEGER:
                    m_ColorAttachments.emplace_back(ColorAttachmentTexture(size, GL_R32I, GL_RED_INTEGER));
                case ColorAttachmentFormat::RGB:
                    m_ColorAttachments.emplace_back(ColorAttachmentTexture(size, GL_RGB8, GL_RGB));
                case ColorAttachmentFormat::RGBA:
                    m_ColorAttachments.emplace_back(ColorAttachmentTexture(size, GL_RGBA8, GL_RGBA));
            }
        }

        create();
    }

    Framebuffer::~Framebuffer()
    {
        if (m_ID) glDeleteFramebuffers(1, &m_ID);
    }

    void Framebuffer::resize(const glm::vec2& new_size)
    {
        if (m_Size != new_size)
        {
            m_Size = new_size;

            for (auto& attachment : m_ColorAttachments)
            {
                attachment.resize(new_size);
            }

            m_RenderBuffer.resize(new_size);

            if (m_ID) glDeleteFramebuffers(1, &m_ID);
            create();
        }
    }

    void Framebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        //dex::Engine::Renderer.clear(); // Clear after binding the framebuffer.
    }

    void Framebuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::clearAttachment(uint32 index, int value)
    {
        m_ColorAttachments.at(index).clear(value);
    }

    int Framebuffer::readPixel(const glm::ivec2& location)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + 1);

        int pixel_data;
        glReadPixels(location.x, location.y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);

        return pixel_data;
    }

    void Framebuffer::create()
    {
        glGenFramebuffers(1, &m_ID);
        bind();

        for (size_t i = 0; i < m_ColorAttachments.size(); i++)
        {
            m_ColorAttachments.at(i).bind();

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorAttachments.at(i).getID(), 0);
        }

        m_RenderBuffer.bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer.getID());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            DEX_LOG_ERROR("<dex::Framebuffer::create()>: Framebuffer not created successfully.");

        unbind();
        m_RenderBuffer.unbind();

        for (auto& attachment : m_ColorAttachments)
        {
            attachment.unbind();
        }
    }
}
