//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS FrameBufferObject - IMPLEMENTATION
//
//=============================================================================
#include "fbo.h"
#include "gl.h"
#include <cassert>
#include <iostream>
///////////////////////////////////////////////////////////////////////////
FrameBufferObject::FrameBufferObject() : frameBufferId_(0), renderBufferId_(0)
{

}
///////////////////////////////////////////////////////////////////////////
FrameBufferObject::~FrameBufferObject()
{
	clear();
}
///////////////////////////////////////////////////////////////////////////
void FrameBufferObject::create(unsigned int _width,
							   unsigned int _height,
							   bool _withRenderBuffer)
{
	clear();
	glGenFramebuffersEXT(1, &frameBufferId_);
	assert(frameBufferId_ != 0);
	if(_withRenderBuffer)
	{
		glGenRenderbuffersEXT(1, &renderBufferId_);
		assert(renderBufferId_ != 0);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferId_);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32, _width, _height);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferId_);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBufferId_);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}
///////////////////////////////////////////////////////////////////////////
void FrameBufferObject::attachTexture(unsigned int _position, unsigned int _textureID)
{
	assert(frameBufferId_ != 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferId_);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _position, GL_TEXTURE_2D, _textureID, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
///////////////////////////////////////////////////////////////////////////
void FrameBufferObject::bind(unsigned int _position) const
{
	assert(frameBufferId_ != 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferId_);
	glDrawBuffer(_position);
	glReadBuffer(_position);

}
///////////////////////////////////////////////////////////////////////////
void FrameBufferObject::unbind() const
{
	assert(frameBufferId_ != 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
///////////////////////////////////////////////////////////////////////////
void FrameBufferObject::clear()
{
	if(frameBufferId_ != 0)
	{
		glDeleteFramebuffersEXT(1, &frameBufferId_);
		frameBufferId_ = 0;
	}
	if(renderBufferId_ != 0)
	{
		glDeleteRenderbuffersEXT(1, &renderBufferId_);
		renderBufferId_ = 0;
	}
}
///////////////////////////////////////////////////////////////////////////
