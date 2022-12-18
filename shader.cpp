#include <glad\gl.h> 
#include "shader.h"

shader::shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	unsigned int vertexShader = CompileShader(vertexShaderPath);
	unsigned int fragmentShader = CompileShader(fragmentShaderPath);
	ID = CreateShaderProgram(vertexShader, fragmentShader);
}

unsigned int shader::CompileShader(std::string file) {
	// determine shader type from file extension, all file extensions are four characters long so we grab the first letter
	const char extensionType = file[file.size() - 4];
	unsigned int shader; // generates a unique code for a vertex shader

	if (extensionType == 'v') {
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (extensionType == 'f') {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else {
		std::cout << "Error: Shader extension type for " << file << " is unreadable.\n";
	}

	std::string sourceString = ReadShaderFile(file);

	// glShaderSource takes this weird parameter for filenames, it allows for multiple shaders
	const GLchar* source[] = { sourceString.c_str() };
	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader); // attaches and compiles the written GLSL shader
	CheckSuccess(0, shader);

	return shader;
}

unsigned int shader::CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	CheckSuccess(1, shaderProgram);

	glDeleteShader(vertexShader); // individual shaders are now obsolete
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

std::string shader::ReadShaderFile(std::string file) {
	std::ifstream readFile(file);
	std::string inputString = "";
	std::string shader = "";

	while (std::getline(readFile, inputString)) {
		shader += inputString + "\n";
	}

	readFile.close();
	return shader;
}

void shader::CheckSuccess(unsigned int type, unsigned int subject) { // accepts '0' for shader and '1' for program
	int success; // checks for success in the shader compilation
	char infoLog[512]; // string to hold error

	if (type == 0) {
		glGetShaderiv(subject, GL_COMPILE_STATUS, &success);
	}
	else if (type == 1) {
		glGetProgramiv(subject, GL_LINK_STATUS, &success);
	}
	else {
		std::cout << "Error: Unexpected input for shader function CheckSuccess.";
	}

	if (!success)
	{
		glGetShaderInfoLog(subject, 512, NULL, infoLog);
		if (type == '0') {
			std::cout << "Error: Shader compilation failed.\n" << infoLog << std::endl;
		}
		else if (type == '1') {
			std::cout << "Error: Shader program compilation failed.\n" << infoLog << std::endl;
		}

	}
}