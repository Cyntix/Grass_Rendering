//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================
//=============================================================================
//
//  CLASS MeshReader
//
//=============================================================================

#ifndef MESHREADER_HH
#define MESHREADER_HH

#include <fstream>
#include <sstream>
#include <locale>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include "../gl/Mesh3D.h"
#define STRING_DEFAULT "NONE_LGG"

#ifdef WIN32
  #define SSCANF sscanf_s
#else
  #define SSCANF sscanf
#endif

namespace
{
	class VertexMergingData
	{
	public:
		VertexMergingData() : indexPosition(0), indexNormal(0), indexUV(0) {}
		unsigned int indexPosition;
		unsigned int indexNormal;
		unsigned int indexUV;
	};
	class VertexMergingDataComparison
	{ 
	public:	
		bool operator()(const VertexMergingData& x, const VertexMergingData& y) const
		{ 
			if(x.indexPosition < y.indexPosition)
				return true;
			if(x.indexPosition == y.indexPosition && x.indexNormal < y.indexNormal)
				return true;
			if(x.indexPosition == y.indexPosition && x.indexNormal == y.indexNormal && x.indexUV < y.indexUV)
				return true;
			return false;
		} 
	};
}


class Mesh3DReader 
{

class MaterialManager
{
	public:
		typedef std::map<std::string, unsigned int> MeshMaterialMap;
		MeshMaterialMap materialsMap;
		std::vector<MeshMaterial *> materials;
};
public:
	//-----------------------------------------------------------------------------
	// Load the obj file as a unique mesh
	static bool read(const std::string& _filenameOBJ, Mesh3D& _mesh, const std::string& _filenameMTL = std::string())
	{
		MaterialManager materialManager;
		materialManager.materialsMap[STRING_DEFAULT] = 0;
		materialManager.materials.push_back(new MeshMaterial);

		return parseOBJFile(_filenameOBJ, _mesh, materialManager);
		//if(!_filenameMTL.empty())
		//	if(!parseMTLFile(_filenameMTL, materialManager)) return false;
		
	}
private:
	static bool parseMTLFile(const std::string& _filenameMTL, MaterialManager& _materialsManager)
	{
		std::ifstream myfile(_filenameMTL.c_str());
		assert(myfile.is_open());
		if(!myfile.is_open()) return false;
		std::string buffer;
		while(std::getline(myfile, buffer))
		{
			std::istringstream lineStream(buffer);
			std::string word;
			lineStream >> word;
			std::transform(word.begin(), word.end(), word.begin(), ::tolower );
			if(word == "newmtl")
			{
				std::string name;
				lineStream >> name;
				assert(_materialsManager.materialsMap.find(name) == _materialsManager.materialsMap.end());
				_materialsManager.materialsMap[name] = (unsigned int)_materialsManager.materials.size();
				_materialsManager.materials.push_back(new MeshMaterial);
			}
			else if(word == "kd")
			{
				Vector3 kd;
				lineStream >> kd.x;
				lineStream >> kd.y;
				lineStream >> kd.z;
				_materialsManager.materials.back()->m_diffuseColor = kd;
			}
			else if(word == "ns")
			{
				double ns;
				lineStream >> ns;
				_materialsManager.materials.back()->m_specularExp = ns;
			}
			else if(word == "map_kd")
			{
				std::string texturePath;
				lineStream >> texturePath;
				#ifdef WIN32
				texturePath = _filenameMTL.substr(0, _filenameMTL.find_last_of("\\")+1) + texturePath;
				#else
				texturePath = _filenameMTL.substr(0, _filenameMTL.find_last_of("/")+1) + texturePath;
				#endif
				_materialsManager.materials.back()->m_diffuseTexture.create(texturePath);
			}
			else if(word == "map_bump")
			{
				std::string texturePath;
				lineStream >> texturePath;
				#ifdef WIN32
					texturePath = _filenameMTL.substr(0, _filenameMTL.find_last_of("\\")+1) + texturePath;
				#else
					texturePath = _filenameMTL.substr(0, _filenameMTL.find_last_of("/")+1) + texturePath;
				#endif
				_materialsManager.materials.back()->m_bumpTexture.create(texturePath);
				_materialsManager.materials.back()->m_bumpTexture.setLayer(1);
			}
		}
		myfile.close();
		return true;
	}
	static bool parseOBJFile(const std::string& _filenameOBJ, Mesh3D& _mesh, MaterialManager& _materialManager)
	{
		std::ifstream myfile(_filenameOBJ.c_str());
		if(!myfile.is_open()) printf("Could not open file %s\n", _filenameOBJ.c_str());
		assert(myfile.is_open());
		if(!myfile.is_open()) return false;
		std::string buffer;
		string line, tmp;
		int id1, id2, id3, id4;
		int nId1, nId2, nId3, nId4;
		int tId1, tId2, tId3, tId4;
		float v1, v2, v3;
		int found1,found2;
		std::string _filenameMTL;
				
		std::vector<Vector3> positions;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;

		std::vector<bool> quads; 
		std::string currentMaterial = STRING_DEFAULT;
		std::vector<unsigned int> materials; 
		std::vector<unsigned int> positionIndices;
		std::vector<unsigned int> normalIndices;
		std::vector<unsigned int> uvIndices; 
		
		
		while(std::getline(myfile, buffer))
		{
			std::istringstream lineStream(buffer);
			std::string word;
			lineStream >> word;
			std::transform(word.begin(), word.end(), word.begin(), ::tolower);
			if(word == "mtllib")
			{
				lineStream >> _filenameMTL;
				#ifdef WIN32
					_filenameMTL = _filenameOBJ.substr(0, _filenameOBJ.find_last_of("\\")+1) + _filenameMTL;
				#else
					_filenameMTL = _filenameOBJ.substr(0, _filenameOBJ.find_last_of("/")+1) + _filenameMTL;
				#endif
				if(!parseMTLFile(_filenameMTL, _materialManager)) 
					return false;
			}
			else if(word == "usemtl")
			{
				lineStream >> currentMaterial;
				if( _materialManager.materialsMap.find(currentMaterial) ==  _materialManager.materialsMap.end() ) {
					printf("### Warning: Mesh3DReader::parseOBJFile - material %s unknown\n", currentMaterial.c_str());
					currentMaterial = STRING_DEFAULT;
				}
			}
			else if(word == "v")
			{
				//position
				SSCANF(buffer.c_str(), "%*s %f %f %f", &v1, &v2, &v3);
				positions.push_back(Vector3(v1,v2,v3));
			}
			else if(word == "vt")
			{
				//texture
				SSCANF(buffer.c_str(), "%*s %f %f", &v1, &v2);
				uvs.push_back(Vector2(v1,v2));
			}
			else if(word == "vn")
			{
				//normal
				SSCANF(buffer.c_str(), "%*s %f %f %f", &v1, &v2, &v3);
				normals.push_back(Vector3(v1,v2,v3));
			}
			else if (word == "f")
			{
				//face
				line = buffer;
				found1 = (int)line.find("/");
				bool quad = false;
				if(found1 == string::npos)
				{
					//only vertex positions
					int n = SSCANF(buffer.c_str(), "%*s %d %d %d %d", &id1, &id2, &id3, &id4);
					if(n == 4) quad = true;
				}
				else 
				{
					//get the part of the string until the second index
					tmp = line.substr(found1+1);
					found2 = (int)tmp.find(" ");
					tmp = tmp.substr(0,found2);
					found2 = (int)tmp.find("/");
					if(found2 == string::npos)
					{
						//vertex - texture
						int n = SSCANF(buffer.c_str(), "%*s %d/%d %d/%d %d/%d %d/%d", &id1, &tId1, &id2, &tId2, &id3, &tId3, &id4, &tId4);
						if(n == 8) quad = true;
						uvIndices.push_back(tId1-1);
						uvIndices.push_back(tId2-1);
						uvIndices.push_back(tId3-1);
						if(quad)
							uvIndices.push_back(tId4-1);
					}
					else 
					{
						tmp = line.substr(found1+1);
						found2 = (int)tmp.find("/");
						if(found2 == 0)
						{
							//vertex - normal
							int n = SSCANF(buffer.c_str(), "%*s %d//%d %d//%d %d//%d %d//%d", &id1, &nId1, &id2, &nId2, &id3, &nId3, &id4, &nId4);
							if(n == 8) quad = true;
						}
						else 
						{
							//vertex - texture - normal
							int n = SSCANF(buffer.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &id1, &tId1, &nId1, &id2, &tId2, &nId2, &id3, &tId3, &nId3, &id4, &tId4, &nId4);
							if(n == 12) quad = true;
							uvIndices.push_back(tId1-1);
							uvIndices.push_back(tId2-1);
							uvIndices.push_back(tId3-1);
							if(quad)
								uvIndices.push_back(tId4-1);
						}
						normalIndices.push_back(nId1-1);
						normalIndices.push_back(nId2-1);
						normalIndices.push_back(nId3-1);
						if(quad)
							normalIndices.push_back(nId4-1);
					}			
				}
				positionIndices.push_back(id1-1);
				positionIndices.push_back(id2-1);
				positionIndices.push_back(id3-1);
				if(quad)
					positionIndices.push_back(id4-1);
				quads.push_back(quad);
				materials.push_back(_materialManager.materialsMap.find(currentMaterial)->second);
			}
		}
		assert(!positionIndices.empty());
		assert(normalIndices.empty() || normalIndices.size() == positionIndices.size());
		assert(uvIndices.empty() || uvIndices.size() == positionIndices.size());
		myfile.close();
	
		std::vector<Vector3> positionsMesh;
		std::vector<Vector3> normalsMesh;
		std::vector<Vector2> uvsMesh;
		std::vector<unsigned int> indicesMesh(positionIndices.size());
		///////////////////////////////////////////////////////////////////
		// Merging

		typedef std::map<VertexMergingData, unsigned int, VertexMergingDataComparison> VertexMerginMap;
		VertexMerginMap vertexMergingMap;
		VertexMerginMap::iterator it;
		for(unsigned int i = 0; i < positionIndices.size(); ++i)
		{
			VertexMergingData vmd;
			vmd.indexPosition = positionIndices[i];
			if(!normalIndices.empty()) vmd.indexNormal = normalIndices[i];
			if(!uvIndices.empty()) vmd.indexUV = uvIndices[i];
			it = vertexMergingMap.find(vmd);
			if(it != vertexMergingMap.end())
			{
				indicesMesh[i] = it->second;
			}
			else
			{
				indicesMesh[i] = (unsigned int)positionsMesh.size();
				vertexMergingMap[vmd] = (unsigned int)positionsMesh.size();
				positionsMesh.push_back(positions[positionIndices[i]]);
				if(!normalIndices.empty()) normalsMesh.push_back(normals[normalIndices[i]]);
				if(!uvIndices.empty()) uvsMesh.push_back(uvs[uvIndices[i]]);
			}
		}
		assert(!positionsMesh.empty());
		assert(normalsMesh.empty() || normalsMesh.size() == positionsMesh.size());
		assert(uvsMesh.empty() || uvsMesh.size() == positionsMesh.size());

		_mesh.clear();
		_mesh.setVertexPositions(positionsMesh);
		_mesh.setVertexNormals(normalsMesh);
		_mesh.setVertexUV(uvsMesh);
		///////////////////////////////////////////////////////////////////
		std::vector<std::vector<unsigned int> >& indices = _mesh.getAllIndices();
		typedef std::map<unsigned int, unsigned int> MaterialPartMap;
		MaterialPartMap materialPartMap;
		MaterialPartMap::iterator itMaterialPart;
		//Triangulate quads if any : work also with non convex quad
		for(int i = 0, j = 0; i < (int)indicesMesh.size(); j++)
		{
			unsigned int materialIndex = materials[j];
			itMaterialPart = materialPartMap.find(materialIndex);
			unsigned int position;
			if(itMaterialPart == materialPartMap.end())
			{
				position = (unsigned int)indices.size();
				materialPartMap[materialIndex] = position;
				indices.push_back(std::vector<unsigned int>());
				_mesh.getAllMaterials().push_back(_materialManager.materials[materialIndex]);
			}
			else
			{
				position = itMaterialPart->second;
			}
			unsigned int i1 = indicesMesh[i];
			unsigned int i2 = indicesMesh[i+1];
			unsigned int i3 = indicesMesh[i+2];
			if(!quads[j])
			{
				indices[position].push_back(i1);
				indices[position].push_back(i2);
				indices[position].push_back(i3);
				i+=3;
			}
			else
			{
				Vector3 v1 = positionsMesh[i1];
				Vector3 v2 = positionsMesh[i2];
				Vector3 v3 = positionsMesh[i3];
				unsigned int i4 = indicesMesh[i+3];
				Vector3 v4 = positionsMesh[i4];

				Vector3 v13 = v3-v1;
				Vector3 v12 = v2-v1;
				Vector3 v14 = v4-v1;

				double a1 = v13.dot(v12);
				double a2 = v13.dot(v14);
				if((a1 >= 0 && a2 <= 0) || (a1 <= 0 && a2 >= 0))
				{
					indices[position].push_back(i1);
					indices[position].push_back(i2);
					indices[position].push_back(i3);
					indices[position].push_back(i1);
					indices[position].push_back(i3);
					indices[position].push_back(i4);
				}
				else
				{
					indices[position].push_back(i1);
					indices[position].push_back(i2);
					indices[position].push_back(i4);
					indices[position].push_back(i2);
					indices[position].push_back(i3);
					indices[position].push_back(i4);
				}
				i+=4;
			}
		}
		
		///////////////////////////////////////////////////////////////////
		return true;
		
	}
	//---------------------------------------------------------------------
};

//=============================================================================
#endif // MESHREADER_HH defined
//=============================================================================

