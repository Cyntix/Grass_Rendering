//=============================================================================
//                                                                            
//   Exercise code for the lecture
//   "Geometric Modeling Based on Polygonal Meshes"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) 2008  Computer Graphics Group, Bielefeld University.
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS GlutViewer
//
//=============================================================================


#ifndef GLUTVIEWER_HH
#define GLUTVIEWER_HH


//== INCLUDES =================================================================

#include <iostream>
#include <map>

#include "gl.h"



//== CLASS DEFINITION =========================================================

	      

/* Simple Glut viewer. 
	 Based on C++ glut interface of George Stetten and Korin Crawford.
*/

class GlutViewer
{
public: //------------------------------------------------ public types
  
  enum DrawMode {WIREFRAME, SOLID_FLAT, SOLID_SMOOTH};
  
  
  
public: //------------------------------------------------ public methods
   
  GlutViewer(const char* _title, int _width, int _height);
  virtual ~GlutViewer();

  
protected: //------------------------------------------------ protected methods

  virtual void display(void) {}
  virtual void reshape(int w, int h) {}
  virtual void motion(int x, int y) {}
  virtual void mouse(int button, int state, int x, int y) {}
  virtual void passivemotion(int x, int y) {}
  virtual void visibility(int visible) {}
  virtual void idle(void) {}
  virtual void special(int key, int x, int y){}
  
  virtual void keyboard(int key, int x, int y);
  virtual void processmenu(int i);
  virtual void create_popup_menu();

  void clear_popup_menu();

  //virtual void draw_scene(DrawMode _draw_mode);
  
	void toggle_idle(bool _b) { glutIdleFunc( _b ? idle__ : NULL );	}
	

	
protected: //--------------------------------------------------- protected data
	
  int  width_, height_;
  DrawMode draw_mode_;

private: //---------------------------------------------------- private methods
	
  static void display__(void);
  static void idle__(void); 
  static void keyboard__(unsigned char key, int x, int y);
  static void motion__(int x, int y);
  static void mouse__(int button, int state, int x, int y);
  static void passivemotion__(int x, int y);
  static void reshape__(int w, int h); 
  static void special__(int key, int x, int y);   
  static void visibility__(int visible);
  static void processmenu__(int i);
	

private: //------------------------------------------------------- private data

  static std::map<int, GlutViewer*>  windows__;
  static GlutViewer* current_window();

  int  windowID_, menuID_;; 
  bool fullscreen_;
  int  bak_left_, bak_top_, bak_width_, bak_height_;
};


//=============================================================================
#endif // GLUTVIEWER_HH defined
//=============================================================================

