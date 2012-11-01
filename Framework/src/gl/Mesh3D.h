//=============================================================================
//                                                                            
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//   Duygu Ceylan
//                                                                            
//=============================================================================
//=============================================================================
//
//   CLASS Mesh3D
//
//=============================================================================

#ifndef MESH3D_HH
#define MESH3D_HH


//== INCLUDES =================================================================

#include <vector>
#include "Object3D.h"
#include "texture.h"
#include "../math/Vector2.h"

using namespace std;

//== CLASS DEFINITION =========================================================

class MeshMaterial
{
public:
    MeshMaterial() :
        m_diffuseColor(1.0, 1.0, 1.0),
        m_specularExp(60.0)
    {}
    
    bool hasDiffuseTexture() const { return m_diffuseTexture.getID() != 0; }
    
    bool hasBumpTexture() const { return m_bumpTexture.getID() != 0; }
    
    Vector3 m_diffuseColor;
    double m_specularExp;
    Texture m_diffuseTexture;
    Texture m_bumpTexture;
};

class Mesh3D : public Object3D
{


public:

	~Mesh3D();
	
	void clear();
	
    // ---------------------------------------------------------------------------------
    
	void setIndices(const vector<unsigned int>& _vertexInd)
    {
        m_vertexIndices.push_back(_vertexInd);
        m_materials.push_back(new MeshMaterial);
    }
    
	void setIndices(const vector<unsigned int>& _vertexInd, MeshMaterial *_material) {
        m_vertexIndices.push_back(_vertexInd);
        m_materials.push_back(_material);
    }
    
	void setVertexPositions(const vector<Vector3>& _vertexPos)
    {
        m_vertexPositions = _vertexPos;
    }
    
	void setVertexNormals(const vector<Vector3>& _vertexNormals)
    {
        m_vertexNormals = _vertexNormals;
    }
    
	void setVertexColors( const vector<Vector3>& _vertexColors )
    {
        m_vertexColors = _vertexColors;
    }
    
	void setVertexUV(const vector<Vector2>& _uvCoord)
    {
        m_vertexUV = _uvCoord;
    }
    
    // ---------------------------------------------------------------------------------
	
	int getNumberOfVertices() const {
        return (int)m_vertexPositions.size();
    }
    
	int getNumberOfFaces(unsigned int part = 0) const {
        return (int)m_vertexIndices[part].size() / 3;
    }

	unsigned int getNumberOfParts() const {
        return (unsigned int)m_vertexIndices.size();
    }
	
    // ---------------------------------------------------------------------------------
    
	//face index: index of the face[0,...,faceNo-1] vertexIndex: [0,1,2]
	unsigned int getFaceVertexIndex(int faceIndex, int vertexIndex, int part = 0) const
    {
        return (m_vertexIndices[part])[faceIndex*3 + vertexIndex];
    }
	
    // ---------------------------------------------------------------------------------
    
	const Vector3& getVertexPosition(int index) const
    {
        return m_vertexPositions[index];
    }
    
	const Vector3& getVertexNormal(int index) const
    {
        return m_vertexNormals[index];
    }
	
    const Vector3& getVertexColor(int index) const
    {
        return m_vertexColors[index];
    }
    
    // ---------------------------------------------------------------------------------
    
	const vector<Vector3>& getVertices() const
    {
        return m_vertexPositions;
    }
    
	const vector<Vector3>& getNormals() const
    {
        return m_vertexNormals;
    }
    
	const vector<Vector2>& getUVs() const
    {
        return m_vertexUV;
    }
    
	const vector<unsigned int>& getIndices(unsigned int part = 0) const
    {
        return m_vertexIndices[part];
    }
    
	const vector<vector<unsigned int> >& getAllIndices() const
    {
        return m_vertexIndices;
    }
    
	vector<Vector3>& getVertices()
    {
        return m_vertexPositions;
    }
    
	vector<Vector3>& getNormals()
    {
        return m_vertexNormals;
    }
    
    
	vector<Vector2>& getUVs()
    {
        return m_vertexUV;
    }
    
	vector<unsigned int>& getIndices(unsigned int part = 0)
    {
        return m_vertexIndices[part];
    }
    
	vector<vector<unsigned int> >& getAllIndices()
    {
        return m_vertexIndices;
    }
    
	const MeshMaterial& getMaterial(unsigned int part = 0) const
    {
        assert( part < m_materials.size() ); return *m_materials[part];
    }
    
	const MeshMaterial& getMaterial(unsigned int part = 0)
    {
        assert( part < m_materials.size() ); return *m_materials[part];
    }
    
	vector<MeshMaterial *>& getAllMaterials()
    {
        return m_materials;
    }
    
	const vector<MeshMaterial *> getAllMaterials() const
    {
        return m_materials;
    }
	
    // ---------------------------------------------------------------------------------
    
	bool hasNormals() const
    {
        return ( m_vertexNormals.size() == m_vertexPositions.size() ) ;
    }
    
	bool hasTangents() const
    {
        return ( m_vertexTangents.size() == m_vertexPositions.size() ) ;
    }
    
	bool hasColors() const
    {
        return ( m_vertexColors.size() == m_vertexPositions.size() ) ;
    }
    
	bool hasUvTextureCoord() const
    {
        return ( m_vertexUV.size() == m_vertexPositions.size() ) ;
    }
    
    // ---------------------------------------------------------------------------------
    
	void* getVertexPointer()
    {
        return &(m_vertexPositions[0]);
    }
    
	void* getNormalPointer()
    {
        return &(m_vertexNormals[0]);
    }
    
	void* getColorPointer()
    {
        return &(m_vertexColors[0]);
    }
    
	void* getUvTextureCoordPointer()
    {
        return &(m_vertexUV[0]);
    }
    
	void* getVertexIndicesPointer(unsigned int part = 0)
    {
        return &(m_vertexIndices[part])[0];
    }
    
    // ---------------------------------------------------------------------------------
    
	// calculate vertex normals given the triangles
	void calculateVertexNormals();
	
    // ---------------------------------------------------------------------------------
    
	// calculate bounding box
	void calculateBoundingBox( Vector3 & bbmin, Vector3 & bbmax );
    
	
private:
	//!stores position of each vertex in the mesh
	vector<Vector3> m_vertexPositions;
						 
	//!stores the indices of the vertices that form each face(triangle) in the mesh
	vector<vector<unsigned int> > m_vertexIndices;
						 
	//!stores the normals of each vertex in the mesh
	vector<Vector3> m_vertexNormals;

	//!stores the tangents of each vertex in the mesh
	vector<Vector3> m_vertexTangents;
						 
	//!stores the colors of each vertex in the mesh
	vector<Vector3> m_vertexColors;
						 
	//!stores the uv texture coordinates
	vector<Vector2> m_vertexUV;

	//!Material of this mesh
	vector<MeshMaterial *> m_materials;
						 
};

#endif /* MESH3D_HH */
