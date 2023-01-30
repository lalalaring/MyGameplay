#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "base/Base.h"
#include "RenderTarget.h"
#include "DepthStencilTarget.h"
#include "material/Image.h"

namespace gameplay
{

/**
 * Defines a frame buffer object that may contain one or more render targets and optionally
 * a depth-stencil target.
 *
 * Frame buffers can be created and used for off-screen rendering, which is useful for
 * techniques such as shadow mapping and post-processing. Render targets within a frame
 * buffer can be both written to and read (by calling RenderTarget::getTexture).
 *
 * When binding a custom frame buffer, you should always store the return value of
 * FrameBuffer::bind and restore it when you are finished drawing to your frame buffer.
 *
 * To bind the default frame buffer, call FrameBuffer::bindDefault.
 */
class FrameBuffer : public Ref
{
    friend class Game;

public:

    /**
     * Get the ID of this FrameBuffer.
     *
     * @return The ID of this FrameBuffer.
     */
    virtual const char* getId() const = NULL;

    /**
     * Gets the width of the frame buffer.
     *
     * @return The width of the frame buffer.
     */
    virtual unsigned int getWidth() const = NULL;

    /**
     * Gets the height of the frame buffer.
     *
     * @return The height of the frame buffer.
     */
    virtual unsigned int getHeight() const = NULL;

    /**
     * Get the number of color attachments available on the current hardware.
     *
     * @return The number of color attachments available on the current hardware.
     */
    virtual unsigned int getMaxRenderTargets() const = NULL;

    /**
     * Set a RenderTarget on this FrameBuffer's color attachment at the specified index.
     *
     * @param target The 2D RenderTarget to set.
     * @param index The index of the color attachment to set.
     */
    virtual void setRenderTarget(RenderTarget* target, unsigned int index = 0) = NULL;

    /**
    * Set a RenderTarget on this FrameBuffer's color attachment at the specified index.
    *
    * @param target The Cubemap RenderTarget to set.
    * @param face The face of the cubemap to target.
    * @param index The index of the color attachment to set.
    */
    virtual void setRenderTarget(RenderTarget* target, Texture::CubeFace face, unsigned int index = 0) = NULL;

    /**
     * Get the RenderTarget attached to the FrameBuffer's color attachment at the specified index.
     *
     * @param index The index of the color attachment to retrieve a RenderTarget from.
     *
     * @return The RenderTarget attached at the specified index.
     */
    virtual RenderTarget* getRenderTarget(unsigned int index = 0) const = NULL;

    /**
     * Returns the current number of render targets attached to this frame buffer.
     *
     * @return The number of render targets attached.
     */
    virtual unsigned int getRenderTargetCount() const = NULL;

    /**
     * Set this FrameBuffer's DepthStencilTarget.
     *
     */
    virtual void createDepthStencilTarget(int format = 0) = NULL;

    /**
     * Determines whether this is the default frame buffer.
     *
     * @return true if this is the default frame buffer, false otherwise.
     */
    virtual bool isDefault() const = NULL;

    /**
     * Binds this FrameBuffer for off-screen rendering and return you the currently bound one.
     *
     * You should keep the return FrameBuffer and store it and call bind() when you rendering is complete.
     *
     * @ return The prevoiouse framebuffer.
     */
    virtual FrameBuffer* bind(unsigned int type = 0x8D40/*GL_FRAMEBUFFER*/) = NULL;

    /**
     * Records a screenshot of what is stored on the current FrameBuffer.
     *
     * @param format The format the Image should be in.
     * @return A screenshot of the current framebuffer's content.
     */
    virtual Image* createScreenshot(Image::Format format = Image::RGBA) = NULL;

    /**
     * Records a screenshot of what is stored on the current FrameBuffer to an Image.
     *
     * The Image must be the same size as the FrameBuffer, otherwise the operation will fail.
     *
     * @param image The Image to write the current framebuffer's content to.
     */
    virtual void getScreenshot(Image* image) = NULL;
};

}

#endif
