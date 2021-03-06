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
#define BUFFER_OFFSET(i) ((void*)(i))

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

	wind_angle = 0;
	wind_pattern_distance_x = 500;
	wind_pattern_distance_z = 1000;

	glGenBuffers(1, &vboGrass);
	glGenBuffers(1, &vboFlowers);

	// load mesh shader
	m_meshShaderDiffuse.create("diffuse.vs", "diffuse.fs");
	m_meshShaderTexture.create("tex.vs","tex.fs");
	m_meshShaderStencil.create("stencil.vs", "stencil.fs");

	m_showTextureSky = false;
	m_showGrass = true;
	m_showAlphaToCoverage = true;
	m_showTransparency = true;

	currentTime = 0.0;
	isWatchOn = false;
	
	timePerMiliSecond = 1 / 180.0;
	totalTimeElapsed = 0;

	sunDirection = Vector3(5, 5, 5);
	m_recSunlightInt = 1.0;
	
	m_SkyScale = 10000;
	m_TerrainScale = 10000;
	m_GrassScale = 150;
	m_PatternsScale = 1000;
	m_FlowerScale = 100;
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

			if(m_Terrain.getMaterial(0).m_density.getID()!=0){
				m_showDensity=true;
			}

			if(m_Terrain.getMaterial(0).m_color_variation.getID()!=0){
				m_show_colorVariation=true;
			}
			break;
		case GRASS:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Grass);
			
			// calculate normals
			if(!m_Grass.hasNormals())
				m_Grass.calculateVertexNormals();
			
			m_showTextureGrass = m_Grass.hasUvTextureCoord();
			break;
		case PARTICLE_PATTERN:{
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Pattern);
			
			// calculate normals
			if(!m_Pattern.hasNormals())
				m_Pattern.calculateVertexNormals();
			break;
		}
		case FLOWER:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Flower);
			
			// calculate normals
			if(!m_Flower.hasNormals())
				m_Flower.calculateVertexNormals();
			
			m_showTextureFlowers = m_Flower.hasUvTextureCoord();
			break;
		default:
			break;
	}	
}

float triangleArea(Vector2 p1, Vector2 p2, Vector2 p3) {
	return (float)1/2*abs((p1.x-p3.x)*(p2.y-p1.y) - (p1.x-p2.x)*(p3.y-p1.y));
}

void GrassRendering::load_particles(GLuint* vbo, vector<Vector3>* particles, Mesh3D* mesh, float scale, int* bufferSize){
	vector<Vector2> terrain_uv = m_Terrain.getUVs();
	vector<Vector2> terrain_uv_good_size;
	for(unsigned int i = 0; i < terrain_uv.size(); i++){ //SEMBLE OK!
		Vector2 vertex = terrain_uv.at(i);
		vertex.x = (vertex.x-0.5)*2*m_TerrainScale;
		vertex.y = (vertex.y-0.5)*2*m_TerrainScale;
		terrain_uv_good_size.push_back(vertex);
	}
	
	Vector3 starting_point = m_Terrain.origin();
	starting_point.x = starting_point.x-m_TerrainScale;
	starting_point.z = starting_point.z-m_TerrainScale;


	m_Pattern.scaleObject(Vector3(m_PatternsScale, m_PatternsScale, m_PatternsScale));
	m_Pattern.translateWorld(starting_point);
	int x_gone = 0;
	do{
		for(int i = 0; i<m_Pattern.getNumberOfVertices(); i++){
			Vector3 pointIn3d = m_Pattern.getTransformation() * m_Pattern.getVertexPosition(i);
			//1.Find the nearest 2D triangle on the terrain.
			Vector2 pointIn2d = Vector2(pointIn3d.x, pointIn3d.z);

			unsigned int indicePoints[3] = {-1, -1, -1};
			float minDistances[3] = {-1, -1, -1};
			for(unsigned int j = 0; j<terrain_uv_good_size.size(); j++){
				float distance = (pointIn2d-terrain_uv_good_size.at(j)).length();
				if(minDistances[0] == -1 || (distance<minDistances[0])){
						indicePoints[0] = j;
						minDistances[0] = distance;
				}
			}
			for(unsigned int j = 0; j<terrain_uv_good_size.size(); j++){
				float distance = (pointIn2d-terrain_uv_good_size.at(j)).length();
				if(minDistances[1] == -1 || distance<minDistances[1] && distance!=minDistances[0]){
					if(distance!=minDistances[0]){
						indicePoints[1] = j;
						minDistances[1] = distance;
					}
				}
			}
			for(unsigned int j = 0; j<terrain_uv_good_size.size(); j++){
				float distance = (pointIn2d-terrain_uv_good_size.at(j)).length();
				if(minDistances[2] == -1 || distance<minDistances[2] && distance!=minDistances[0] && distance!=minDistances[1]){
					if(distance!=minDistances[0] && distance!=minDistances[1]){
						indicePoints[2] = j;
						minDistances[2] = distance;
					}
				}
			}

			//2.Interpolate the point in 3D with the barycentric coordinates.
			Vector2 p1 = terrain_uv_good_size.at(indicePoints[0]);
			Vector2 p2 = terrain_uv_good_size.at(indicePoints[1]);
			Vector2 p3 = terrain_uv_good_size.at(indicePoints[2]);
			
			float triangle_area = triangleArea(p1, p2, p3);
			float s1_area = triangleArea(pointIn2d, p2, p3);
			float s2_area = triangleArea(p1, pointIn2d, p3);
			float s3_area = triangleArea(p1, p2, pointIn2d);

			float s1 = s1_area/triangle_area;
			float s2 = s2_area/triangle_area;
			float s3 = s3_area/triangle_area;

			if(s1+s2+s3<=1){
			Vector3 p1_3D = m_Terrain.getTransformation() * m_Terrain.getVertexPosition(indicePoints[0]);
			Vector3 p2_3D = m_Terrain.getTransformation() * m_Terrain.getVertexPosition(indicePoints[1]);
			Vector3 p3_3D = m_Terrain.getTransformation() * m_Terrain.getVertexPosition(indicePoints[2]);

			Vector3 interpolatedPoint = s1*p1_3D + s2*p2_3D + s3*p3_3D;
			(*particles).push_back(interpolatedPoint);
			}
		}
		if(m_Pattern.origin().x<=m_TerrainScale){			
			m_Pattern.translateWorld(Vector3(m_PatternsScale, 0, 0));
			x_gone += m_PatternsScale;
		}else {
			m_Pattern.translateWorld(Vector3(-x_gone, 0, m_PatternsScale));
			x_gone = 0;
		}
	}while(abs(m_Pattern.origin().z)<=m_TerrainScale);
	m_Pattern.setTransformation(Matrix4().loadIdentity());

	//calculates the min and max positions of the terrain (in x and z)
	float minX = m_Terrain.getVertexPosition(0).x;
	float maxX = minX;
	float minY = m_Terrain.getVertexPosition(0).y;
	float maxY = minY;
	float minZ = m_Terrain.getVertexPosition(0).z;
	float maxZ = minX;
	for(int i = 1; i < m_Terrain.getNumberOfVertices(); ++i){
		float xPos = m_Terrain.getVertexPosition(i).x;
		float yPos = m_Terrain.getVertexPosition(i).y;
		float zPos = m_Terrain.getVertexPosition(i).z;
		if(xPos < minX) {
			minX = xPos;
		} else if(xPos > maxX) {
			maxX = xPos;
		}
		if(yPos < minY) {
			minY = yPos;
		} else if(yPos > maxY) {
			maxY = yPos;
		}
		if(zPos < minZ) {
			minZ = zPos;
		} else if(zPos > maxZ) {
			maxZ = zPos;
		}
	}
	minX = minX * m_TerrainScale;
	maxX = maxX * m_TerrainScale;
	minY = minY * m_TerrainScale;
	maxY = maxY * m_TerrainScale;
	minZ = minZ * m_TerrainScale;
	maxZ = maxZ * m_TerrainScale;

	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, *vbo); //activation of the buffer
	*bufferSize = (*particles).size()*(2*6*3*sizeof(double) + 2*6*3*sizeof(double) + 2*6*2*sizeof(double) + 2*6*4*sizeof(double));
	glBufferData(GL_ARRAY_BUFFER, *bufferSize, NULL, GL_STREAM_DRAW); //allocation of memory We double the capacity for the texture coordinates
	
	unsigned int densityWidth = 0;
	unsigned int densityHeight = 0;
	GLubyte * densityData;
	if(m_showDensity){
		densityData = m_Terrain.getMaterial().m_density.getData();
		densityWidth = m_Terrain.getMaterial().m_density.getWidth();
		densityHeight = m_Terrain.getMaterial().m_density.getHeight();
	}

	unsigned int colorVariationWidth = 0;
	unsigned int colorVariationHeight = 0;
	GLubyte * colorVariationData;
	if(m_show_colorVariation){
		colorVariationData = m_Terrain.getMaterial().m_color_variation.getData();
		colorVariationWidth = m_Terrain.getMaterial().m_color_variation.getWidth();
		colorVariationHeight = m_Terrain.getMaterial().m_color_variation.getHeight();
	}

	for(int i = 0; i<(int)(*particles).size(); i++){

		//draws only if on the terrain
		double particleXpos = (*particles).at(i).x;
		double particleYpos = (*particles).at(i).y;
		double particleZpos = (*particles).at(i).z;

		if(particleXpos <= maxX && particleXpos >= minX && particleYpos <= maxY && particleYpos >= minY && particleZpos <= maxZ && particleZpos >= minZ){

			float densityColor = 1;
			float probability = drand48();
			//Compute density
			if(m_showDensity){
				unsigned int densityPixelXpos = (particleXpos+m_TerrainScale)/(2*m_TerrainScale)*densityWidth;
				unsigned int densityPixelYpos = densityHeight-(particleZpos+m_TerrainScale)/(2*m_TerrainScale)*densityHeight;
				if(densityPixelXpos < densityWidth && densityPixelYpos < densityHeight){
					densityColor = (float)densityData[(densityPixelXpos + (densityPixelYpos*densityWidth))*3]/256;
				}
			}
			(*mesh).translateWorld((*particles).at(i));

			if(probability  < densityColor){
			double data[2*6*3];
			double dataNormals[2*6*3];
			double dataCoords[2*6*2];

			//Compute color Variation
			float colorVariation = 1;
			if(m_show_colorVariation){
				unsigned int colorVariationPixelXpos = (particleXpos+m_TerrainScale)/(2*m_TerrainScale)*colorVariationWidth;
				unsigned int colorVariationPixelYpos = colorVariationHeight-(particleZpos+m_TerrainScale)/(2*m_TerrainScale)*colorVariationHeight;
				if(colorVariationPixelXpos < colorVariationWidth && colorVariationPixelYpos < colorVariationHeight){
					colorVariation = (double)(colorVariationData[(colorVariationPixelXpos + (colorVariationPixelYpos*colorVariationWidth))*3])/256; //used in the buffer directly
				}
			}

			float arbitraryAngle = drand48() * 2 * M_PI;
			float arbitrarySize = ((densityColor * drand48() * scale) + scale);
			(*mesh).rotateObject(Vector3(0,1,0), arbitraryAngle);
			(*mesh).scaleObject(Vector3(arbitrarySize, arbitrarySize, arbitrarySize));
			//Vertices
			data[0] = getVertex(mesh, 0, false).x;
			data[1] = getVertex(mesh, 0, false).y;
			data[2] = getVertex(mesh, 0, false).z;
			data[3] = getVertex(mesh, 2, false).x;
			data[4] = getVertex(mesh, 2, false).y;
			data[5] = getVertex(mesh, 2, false).z;
			data[6] = getVertex(mesh, 3, false).x;
			data[7] = getVertex(mesh, 3, false).y;
			data[8] = getVertex(mesh, 3, false).z;
			data[9] = getVertex(mesh, 2, false).x;
			data[10] = getVertex(mesh, 2, false).y;
			data[11] = getVertex(mesh, 2, false).z;
			data[12] = getVertex(mesh, 3, false).x;
			data[13] = getVertex(mesh, 3, false).y;
			data[14] = getVertex(mesh, 3, false).z;
			data[15] = getVertex(mesh, 1, false).x;
			data[16] = getVertex(mesh, 1, false).y;
			data[17] = getVertex(mesh, 1, false).z;

			data[18] = getVertex(mesh, 0, true).x;
			data[19] = getVertex(mesh, 0, true).y;
			data[20] = getVertex(mesh, 0, true).z;
			data[21] = getVertex(mesh, 2, true).x;
			data[22] = getVertex(mesh, 2, true).y;
			data[23] = getVertex(mesh, 2, true).z;
			data[24] = getVertex(mesh, 3, true).x;
			data[25] = getVertex(mesh, 3, true).y;
			data[26] = getVertex(mesh, 3, true).z;
			data[27] = getVertex(mesh, 2, true).x;
			data[28] = getVertex(mesh, 2, true).y;
			data[29] = getVertex(mesh, 2, true).z;
			data[30] = getVertex(mesh, 3, true).x;
			data[31] = getVertex(mesh, 3, true).y;
			data[32] = getVertex(mesh, 3, true).z;
			data[33] = getVertex(mesh, 1, true).x;
			data[34] = getVertex(mesh, 1, true).y;
			data[35] = getVertex(mesh, 1, true).z;

			//Normals
			int normalPointer = (*particles).size()*(2*6*3*sizeof(double));
			dataNormals[0] = getVertexNormal(mesh, 0, false).x;
			dataNormals[1] = getVertexNormal(mesh, 0, false).y;
			dataNormals[2] = getVertexNormal(mesh, 0, false).z;
			dataNormals[3] = getVertexNormal(mesh, 2, false).x;
			dataNormals[4] = getVertexNormal(mesh, 2, false).y;
			dataNormals[5] = getVertexNormal(mesh, 2, false).z;
			dataNormals[6] = getVertexNormal(mesh, 3, false).x;
			dataNormals[7] = getVertexNormal(mesh, 3, false).y;
			dataNormals[8] = getVertexNormal(mesh, 3, false).z;
			dataNormals[9] = getVertexNormal(mesh, 2, false).x;
			dataNormals[10] = getVertexNormal(mesh, 2, false).y;
			dataNormals[11] = getVertexNormal(mesh, 2, false).z;
			dataNormals[12] = getVertexNormal(mesh, 3, false).x;
			dataNormals[13] = getVertexNormal(mesh, 3, false).y;
			dataNormals[14] = getVertexNormal(mesh, 3, false).z;
			dataNormals[15] = getVertexNormal(mesh, 1, false).x;
			dataNormals[16] = getVertexNormal(mesh, 1, false).y;
			dataNormals[17] = getVertexNormal(mesh, 1, false).z;
			dataNormals[18] = getVertexNormal(mesh, 0, true).x;
			dataNormals[19] = getVertexNormal(mesh, 0, true).y;
			dataNormals[20] = getVertexNormal(mesh, 0, true).z;
			dataNormals[21] = getVertexNormal(mesh, 2, true).x;
			dataNormals[22] = getVertexNormal(mesh, 2, true).y;
			dataNormals[23] = getVertexNormal(mesh, 2, true).z;
			dataNormals[24] = getVertexNormal(mesh, 3, true).x;
			dataNormals[25] = getVertexNormal(mesh, 3, true).y;
			dataNormals[26] = getVertexNormal(mesh, 3, true).z;
			dataNormals[27] = getVertexNormal(mesh, 2, true).x;
			dataNormals[28] = getVertexNormal(mesh, 2, true).y;
			dataNormals[29] = getVertexNormal(mesh, 2, true).z;
			dataNormals[30] = getVertexNormal(mesh, 3, true).x;
			dataNormals[31] = getVertexNormal(mesh, 3, true).y;
			dataNormals[32] = getVertexNormal(mesh, 3, true).z;
			dataNormals[33] = getVertexNormal(mesh, 1, true).x;
			dataNormals[34] = getVertexNormal(mesh, 1, true).y;
			dataNormals[35] = getVertexNormal(mesh, 1, true).z;

			//TexCoords
			int texPointer = 2*normalPointer;
			dataCoords[0] = getTexCoord(mesh, 0).x;
			dataCoords[1] = getTexCoord(mesh, 0).y;
			dataCoords[2] = getTexCoord(mesh, 2).x;
			dataCoords[3] = getTexCoord(mesh, 2).y;
			dataCoords[4] = getTexCoord(mesh, 3).x;
			dataCoords[5] = getTexCoord(mesh, 3).y;
			dataCoords[6] = getTexCoord(mesh, 2).x;
			dataCoords[7] = getTexCoord(mesh, 2).y;
			dataCoords[8] = getTexCoord(mesh, 3).x;
			dataCoords[9] = getTexCoord(mesh, 3).y;
			dataCoords[10] = getTexCoord(mesh, 1).x;
			dataCoords[11] = getTexCoord(mesh, 1).y;

			dataCoords[12] = getTexCoord(mesh, 0).x;
			dataCoords[13] = getTexCoord(mesh, 0).y;
			dataCoords[14] = getTexCoord(mesh, 2).x;
			dataCoords[15] = getTexCoord(mesh, 2).y;
			dataCoords[16] = getTexCoord(mesh, 3).x;
			dataCoords[17] = getTexCoord(mesh, 3).y;
			dataCoords[18] = getTexCoord(mesh, 2).x;
			dataCoords[19] = getTexCoord(mesh, 2).y;
			dataCoords[20] = getTexCoord(mesh, 3).x;
			dataCoords[21] = getTexCoord(mesh, 3).y;
			dataCoords[22] = getTexCoord(mesh, 1).x;
			dataCoords[23] = getTexCoord(mesh, 1).y;

			int colorPointer = texPointer + (*particles).size()*(2*6*2*sizeof(double));
			//Note: there should be one color per vertex...
			double colorData[2*6*4];
			for(int k=0; k<2*6; k++){
				colorData[k*4] = colorVariation;
				colorData[k*4 + 1] = colorVariation;
				colorData[k*4 + 2] = colorVariation;
				colorData[k*4 + 3] = densityColor;
			}

			glBufferSubData(GL_ARRAY_BUFFER, i*2*6*3*sizeof(double), 2*6*3*sizeof(double), data);
			glBufferSubData(GL_ARRAY_BUFFER, normalPointer + i*2*6*3*sizeof(double), 2*6*3*sizeof(double), dataNormals);
			glBufferSubData(GL_ARRAY_BUFFER, texPointer+i*2*6*2*sizeof(double), 2*6*2*sizeof(double), dataCoords);
			glBufferSubData(GL_ARRAY_BUFFER, colorPointer+i*2*6*4*sizeof(double), 2*6*4*sizeof(double), colorData);
			}
			(*mesh).setTransformation(Matrix4().loadIdentity());
			}
		}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GrassRendering::load_grass(){
	cout<<"Generation of grass...\n";
	load_particles(&vboGrass, &grass_particles, &m_Grass, m_GrassScale, &vboGrassSize);
	cout<<"\n\n";
}

void GrassRendering::load_flowers(){
	cout<<"Generation of flowers...\n";
	m_PatternsScale *=5;
	load_particles(&vboFlowers, &flowers_particles, &m_Flower, m_FlowerScale, &vboFlowersSize);
	cout<<"\n\n";
}

Vector3 GrassRendering::getVertex(Mesh3D* mesh, int i, boolean cross){
	Vector3 vertexObject = (*mesh).getVertexPosition(i);
	if(cross){
		(*mesh).rotateObject(Vector3(0, 1, 0), M_PI/2);
	}
	Matrix4 trans = (*mesh).getTransformation();
	if(cross){
		(*mesh).rotateObject(Vector3(0, 1, 0), -M_PI/2);
	}
	return trans*vertexObject;
}

Vector3 GrassRendering::getVertexNormal(Mesh3D* mesh, int i, boolean cross){
	Vector3 vertexNormal = (*mesh).getVertexNormal(i);
	if(cross){
		(*mesh).rotateObject(Vector3(0, 1, 0), M_PI/2);
	}
	Matrix4 trans = (*mesh).getTransformation();
	if(cross){
		(*mesh).rotateObject(Vector3(0, 1, 0), -M_PI/2);
	}
	return trans*vertexNormal;
}

Vector2 GrassRendering::getTexCoord(Mesh3D* mesh, int i){
	return (*mesh).getVertexUVs(i);
}



//-------------------------------------------------------------------------------

void
GrassRendering::
keyboard(int key, int x, int y)
{
	switch (key)
	{			
		case 't':
			m_showTextureSky = !m_showTextureSky;
			if(!m_Sky.hasUvTextureCoord()) m_showTextureSky = false;
			
			m_showTextureTerrain = !m_showTextureTerrain;
			if(!m_Terrain.hasUvTextureCoord()) m_showTextureTerrain = false;

			m_showTextureGrass = !m_showTextureGrass;
			if(!m_Grass.hasUvTextureCoord()) m_showTextureGrass = false;

			m_showTextureFlowers = !m_showTextureFlowers;
			if(!m_Grass.hasUvTextureCoord()) m_showTextureFlowers = false;
			break;
		case 'g':
			m_showGrass = !m_showGrass;
			break;
		case 'z':
			m_showTransparency = !m_showTransparency;
			break;
		case 'a':
			m_showAlphaToCoverage = !m_showAlphaToCoverage;
			break;
		case 'c':
			if(m_Terrain.getMaterial(0).m_color_variation.getID()!=0){
				m_show_colorVariation = !m_show_colorVariation;
			}
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
		case 'h':
			cout<<"HELP:\nPress 'g' to enable or disable the grass presence\nPress 't' to enable or disable the use of textures"
				<<"\nPress 'z' to enable or disable the transparency\nPress 'a' to enable or disable the alpha to coverage"
				<<"\nPress 'c' to enable or disable the color variation\nPress 'h' to show the help once again"
				<<"\nPress the space bar to enable or disable the wind simulation\nPress up to speed up the wind simulation"
				<<"\nPress down to slow down the wind simulation";
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
			timePerMiliSecond += 0.001;
			if(timePerMiliSecond > 0.1)
				timePerMiliSecond = 0.1;
			break;
		case GLUT_KEY_DOWN:
			timePerMiliSecond -= 0.001;
			if(timePerMiliSecond < 0.001)
				timePerMiliSecond = 0.001;
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
		float timeElapsed = timePerMiliSecond * (currentTime-prevTime);
		totalTimeElapsed += timeElapsed/2;
		
		//INSERT ANIMATION
		wind_angle += timeElapsed/5;
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
	
	draw_sky();
	
	draw_terrain(m_showTextureTerrain);

	draw_grass();
	draw_flowers();
}

void GrassRendering::draw_grass(){
	draw_buffer(m_meshShaderStencil, vboGrass, &m_Grass, &grass_particles, m_showTextureGrass, &vboGrassSize, m_GrassScale);
}

void GrassRendering::draw_flowers(){
	draw_buffer(m_meshShaderStencil, vboFlowers, &m_Flower, &flowers_particles , m_showTextureFlowers, &vboFlowersSize, m_FlowerScale);
}

void GrassRendering::draw_buffer(Shader& sh, GLuint vbo, Mesh3D* mesh, vector<Vector3>* particles, boolean showTexture, int* bufferSize, double billboardScale){
	m_meshShaderStencil.bind();

	m_meshShaderStencil.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderStencil.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	m_meshShaderStencil.setMatrix3x3Uniform("worldcameraNormal", m_camera.getTransformation().Transpose());
	m_meshShaderStencil.setVector3Uniform("lightcolor", m_recSunlightInt, m_recSunlightInt, m_recSunlightInt);
	sh.setMatrix4x4Uniform("modelworld", (*mesh).getTransformation());	
	m_meshShaderStencil.setMatrix3x3Uniform("modelworldNormal", (*mesh).getTransformation().Inverse().Transpose());
	m_meshShaderStencil.setVector3Uniform("sunDirection", sunDirection.x, sunDirection.y, sunDirection.z);
	sh.setIntUniform("useTransparency", m_showTransparency);
	sh.setIntUniform("useAlphaToCoverage", m_showAlphaToCoverage);
	sh.setFloatUniform("wind_angle", wind_angle);
	sh.setFloatUniform("wind_pattern_distance_x", wind_pattern_distance_x);
	sh.setFloatUniform("wind_pattern_distance_z", wind_pattern_distance_z);
	sh.setFloatUniform("billboard_scale", billboardScale);

	if(m_showGrass){
		if(m_showTransparency && m_showAlphaToCoverage){
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		if(showTexture){
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if(m_show_colorVariation){
			glEnableClientState(GL_COLOR_ARRAY);
			sh.setIntUniform("useColorVariation", 1);
		}else{
			sh.setIntUniform("useColorVariation", 0);
		}
	
		glVertexPointer(3, GL_DOUBLE, 0, BUFFER_OFFSET(0));
		glNormalPointer(GL_DOUBLE, 0, BUFFER_OFFSET((*particles).size()*(2*6*3*sizeof(double))));
		if(showTexture){
			glTexCoordPointer(2, GL_DOUBLE, 0, BUFFER_OFFSET(2*(*particles).size()*(2*6*3*sizeof(double))));
		}
		glColorPointer(4, GL_DOUBLE, 0, BUFFER_OFFSET((*particles).size()*(2*(2*6*3*sizeof(double)) + 2*6*2*sizeof(double))));

		sh.setIntUniform("useTexture", showTexture && (*mesh).getMaterial().hasDiffuseTexture());
		sh.setVector3Uniform("diffuseColor", 
							(*mesh).getMaterial().m_diffuseColor.x, 
							(*mesh).getMaterial().m_diffuseColor.y, 
							(*mesh).getMaterial().m_diffuseColor.z );
		sh.setFloatUniform("specularExp", (*mesh).getMaterial().m_specularExp);

		if(showTexture && (*mesh).getMaterial().hasDiffuseTexture())
			{
				(*mesh).getMaterial().m_diffuseTexture.bind();
				sh.setIntUniform("texture", (*mesh).getMaterial().m_diffuseTexture.getLayer());
				if((*mesh).getMaterial().hasAlphaTexture()){
					(*mesh).getMaterial().m_alphaTexture.bind();
					sh.setIntUniform("alpha", (*mesh).getMaterial().m_alphaTexture.getLayer());
				}
			}
		
		glDrawArrays(GL_TRIANGLES, 0, (*particles).size()*2*6);
			cout<<"\n";

			if(showTexture && (*mesh).getMaterial().hasDiffuseTexture())
			{
				(*mesh).getMaterial().m_diffuseTexture.unbind();
				(*mesh).getMaterial().m_alphaTexture.unbind();
			}
	
		if(m_show_colorVariation){
			glDisableClientState(GL_COLOR_ARRAY);
		}
		if(showTexture){
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		if(m_showTransparency && m_showAlphaToCoverage){
			glDisable(GL_BLEND);
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
		}
	}

	m_meshShaderStencil.unbind();
}

void GrassRendering::draw_sky()
{
	m_meshShaderTexture.bind();
	glDisable(GL_DEPTH_TEST);

	m_Sky.setIdentity();
	m_Sky.scaleObject(Vector3(m_SkyScale, m_SkyScale, m_SkyScale));
	m_Sky.translateWorld(Vector3(m_camera.origin()));
	m_Sky.getMaterial(0).m_diffuseTexture.bind();
	m_meshShaderTexture.setMatrix4x4Uniform("modelworld", m_Sky.getTransformation() );
	m_meshShaderTexture.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderTexture.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	m_meshShaderTexture.setIntUniform("texture", m_Sky.getMaterial(0).m_diffuseTexture.getLayer());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer( 3, GL_DOUBLE, 0, m_Sky.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, m_Sky.getNormalPointer() );
	glTexCoordPointer( 2, GL_DOUBLE, 0, m_Sky.getUvTextureCoordPointer() );
	
	for(unsigned int i = 0; i < m_Sky.getNumberOfParts(); i++)
	{
		glDrawElements( GL_TRIANGLES, m_Sky.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, m_Sky.getVertexIndicesPointer(i) );
	}
	cout<<"\n";
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_DEPTH_TEST);	
	m_meshShaderTexture.unbind();	
}

void GrassRendering::draw_terrain(bool showTexture)
{
	m_meshShaderDiffuse.bind();

	m_meshShaderDiffuse.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	m_meshShaderDiffuse.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderDiffuse.setMatrix3x3Uniform("worldcameraNormal", m_camera.getTransformation().Transpose());	
	m_meshShaderStencil.setVector3Uniform("sunDirection", sunDirection.x, sunDirection.y, sunDirection.z);
	m_meshShaderDiffuse.setVector3Uniform("lightcolor", m_recSunlightInt, m_recSunlightInt, m_recSunlightInt);
	m_meshShaderDiffuse.setMatrix4x4Uniform("modelworld", m_Terrain.getTransformation());
	m_meshShaderDiffuse.setMatrix3x3Uniform("modelworldNormal", m_Terrain.getTransformation().Inverse().Transpose());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if(showTexture)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
	glVertexPointer( 3, GL_DOUBLE, 0, m_Terrain.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, m_Terrain.getNormalPointer() );
	if(showTexture)
		glTexCoordPointer( 2, GL_DOUBLE, 0, m_Terrain.getUvTextureCoordPointer() );
			
	for(unsigned int i = 0; i < m_Terrain.getNumberOfParts(); i++)
	{
		m_meshShaderDiffuse.setIntUniform("useTexture", showTexture && m_Terrain.getMaterial(i).hasDiffuseTexture());
		m_meshShaderDiffuse.setVector3Uniform("diffuseColor", 
							 m_Terrain.getMaterial(i).m_diffuseColor.x, 
							 m_Terrain.getMaterial(i).m_diffuseColor.y, 
							 m_Terrain.getMaterial(i).m_diffuseColor.z );
		if(showTexture && m_Terrain.getMaterial(i).hasDiffuseTexture())
		{
			m_Terrain.getMaterial(i).m_diffuseTexture.bind();
			m_meshShaderDiffuse.setIntUniform("texture", m_Terrain.getMaterial(i).m_diffuseTexture.getLayer());
			if(m_Terrain.getMaterial(i).hasNormalMapTexture()){
				m_Terrain.getMaterial(i).m_normal_mapTexture.bind();
				m_meshShaderDiffuse.setIntUniform("normal_map", m_Terrain.getMaterial(i).m_normal_mapTexture.getLayer());
			}
		}
		glDrawElements( GL_TRIANGLES, m_Terrain.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, m_Terrain.getVertexIndicesPointer(i) );
		if(showTexture && m_Terrain.getMaterial(i).hasDiffuseTexture())
		{
			m_Terrain.getMaterial(i).m_diffuseTexture.unbind();
			m_Terrain.getMaterial(i).m_alphaTexture.unbind();
		}
	}
			
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if(showTexture)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	m_meshShaderDiffuse.unbind();
}


//=============================================================================
