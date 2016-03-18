#include "objmesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
ObjMeshException::ObjMeshException(const std::string &warn)
{
	std::ostringstream oss;
	oss<<"Error : "<<warn;
	std::cout<<std::endl<<oss.str()<<std::endl;
}

ObjMeshException::ObjMeshException(const std::string &warn, const std::string & filename, unsigned int line)
{
	std::ostringstream oss;
	oss<<"Error : error in file"<<filename<<"of the line"<<line<<warn;
	std::cout<<std::endl<<oss.str()<<std::endl;
}

ObjMeshException::ObjMeshException(const std::string & warn, unsigned int num)
{
	std::ostringstream oss;
	oss<<"Warn : "<<num<<warn;
	std::cout<<std::endl<<oss.str()<<std::endl;
}

ObjMesh::ObjMesh(const std::string & filename)
{
	m_filename = filename;
}

bool ObjMesh::readFile()
{
	return readFile(m_filename);
}

bool ObjMesh::readFile(const std::string &filename)
{
	std::ifstream inf(filename.c_str());
	if(!inf)
	{
		std::string message = "can't open this.obj file";
		message.append(filename);
		throw ObjMeshException(message);
		return false;
	}
	const int maxline = 1000;
	int numLine = 0;
	char line[maxline];
	while(inf)
	{
 		numLine++;
		inf.getline(line,maxline);
		if (strncmp(line,"v ",2) == 0)
		{//verticle
			double x,y,z;
			if(sscanf(line,"v %lf %lf %lf\n",&x,&y,&z)<3)
			{
				throw ObjMeshException("Invalid veticle", filename, numLine);
				return false;
			}
			Vec3 v = Vec3(x,y,z);
			verticlePos.push_back(v);
		}
		else if (strncmp(line,"vn ",3)== 0 )
		{//normal
			double xn,yn,zn;
			if(sscanf(line,"vn %lf %lf %lf\n",&xn,&yn,&zn)<3)
			{
				throw ObjMeshException("Invaild normal ",filename,numLine);
				return false;
			}
			Vec3 vn = Vec3(xn,yn,zn);
			vn = normalize(vn);
			verticleNormal.push_back(vn);
		}
		else if (strncmp(line,"vt ",3) == 0)
		{//texture
			double xt,yt;
			if(sscanf(line,"vt %lf %lf\n",&xt,&yt) < 2)
			{
				throw ObjMeshException("Invaild texture ",filename,numLine);
				return false;
			}
			Vec2 vt = Vec2(xt,yt);
			verticleTexture.push_back(vt);
		}
		else if(strncmp(line,"f ",2) == 0)
		{//face 
			char * faceLine = &line[2];
			char * curPos = faceLine;
			Face face;
			while(*curPos !='\0')
			{
				char *headIdx = curPos;
				while(*headIdx != ' ' && *headIdx != '\0' )
					headIdx++;
				
				bool wSpace = false;
				if(*headIdx == ' ')
				{
					*headIdx = '\0';
					wSpace = true;
				}
		        unsigned int pos;
				unsigned int nor;
				unsigned int tex;
				bool hasNormal= false;
				bool hasTexture = false;
				if( strstr(curPos,"//") !=NULL )
				{
					if(sscanf(curPos,"%d//%d",&pos,&nor)<2)
					{
						throw ObjMeshException("Invalid face ",filename,numLine);
						return false;
					}
					else
						hasNormal = true;
				}
				else
				{
					if(sscanf(curPos,"%d/%d/%d",&pos,&tex,&nor)<3)
					{
						if(strstr(curPos,"/") != NULL)
						{
							if(sscanf(curPos,"%d/%d",&pos,&tex)<2)
							{
								throw ObjMeshException("Invaild face ",filename,numLine);
								return false;
							}
							else
								hasTexture = true;
						}
						else
						{
							if(sscanf(curPos,"%d",&pos)<1)
							{
								throw ObjMeshException("Invaild face ",filename,numLine);
								return false;
							}
						}
					}
					else
					{
						hasNormal = true;
						hasTexture = true;
					}
				}
				if(hasNormal && hasTexture)
					face.addVerticle(Verticle(pos,nor,tex));
				else if(hasNormal && !(hasTexture))
					face.addVerticle(Verticle(pos,nor));
				else
					face.addVerticle(Verticle(pos));
				if(wSpace)
				{
					curPos  = headIdx+1;
					*headIdx = ' '; 
				}
				else
				{
					curPos = headIdx;
				}
			}
			face.setExist(true);
			faces.push_back(face);
			numLine++;
		}
		else{}
	}
	inf.close();
	return true;
}

bool ObjMesh::saveObjFile(const std::string &filename)
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
	const int m_PosSize  = verticlePos.size();
	const int m_NorSize  = verticleNormal.size();
	const int m_TexSize  = verticleTexture.size();
	const int m_FaceSize = faces.size();
	for( int i=0; i<m_PosSize; i++ )
	{
		outf<<"v "<<verticlePos[i].getLoc(0)<<" "<<verticlePos[i].getLoc(1)<<" "<<verticlePos[i].getLoc(2)<<"\n";
	}
	for( int i=0; i<m_NorSize; i++)
	{
		outf<<"vn "<<verticleNormal[i].getLoc(0)<<" "<<verticleNormal[i].getLoc(1)<<" "<<verticleNormal[i].getLoc(2)<<"\n";
	}
	if(m_TexSize != 0)
	{
		for( int i=0; i<m_TexSize; i++)
		{
			outf<<"vt "<<verticleTexture[i].getLoc(0)<<" "<<verticleTexture[i].getLoc(1)<<"\n";
		}
		for(int i=0; i<m_FaceSize; i++)
		{
			outf<<"f "<<faces[i].getVerticle(0).getPosIndex()<<"/"<<faces[i].getVerticle(0).getTextureIndex()<<"/"<<faces[i].getVerticle(0).getNorIndex();
			outf<<" "<<faces[i].getVerticle(1).getPosIndex()<<"/"<<faces[i].getVerticle(1).getTextureIndex()<<"/"<<faces[i].getVerticle(1).getNorIndex();
			outf<<" "<<faces[i].getVerticle(2).getPosIndex()<<"/"<<faces[i].getVerticle(2).getTextureIndex()<<"/"<<faces[i].getVerticle(2).getNorIndex()<<"\n";
		}
	}
	else
	{
		for(int i=0; i<m_FaceSize; i++)
		{
			outf<<"f "<<faces[i].getVerticle(0).getPosIndex()<<"//"<<faces[i].getVerticle(0).getNorIndex();
			outf<<" "<<faces[i].getVerticle(1).getPosIndex()<<"//"<<faces[i].getVerticle(1).getNorIndex();
			outf<<" "<<faces[i].getVerticle(2).getPosIndex()<<"//"<<faces[i].getVerticle(2).getNorIndex()<<"\n";
		}
	}	
	outf.close();
}

bool ObjMesh::calculateNeightborFace()
{
	const int facesSize = faces.size();
	if(facesSize < 2)
	{
		std::string message = "The numble of face in mesh is less than two .";
		throw ObjMeshException(message);
		return false;
	}
	for(int i=0;i<facesSize;i++)
	{
		int s1=-1,s2=-1,s3=-1;
		for(int j=0;j<facesSize;j++)
		{
			if(j==i) continue;
			if(faces[j].isContainVerticle(faces[i].getVerticle(0),faces[i].getVerticle(1)))
			{
				s1 = j;
			}
			if(faces[j].isContainVerticle(faces[i].getVerticle(0),faces[i].getVerticle(2)))
			{
				s2 = j;
			}
			if(faces[j].isContainVerticle(faces[i].getVerticle(1),faces[i].getVerticle(2)))
			{
				s3 = j;
			}
		}
		if(s1==-1 && s2 == -1 && s3 == -1)
		{
			std::string message = " face is independent .";
			throw ObjMeshException(message,i+1);
			return false;
		}
		else
		{
			//if(s1==-1)
			//{
			//	//faces[i].addFace(faces[facesSize]);
			//	faces[i].addFaceIndex(facesSize);
			//}
			//else
			//{
			//	//faces[i].addFace(faces[s1]);
			//	faces[i].addFaceIndex(s1);
			//}
			//if(s2==-1)
			//{
			//	//faces[i].addFace(faces[facesSize]);
			//	faces[i].addFaceIndex(facesSize);
			//}
			//else
			//{
			//	//faces[i].addFace(faces[s2]);
			//	faces[i].addFaceIndex(s2);
			//}
			//if(s3==-1)
			//{
			//	//faces[i].addFace(faces[facesSize]);
			//	faces[i].addFaceIndex(facesSize);
			//}
			//else
			//{
			//	//faces[i].addFace(faces[s3]);
			//	faces[i].addFaceIndex(s3);
			//}
			faces[i].addFaceIndex(s1);
			faces[i].addFaceIndex(s2);
			faces[i].addFaceIndex(s3);
		}
	}
	return true;
}

bool ObjMesh::otherAlgorithmCalculateNeightborFace()
{
	return false;
}

void ObjMesh::saveVerticle(const std::string &filename)
{
	saveVerticle(filename,this->verticlePos);
}

void ObjMesh::saveVerticle(const std::string &filename,std::vector<Vec3> vList)
{
	std::string m_VerFile = filename;
	m_VerFile.append(".node");
	std::ofstream outf(m_VerFile);
	if(!outf)
	{
		std::string message = "can't open this file ";
		message.append(m_VerFile);
		throw ObjMeshException(message);
	}
	const int verticlePosSize  = vList.size();
	outf<<verticlePosSize<<" 3 0 0\n";
	for( int i=0; i<verticlePosSize; i++ )
	{
		outf<<" "<<i+1<<" "<<vList[i].getLoc(0)<<" "<<vList[i].getLoc(1)<<" "<<vList[i].getLoc(2)<<"\n";
	}
	outf.close();
}

void ObjMesh::saveFace(const std::string &filename)
{
	saveFace(filename,this->faces);
}

void ObjMesh::saveFace(const std::string &filename, std::vector<Face> fList)
{
	std::string m_FaceFile = filename;
	m_FaceFile.append(".face");
	std::ofstream outf(m_FaceFile);
	if(!outf)
	{
		std::string message = "can't open this file ";
		message.append(m_FaceFile);
		throw ObjMeshException(message);
	}
	const int facesSize  = fList.size();
	outf<<facesSize<<"\t"<<"1\n";
	for( int i=0; i<facesSize; i++ )
	{
		outf<<"\t"<<i+1<<"\t"<<fList[i].getVerticle(0).getPosIndex()<<"\t"<<fList[i].getVerticle(1).getPosIndex()<<"\t"<<fList[i].getVerticle(2).getPosIndex()<<"\t"<<"0"<<"\n";
	}
	outf.close();
}

void ObjMesh::setFilename(std::string &filename)
{
	m_filename = filename;
}

std::string ObjMesh::getFileName()
{
	return m_filename;
}