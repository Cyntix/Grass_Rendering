//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//   Thibaut Weise, Yuliy Schwartzburg
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS Object3D
//
//=============================================================================


#ifndef OBJECT3D_HH
#define OBJECT3D_HH


//== INCLUDES =================================================================

#include "../math/Matrix4.h"

//== CLASS DEFINITION =========================================================




/*  
	 Basic 3D Object
*/

class Object3D
{
  
public:
    //------------------------------------------------ public methods

	//! constructor which sets the object to world transformation to identity
	Object3D()
    {
		m_transformationMatrix.loadIdentity();
	}
	
	//! destructor
	virtual ~Object3D() {}
	
	//! return the transformation from object to world coordinates
	const Matrix4 & getTransformation() const
    {
        return m_transformationMatrix;
    }

	//! set the current transformation
	void setTransformation( const Matrix4 & mat )
    {
        m_transformationMatrix = mat;
    }
	
	//! return origin of object in world coordinates
	Vector3 origin() const {
		return m_transformationMatrix * Vector3(0,0,0);
	}

	//! set transformation matrix to identity
	void setIdentity() {
		m_transformationMatrix.loadIdentity();
	}
	
	//! translates the object in the world coordinate system
	void translateWorld( const Vector3 & _trans )
	{
        Matrix4 translationMatrix = getTranslationMatrix(_trans);
#ifndef STRIP_CODE_1
		m_transformationMatrix = translationMatrix * m_transformationMatrix;
#else
		// ((( Exercise 3.4 )))
#endif
	}
	
	
	//! translates the object in the object coordinate systems
	void translateObject( const Vector3 & _trans )
	{
        Matrix4 translationMatrix = getTranslationMatrix(_trans);
#ifndef STRIP_CODE_1
		m_transformationMatrix = m_transformationMatrix * translationMatrix;
#else
		// ((( Exercise 3.4 )))
#endif
	}
	
	//! scales the object in the world coordinate system
	void scaleWorld( const Vector3 & _scl )
	{
        Matrix4 scaleMatrix = getScaleMatrix(_scl);
#ifndef STRIP_CODE_2
		m_transformationMatrix = scaleMatrix * m_transformationMatrix;
#else
		// ((( Exercise 3.4 )))
#endif
	}

	//! scales the object in the object coordinate systems
	void scaleObject( const Vector3 & _scl )
	{
        Matrix4 scaleMatrix = getScaleMatrix(_scl);
#ifndef STRIP_CODE_2
		m_transformationMatrix = m_transformationMatrix * scaleMatrix;
#else
		// ((( Exercise 3.4 )))
#endif
	}

	//! rotates the object in the world coordinate system
	void rotateWorld( const Vector3& _axis, float _angle )
	{
        Matrix4 rotationMatrix = getRotationMatrix(_axis,_angle);
#ifndef STRIP_CODE_2
		m_transformationMatrix = rotationMatrix * m_transformationMatrix;
#else
		// ((( Exercise 3.4 )))
#endif
	}

	//! rotates the object in the object coordinate system
	void rotateObject( const Vector3& _axis, float _angle )
	{
        Matrix4 rotationMatrix = getRotationMatrix(_axis,_angle);
#ifndef STRIP_CODE_2
		m_transformationMatrix = m_transformationMatrix * rotationMatrix;
#else
		// ((( Exercise 3.4 )))
#endif
	}


	//! rotates the object around an arbitrary axis in world coordinate system
	void rotateAroundAxisWorld( const Vector3 & _pt, const Vector3& _axis, float _angle )
	{
        Matrix4 rotationMatrix = getRotationMatrix(_axis,_angle);
#ifndef STRIP_CODE
		// translate to object center, rotate, and move object center back
		m_transformationMatrix = 
			getTranslationMatrix(_pt) * rotationMatrix * getTranslationMatrix(-_pt) *
		m_transformationMatrix;
#else
		// ((( Exercise 3.4 )))
#endif
	}

	//! rotates the object around an arbitrary axis in object coordinate system
	void rotateAroundAxisObject( const Vector3 & _pt, const Vector3& _axis, float _angle )
	{
        Matrix4 rotationMatrix = getRotationMatrix(_axis,_angle);
#ifndef STRIP_CODE_2
		// translate to object center, rotate, and move object center back
		m_transformationMatrix = m_transformationMatrix * 
		getTranslationMatrix(_pt) * rotationMatrix * getTranslationMatrix(-_pt);
#else
		// ((( Exercise 3.4 )))
#endif
	}


	//! calculate translation matrix from vector
	static Matrix4 getTranslationMatrix(const Vector3 & _trans) {

		Matrix4 translationMatrix;
        
#ifndef STRIP_CODE_1
		translationMatrix = Matrix4(
                     1,0,0,_trans.x,
                     0,1,0,_trans.y,
                     0,0,1,_trans.z,
                     0,0,0,1
                     );
#else
		// ((( Exercise 3.4 )))
        translationMatrix.loadIdentity();
#endif
		
		return translationMatrix;
	}
	
	//! calculate scale matrix from vector
	static Matrix4 getScaleMatrix(const Vector3 & _scale) {
		
		Matrix4 scaleMatrix;
        
#ifndef STRIP_CODE_2
		scaleMatrix = Matrix4(
					 _scale.x,0,0,0,
					 0,_scale.y,0,0,
					 0,0,_scale.z,0,
					 0,0,0,1
					 );
#else
		// ((( Exercise 3.4 )))
        scaleMatrix.loadIdentity();
#endif
		
		return scaleMatrix;
	}
	
	//! calculate rotation matrix from rotation axis and angle in radian
	static Matrix4 getRotationMatrix(const Vector3 & axis, double angle) {
		double cosa = cos(angle);
		double sina = sin(angle);
		Matrix4 rotationMatrix;

#ifndef STRIP_CODE_2
		rotationMatrix.m[0][0] = cosa + (1-cosa)*axis.x*axis.x; 
		rotationMatrix.m[0][1] = (1-cosa)*axis.x*axis.y - axis.z*sina; 
		rotationMatrix.m[0][2] = (1-cosa)*axis.x*axis.z + axis.y*sina;  
		rotationMatrix.m[0][3] = 0.f;
		
		rotationMatrix.m[1][0] = (1-cosa)*axis.x*axis.y + axis.z*sina; 
		rotationMatrix.m[1][1] = cosa + (1-cosa)*axis.y*axis.y; 
		rotationMatrix.m[1][2] = (1-cosa)*axis.y*axis.z - axis.x*sina;  
		rotationMatrix.m[1][3] = 0.f;
		
		rotationMatrix.m[2][0] = (1-cosa)*axis.x*axis.z - axis.y*sina; 
		rotationMatrix.m[2][1] = (1-cosa)*axis.y*axis.z + axis.x*sina; 
		rotationMatrix.m[2][2] = cosa + (1-cosa)*axis.z*axis.z;  
		rotationMatrix.m[2][3] = 0.f;
		
		rotationMatrix.m[3][0] = 0.f;
		rotationMatrix.m[3][1] = 0.f;
		rotationMatrix.m[3][2] = 0.f;
		rotationMatrix.m[3][3] = 1.f;
#else
		// ((( Exercise 3.4 )))
		rotationMatrix.loadIdentity();
#endif
		return rotationMatrix;
	}
	
protected:
    //------------------------------------------------------- protected data
    //------------- any subclasses of Object3D will have access to this data

	//! transformation matrix from object to world coordinate system
	Matrix4 m_transformationMatrix;
};


//=============================================================================
#endif // OBJECT3D_HH defined
//=============================================================================

