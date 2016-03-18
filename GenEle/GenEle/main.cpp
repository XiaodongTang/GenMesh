#include "tetmesh.h"
void Test(std::string m_filename,std::string outfileName)
{
	bool isSave = false;
	TetMesh tetMesh = TetMesh();
	tetMesh.setFilename(m_filename);
	tetMesh.setScalar(1.0);
	if(!tetMesh.readFile()) return;
	else
	{
		std::string message = "the part of reading file is right!";
		std::cout<<message<<std::endl;
	}
	if(!tetMesh.calculateNeightborFace())return;
	else
	{
		std::string message = "the part of calculating neightbour face is right!";
		std::cout<<message<<std::endl;
	}
	if(!tetMesh.createVerticle())return;
	else
	{
		std::string message = "the part of create verticle is right!";
		std::cout<<message<<std::endl;
	}
	if(!tetMesh.createElement())return;
	else
	{
		std::string message = "the part of create element is right!";
		std::cout<<message<<std::endl;
	}
	isSave = true;
	if(isSave)
	{
		tetMesh.saveGenElement(outfileName);
		tetMesh.saveGenVerticle(outfileName);
		tetMesh.saveGenVegFile(outfileName);
	}
}
void Test2(std::string filename,std::string outfilename)
{
	TetMesh tetMesh = TetMesh();
	if(!tetMesh.readFile(filename)) return;
	else
	{
		std::string message = "the part of reading file is right!";
		std::cout<<message<<std::endl;
	}
	if(!tetMesh.saveObjFile(outfilename))return;
	else
	{
		std::string message = "\nthe part of reading file is right!\n";
		std::cout<<message<<std::endl;
	}
}
int main(int argc, char** argv)
{
	if(argc < 3)
	{
		std::cout<< "Usage: " << argv[0] << "[obj mesh file] [out file name]" <<std::endl;
		std::cout<< "Generates the volumetric mesh with the obj mesh file."<< std::endl;
		std::cout<< "generates a node file filename.node , a element file filename.ele and a file filename.veg."<<std::endl;
		//return 1;
	}
	char * objMesh = "YY0712Cut.obj";
	char * outputFile = "YY0712Cut";

	Test(objMesh,outputFile);
	return 0;
}