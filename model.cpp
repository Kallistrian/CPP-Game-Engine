#include "model.h"

Model::Model(const std::string& file) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file,
		aiProcess_Triangulate | // post-processing
		aiProcess_FlipUVs     );

	numMeshes = scene->mNumMeshes;
	meshIndices = new unsigned int[numMeshes]; // MEMORY LEAK LOL XD NOT GOING TO DELETE THIS ONE XD
	VAOs = new unsigned int[numMeshes];
	transforms = new glm::mat4[numMeshes];

	// ProcessNodes(scene->mRootNode); // error: doesn't return

	if (!scene) {
		std::cout << importer.GetErrorString();
	}

	for (unsigned int mesh = 0; mesh < (scene->mNumMeshes); mesh++) {
		ProcessMesh(scene, mesh);
	}
}

void Model::ProcessMesh(const aiScene* scene, unsigned int meshNum) { // returns the code to a VAO
	// generate and bind the vertex array
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// generate and bind the vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// generate and bind the element buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// |position          | |normal            | |texCoords|
    // -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	numVertices = scene->mMeshes[meshNum]->mNumVertices;
	numFaces = scene->mMeshes[meshNum]->mNumFaces;
	const unsigned short vertexSize = 8; // number of values in one vertex
	const unsigned short positionOffset = 0;
	const unsigned short normalOffset = 3;
	const unsigned short textureOffset = 6;
	const unsigned int verticesSize = numVertices * vertexSize; // number of values in the vertex array
	float* vertices = new float[verticesSize];

	const unsigned int numIndices = numFaces * 3;
	unsigned int* indices = new unsigned int[numFaces * 3];

	for (unsigned int vertex = 0; vertex < numVertices; vertex++) {
		// position
		vertices[vertex * vertexSize + positionOffset] = (scene->mMeshes[meshNum]->mVertices[vertex].x);
		vertices[vertex * vertexSize + positionOffset + 1] = (scene->mMeshes[meshNum]->mVertices[vertex].y);
		vertices[vertex * vertexSize + positionOffset + 2] = (scene->mMeshes[meshNum]->mVertices[vertex].z);
		// normal
		vertices[vertex * vertexSize + normalOffset] = (scene->mMeshes[meshNum]->mNormals[vertex].x);
		vertices[vertex * vertexSize + normalOffset + 1] = (scene->mMeshes[meshNum]->mNormals[vertex].y);
		vertices[vertex * vertexSize + normalOffset + 2] = (scene->mMeshes[meshNum]->mNormals[vertex].z);
		// texCoord
		vertices[vertex * vertexSize + textureOffset] = (scene->mMeshes[meshNum]->mTextureCoords[0][vertex].x);
		vertices[vertex * vertexSize + textureOffset + 1] = (scene->mMeshes[meshNum]->mTextureCoords[0][vertex].y);
	}

	// Initializing Indices // 
	for (unsigned int face = 0; face < scene->mMeshes[meshNum]->mNumFaces; face++) {
		indices[face * 3 + 0] = (scene->mMeshes[meshNum]->mFaces[face].mIndices[0]);
		indices[face * 3 + 1] = (scene->mMeshes[meshNum]->mFaces[face].mIndices[1]);
		indices[face * 3 + 2] = (scene->mMeshes[meshNum]->mFaces[face].mIndices[2]);
	}
	// Initializing Indices // 

	std::cout << std::endl;

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)positionOffset);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(normalOffset * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(textureOffset * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load vertex and index data
	glBufferData(GL_ARRAY_BUFFER, verticesSize*sizeof(float), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), indices, GL_STATIC_DRAW);
	std::cout << std::endl; 

	// store mesh indices, VAO
	meshIndices[meshNum] = numIndices;
	VAOs[meshNum] = VAO;
	// store transform associated with this mesh
	bool found = false;
	FindMeshTransform(scene->mMeshes[meshNum]->mName, scene->mRootNode, meshNum, found);

	delete[] vertices; // free memory after use
	delete[] indices;
}

void Model::FindMeshTransform(aiString meshNode, aiNode* node, unsigned int mesh, bool& found) {
	if (node->mName == meshNode) {
		aiMatrix4x4 aiMat = node->mTransformation;
		transforms[mesh] = glm::mat4(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
			aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
			aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
			aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);
		//transforms[mesh] = glm::transpose(transforms[mesh]);
		while (node->mParent != nullptr) {
			aiMatrix4x4 aiParentMat = node->mParent->mTransformation;
			glm::mat4 parent = glm::mat4(aiParentMat.a1, aiParentMat.b1, aiParentMat.c1, aiParentMat.d1,
				aiParentMat.a2, aiParentMat.b2, aiParentMat.c2, aiParentMat.d2,
				aiParentMat.a3, aiParentMat.b3, aiParentMat.c3, aiParentMat.d3,
				aiParentMat.a4, aiParentMat.b4, aiParentMat.c4, aiParentMat.d4);
			//parent = glm::transpose(parent);
			transforms[mesh] = transforms[mesh] * parent;
			node = node->mParent;
		}
		found = true;
		return;
	}
	for (unsigned int child = 0; child < node->mNumChildren; child++) {
		FindMeshTransform(meshNode, node->mChildren[child], mesh, found);
		if (found) {
			return;
		}
	}
}

void Model::Draw(Shader shader) {
	for (unsigned int mesh = 0; mesh < numMeshes; mesh++) {
		glBindVertexArray(VAOs[mesh]);
		int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transforms[mesh]));
		glDrawElements(GL_TRIANGLES, meshIndices[mesh], GL_UNSIGNED_INT, 0);
	}
}