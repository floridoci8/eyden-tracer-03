#include "Scene.h"

#include "ShaderEyelight.h"
#include "ShaderPhong.h"

#include "PrimTriangle.h"
#include <fstream>
void CScene::ParseOBJ(const std::string& fileName)
{
	std::cout << "Parsing OBJFile : " << fileName << std::endl;

	std::shared_ptr<IShader> pShader = std::make_shared<CShaderEyelight>(RGB(1, 1, 1));
	// --- PUT YOUR CODE HERE ---
	/*Partial reference: https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ */
	
	std::vector<Vec3f> vertices;

	std::ifstream file;
	file.open(fileName);
	if(!file.is_open()){
		std::cout << "Can't open file!" << std::endl;
		exit(1);
	}

	while(!file.eof()){
		char lineHeader = file.get();

		if(lineHeader == 'v'){
			Vec3f vertex;
			file >> vertex[0] >> vertex[1] >> vertex[2];

			vertex[0] *= 100;
			vertex[1] *= 100;
			vertex[2] *= 100;
			vertices.push_back(vertex);

		}else if(lineHeader == 'f'){
			unsigned int v[3];
			file >> v[0] >> v[1] >> v[2];

			Add(std::make_shared<CPrimTriangle>(vertices[v[0]-1],
				vertices[v[1]-1], vertices[v[2]-1], pShader));
			
		}
	}
	file.close();

	std::cout << "Finished Parsing" << std::endl;
}

