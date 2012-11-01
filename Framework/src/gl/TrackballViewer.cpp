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
//   CLASS TrackballViewer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "TrackballViewer.h"

//== IMPLEMENTATION ========================================================== 


TrackballViewer::
TrackballViewer(const char* _title, int _width, int _height)
: GlutViewer(_title, _width, _height)
{
  // init mouse buttons
  for (int i=0; i<10; ++i)
    button_down_[i] = false;
  
	
  init();
}


//-----------------------------------------------------------------------------


void 
TrackballViewer::
init()
{
  // OpenGL state
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glColor4f(0.0, 0.0, 0.0, 1.0);
  glDisable( GL_DITHER );
  glEnable( GL_DEPTH_TEST );

  set_scene_pos(Vector3(0.0, 0.0, 0.0), 1.0);
}

 
//-----------------------------------------------------------------------------


void
TrackballViewer::
reshape(int _w, int _h)
{
  width_  = _w; 
  height_ = _h;
  
  m_camera.setSize(_w,_h);
  glViewport(0, 0, _w, _h);
  glutPostRedisplay();
}


//----------------------------------------------------------------------------
// set scene to view position
void
TrackballViewer::
set_scene_pos( const Vector3& _pos, float _radius )
{
  m_center = _pos;
  // set camera radius so that is can observe the scene
  m_camera.setRadius(_radius);  
	
  view_all();
}



//----------------------------------------------------------------------------
// reset the camera so that it sees the whole scene
void
TrackballViewer::
view_all()
{
	// move camera to world coordinate center
	m_camera.translateWorld( - m_camera.origin() );
	
	// move camera to object center
	m_camera.translateWorld( m_center );
	
	// move camera (zoom) so that object center is in negative view direction of camera
	m_camera.zoomCamera(-1.0);
	
	// keep camera rotation center in camera coordinates
	m_camera_rotation_depth = (m_camera.getTransformation() * m_center).z;
}




//----------------------------------------------------------------------------


bool
TrackballViewer::
map_to_sphere( int _x, int _y, Vector3& _v3D )
{
    if ( (_x >= 0) && (_x <= width_) && (_y >= 0) && (_y <= height_) ) 
    {
        double x  = (double)(_x - 0.5*width_)  / (double)width_;
        double y  = (double)(0.5*height_ - _y) / (double)height_;
        double sinx         = sin(M_PI * x * 0.5);
        double siny         = sin(M_PI * y * 0.5);
        double sinx2siny2   = sinx * sinx + siny * siny;
        
        _v3D.x = sinx;
        _v3D.y = siny;
        _v3D.z = sinx2siny2 < 1.0 ? sqrt(1.0 - sinx2siny2) : 0.0;
        
        return true;
    }
    else return false;
}


//-----------------------------------------------------------------------------


void 
TrackballViewer::
display()
{
	draw_scene(draw_mode_);	
	
	// switch back-buffer to front-buffer
	glutSwapBuffers();
}




//-----------------------------------------------------------------------------


void 
TrackballViewer::
keyboard(int key, int x, int y) 
{
	//switch (key)
	//{
	//	default:
	//	{
			GlutViewer::keyboard(key, x, y);
	//		break;
	//	}
	//}
}


//-----------------------------------------------------------------------------


void
TrackballViewer::
special(int key, int x, int y)
{
	//switch (key)
	//{			
	//	default: 
	//	{
			GlutViewer::special(key, x, y);
	//		break;
	//	}
	//}
}


//-----------------------------------------------------------------------------

void TrackballViewer::idle()
{
	GlutViewer::idle();
}

//-----------------------------------------------------------------------------

void 
TrackballViewer::
draw_scene(DrawMode _draw_mode)
{
	// clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	Matrix4 projectionMatrixGL = 
	(m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse()).Transpose();
	glMultMatrixd( projectionMatrixGL.dataBlock() );
	
	glMatrixMode( GL_MODELVIEW );
    
	
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
	//Matrix4 modelViewMatrixGL = m_object.getTransformation().Transpose();
	//glMultMatrixd( modelViewMatrixGL.dataBlock() );
	
	
  glDisable(GL_LIGHTING);
  glColor3f(0,0,0);
  glutSolidTeapot(0.5);
	
}


//-----------------------------------------------------------------------------


void 
TrackballViewer::
mouse(int button, int state, int x, int y)
{
  // mouse press
  if (state == GLUT_DOWN)
  {
    last_x_ = x;
    last_y_ = y;
    last_point_ok_ = map_to_sphere( x, y, last_point_3D_ );
    button_down_[button] = true;
  }


  // mouse release
  else
  {
    last_point_ok_ = false;
    button_down_[button] = false;

    // GLUT: button 3 or 4 -> mouse wheel clicked
    if (button == 3)       
      zoom(0, (int)(y - 0.05*width_));
    else if (button == 4)
      zoom(0, (int)(y + 0.05*width_));
  }

	modifiers_ = glutGetModifiers();
	
  glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void 
TrackballViewer::
motion(int x, int y)
{
  // zoom
  if ((button_down_[0] && button_down_[1]) ||
			(button_down_[0] && (modifiers_==GLUT_ACTIVE_SHIFT)))
  {
    zoom(x, y);
  }

  // translation
  else if (button_down_[1] ||
					 (button_down_[0] && (modifiers_==GLUT_ACTIVE_ALT)))
  {
    translation(x, y);
  }

  // rotation
  else if (button_down_[0])
  {
    rotation(x, y);
  }
	
	
  // remeber points
  last_x_ = x;
  last_y_ = y;
  last_point_ok_ = map_to_sphere(x, y, last_point_3D_);

  glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void 
TrackballViewer::
rotation(int x, int y)
{
  if (last_point_ok_) 
  {
    Vector3  new_point_3D;
    bool   new_point_ok;

    new_point_ok = map_to_sphere(x, y, new_point_3D);
    
    if (new_point_ok)
    {
      Vector3 axis      = last_point_3D_.cross( new_point_3D );
      float cos_angle =last_point_3D_.dot( new_point_3D );

      if (fabs(cos_angle) < 1.0) 
      {
		  axis.normalize();
		  float angle = 2.0*acos(cos_angle);
		  // rotate camera around point
		  m_camera.rotateAroundAxisObject(Vector3(0,0,-m_camera_rotation_depth),axis,-angle);
      }
    }
  }
}


//-----------------------------------------------------------------------------


void 
TrackballViewer::
translation(int x, int y)
{
  // change in x and y since last mouse event
  float dx = -(x - last_x_)/float(width_);
  float dy = -(y - last_y_)/float(height_);
	
  // transform world center into camera coordinates
  Vector3 ptCamera = m_camera.getTransformation().Inverse() * m_center;
		
  // get depth, -1 du to negative viewing direction of OpenGL
  double z = -ptCamera.z;
		
	// find scaling of dx and dy from window coordiantes to near plane
	// coordiantes and from there to camera coordinates at the object's depth
	double near = m_camera.getNearPlane();
    
    double top, bottom, left, right;
    m_camera.getScreenExtents(top, bottom, left, right);
		
	// translate the camera accordingly
	float tx=2.0 * dx  * right / near * z;
	float ty = -2.0 * dy * top / near * z;
	m_camera.translateObject( Vector3( tx, ty, 0.0) );
	
	
}


//-----------------------------------------------------------------------------


void 
TrackballViewer::
zoom(int x, int y)
{
  float dy = y - last_y_;
  float h  = height_;
	
  float frac = dy / h;
  float dist = m_camera.getRadius() *  frac * 3.0;
	
	m_camera.translateObject( Vector3(0,0, dist ) );
	m_camera_rotation_depth += dist;
}




//=============================================================================
