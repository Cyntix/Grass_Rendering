//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//   Thibaut Weise, Yuliy Schwartzburg
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS Camera3D
//
//=============================================================================


#ifndef CAMERA3D_HH
#define CAMERA3D_HH


//== INCLUDES =================================================================

#include "Object3D.h"

//== CLASS DEFINITION =========================================================



/*
     Perspective Camera
*/

class Camera3D : public Object3D
{
    
public: //------------------------------------------------ public methods
    
	//! constructor the creates a basic perspective camera
	Camera3D() : Object3D()
    {
        reset();
		updateProjectionMatrix();
	}
	
	
	//! destructor
	virtual ~Camera3D() {}
	
    //! reset to default values
    void reset()
    {
		m_fovy = 45.0;
		m_near = 0.1;
		m_far  = 10.0;
		m_width = 1;
		m_height = 1;
		m_radius = 1;
    }
	
	//! returns the projection matrix from camera coordinates to image coordinates
	const Matrix4 & getProjectionMatrix() const { return m_perspectiveProjectionMatrix; }
	
	
	//! set the size of the camera
	void setSize( int width, int height )
    { 
		m_width = width;
		m_height = height; 
		updateProjectionMatrix();
	}
    
    //! get the width of the camera
    int getWidth() { return m_width; }
    
    //! get the height of the camera
    int getHeight() { return m_height; }
	
	//! get the radius of the scene
	double getRadius() { return m_radius; }
	
	//! set the radius of the scene the camera should capture - sets the clipping planes accordingly
	void setRadius( double radius )
    {
		m_radius = radius;
		setClippingPlanes(0.01*radius, 10.0*radius);
	}
	
	
	//! set near and far clipping plane
	void setClippingPlanes( double near, double far )
    {
		m_near = near;
		m_far = far;
		updateProjectionMatrix();
	}
	
	
	//! set field of view (y-direction) of camera in degrees, field of view of x-direction is calculated from camera aspect ratio
	void setFOV( double fovy )
    {
		m_fovy = fovy;
		updateProjectionMatrix();
	}
	
	
	//! zoom camera with fraction between [0,1]
	void zoomCamera( double frac )
    {
		// calculate zoom vector in camera coordinates
		Vector3 zoomVec( 0, 0, -m_radius * frac * 3.0 );
		
		translateObject( zoomVec );
	}
	
	//! return near clipping plane
	double getNearPlane() { return m_near; }
	
	//! return far clipping plane
	double getFarPlane() { return m_far; }
	
    //! return screen extents
    void getScreenExtents(double &top, double &bottom, double &left, double &right)
    {
#ifndef STRIP_CODE
		top = getNearPlane() * tan( (m_fovy / 2.0) * (M_PI / 180.0) );
		bottom = -1*top;
        
		double aspect = (double) m_width / (double) m_height;
		left = bottom * aspect;
        right = top * aspect;
#else
		// ((( Exercise 3.2.4 )))
        top = -1;
        bottom = 1;
        left = -1;
        right = 1;
#endif      
    }
	
	
protected:
	//! calculate the perspective projection matrix
	void updateProjectionMatrix()
    {
#ifndef STRIP_CODE
		double far = getFarPlane();
		double near = getNearPlane();
        
		double top, bottom, left, right;
        getScreenExtents(top, bottom, left, right);
		
		double fx = 2*near/(right-left);
		double fy = 2*near/(top-bottom);
		double fz = -(far+near)/(far-near);
		double fw = -2*far*near/(far-near);
		
		m_perspectiveProjectionMatrix = Matrix4(
			fx, 0, 0, 0,
			0, fy, 0, 0,
			0, 0, fz, fw,
			0, 0, -1, 0
		);
#else
		// ((( Exercise 3.2.4 )))
        // replace this
		m_perspectiveProjectionMatrix = Matrix4(
                                                1, 0, 0, 0,
                                                0, 1, 0, 0,
                                                0, 0, 1, 1,
                                                0, 0, -1, 0
                                                );
#endif
	}
	
private: //------------------------------------------------------- private data

	int m_width;		// camera width
	int m_height;		// camera height
	double  m_fovy;		// field of view
	double m_near;		// near clipping plane
	double m_far;		// far clipping plane
	double m_radius;	// camera view radius
	
	Matrix4 m_perspectiveProjectionMatrix;
};


//=============================================================================
#endif // CAMERA_HH defined
//=============================================================================

