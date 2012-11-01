//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS Texture - IMPLEMENTATION
//
//=============================================================================
#include "texture.h"
#include <fstream>
#include <cassert>
///////////////////////////////////////////////////////////////////////////
Texture::Texture() : id_(0), layer_(0), width_(0), height_(0)
{
}
///////////////////////////////////////////////////////////////////////////
Texture::Texture(unsigned int _width, 
				 unsigned int _height, 
				 unsigned int _internalFormat, 
				 unsigned int _format,
				 unsigned int _type) : id_(0), layer_(0), width_(0), height_(0)
{
	create(_width, _height, _internalFormat, _format, _type, NULL);
}
///////////////////////////////////////////////////////////////////////////
Texture::~Texture()
{
	clear();
}
///////////////////////////////////////////////////////////////////////////
void Texture::create(unsigned int _width, 
					 unsigned int _height, 
					 unsigned int _internalFormat, 
					 unsigned int _format,
					 unsigned int _type,
					 void * _data,
					 int _param)
{
	assert(_width <= 4096 && _height <= 4096);
	clear();
	width_ = _width;
	height_ = _height;
	glGenTextures(1, &id_);
	assert(id_ != 0);
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, width_, height_, 0, _format, _type, _data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _param);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0); 
}
///////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
typedef struct
{
	unsigned char  identsize;          // size of ID field that follows 18 byte header (0 usually)
	unsigned char  colourmaptype;      // type of colour map 0=none, 1=has palette
	unsigned char  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

	short colourmapstart;			   // first colour map entry in palette
	short colourmaplength;			   // number of colours in palette
	unsigned char  colourmapbits;      // number of bits per palette entry 15,16,24,32

	short xstart;             // image x origin
	short ystart;             // image y origin
	short width;              // image width in pixels
	short height;             // image height in pixels
	unsigned char  bits;               // image bits per pixel 8,16,24,32
	unsigned char  descriptor;         // image descriptor bits (vh flip bits)

	// pixel data follows header
} TGA_HEADER;
#pragma pack(pop)
///////////////////////////////////////////////////////////////////////////
void Texture::create(const std::string& _fileName)
{
	std::ifstream stream(_fileName.c_str(), std::ios::binary);
	assert(stream.is_open());
	if(!stream.is_open()) return;
	TGA_HEADER header;
	stream.read((char *)(&header), sizeof(TGA_HEADER));
	assert(header.width <= 4096 && header.height <= 4096 && header.imagetype == 2 && header.bits == 24);
	clear();
	width_ = header.width;
	height_ = header.height;
	unsigned int sizeImg = width_*height_;
	char *data = new char[sizeImg*3];
	stream.read(data, sizeImg*3);
	for(unsigned int i = 0; i < sizeImg; i++)
	{
		unsigned pos = i*3;
		unsigned char red = data[pos];
		data[pos] = data[pos + 2];
		data[pos + 2] = red;
	}
	glGenTextures(1, &id_);
	assert(id_ != 0);
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0); 
	stream.close();
	delete [] data;
}
///////////////////////////////////////////////////////////////////////////
void Texture::write(const std::string& _fileName) const
{
	assert(id_ != 0);
	glBindTexture(GL_TEXTURE_2D, id_);
	unsigned int sizeImg = width_*height_;
	char *data = new char[sizeImg*3];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	std::ofstream stream(_fileName.c_str(), std::ios::binary);
	assert(stream.is_open());
	if(!stream.is_open()) return;
	TGA_HEADER header;
	header.identsize = 0;          // size of ID field that follows 18 byte header (0 usually)
	header.colourmaptype = 0;      // type of colour map 0=none, 1=has palette
	header.imagetype = 2;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	header.colourmapstart = 0;     // first colour map entry in palette
	header.colourmaplength = 0;    // number of colours in palette
	header.colourmapbits=0;        // number of bits per palette entry 15,16,24,32
	header.xstart = 0;             // image x origin
	header.ystart = 0;             // image y origin
	header.width = (short)width_;         // image width in pixels
	header.height = (short)height_;       // image height in pixels
	header.bits = 24;              // image bits per pixel 8,16,24,32
	header.descriptor = 0;         // image descriptor bits (vh flip bits)
	stream.write((char*)(&header), sizeof(TGA_HEADER));
	for(unsigned int i = 0; i < sizeImg; i++)
	{
		unsigned pos = i*3;
		unsigned char red = data[pos];
		data[pos] = data[pos + 2];
		data[pos + 2] = red;
	}
	stream.write(data, sizeImg*3);
	stream.close();
	delete [] data;
	glBindTexture(GL_TEXTURE_2D, 0); 
}
///////////////////////////////////////////////////////////////////////////
void Texture::bind() const
{
	assert(id_ != 0);
	glEnable(GL_TEXTURE_2D); 
	glActiveTextureARB(GL_TEXTURE0_ARB+layer_); 
	glBindTexture(GL_TEXTURE_2D, id_);
}
///////////////////////////////////////////////////////////////////////////
void Texture::unbind() const
{
	assert(id_ != 0);
	glActiveTextureARB(GL_TEXTURE0_ARB+layer_); 
	glBindTexture(GL_TEXTURE_2D, 0); 
	glDisable(GL_TEXTURE_2D);
}
///////////////////////////////////////////////////////////////////////////
void Texture::setLayer(unsigned int _layer)
{
	layer_ = _layer;
}
///////////////////////////////////////////////////////////////////////////
unsigned int Texture::getLayer() const
{
	return layer_;
}
///////////////////////////////////////////////////////////////////////////
unsigned int Texture::getID() const
{
	return id_;
}
///////////////////////////////////////////////////////////////////////////
void Texture::clear()
{
	if(id_ != 0)
	{
		glDeleteTextures(1, &id_);
		id_ = 0;
		layer_ = 0;
		width_ = 0;
		height_ = 0;
	}
}
///////////////////////////////////////////////////////////////////////////