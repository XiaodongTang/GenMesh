#ifndef OBJMESH_H
#define OBJMESH_H

#include "vec3.h"
#include <string.h>
#include <string>
#include <vector>
class ObjMeshException
{
public:
	ObjMeshException(const std::string & warn);
	ObjMeshException(const std::string & warn, const std::string & filename, unsigned int line);
	ObjMeshException(const std::string & warn, unsigned int num);
	std::string getWarn()  const {return warn;}
protected:
	std::string warn;
};
class ObjMesh
{
public:
	class Verticle
	{
	public:

		explicit Verticle(const unsigned int & posIndex_) : posIndex(posIndex_),normalIndex(std::make_pair(false,0)),textureIndex(std::make_pair(false,0)){}
		explicit Verticle(const unsigned int & posIndex_,const unsigned int & normalIndex_):posIndex(posIndex_),normalIndex(std::make_pair(true,normalIndex_)),textureIndex(std::make_pair(false,0)){}
		explicit Verticle(const unsigned int & posIndex_,const unsigned int & normalIndex_,const unsigned int & textureIndex_):posIndex(posIndex_),normalIndex(std::make_pair(true,normalIndex_)),textureIndex(std::make_pair(true,textureIndex_)){}
		
		inline bool hasNormalIndex() const { return normalIndex.first; }
		inline unsigned int getNorIndex()  {return normalIndex.second;}
		inline void setNorIndex(unsigned int norIndex_){normalIndex = std::make_pair(true,norIndex_);}

		inline bool hasTextureIndex() const {return textureIndex.first;}
		inline unsigned int getTextureIndex() const {return textureIndex.second;}
		inline void setTextureIndex(unsigned int texIndex_){textureIndex = std::make_pair(true,texIndex_);}

		inline unsigned int getPosIndex() const { return posIndex; }
		inline void setPosIndex(unsigned int posIndex_){posIndex = posIndex_;} 

		inline bool isEqual(const unsigned int & posIndex_)
		{
			if (posIndex == posIndex_) return true;
			else return false;
		}
	protected:
		unsigned int posIndex;
		std::pair< bool,unsigned int > normalIndex;
		std::pair< bool,unsigned int > textureIndex;
	};
	class Face
	{
	public:
		explicit Face():isExist(false){}
		explicit Face( Verticle &v1,  Verticle &v2, Verticle &v3):isExist(true)
		{
			verticles.reserve(3);
			verticles.push_back(v1);
			verticles.push_back(v2);
			verticles.push_back(v3);
		}
		//explicit Face( Verticle &v1, Verticle &v2, Verticle &v3, Face &f1, Face &f2, Face &f3):isExist(true)
		//{
		//	verticles.reserve(3);
		//	verticles.push_back(v1);
		//	verticles.push_back(v2);
		//	verticles.push_back(v3);
		//	facelist.reserve(3);
		//	facelist.push_back(f1);
		//	facelist.push_back(f2);
		//	facelist.push_back(f3);
		//}
		//
		inline void addVerticle( Verticle &v){verticles.push_back(v);}
		inline unsigned int getNumVerticles()  {return verticles.size();}
		inline Verticle getVerticle(unsigned int vIndex) const { return verticles[vIndex]; }

		//inline unsigned int getVerticlePos(unsigned int vIndex){return verticles[vIndex].getPosIndex();}
		//inline unsigned int getVerticleNor(unsigned int vIndex){return verticles[vIndex].getNorIndex();}
		//inline unsigned int getVerticleTex(unsigned int vIndex){return verticles[vIndex].getTextureIndex();}

		//inline void addFace( Face &f){facelist.push_back(f);}
		//inline Face getFace(unsigned int fIndex) { return facelist[fIndex]; }

		inline void addFaceIndex( int fIndex) {faceIndexList.push_back(fIndex);}
		inline unsigned int getFaceIndex(unsigned int fIndex){ return faceIndexList[fIndex];}

		inline bool isContainVerticle( Verticle &v1, Verticle &v2)
		{
			bool isv1 = false,isv2=false;
			for(unsigned int i=0;i < verticles.size();i++)
			{
				if(v1.getPosIndex() == verticles[i].getPosIndex())
				{
					isv1 = true;
				}
				if(v2.getPosIndex() == verticles[i].getPosIndex())
				{
					isv2 = true;
				}
			}
			return isv1 && isv2;
		}
		inline unsigned int ContainVerticle( Verticle &v1, Verticle &v2)
		{
			unsigned int isv1=0,isv2=0;
			for(unsigned int i=0;i< verticles.size();i++)
			{
				if(v1.getPosIndex() == verticles[i].getPosIndex())
				{
					isv1 = i;
				}
				if(v2.getPosIndex() == verticles[i].getPosIndex())
				{
					isv2 = i;
				}
			}
			return isv1+isv2;
		}

		inline void setExist(bool isExist_){isExist = isExist_;}
		inline bool getExist(){return isExist;}
		inline bool isEqurt(Face & face)
		{
			bool isEqurt_ = true;
			for(unsigned int i=0;i<verticles.size();i++)
			{
				if(verticles[i].getPosIndex() != face.verticles[i].getPosIndex())
					isEqurt_ = false;
			}
			return isEqurt_;
		}

	protected:
		std::vector<Verticle> verticles;
		//std::vector<Face> facelist;
		std::vector<int> faceIndexList;
		bool isExist;
	};

	explicit ObjMesh(const std::string & filename);
	explicit ObjMesh(){}

	inline unsigned int getNumFaces() const {return faces.size();}

	void setFilename(std::string &filename);
	std::string getFileName();

	bool readFile();
	bool readFile(const std::string & filename);

	void saveVerticle(const std::string &filename,std::vector<Vec3> vList);
	void saveVerticle(const std::string &filename);

	void saveFace(const std::string &filename);
	void saveFace(const std::string &filename, std::vector<Face> fList);

	bool saveObjFile(const std::string & filename);

	bool calculateNeightborFace();
	bool otherAlgorithmCalculateNeightborFace();

protected:
	std::vector< Vec3 > verticlePos;
	std::vector< Vec3 > verticleNormal;
	std::vector< Vec2 > verticleTexture;
	std::vector< Verticle > verticleList;
	std::vector< Face > faces;
	std::string m_filename;
};

#endif