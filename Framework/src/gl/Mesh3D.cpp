//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//   Duygu Ceylan
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS Mesh3D - IMPLEMENTATION
//
//=============================================================================

#include <assert.h>
#include "Mesh3D.h"


// ---------------------------------------------------------------------------------

Mesh3D::~Mesh3D()
{
	clear();
}

void Mesh3D::clear()
{
	vector<Vector3>().swap(m_vertexPositions);
	vector<vector<unsigned int> >().swap(m_vertexIndices);
	vector<Vector3>().swap(m_vertexNormals);
	vector<Vector2>().swap(m_vertexUV);
	for(int i = 0; i < (int)m_materials.size(); ++i)
		delete m_materials[i];
	vector<MeshMaterial *>().swap(m_materials);
}

// ---------------------------------------------------------------------------------
// calculate vertex normals given the triangles
void 
Mesh3D::
calculateVertexNormals()
{
	// create 0-normals
	m_vertexNormals = std::vector< Vector3 >( getNumberOfVertices(), Vector3(0,0,0) );
	
	for(int i = 0; i < getNumberOfFaces(); i++) {
		int v1 = getFaceVertexIndex(i,0);
		int v2 = getFaceVertexIndex(i,1);
		int v3 = getFaceVertexIndex(i,2);
		
		assert( v1 < getNumberOfVertices() );
		assert( v2 < getNumberOfVertices() );
		assert( v3 < getNumberOfVertices() );
		
		Vector3 p = getVertexPosition( v1 );
		Vector3 q = getVertexPosition( v2 );
		Vector3 r = getVertexPosition( v3 );
		Vector3 n = (q-p).cross(r-p).normalize();
		
		m_vertexNormals[v1] += n;
		m_vertexNormals[v2] += n;
		m_vertexNormals[v3] += n;
	}
	
	for(int i = 0; i < getNumberOfVertices(); i++) {
		assert( m_vertexNormals[i].length() > 0 );
		m_vertexNormals[i] = m_vertexNormals[i].normalize();
	}

}



// ---------------------------------------------------------------------------------
// calculate bounding box
void 
Mesh3D::
calculateBoundingBox( Vector3 & bbMin, Vector3 & bbMax )
{
	
	if (!m_vertexPositions.empty())
	{
		bbMin = m_vertexPositions[0];
		bbMax = m_vertexPositions[0];
		
		std::vector<Vector3>::const_iterator  p_it(m_vertexPositions.begin());
		std::vector<Vector3>::const_iterator  p_end(m_vertexPositions.end());
		
		for (; p_it!=p_end; ++p_it)
		{
			if( (*p_it).x < bbMin.x ) bbMin.x = (*p_it).x;
			else if ( (*p_it).x > bbMax.x ) bbMax.x = (*p_it).x;
			
			if( (*p_it).y < bbMin.y ) bbMin.y = (*p_it).y;
			else if ( (*p_it).y > bbMax.y ) bbMax.y = (*p_it).y;
			
			if( (*p_it).z < bbMin.z ) bbMin.z = (*p_it).z;
			else if ( (*p_it).z > bbMax.z ) bbMax.z = (*p_it).z;
		}
	}
	else {
		printf("No vertex positions to calculate bounding box!\n");
		assert(false);
	}
}

