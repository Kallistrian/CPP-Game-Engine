#ifndef SHADER_H 
#define SHADER_H
#include <glad\gl.h>
#include <fstream>
#include <iostream>
#include <string>

class Shader {
private:
	unsigned int vertexShader;
	unsigned int fragmentShader;
public:
	unsigned int ID;
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

private:
	std::string ReadShaderFile(const std::string& file);
	unsigned int CompileShader(const std::string& file);
	unsigned int CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
	void CheckSuccess(unsigned int type, unsigned int subject);
};

#endif