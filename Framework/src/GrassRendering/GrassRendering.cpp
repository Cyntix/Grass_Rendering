//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================
//=============================================================================
//
//  CLASS MeshViewer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "GrassRendering.h"
#include "../utils/Mesh3DReader.h"
#include <stdio.h>
#define drand48() (double)rand()/RAND_MAX
//== IMPLEMENTATION ========================================================== 


GrassRendering::
GrassRendering(const char* _title, int _width, int _height)
: TrackballViewer(_title, _width, _height)
{
  init();
}


//-----------------------------------------------------------------------------


void 
GrassRendering::
init()
{
  // initialize parent
  TrackballViewer::init();

  // set camera to look at world coordinate center
  set_scene_pos(Vector3(0.0, 0.0, 0.0), 2.0);
	
	// load mesh shader
	m_meshShaderDiffuse.create("diffuse.vs", "diffuse.fs");
	m_meshShaderTexture.create("tex.vs","tex.fs");
	m_meshShaderStencil.create("stencil.vs", "stencil.fs");

	m_showTextureSky = false;
	
	currentTime = 0.0;
	isWatchOn = false;
	
	daysPerMiliSecond = 1 / 180.0;
	totalDaysElapsed = 0;

	//FOR THE MOMENT, THE LIGHT HAS NO INCIDENCE IN GSLS
	m_light.origin() = Vector3(0.0, 0.0, 0.0);
	m_recSunlightInt = 1.0;
	
	m_SkyScale = 5000.0;
	m_TerrainScale = 5000.0;
	m_GrassScale = 750.0;
	m_ParticlesScale = 4000.0;
}


//-----------------------------------------------------------------------------

void
GrassRendering::
load_mesh(const std::string& filenameObj, MeshType type)
{
	Vector3 bbmin, bbmax;
	double radius;
	Vector3 center;
	switch(type)
	{
		case SKY:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Sky);
			
			// calculate normals
			if(!m_Sky.hasNormals())
				m_Sky.calculateVertexNormals();
			
			
			m_Sky.calculateBoundingBox(bbmin, bbmax);
			
			radius = 0.5*(bbmin - bbmax).length();
			center = 0.5*(bbmin + bbmax);
			
			m_Sky.scaleObject(Vector3(m_SkyScale, m_SkyScale, m_SkyScale));
			radius *= 700.0;
			
			set_scene_pos(m_Sky.origin(), 5000.0);
			
			m_showTextureSky = m_Sky.hasUvTextureCoord();
			
			break;

		case TERRAIN:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Terrain);
			
			// calculate normals
			if(!m_Terrain.hasNormals())
				m_Terrain.calculateVertexNormals();
			
			m_Terrain.scaleObject(Vector3(m_TerrainScale, m_TerrainScale, m_TerrainScale));
			m_showTextureTerrain = m_Terrain.hasUvTextureCoord();
			break;
		case GRASS:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Grass);
			
			// calculate normals
			if(!m_Grass.hasNormals())
				m_Grass.calculateVertexNormals();
			
			m_Grass.scaleObject(Vector3(m_GrassScale, m_GrassScale, m_GrassScale));
			m_showTextureGrass = m_Grass.hasUvTextureCoord();
			break;
		case PARTICLES:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Particles);
			
			// calculate normals
			if(!m_Particles.hasNormals())
				m_Particles.calculateVertexNormals();
			
			m_Particles.scaleObject(Vector3(m_ParticlesScale, m_ParticlesScale, m_ParticlesScale));
			break;
		default:
			break;
	}
	
}

//-------------------------------------------------------------------------------

void
GrassRendering::
keyboard(int key, int x, int y)
{
	switch (key)
	{			
		case 'h':
			printf("Help:\n");
			printf("'h'\t-\thelp\n");
			printf("'t'\t-\ttoggle texture\n");
			printf("'arrow keys\t-\tchange speed of rotation\n");
			break;
		case 't':
			m_showTextureSky = !m_showTextureSky;
			if(!m_Sky.hasUvTextureCoord()) m_showTextureSky = false;
			
			m_showTextureTerrain = !m_showTextureTerrain;
			if(!m_Terrain.hasUvTextureCoord()) m_showTextureTerrain = false;

			m_showTextureGrass = !m_showTextureGrass;
			if(!m_Grass.hasUvTextureCoord()) m_showTextureGrass = false;
			break;
		case ' ':
			if(isWatchOn)
			{
				watch.stop();
				currentTime = 0.0;
			}
			else 
			{
				watch.start();
			}

			isWatchOn = !isWatchOn;
			break;
		default:
			TrackballViewer::special(key, x, y);
			break;
	}
	
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------


void
GrassRendering::
special(int key, int x, int y)
{
	switch (key)
	{			
		case GLUT_KEY_UP:
			daysPerMiliSecond += 0.001;
			if(daysPerMiliSecond > 0.1)
				daysPerMiliSecond = 0.1;
			break;
		case GLUT_KEY_DOWN:
			daysPerMiliSecond -= 0.001;
			if(daysPerMiliSecond < 0.001)
				daysPerMiliSecond = 0.001;
			break;
		default:
			TrackballViewer::special(key, x, y);
			break;
	}
	
	glutPostRedisplay();
}


//-----------------------------------------------------------------------------

void GrassRendering::idle()
{
	if(isWatchOn)
	{
		float prevTime = currentTime;
		currentTime = watch.stop();
		float daysElapsed = daysPerMiliSecond * (currentTime-prevTime);
		totalDaysElapsed += daysElapsed;
		
		//INSERT ANIMATION

		glutPostRedisplay();
	}
}

//-----------------------------------------------------------------------------

void 
GrassRendering::
draw_scene(DrawMode _draw_mode)
{	
	// clear screen
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	m_meshShaderTexture.bind(); 
	
	// set parameters
	m_meshShaderTexture.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderTexture.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	
	
	//sky
	glDisable(GL_DEPTH_TEST);
	m_Sky.setIdentity();
	m_Sky.scaleObject(Vector3(m_SkyScale, m_SkyScale, m_SkyScale));
	m_Sky.translateWorld(Vector3(m_camera.origin()));
	m_meshShaderTexture.setMatrix4x4Uniform("modelworld", m_Sky.getTransformation() );
	m_Sky.getMaterial(0).m_diffuseTexture.bind();
	m_meshShaderTexture.setIntUniform("texture", m_Sky.getMaterial(0).m_diffuseTexture.getLayer());
	draw_object(m_meshShaderTexture, m_Sky);
	glEnable(GL_DEPTH_TEST);	

	m_meshShaderTexture.unbind();
	
	//-------------------------------
	
	m_meshShaderDiffuse.bind();
	m_meshShaderDiffuse.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderDiffuse.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	m_meshShaderDiffuse.setMatrix3x3Uniform("worldcameraNormal", m_camera.getTransformation().Transpose());
	m_meshShaderDiffuse.setVector3Uniform("lightcolor", m_recSunlightInt, m_recSunlightInt, m_recSunlightInt);

	//terrain
	m_meshShaderDiffuse.setMatrix3x3Uniform("modelworldNormal", m_Terrain.getTransformation().Inverse().Transpose());
	draw_object(m_meshShaderDiffuse, m_Terrain, m_showTextureTerrain);
	m_meshShaderDiffuse.unbind();

	//grass
	m_meshShaderStencil.bind();
	m_meshShaderStencil.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderStencil.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	m_meshShaderStencil.setMatrix3x3Uniform("worldcameraNormal", m_camera.getTransformation().Transpose());
	m_meshShaderStencil.setVector3Uniform("lightcolor", m_recSunlightInt, m_recSunlightInt, m_recSunlightInt);

	for(int i = 0; i<m_Particles.getNumberOfVertices(); i++) {
		m_Grass.translateWorld(m_Particles.getVertexPosition(i)*m_ParticlesScale);
		draw_billboard(m_meshShaderStencil, m_Grass);
		m_Grass.translateWorld(-m_Particles.getVertexPosition(i)*m_ParticlesScale);
	}

	m_meshShaderStencil.unbind();
}

void GrassRendering::draw_billboard(Shader& sh, Mesh3D& mesh){
	Vector3 centerGrassToCamera = (m_camera.origin()- m_Grass.origin());
	Vector3 projection_on_y = Vector3(centerGrassToCamera.x, 0, centerGrassToCamera.z).normalize();
	Vector3 y_axis = Vector3(0, 1, 0);
	Vector3 z_axis = Vector3(0, 0, 1);
	float grassRotationAngle = acos(projection_on_y.dot(z_axis));
	if(m_camera.origin().x < 0) {
		grassRotationAngle = -grassRotationAngle;
	}

	m_Grass.rotateObject(y_axis, grassRotationAngle);
	m_meshShaderStencil.setMatrix3x3Uniform("modelworldNormal", m_Grass.getTransformation().Inverse().Transpose());
	draw_object(m_meshShaderStencil, m_Grass, m_showTextureGrass);
	m_Grass.rotateObject(y_axis, -grassRotationAngle);	
}


void GrassRendering::draw_object(Shader& sh, Mesh3D& mesh)
{
	sh.setMatrix4x4Uniform("modelworld", mesh.getTransformation() );
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer( 3, GL_DOUBLE, 0, mesh.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh.getNormalPointer() );
	glTexCoordPointer( 2, GL_DOUBLE, 0, mesh.getUvTextureCoordPointer() );
	
	for(unsigned int i = 0; i < mesh.getNumberOfParts(); i++)
	{
		glDrawElements( GL_TRIANGLES, mesh.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, mesh.getVertexIndicesPointer(i) );
	}
	cout<<"\n";
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

void GrassRendering::draw_object(Shader& sh, Mesh3D& mesh, bool showTexture)
{
	sh.setMatrix4x4Uniform("modelworld", mesh.getTransformation() );
	m_meshShaderStencil.setMatrix3x3Uniform("modelworldNormal", mesh.getTransformation().Inverse().Transpose());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if(showTexture)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
	glVertexPointer( 3, GL_DOUBLE, 0, mesh.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh.getNormalPointer() );
	if(showTexture)
		glTexCoordPointer( 2, GL_DOUBLE, 0, mesh.getUvTextureCoordPointer() );
			
	for(unsigned int i = 0; i < mesh.getNumberOfParts(); i++)
	{
		sh.setIntUniform("useTexture", showTexture && mesh.getMaterial(i).hasDiffuseTexture());
		sh.setVector3Uniform("diffuseColor", 
							 mesh.getMaterial(i).m_diffuseColor.x, 
							 mesh.getMaterial(i).m_diffuseColor.y, 
							 mesh.getMaterial(i).m_diffuseColor.z );
		sh.setFloatUniform("specularExp", mesh.getMaterial(i).m_specularExp);
		if(showTexture && mesh.getMaterial(i).hasDiffuseTexture())
		{
			mesh.getMaterial(i).m_diffuseTexture.bind();
			sh.setIntUniform("texture", mesh.getMaterial(i).m_diffuseTexture.getLayer());
			if(mesh.getMaterial(i).hasAlphaTexture()){
				mesh.getMaterial(i).m_alphaTexture.bind();
				sh.setIntUniform("alpha", mesh.getMaterial(i).m_alphaTexture.getLayer());
			}
		}
		glDrawElements( GL_TRIANGLES, mesh.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, mesh.getVertexIndicesPointer(i) );
		if(showTexture && mesh.getMaterial(i).hasDiffuseTexture())
		{
			mesh.getMaterial(i).m_diffuseTexture.unbind();
			mesh.getMaterial(i).m_alphaTexture.unbind();
		}
	}
			
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if(showTexture)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}


//=============================================================================
