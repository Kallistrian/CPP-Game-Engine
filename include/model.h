#ifndef MODEL_H 
#define MODEL_H
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glad\gl.h>
#include <glm/glm.hpp> // openGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <algorithm>
#include <memory>
#include <iostream>
#include <string>

class Model {
private:
	
public:
	unsigned int numVertices;
	unsigned int numFaces;
	unsigned int numMeshes;
	unsigned int* meshIndices; 
	unsigned int* VAOs;
	glm::mat4* transforms;
	void Model::ProcessMesh(const aiScene* scene, unsigned int);
	void Model::FindMeshTransform(aiString meshNode, aiNode* node, unsigned int mesh, bool&);
	Model(const std::string& file);
	void Draw(Shader shader);
};

#endif