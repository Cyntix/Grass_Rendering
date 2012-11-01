//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS Shader - IMPLEMENTATION
//
//=============================================================================
#include "shader.h"
#include "gl.h"
#include <cassert>
#include <fstream>
#include <iostream>
///////////////////////////////////////////////////////////////////////////
Shader::Shader() : programObj_(0)
{
}
///////////////////////////////////////////////////////////////////////////
Shader::Shader(const std::string _vertexFileName, const std::string& _fragmentFileName) : programObj_(0)
{
	create(_vertexFileName, _fragmentFileName);
}
///////////////////////////////////////////////////////////////////////////
Shader::~Shader()
{
	if(programObj_)
		glDeleteShader(programObj_);
}
///////////////////////////////////////////////////////////////////////////
void Shader::create(const std::string _vertexFileName, const std::string& _fragmentFileName)
{
	clear();
	assert(!_vertexFileName.empty() && !_fragmentFileName.empty());
	// Vertex Shader Loading
	unsigned int vertexShader;
	std::ifstream fileVs;
	fileVs.open(_vertexFileName.c_str(), std::ios::binary);
	assert(fileVs.is_open());
	if(fileVs.is_open())
	{  
		fileVs.seekg(0, std::ios::end);    
		unsigned int fileSize = (unsigned int)(fileVs.tellg());
		assert(fileSize != 0);
		fileVs.seekg(std::ios::beg);       
		char *bufferVs = new char[fileSize+1];
		fileVs.read(bufferVs, fileSize);
		fileVs.close();
		bufferVs[fileSize] = '\0';
		///////////////////////////////////////////////////////////////////
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		assert(vertexShader != 0);
		glShaderSource(vertexShader, 1, (const char **)(&bufferVs), NULL);
		glCompileShader(vertexShader);
		delete [] bufferVs;
		//Get Compilation Information
		int compiled;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
		if(compiled == 0) 
		{
			int length;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
			char *str = new char[length];
			glGetShaderInfoLog(vertexShader, length, NULL, str);
			std::cerr << "Vertex shader error " << str << std::endl;
			delete [] str;
			assert(false);
			return;
		}
	}
	else
	{
		std::cerr << "Didn't succeed to open the vertex shader file!" << std::endl;
		assert(false);
		return;
	}

	// Fragment Shader Loading
	unsigned int fragmentShader;
	std::ifstream fileFs;
	fileFs.open(_fragmentFileName.c_str(), std::ios::binary);
	assert(fileFs.is_open());
	if(fileFs.is_open())
	{  
		fileFs.seekg(0, std::ios::end);    
		unsigned int fileSize = (unsigned int)(fileFs.tellg());
		assert(fileSize != 0);
		fileFs.seekg(std::ios::beg);       
		char *bufferFs = new char[fileSize+1];
		fileFs.read(bufferFs, fileSize);
		fileFs.close();
		bufferFs[fileSize] = '\0';
		///////////////////////////////////////////////////////////////////
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		assert(fragmentShader != 0);
		glShaderSource(fragmentShader, 1, (const char **)(&bufferFs), NULL);
		glCompileShader(fragmentShader);
		delete [] bufferFs;
		//Get Compilation Information
		int compiled;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
		if(compiled == 0) 
		{
			int length;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
			char *str = new char[length];
			glGetShaderInfoLog(fragmentShader, length, NULL, str);
			std::cerr << "Fragment shader Error " << str << std::endl;
			delete [] str;
			assert(false);
			return;
		}
	}
	else
	{
		std::cerr << "Didn't succeed to open the fragment shader file!" << std::endl;
		assert(false);
		return;
	}

	//Bind Vertex and Fragment
	programObj_ = glCreateProgram();
	assert(programObj_ != 0);
	glAttachShader(programObj_, vertexShader);
	glAttachShader(programObj_, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glLinkProgram(programObj_);
	//Get Linker Information
	int linked;
	glGetProgramiv(programObj_, GL_LINK_STATUS, &linked);
	if(linked == 0)
	{
		int length;
		glGetProgramiv(programObj_, GL_INFO_LOG_LENGTH, &length);
		char *str = new char[length];
		glGetProgramInfoLog(programObj_, length, NULL, str);
		std::cerr << "Linker error " << str << std::endl;
		delete [] str;
		clear();
		assert(false);
	}
}
///////////////////////////////////////////////////////////////////////////
void Shader::bind() const
{
	assert(programObj_ != 0);
	glUseProgram(programObj_);
}
///////////////////////////////////////////////////////////////////////////
void Shader::unbind() const 
{
	assert(programObj_ != 0);
	glUseProgram(NULL);
}
///////////////////////////////////////////////////////////////////////////
void Shader::setFloatUniform(const std::string& _name, float _f) const
{
	assert(programObj_ != 0);
	glUniform1fARB(getUniformLocation(_name.c_str()), _f);
}
///////////////////////////////////////////////////////////////////////////
void Shader::setIntUniform(const std::string& _name, int _f) const
{
	assert(programObj_ != 0);
	glUniform1iARB(getUniformLocation(_name.c_str()), _f);
}
///////////////////////////////////////////////////////////////////////////
void Shader::setVector2Uniform(const std::string& _name, float _v1, float _v2) const
{
	assert(programObj_ != 0);
	glUniform2fARB(getUniformLocation(_name.c_str()), _v1, _v2);
}
///////////////////////////////////////////////////////////////////////////
void Shader::setVector3Uniform(const std::string& _name, float _v1, float _v2, float _v3) const
{
	assert(programObj_ != 0);
	glUniform3fARB(getUniformLocation(_name.c_str()), _v1, _v2, _v3);
}
///////////////////////////////////////////////////////////////////////////
void Shader::setVector4Uniform(const std::string& _name, float _v1, float _v2, float _v3, float _v4) const
{
	assert(programObj_ != 0);
	glUniform4fARB(getUniformLocation(_name.c_str()), _v1, _v2, _v3, _v4);
}
///////////////////////////////////////////////////////////////////////////
void Shader::setMatrix3x3Uniform(const std::string& _name, const float *_matrix) const
{
	assert(programObj_ != 0);
	glUniformMatrix3fv(getUniformLocation(_name.c_str()), 1, false, _matrix);
}


///////////////////////////////////////////////////////////////////////////
void Shader::setMatrix3x3Uniform(const std::string& _name, const Matrix4 & _matrix) const {
	assert(programObj_ != 0);
	float mat[9];
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			mat[i*3+j] = _matrix.m[i][j];
	glUniformMatrix3fv(getUniformLocation(_name.c_str()), 1, true, mat );
}

///////////////////////////////////////////////////////////////////////////
void Shader::setMatrix4x4Uniform(const std::string& _name, const float *_matrix) const
{
	assert(programObj_ != 0);
	glUniformMatrix4fv(getUniformLocation(_name.c_str()), 1, false, _matrix);
}

///////////////////////////////////////////////////////////////////////////
void Shader::setMatrix4x4Uniform(const std::string& _name, const Matrix4 & _matrix) const {
	assert(programObj_ != 0);
	float mat[16];
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mat[i*4+j] = _matrix.m[i][j];
	glUniformMatrix4fv(getUniformLocation(_name.c_str()), 1, true, mat );
}

///////////////////////////////////////////////////////////////////////////
int Shader::getUniformLocation(const std::string& _name) const
{
	assert(programObj_ != 0);
	int loc = glGetUniformLocation(programObj_, _name.c_str());
	if(loc == -1) printf("No uniform %s\n", _name.c_str());
	//assert(loc != -1);
	return loc;
}
///////////////////////////////////////////////////////////////////////////
void Shader::clear()
{
	if(programObj_ != 0)
	{
		glDeleteShader(programObj_);
		programObj_ = 0;
	}
}
///////////////////////////////////////////////////////////////////////////