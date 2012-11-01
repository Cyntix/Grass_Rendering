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
//  CLASS TrackballViewer
//
//=============================================================================


#ifndef TRACKBALL_VIEWER_HH
#define TRACKBALL_VIEWER_HH


//== INCLUDES =================================================================
#include <string>
#include <vector>

#include "GlutViewer.h"
#include "../math/Matrix4.h"
#include "../math/Vector3.h"

#include "Camera3D.h"

//== CLASS DEFINITION =========================================================


	      
/*
 TrackballViewer implements a virtual trackball interaction.
 The scene is expected to be contained in the unit sphere.
 */
class TrackballViewer : public GlutViewer
{
public:
    
    TrackballViewer(const char* _title, int _width, int _height);
    
    
    /* Sets the center and size of the whole scene. 
     The _center is used as fixpoint for rotations and for adjusting
     the camera/viewer (see view_all()). */
    void set_scene_pos( const Vector3 & _pos, float _radius );  
    
    /* view the whole scene: the eye point is moved far enough from the
     center so that the whole scene is visible. */
    void view_all();
    
    
protected:
    
	// overloaded GUI function
    virtual void init();
    virtual void reshape(int w, int h); 
    virtual void motion(int x, int y);
    virtual void mouse(int button, int state, int x, int y);
    virtual void keyboard(int key, int x, int y);
    virtual void special(int key, int x, int y);
    virtual void display();
    virtual void idle();
    
    virtual void draw_scene(DrawMode _draw_mode);
    
    // mouse events trigger rotation, translation, or zoom
    void rotation(int x, int y);
    void translation(int x, int y);
    void zoom(int x, int y);
    
    
    // virtual trackball: map 2D screen point to unit sphere
    bool map_to_sphere(int x, int y, Vector3& _result);
    
    
    
protected:
    
    // trackball helpers
    int      last_x_, last_y_;
    Vector3  last_point_3D_;
    bool     last_point_ok_;
    bool     button_down_[10];
    int      modifiers_;
    
	Vector3 m_center;
	
	// camera object
	Camera3D m_camera;
	
	double m_camera_rotation_depth;
	
	double m_movRadius;
	
};


//=============================================================================
#endif // TRACKBALL_VIEWER_HH defined
//=============================================================================
