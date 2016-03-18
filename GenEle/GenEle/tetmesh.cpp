#include "tetmesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
#include <string>

TetMesh::TetMesh(const std::string & filename_):ObjMesh(filename_)
{
}

void TetMesh::saveGenVegFile(const std::string &filename)
{
	std::string m_VegFile = filename;
	m_VegFile.append(".veg");
	std::ofstream outf(m_VegFile);
	if(!outf)
	{
		std::string message = "can't open this file";
		message.append(m_VegFile);
		throw ObjMeshException(message);
	}
	outf<<"*VERTICES\n";
	outf<<"*INCLUDE "<<filename<<".node\n";
	outf<<"*ELEMENTS\n";
	outf<<"TETS\n";
	outf<<"*INCLUDE "<<filename<<".ele\n";
	outf<<"*MATERIAL BODY\n";
	outf<<"ENU, 500, 1000, 0.25\n";
	outf<<"*REGION\n";
	outf<<"allElements, BODY\n";
}

void TetMesh::saveGenElement(const std::string &filename)
{
	std::string m_VerFile = filename;
	m_VerFile.append(".ele");
	std::ofstream outf(m_VerFile);
	if(!outf)
	{
		std::string message = "can't open this file ";
		message.append(m_VerFile);
		throw ObjMeshException(message);
	}
	int newEleSize  = newEle.size();
	outf<<newEleSize<<" 4 0 0\n";
	for( int i=0; i<newEleSize; i++ )
	{
		outf<<" "<<i+1<<" "<<newEle[i].getVerticle(0).getPosIndex()+1<<" "<<newEle[i].getVerticle(1).getPosIndex()+1<<" "<<newEle[i].getVerticle(2).getPosIndex()+1<<" "<<newEle[i].getVerticle(3).getPosIndex()+1<<"\n";
	}
	outf.close();
}

void TetMesh::saveGenVerticle(const std::string &filename)
{
	this->saveVerticle(filename,newPos);
}

void TetMesh::saveGenFace(const std::string &filename)
{
	this->saveFace(filename,newFac);
}

void TetMesh::saveGenSurfaceFile(const std::string &filename)
{
	std::string m_VerFile = filename;
	m_VerFile.append(".obj");
	std::ofstream outf(m_VerFile);
	if(!outf)
	{
		std::string message = "can't open this file ";
		message.append(m_VerFile);
		throw ObjMeshException(message);
	}
	const int newPosSize  = newPos.size();
	const int newNorSize  = newNor.size();
	const int newTexSize  = newTex.size();
	
	for( int i=0; i<newPosSize; i++ )
	{
		outf<<"v "<<newPos[i].getLoc(0)<<" "<<newPos[i].getLoc(1)<<" "<<newPos[i].getLoc(2)<<"\n";
	}
	for( int i=0; i<newNorSize; i++)
	{
		outf<<"vn "<<newNor[i].getLoc(0)<<" "<<newNor[i].getLoc(1)<<" "<<newNor[i].getLoc(2)<<"\n";
	}
	if(newTexSize != 0)
	{
		for( int i=0; i<newTexSize; i++)
		{
			outf<<"vt "<<newTex[i].getLoc(0)<<" "<<newTex[i].getLoc(1)<<"\n";
		}
	}

	unsigned int newFaceListSize = newFaceList.size();
	std::set<Face>::iterator newFaceListIter = newFaceList.begin();
	if(newFaceListIter->getVerticle(0).hasTextureIndex())
	{
		while(newFaceListIter != newFaceList.end())
		{
			outf<<"f "<<newFaceListIter->getVerticle(0).getPosIndex()<<"/"<<newFaceListIter->getVerticle(0).getTextureIndex()<<"/"<<newFaceListIter->getVerticle(0).getNorIndex();
			outf<<" "<<newFaceListIter->getVerticle(1).getPosIndex()<<"/"<<newFaceListIter->getVerticle(1).getTextureIndex()<<"/"<<newFaceListIter->getVerticle(1).getNorIndex();
			outf<<" "<<newFaceListIter->getVerticle(2).getPosIndex()<<"/"<<newFaceListIter->getVerticle(2).getTextureIndex()<<"/"<<newFaceListIter->getVerticle(2).getNorIndex()<<"\n";
			newFaceListIter++;
		}
	}
	else
	{
		while(newFaceListIter != newFaceList.end())
		{
			outf<<"f "<<newFaceListIter->getVerticle(0).getPosIndex()<<"//"<<newFaceListIter->getVerticle(0).getNorIndex();
			outf<<" "<<newFaceListIter->getVerticle(1).getPosIndex()<<"//"<<newFaceListIter->getVerticle(1).getNorIndex();
			outf<<" "<<newFaceListIter->getVerticle(2).getPosIndex()<<"//"<<newFaceListIter->getVerticle(2).getNorIndex()<<"\n";
			newFaceListIter++;
		}
	}
	
	outf.close();
}

Vec3 TetMesh::calculateFaceCore( Face &face)
{
	Vec3 sum = Vec3(0,0,0);
	int numVer = face.getNumVerticles();
	for (int i=0;i<numVer;i++)
		sum += this->verticlePos[face.getVerticle(i).getPosIndex()-1];
	sum /=(double)numVer;
	return sum;
}

Vec3 TetMesh::calculateFaceNormal( Face &face)
{
	Vec3 edge1 = this->verticlePos[face.getVerticle(0).getPosIndex()-1];
	edge1 = edge1 - this->verticlePos[face.getVerticle(1).getPosIndex()-1];
	Vec3 edge2 = this->verticlePos[face.getVerticle(0).getPosIndex()-1];
	edge2 = edge2 - this->verticlePos[face.getVerticle(2).getPosIndex()-1];
	Vec3 normal = cross(edge1,edge2);
	normal = normalize(normal);

	if(dot(normal,this->verticleNormal[face.getVerticle(0).getNorIndex()-1] ) >0.0
		|| dot(normal,this->verticleNormal[face.getVerticle(1).getNorIndex()-1]) >0.0
		|| dot(normal,this->verticleNormal[face.getVerticle(2).getNorIndex()-1]) >0.0 )
	{
		if(dot(normal,this->verticleNormal[face.getVerticle(0).getNorIndex()-1]) >0.0
			&& dot(normal,this->verticleNormal[face.getVerticle(1).getNorIndex()-1]) >0.0
			&& dot(normal,this->verticleNormal[face.getVerticle(2).getNorIndex()-1]) >0.0 )
			normal *= 1;
	}
	else
	{
		normal *= (-1);
	}
	return normal;
}

bool TetMesh::createVerticle()
{
	const  int verticlePosSize = verticlePos.size();
	const  int verticleNormalSize = verticleNormal.size();
	const  int verticleTextureSize = verticleTexture.size();
	const  int facesSize = faces.size();
	unsigned int testSize = 0;
	if(facesSize == 0 || verticlePosSize == 0 || verticleNormalSize == 0)
	{
		std::string message = "face,verticlePos or verticleNor is emplily!";
		throw ObjMeshException(message);
		return false;
	}
	for(int i=0; i<verticlePosSize; i++)
	{
		Vec3 m_Ver = verticlePos[i];
		Vec3 m_Nor = verticleNormal[i];
		newPos.push_back(m_Ver);
		newNor.push_back(m_Nor);
		if(verticleTextureSize != 0)
		{
			Vec2 m_Tex = verticleTexture[i];
			newTex.push_back(m_Tex);
			newVer.push_back(Verticle(testSize,testSize,testSize));
		}
		else
			newVer.push_back(Verticle(testSize,testSize));
		testSize++;
	}
	for(int i=0; i<verticlePosSize; i++)
	{
		Vec3 m_Ver = movingVerticle(i);
		newPos.push_back(m_Ver);
		Vec3 m_Nor = movingNormal(i);
		newNor.push_back(m_Nor);
		if(verticleTextureSize != 0)
		{
			Vec2 m_Tex = Vec2(0.0,0.0);
			newTex.push_back(m_Tex);
			newVer.push_back(Verticle(testSize,testSize,testSize));
		}
		else
			newVer.push_back(Verticle(testSize,testSize));
		testSize++;
	}
	for(int i=0;i<facesSize;i++)
	{
		if(faces[i].getNumVerticles() == 0)
		{
			std::string message = "this face have't points";
			throw ObjMeshException(message,i);
			return false;
		}
		Vec3 m_Nor = calculateFaceNormal(faces[i]);
		Vec3 m_Ver = movingFaceCore(calculateFaceCore(faces[i]),m_Nor);
		m_Nor = movingNormal(m_Nor);
		newPos.push_back(m_Ver);
		newNor.push_back(m_Nor);
		if(verticleTextureSize != 0)
		{
			Vec2 m_Tex = Vec2(0.0,0.0);
			newTex.push_back(m_Tex);
			newVer.push_back(Verticle(testSize,testSize,testSize));
		}
		else
			newVer.push_back(Verticle(testSize,testSize));
		testSize++;
	}
	if(newVer.size() != (verticlePosSize+verticleNormalSize+facesSize))
	{
		std::string message = "the new producted point has a wrong numble";
		throw ObjMeshException(message);
		return false;
	}
	return true;
}

bool TetMesh::createElement()
{
	const  int facesSize = faces.size();
	const  int verticlePosSize = verticlePos.size();
	if(facesSize == 0 )
	{
		std::string message = "face lists is emplily!";
		throw ObjMeshException(message);
		return false;
	}
	for (int i=0; i<facesSize; i++)
	{
		int A=-1,B=-1,C=-1,M=-1,M1=-1,M2=-1,M3=-1,A1=-1,B1=-1,C1=-1;
		if (!faces[i].getExist())
		{
			std::string message = " face is't exist";
			throw ObjMeshException(message,i);
			return false;
		}
		//init point index
		A = faces[i].getVerticle(0).getPosIndex()-1;
		A1 = verticlePosSize + A;
		B = faces[i].getVerticle(1).getPosIndex()-1;
		B1 = verticlePosSize + B;
		C = faces[i].getVerticle(2).getPosIndex()-1;
		C1 = verticlePosSize + C;
		M = verticlePosSize*2 + i ;
		M1 = verticlePosSize*2 + faces[i].getFaceIndex(0) ;
		M2 = verticlePosSize*2 + faces[i].getFaceIndex(1) ;
		M3 = verticlePosSize*2 + faces[i].getFaceIndex(2) ;
		// generate element 
		if(A>B)
		{
			if(B>C)
				newEle.push_back(Element(newVer[C],newVer[B],newVer[A],newVer[M]));
			else if(C>A)
				newEle.push_back(Element(newVer[B],newVer[A],newVer[C],newVer[M]));
			else 
				newEle.push_back(Element(newVer[B],newVer[C],newVer[A],newVer[M]));
		}
		else
		{
			if(A>C)
				newEle.push_back(Element(newVer[C],newVer[A],newVer[B],newVer[M]));
			else if(C>B)
				newEle.push_back(Element(newVer[A],newVer[B],newVer[C],newVer[M]));
			else
				newEle.push_back(Element(newVer[A],newVer[C],newVer[B],newVer[M]));
		}
		int a = faces[i].getFaceIndex(0) - i;
		if( a > 0 )
		{
			if(A>B)
				newEle.push_back(Element(newVer[B],newVer[A],newVer[M],newVer[M1]));
			else
				newEle.push_back(Element(newVer[A],newVer[B],newVer[M],newVer[M1]));
			newEle.push_back(Element(newVer[A],newVer[A1],newVer[M],newVer[M1]));
			newEle.push_back(Element(newVer[B],newVer[B1],newVer[M],newVer[M1]));
		}
		a = faces[i].getFaceIndex(1) - i;
		if(a >0 )
		{
			if(A>C)
				newEle.push_back(Element(newVer[C],newVer[A],newVer[M],newVer[M2]));
			else
				newEle.push_back(Element(newVer[A],newVer[C],newVer[M],newVer[M2]));
			newEle.push_back(Element(newVer[A],newVer[A1],newVer[M],newVer[M2]));
			newEle.push_back(Element(newVer[C],newVer[C1],newVer[M],newVer[M2]));
		}
		a = faces[i].getFaceIndex(2) - i;
		if(a > 0)
		{
			if(B>C)
				newEle.push_back(Element(newVer[C],newVer[B],newVer[M],newVer[M3]));
			else
				newEle.push_back(Element(newVer[B],newVer[C],newVer[M],newVer[M3]));
			newEle.push_back(Element(newVer[B],newVer[B1],newVer[M],newVer[M3]));
			newEle.push_back(Element(newVer[C],newVer[C1],newVer[M],newVer[M3]));
		}
	}
	return true;
}

bool TetMesh::createFace()
{
	const int newEleSize = newEle.size();
	for(int i=0;i<newEleSize;i++)
	{
		Face *face;
		//problem
		//#define JUDCE_FACE(q0,q1,q2) \
		//face = new Face(newEle[i].getVerticle(q0),newEle[i].getVerticle(q1),newEle[i].getVerticle(q2));\
		//if(newFaceList.find(*face) != newFaceList.end())\
		//{\
		//newFaceList.erase(*face);\
		//}\
		//else\
		//{\
		//newFaceList.insert(*face);\
		//}\
		//delete(face);

		//JUDCE_FACE(0,1,2)
		//JUDCE_FACE(0,1,3)
		//JUDCE_FACE(0,2,3)
		//JUDCE_FACE(1,2,3)

		#undef PROCESS_FACE3

	}


	return true;
}

Vec3 TetMesh::movingVerticle(unsigned int index)
{
	Vec3 m_Ver = this->verticlePos[index];
	Vec3 m_Nor = this->verticleNormal[index];
	m_Ver = m_Ver - m_Nor * scalar;
	return m_Ver;
}

Vec3 TetMesh::movingNormal(unsigned int index)
{
	Vec3 m_Nor = this->verticleNormal[index];
	m_Nor *= -1;
	return m_Nor;
}

Vec3 TetMesh::movingFaceCore(Vec3 v,Vec3 n)
{
	return (v-n*scalar);
}

Vec3 TetMesh::movingNormal(Vec3 n)
{
	return n*(-1);
}