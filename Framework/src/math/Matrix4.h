/****************************************************************************
|*  Matrix4.h
|*
|*  Definition of a 4x4 double matrix and its basic functionality.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Balint Miklos, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _MATRIX4_H
#define _MATRIX4_H


#include <math.h>
#include <assert.h>
#include <iostream>


#include "Vector3.h"
#include "Vector4.h"


class Matrix4 {
public:
	// Matrix4 Constructors
	////////////////////////

	Matrix4(double m_00=0, double m_01=0, double m_02=0, double m_03=0,
	        double m_10=0, double m_11=0, double m_12=0, double m_13=0,
			double m_20=0, double m_21=0, double m_22=0, double m_23=0,
			double m_30=0, double m_31=0, double m_32=0, double m_33=0) {
		m[0][0] = m_00; m[0][1] = m_01; m[0][2] = m_02;  m[0][3] = m_03;
		m[1][0] = m_10; m[1][1] = m_11; m[1][2] = m_12;  m[1][3] = m_13;
		m[2][0] = m_20; m[2][1] = m_21; m[2][2] = m_22;  m[2][3] = m_23;
		m[3][0] = m_30; m[3][1] = m_31; m[3][2] = m_32;  m[3][3] = m_33;
	}

	Matrix4(double n[4][4]) {
		m[0][0]=n[0][0]; m[0][1]=n[0][1]; m[0][2]=n[0][2]; m[0][3]=n[0][3];
		m[1][0]=n[1][0]; m[1][1]=n[1][1]; m[1][2]=n[1][2]; m[1][3]=n[1][3];
		m[2][0]=n[2][0]; m[2][1]=n[2][1]; m[2][2]=n[2][2]; m[2][3]=n[2][3];
		m[3][0]=n[3][0]; m[3][1]=n[3][1]; m[3][2]=n[3][2]; m[3][3]=n[3][3];
	}

	Matrix4(const Vector3 a1, const Vector3 a2, const Vector3 a3) {
		m[0][0] = a1.x; m[0][1] = a2.x; m[0][2] = a3.x;  m[0][3] = 0.f;
		m[1][0] = a1.y; m[1][1] = a2.y; m[1][2] = a3.y;  m[1][3] = 0.f;
		m[2][0] = a1.z; m[2][1] = a2.z; m[2][2] = a3.z;  m[2][3] = 0.f;
		m[3][0] = 0.f;  m[3][1] = 0.f;  m[3][2] = 0.f;   m[3][3] = 1.f;
	}


	Matrix4(const Vector4 a1, const Vector4 a2, const Vector4 a3) {
		m[0][0] = a1.x; m[0][1] = a2.x; m[0][2] = a3.x;  m[0][3] = 0.f;
		m[1][0] = a1.y; m[1][1] = a2.y; m[1][2] = a3.y;  m[1][3] = 0.f;
		m[2][0] = a1.z; m[2][1] = a2.z; m[2][2] = a3.z;  m[2][3] = 0.f;
		m[3][0] = a1.w; m[3][1] = a2.w; m[3][2] = a3.w;  m[3][3] = 1.f;
	}


	// Matrix4-Matrix4 Operations
	/////////////////////////////

	Matrix4 operator+(const Matrix4 &n) const {
		return Matrix4(m[0][0]+n.m[0][0], m[0][1]+n.m[0][1], m[0][2]+n.m[0][2], m[0][3]+n.m[0][3],
		               m[1][0]+n.m[1][0], m[1][1]+n.m[1][1], m[1][2]+n.m[1][2], m[1][3]+n.m[1][3],
		               m[2][0]+n.m[2][0], m[2][1]+n.m[2][1], m[2][2]+n.m[2][2], m[2][3]+n.m[2][3],
		               m[3][0]+n.m[3][0], m[3][1]+n.m[3][1], m[3][2]+n.m[3][2], m[3][3]+n.m[3][3]);
	}
	
	Matrix4& operator+=(const Matrix4 &n) {
		m[0][0]+=n.m[0][0]; m[0][1]+=n.m[0][1]; m[0][2]+=n.m[0][2]; m[0][3]+=n.m[0][3];
		m[1][0]+=n.m[1][0]; m[1][1]+=n.m[1][1]; m[1][2]+=n.m[1][2]; m[1][3]+=n.m[1][3];
		m[2][0]+=n.m[2][0]; m[2][1]+=n.m[2][1]; m[2][2]+=n.m[2][2]; m[2][3]+=n.m[2][3];
		m[3][0]+=n.m[3][0]; m[3][1]+=n.m[3][1]; m[3][2]+=n.m[3][2]; m[3][3]+=n.m[3][3];
		return *this;
	}

	Matrix4 operator-(const Matrix4 &n) const {
		return Matrix4(m[0][0]-n.m[0][0], m[0][1]-n.m[0][1], m[0][2]-n.m[0][2], m[0][3]-n.m[0][3],
		               m[1][0]-n.m[1][0], m[1][1]-n.m[1][1], m[1][2]-n.m[1][2], m[1][3]-n.m[1][3],
		               m[2][0]-n.m[2][0], m[2][1]-n.m[2][1], m[2][2]-n.m[2][2], m[2][3]-n.m[2][3],
		               m[3][0]-n.m[3][0], m[3][1]-n.m[3][1], m[3][2]-n.m[3][2], m[3][3]-n.m[3][3]);
	}
	
	Matrix4& operator-=(const Matrix4 &n) {
		m[0][0]-=n.m[0][0]; m[0][1]-=n.m[0][1]; m[0][2]-=n.m[0][2]; m[0][3]-=n.m[0][3];
		m[1][0]-=n.m[1][0]; m[1][1]-=n.m[1][1]; m[1][2]-=n.m[1][2]; m[1][3]-=n.m[1][3];
		m[2][0]-=n.m[2][0]; m[2][1]-=n.m[2][1]; m[2][2]-=n.m[2][2]; m[2][3]-=n.m[2][3];
		m[3][0]-=n.m[3][0]; m[3][1]-=n.m[3][1]; m[3][2]-=n.m[3][2]; m[3][3]-=n.m[3][3];
		return *this;
	}

	bool operator==(const Matrix4 &n) const {
		return m[0][0]==n.m[0][0] && m[0][1]==n.m[0][1] && m[0][2]==n.m[0][2] && m[0][3]==n.m[0][3] &&
		       m[1][0]==n.m[1][0] && m[1][1]==n.m[1][1] && m[1][2]==n.m[1][2] && m[1][3]==n.m[1][3] &&
		       m[2][0]==n.m[2][0] && m[2][1]==n.m[2][1] && m[2][2]==n.m[2][2] && m[2][3]==n.m[2][3] &&
		       m[3][0]==n.m[3][0] && m[3][1]==n.m[3][1] && m[3][2]==n.m[3][2] && m[3][3]==n.m[3][3];
	}
	
	Matrix4 operator*(const Matrix4 &n) const {
		Matrix4 o;
		for(int i = 0; i  < 4; i++) {
			for(int j = 0; j < 4; j++) {
				double v = 0;
				for(int k = 0; k < 4; k++) {
					v += m[i][k] * n.m[k][j];
				}
				o.m[i][j] = v;
			}
		}
		return o;
	}


	// Matrix4-Vector operations
	////////////////////////////

	Vector3 operator*(const Vector3 &v) const {
		Vector3 u =Vector3(m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3],
		                   m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3],
						   m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]);
		double w = m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3];
		return u/w;
	}


	Vector4 operator*(const Vector4 &v) const {
		Vector4 u = Vector4(m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + v.w*m[0][3],
		                   m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + v.w*m[1][3],
						   m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + v.w*m[2][3], 
						   m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + v.w*m[3][3]);
		if(u.w != 0)
			return u/u.w;
		else 
			return u;
	}


	// Matrix4-double operations
	///////////////////////////

	Matrix4 operator*(double f) const {
		return Matrix4(m[0][0]*f, m[0][1]*f, m[0][2]*f,  m[0][3]*f,
		               m[1][0]*f, m[1][1]*f, m[1][2]*f,  m[1][3]*f,
		               m[2][0]*f, m[2][1]*f, m[2][2]*f,  m[2][3]*f,
		               m[3][0]*f, m[3][1]*f, m[3][2]*f,  m[3][3]*f);
	}
	
	Matrix4 &operator*=(double f) {
		m[0][0]*=f; m[0][1]*=f; m[0][2]*=f;  m[0][3]*=f;
		m[1][0]*=f; m[1][1]*=f; m[1][2]*=f;  m[1][3]*=f;
		m[2][0]*=f; m[2][1]*=f; m[2][2]*=f;  m[2][3]*=f;
		m[3][0]*=f; m[3][1]*=f; m[3][2]*=f;  m[3][3]*=f;
		return *this;
	}

	Matrix4 operator/(double f) const {
		assert(f!=0);
		return Matrix4(m[0][0]/f, m[0][1]/f, m[0][2]/f,  m[0][3]/f,
		               m[1][0]/f, m[1][1]/f, m[1][2]/f,  m[1][3]/f,
		               m[2][0]/f, m[2][1]/f, m[2][2]/f,  m[2][3]/f,
		               m[3][0]/f, m[3][1]/f, m[3][2]/f,  m[3][3]/f);
	}
	
	Matrix4 &operator/=(double f) {
		assert(f!=0);
		m[0][0]/=f; m[0][1]/=f; m[0][2]/=f;  m[0][3]/=f;
		m[1][0]/=f; m[1][1]/=f; m[1][2]/=f;  m[1][3]/=f;
		m[2][0]/=f; m[2][1]/=f; m[2][2]/=f;  m[2][3]/=f;
		m[3][0]/=f; m[3][1]/=f; m[3][2]/=f;  m[3][3]/=f;
		return *this;
	}

	
	// Matrix4 self operations
	//////////////////////////

	Matrix4 operator-() const {
		return Matrix4(-m[0][0], -m[0][1], -m[0][2],  -m[0][3],
		               -m[1][0], -m[1][1], -m[1][2],  -m[1][3],
		               -m[2][0], -m[2][1], -m[2][2],  -m[2][3],
		               -m[3][0], -m[3][1], -m[3][2],  -m[3][3]);
	}

	Matrix4 Transpose() const {
	return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
	               m[0][1], m[1][1], m[2][1], m[3][1],
	               m[0][2], m[1][2], m[2][2], m[3][2],
	               m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	Matrix4 Inverse() const {
		int indxc[4], indxr[4];
		int ipiv[4] = { 0, 0, 0, 0 };
		double minv[4][4];
		double temp;
		
		for (int s=0; s<4; s++) {
			for (int t=0; t<4; t++) {
				minv[s][t] = m[s][t];
			}
		}

		for (int i = 0; i < 4; i++) {
			int irow = -1, icol = -1;
			double big = 0.;
			// Choose pivot
			for (int j = 0; j < 4; j++) {
				if (ipiv[j] != 1) {
					for (int k = 0; k < 4; k++) {
						if (ipiv[k] == 0) {
							if (fabs(minv[j][k]) >= big) {
								big = double(fabs(minv[j][k]));
								irow = j;
								icol = k;
							}
						}
						else if (ipiv[k] > 1) {
							std::cout << "ERROR: Singular matrix in MatrixInvert\n";
						}
					}
				}
			}
			++ipiv[icol];
			// Swap rows _irow_ and _icol_ for pivot
			if (irow != icol) {
				for (int k = 0; k < 4; ++k){
					temp = minv[irow][k];
					minv[irow][k] = minv[icol][k];
					minv[icol][k] = temp;
				}
			}
			indxr[i] = irow;
			indxc[i] = icol;
			if (minv[icol][icol] == 0.){
				std::cout << "Singular matrix in MatrixInvert\n";
			}
			// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
			double pivinv = 1.f / minv[icol][icol];
			minv[icol][icol] = 1.f;
			for (int j = 0; j < 4; j++) {
				minv[icol][j] *= pivinv;
			}
			
			// Subtract this row from others to zero out their columns
			for (int j = 0; j < 4; j++) {
				if (j != icol) {
					double save = minv[j][icol];
					minv[j][icol] = 0;
					for (int k = 0; k < 4; k++) {
						minv[j][k] -= minv[icol][k]*save;
					}
				}
			}
		}
		// Swap columns to reflect permutation
		for (int j = 3; j >= 0; j--) {
			if (indxr[j] != indxc[j]) {
				for (int k = 0; k < 4; k++){
					temp = minv[k][indxr[j]];
					minv[k][indxr[j]] = minv[k][indxc[j]];
					minv[k][indxc[j]] = temp;
				}
			}
		}
		return Matrix4(minv);
	}

	Matrix4 loadIdentity() {
		m[0][0] = 1.f; m[0][1] = 0.f; m[0][2] = 0.f;  m[0][3] = 0.f;
		m[1][0] = 0.f; m[1][1] = 1.f; m[1][2] = 0.f;  m[1][3] = 0.f;
		m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = 1.f;  m[2][3] = 0.f;
		m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f;  m[3][3] = 1.f;
		return *this;
	}


	void print() const {
		for (int i=0; i<4; i++) {
			for (int j=0; j<4; j++) {
				std::cout << m[i][j];
			}
			std::cout << "\n";
		}
	}


	double * dataBlock() {
		return m[0];
	}
	
	const double * dataBlock() const {
		return m[0];
	}
	
	// Vector3 Data
	double m[4][4];
};

inline Matrix4 operator*(double f, const Matrix4 & m) {
	return (m * f);
}

#endif //_MATRIX4_H
