#ifndef TETMESH_H
#define TETMESH_H
#include "objmesh.h"
#include <string.h>
#include <string>
#include <vector>
#include <set>
class TetMesh : public ObjMesh
{
public:
	friend class Verticle;

	class Element
	{
	public:
		explicit Element(){}
		explicit Element( Verticle &v1,  Verticle &v2, Verticle &v3, Verticle &v4)
		{
			verticles.reserve(4);
			verticles.push_back(v1);
			verticles.push_back(v2);
			verticles.push_back(v3);
			verticles.push_back(v4);
		}
		inline void addVerticle( Verticle &v){verticles.push_back(v);}
		inline unsigned int getNumVerticles()  {return this->verticles.size();}
		inline Verticle getVerticle(unsigned int vIndex) { return verticles[vIndex];}
	protected:
		std::vector<Verticle> verticles;
	};
	explicit TetMesh(const std::string & filename);
	explicit TetMesh():ObjMesh(){}

	void saveGenVegFile(const std::string &filename);
	void saveGenElement(const std::string &filename);
	void saveGenVerticle(const std::string &filename);
	void saveGenFace(const std::string &filename);
	void saveGenSurfaceFile(const std::string &filename);

	Vec3 calculateFaceCore( Face &face);
	Vec3 calculateFaceNormal( Face &face);

	Vec3 movingVerticle(unsigned int index);
	Vec3 movingNormal(unsigned int index);
	Vec3 movingNormal(Vec3 n);
	Vec3 movingFaceCore(Vec3 v,Vec3 n);

	bool createVerticle();
	bool createElement();
	bool createFace();
	inline void setScalar(double scalar_){scalar = scalar_;}
protected:
	double scalar;
	std::vector<Vec3> newPos;
	std::vector<Vec3> newNor;
	std::vector<Vec2> newTex;
	std::vector<Verticle> newVer;
	std::vector<Face> newFac;
	std::set<Face> newFaceList;
	std::vector<Element> newEle;
};

#endif