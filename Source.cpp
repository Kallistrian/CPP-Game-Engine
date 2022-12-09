#include <glad\gl.h> // this extension loader library loads OpenGL and some extensions to it
#include <GLFW\glfw3.h> // The GLFW library is used to access OS-specific tasks such as opening windows, reading keyboard input, rendering, etc.
#define STB_IMAGE_IMPLEMENTATION // this prevents everything from breaking by compiling multiple times
#include <stb_image.h> // this is used to load image files
#include <iostream>

// Constants //
const unsigned short windowX = 640;
const unsigned short windowY = 480;
const char* const windowName = "C++ Game"; //   c-string as GLFW doesn't like stl strings
// Constants //

// Shaders //
char* vertexShaderSource =   "#version 330 compatibility\n" // shaders start with the OpenGL version
							 "layout(location = 0) in vec3 aPos;\n" // Property 1 in the vertex (Position)
							 "layout(location = 1) in vec3 aColor;\n" // Property 2 in the vertex (Color)
							 "layout(location = 2) in vec2 aTexCoord;\n" // Property 3 in the vertex (Texture Coordinate)
							 "out vec3 ourColor;\n" // these two lines send color and text coord to the fragment shader
						     "out vec2 TexCoord;\n"
							 "void main()\n"
							 "{\n"
							 "	gl_Position = vec4(aPos, 1.0);\n" // takes xyz and passes them through the shader (the extra 1.0 is for weird stuff)
							 "	ourColor = aColor;\n"
							 "	TexCoord = aTexCoord;\n"
							 "};\n";
char* fragmentShaderSource = "#version 330 compatibility\n"
							 "out vec4 FragColor;\n"
							 "in vec3 ourColor;\n"
							 "in vec2 TexCoord;\n"
							 "uniform sampler2D ourTexture;\n" // this does.. something, you need one of them per texture to sample it or something
							 "void main()\n"
							 "{\n"
							 "	FragColor = texture(ourTexture, TexCoord);\n" // each fragment figures out what color it should be from the color
							 "}\n";
// Shaders //

int main() {
	// Initialization //
	if (!glfwInit()) // initializes GLFW and checks for success
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		// Initialization failed
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // sets minimum requirement for OpenGL 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // sets minimum requirement for OpenGL x.3

	GLFWwindow* window = glfwCreateWindow(windowX, windowY, windowName, NULL, NULL); // creates window
	if (!window)
	{
		std::cout << "Window/OpenGL context creation failed" << std::endl;
	}

	glfwMakeContextCurrent(window); // sets current opengl context to generated window
	glfwSwapInterval(1); // v-sync

	if (!gladLoadGL(glfwGetProcAddress)) { // initializes glad  and checks for success
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	// Initialization //

	// Shader //
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
	// Shader //

	// Drawing Initialization // 
	// Vertices //
	float vertices[] = {
		// The position coordinates are based on a normal xy graph centered at the origin.
		// The color values have are clamped from 0 to 1.
		// The texture coordinates are based on the lower left corner of the texture file.
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	/* OpenGL will use the below indices to avoid having overlapping/extra vertices
	(for example on the shared vertices on the two triangles making a rectangle)*/
	unsigned int indices[] = {  
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	// Vertices //

	/*
	VBO - Passing vertex objects to the GPU is slow, so we want to pass
		  a lot of them at the same time. A VBO is a vertex buffer object
		  which stores multiple vertex objects for this purpose.
	VAO - A Vertex Array Object stores the attribute configurations for
		  those vertexes. This is useful because when you change what you
		  render you wouldn't want to restate the configurations every time.
	EBO - An Element Buffer Object stores the indices at line 115 for indexed
		  drawing, which again helps avoid overlapping vertices.
	
	*/
	// declare
	unsigned int VBO, VAO, EBO; 
	// generate
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind & set data
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Each of those big VertexAttributePointer statements below have a lot of params.
	The second is the size of that attribute, for example there are 3 floats in the
	3D position attribute. The fifth is the space from the start of one vertex to the
	start of another. The last parameter is the offset before that specific attribute
	begins in that vertex. For example, the color data starts after the 3 position
	attributes at the beginning so it is 3*sizeof(float).*/
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Drawing Initialization // 

	// Texture //
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load("..\\..\\..\\container.jpg", &width, &height, &nrChannels, 0); // loads image path relative to exe
	if (data) // checks that image loaded successfully
	{
		// generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // mipmaps are smaller versions of the texture that get viewed from a distance
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl << stbi_failure_reason() << std::endl;
	}
	stbi_image_free(data);
	// Texture //

	// Render Loop //
	float rValue = 0.2; // will be used to make background a pulsing blue color
	float gValue = 0.3;
	float bValue = 0.4;
	bool rise = true;

	while (!glfwWindowShouldClose(window)) 
	{
		// viewport 
		int width, height;
		glfwGetFramebufferSize(window, &width, &height); // auto-adjusts if user resizes window
		glViewport(0, 0, width, height); // a single viewport filling the window

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

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw that rectangle that has a texture mapped onto it!

		// update
		glfwSwapBuffers(window); // rendering buffer, basically it swaps between two frames super fast while changing the other
		glfwPollEvents(); // this keeps the window system in contact with GLFW and processes stuff
	}
	// Render Loop //

	glfwDestroyWindow(window);
	return EXIT_SUCCESS;
}