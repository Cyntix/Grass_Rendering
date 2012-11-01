//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS FrameBufferObject
//
//=============================================================================
#ifndef __FBO_H__
#define __FBO_H__
class FrameBufferObject
{
	public:
		FrameBufferObject();
		~FrameBufferObject();
		void create(unsigned int _width,
					unsigned int _height,
					bool _withRenderBuffer = true);
		//Attach a texture to the FBO
		void attachTexture(unsigned int _position, unsigned int _textureID);
		
		void bind(unsigned int _position) const;
		void unbind() const;
	
	private:
		void clear();
		unsigned int frameBufferId_;
		unsigned int renderBufferId_;
};
#endif