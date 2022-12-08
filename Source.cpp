#include <glad\gl.h> // this extension loader library loads OpenGL and some extensions to it
#include <GLFW\glfw3.h> // The GLFW library is used to access OS-specific tasks such as opening windows, reading keyboard input, rendering, etc.
#include <iostream>

// Constants //
const unsigned short windowX = 640;
const unsigned short windowY = 480;
const char* const windowName = "C++ Game"; //   c-string as GLFW doesn't like stl strings
// Constants //

// Shaders //
const char* vertexShaderSource = "#version 330 core\n" // shaders are read as strings
							     "layout (location = 0) in vec3 aPos;\n" // input
							  	 "void main()\n" // shaders use a lot of vectors to represent 3D points
								 "{\n" // this one maps a vec3 onto a vec4 (the 4th value is used for something special not space)
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // output
								 "}\0";
const char* fragmentShaderSource = "#version 330 core\n" 
								   "out vec4 FragColor;\n" // outputs color for rendered pixels 
								   "void main()\n"
								   "{\n"
								   "	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\0";
// Shaders //

// Triangle Coordinates //
const float vertices[] = {
	-0.5f, -0.5f, 0.0f, // coordinates for OpenGL are bounded from 0.0 to 1.0 based on an xy graph
	 0.5f, -0.5f, 0.0f, // these are called "normalized device coordinates"
	 0.0f,  0.5f, 0.0f
};
// Triangle Coordinates //


int main() {
	if (!glfwInit()) // initializes GLFW and checks for success
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return EXIT_FAILURE;
		// Initialization failed
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // sets minimum requirement for OpenGL 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // sets minimum requirement for OpenGL x.3

	GLFWwindow* window = glfwCreateWindow(windowX, windowY, windowName, NULL, NULL); // creates window
	if (!window)
	{
		return EXIT_FAILURE;
		// Window or OpenGL context creation failed
	}

	glfwMakeContextCurrent(window); // sets current opengl context to generated window
	glfwSwapInterval(1); // v-sync

	if (!gladLoadGL(glfwGetProcAddress)) { // initializes glad  and checks for success
		std::cout << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
		// Initialization failed
	}

	// we need a shader now, write it in GLSL!
	unsigned int vertexShader; // creates and designates it as the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader); // attaches and compiles the written GLSL shader

	int  success; // checks for success in the shader compilation
	char infoLog[512]; // string to hold error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader; // fragment shader set-up
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// combining the shaders into a shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for failure
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glUseProgram(shaderProgram); // every shader/rendering call will now use the program
	glDeleteShader(vertexShader); // individual shaders are now obsolete
	glDeleteShader(fragmentShader);


	// lets make a VAO to store vertex configurations for future use instead of restating them every time
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO; // we want to sent lots of vertices at a time to the gpu as it is more efficient
	glGenBuffers(1, &VBO); // generates the unique ID for a buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // assigns the new buffer as the official one-and-only Vertex Buffer

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // passes vertices into the buffer

	// tell OpenGL how it should interpret vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Param 1 - specifies the vertex attribute to configure, the position
				 vertex attribute was specified in the vertex shader as 0
	   Param 2 - size of the VA, it's a 3 coord vector
	   Param 3 - type of data
	   Param 4 - normalizes data, not relevant
	   Param 5 - space between VAs (they are tightly packed so 0 works too)
	   Param 6 - offset of where position data starts in the vertex buffer (it's a void* for some reason so cast it)*/
	glEnableVertexAttribArray(0); // enables vertex attributes which are off by default
	
	float rValue = 0.2;
	float gValue = 0.3;
	float bValue = 0.4;
	bool rise = true;

	while (!glfwWindowShouldClose(window)) // render loop
	{
		// viewport 
		int width, height;
		glfwGetFramebufferSize(window, &width, &height); // auto-adjusts if user resizes window
		glViewport(0, 0, width, height); // a single viewport filling the window

		// input
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			
		}

		// render
		glClearColor(rValue, gValue, bValue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (rise == true) {
			rValue += 0.01;
			gValue += 0.01;
			bValue += 0.01;
			if (bValue >= 0.95) {
				rise = false;
			}
		}
		else {
			rValue -= 0.01;
			gValue -= 0.01;
			bValue -= 0.01;
			if (bValue <= 0.05) {
				rise = true;
			}
		}
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// update
		glfwSwapBuffers(window); // rendering buffer
		glfwPollEvents(); // this keeps the window system in contact with GLFW and processes stuff
	}

	glfwDestroyWindow(window);
	return EXIT_SUCCESS;
}