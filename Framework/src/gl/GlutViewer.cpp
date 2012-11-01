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
//   CLASS GlutViewer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "GlutViewer.h"


//== IMPLEMENTATION ========================================================== 


std::map<int, GlutViewer*>  GlutViewer::windows__;


//-----------------------------------------------------------------------------


GlutViewer::
GlutViewer(const char* _title, int _width, int _height)
  : width_(_width), height_(_height), draw_mode_(SOLID_SMOOTH), fullscreen_(false) 
{
  // create window
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA | GLUT_MULTISAMPLE );
  //glutInitDisplayString("rgb double alpha depth>=16 samples>=16");
  glutInitWindowSize(_width, _height);
  windowID_ = glutCreateWindow(_title);
  windows__[windowID_] = this;


  // register callbacks
  glutDisplayFunc(display__);
  glutKeyboardFunc(keyboard__);
  glutSpecialFunc(special__);
  glutMouseFunc(mouse__);
  glutMotionFunc(motion__);
  glutPassiveMotionFunc(passivemotion__);
  glutReshapeFunc(reshape__); 
  glutVisibilityFunc(visibility__);
	glutIdleFunc(idle__);
  
  //Init glew for windows
  #ifdef _WIN32
    glewInit();
  #endif

  // create popupmenu
  //create_popup_menu();
  //glEnable(GL_MULTISAMPLE); 
}
  

//-----------------------------------------------------------------------------


GlutViewer::
~GlutViewer()
{
  glutDestroyWindow(windowID_);
  glutDestroyMenu(menuID_);
}


//-----------------------------------------------------------------------------


GlutViewer* GlutViewer::current_window() { 
  return windows__[glutGetWindow()]; 
}

void GlutViewer::display__(void) {
  current_window()->display();
}

void GlutViewer::idle__(void) {
  current_window()->idle();
} 

void GlutViewer::keyboard__(unsigned char key, int x, int y) {
  current_window()->keyboard((int)key, x, y);
}

void GlutViewer::motion__(int x, int y) {
  current_window()->motion(x, y);
}

void GlutViewer::mouse__(int button, int state, int x, int y) {
  current_window()->mouse(button, state, x, y);
}

void GlutViewer::passivemotion__(int x, int y) {
  current_window()->passivemotion(x, y);
}

void GlutViewer::reshape__(int w, int h) {
  current_window()->reshape(w, h);
}

void GlutViewer::special__(int key, int x, int y) {
  current_window()->special(key, x, y);
}   

void GlutViewer::visibility__(int visible) {
  current_window()->visibility(visible);
}

void GlutViewer::processmenu__(int id) {
  current_window()->processmenu(id);
}


//----------------------------------------------------------------------------


void GlutViewer::create_popup_menu()
{
	// register 'processmenu__' as callback
  // to popupmenu events
  menuID_ = glutCreateMenu(processmenu__);
	
	// add entries to the popupmenu
	glutAddMenuEntry("Wireframe",    WIREFRAME);
	glutAddMenuEntry("Solid Flat",   SOLID_FLAT);
	glutAddMenuEntry("Solid Smooth", SOLID_SMOOTH);
	
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


//----------------------------------------------------------------------------


void GlutViewer::clear_popup_menu()
{
  glutDestroyMenu(menuID_);
  menuID_ = glutCreateMenu(processmenu__);
}


//----------------------------------------------------------------------------


void GlutViewer::processmenu(int id)
{
  switch(id)
  {
    case WIREFRAME:
      draw_mode_ = WIREFRAME;
      break;
    case SOLID_FLAT:
      draw_mode_ = SOLID_FLAT;
      break;
    case SOLID_SMOOTH:
      draw_mode_ = SOLID_SMOOTH;
      break;
    default:
      std::cerr << "Draw mode not supported! Switching to 'Wireframe'." << std::endl;
      draw_mode_ = WIREFRAME;
  }

  glutPostRedisplay();
}


//----------------------------------------------------------------------------


void GlutViewer::keyboard(int key, int x, int y) 
{
  switch (key)
  {
    case 'f': 
    {
      if (!fullscreen_) 
      {
				bak_left_   = glutGet(GLUT_WINDOW_X);
				bak_top_    = glutGet(GLUT_WINDOW_Y);
				bak_width_  = glutGet(GLUT_WINDOW_WIDTH);
				bak_height_ = glutGet(GLUT_WINDOW_HEIGHT);
				glutFullScreen();
				fullscreen_ = true;
      }
      else
      {
				glutReshapeWindow(bak_width_, bak_height_);
				glutPositionWindow(bak_left_, bak_top_);
				fullscreen_ = false;
      }
      break;
    }

    // ESC
	case 27:  
    {
      exit(0); 
      break;
    }
	}
}


//=============================================================================
