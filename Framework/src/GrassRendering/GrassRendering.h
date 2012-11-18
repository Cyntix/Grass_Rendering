//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================
//=============================================================================
//
//  CLASS MeshViewer
//
//=============================================================================


#ifndef MESH_VIEWER_HH
#define MESH_VIEWER_HH


//== INCLUDES =================================================================
#include <string>
#include <vector>

#include "../gl/TrackballViewer.h"
#include "../gl/shader.h"
#include "../gl/Mesh3D.h"
#include "../gl/Light3D.h"
#include "../gl/texture.h"
#include "../utils/StopWatch.h"

//== CLASS DEFINITION =========================================================
	      
/*
 MeshViewer.
*/

enum MeshType
{
	SKY = 0,
	TERRAIN,
	GRASS,
	PARTICLE_PATTERN,
};

class GrassRendering : public TrackballViewer
{
public:
   
	GrassRendering(const char* _title, int _width, int _height);
  
	void load_mesh(const std::string& filenameObj, MeshType type);

	void load_grass();
	
	Mesh3D& getSky() {return m_Sky; }

protected:

	// overloaded GUI function
	virtual void init();
	virtual void keyboard(int key, int x, int y);
	virtual void special(int key, int x, int y);
	virtual void idle();
	
	virtual void draw_scene(DrawMode _draw_mode);

	void draw_buffer(Shader& sh, boolean showTexture);
    void draw_sky();
	void draw_terrain(bool showTexture);

	Vector3 GrassRendering::getVertex(int i, boolean rotation);
	Vector3 GrassRendering::getVertexNormal(int i, boolean rotation);
	Vector2 GrassRendering::getTexCoord(int i);

protected:
	
	// mesh objects
	Mesh3D m_Sky;
	Mesh3D m_Terrain;
	Mesh3D m_Grass;

	//particles system
	vector<Vector3> particles;
	Mesh3D m_Pattern1;
	//Vertex buffer
	GLuint vbo;

	//animation
	float direction;
	boolean up;
	// directional light
	Light3D m_light;
	float m_recSunlightInt;
	
	// mesh shader
	Shader m_meshShaderDiffuse;
	Shader m_meshShaderPhong;
	Shader m_meshShaderTexture;
	Shader m_meshShaderStencil;
	
	bool m_showGrass;
	bool m_showTextureSky;
	bool m_showTextureTerrain;
	bool m_showTextureGrass;
	bool m_showAlphaToCoverage;
	
	//scaling factors
	float m_SkyScale;
	float m_TerrainScale;
	float m_GrassScale;
	float m_ParticlesScale;
	float m_PatternsScale;
	
	//timer
	StopWatch watch;
	bool isWatchOn;
	
	float daysPerMiliSecond;
	float totalDaysElapsed;
	float currentTime;
};


//=============================================================================
#endif // MESH_VIEWER_HH defined
//=============================================================================
