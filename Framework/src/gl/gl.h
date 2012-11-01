//=============================================================================
//                                                                            
//   Exercise code for the lecture
//   "Geometric Modeling Based on Polygonal Meshes"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) 2008  Computer Graphics Group, Bielefeld University.
//                                                                            
//=============================================================================

#ifndef GL_WRAPPERS_HH
#define GL_WRAPPERS_HH

//== INCLUDES =================================================================

//#include <iostream>


// Include files for OpenGL and GLUT have different paths under MacOS...

// Mac OS X
#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>

// Windows
#elif _WIN32
#  include <stdlib.h>
#  include <glew.h>
#  include <glut.h>
#  include <GL/gl.h>
#  include <GL/glu.h>

// Unix
#else
#  include <GL/glut.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#define STRIP_CODE__


//-----------------------------------------------------------------------------


// Nice wrapper that outputs all current OpenGL errors to std::cerr.
// inline void glCheckErrors()
// {
//   GLenum error;
//   while ((error = glGetError()) != GL_NO_ERROR)
//   {
//     std::cerr << "GL error: " << gluErrorString(error) << std::endl;
//   }
// }


//=============================================================================
#endif // GL_WRAPPERS_HH defined
//=============================================================================
