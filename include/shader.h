#ifndef SHADER_H 
#define SHADER_H
#include <iostream>
#include <fstream>
#include <string>

class shader {
public:
	unsigned int ID;
	shader(std::string vertexShaderPath, std::string fragmentShaderPath);

private:
	std::string ReadShaderFile(std::string file);
	unsigned int CompileShader(std::string file);
	unsigned int CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
	void CheckSuccess(unsigned int type, unsigned int subject);
};

#endif