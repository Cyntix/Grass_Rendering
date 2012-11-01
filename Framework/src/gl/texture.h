//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS Texture
//
//=============================================================================
#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include <string>
#include "gl.h"
class Texture 
{
public:
	Texture();
	Texture(unsigned int _width, 
			unsigned int _height, 
			unsigned int _internalFormat, 
			unsigned int _format,
			unsigned int _type);
	~Texture();
	void create(unsigned int _width, 
				unsigned int _height, 
				unsigned int _internalFormat, 
				unsigned int _format,
				unsigned int _type,
				void * _data = NULL,
				int _param = GL_LINEAR);
	// Read RGB TGA file
	void create(const std::string& _fileName);
	// Write the texture in a RGB TGA file
	void write(const std::string& _fileName) const;

	void bind() const;
	void unbind() const;
	
	//Multi Texturing: channel of this texture
	void setLayer(unsigned int _layer);
	unsigned int getLayer() const;
	
	//Id of this texture
	unsigned int getID() const;
private:
	void clear();
	unsigned int id_;
	unsigned int layer_;
	unsigned int width_;
	unsigned int height_;
};
#endif