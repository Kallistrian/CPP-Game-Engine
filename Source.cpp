#include <glad\gl.h> // this extension loader library loads OpenGL and some extensions to it
#include <GLFW\glfw3.h> // The GLFW library is used to access OS-specific tasks such as opening windows, reading keyboard input, rendering, etc.
#define STB_IMAGE_IMPLEMENTATION // this prevents everything from breaking by compiling multiple times
#include <stb_image.h> // this is used to load image files
#include <glm/glm.hpp> // openGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>


// Constants //
const unsigned short windowX = 640;
const unsigned short windowY = 480;
const char* const windowName = "C++ Game"; //   c-string as GLFW doesn't like stl strings
// Constants //

// Function Prototypes //
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // mouse input function
void processInput(GLFWwindow* window);
// Function Prototypes //

// global vars OH NO
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400;
float lastY = 300; // stores the last mouse position (initialized to center of screen)
bool firstMouse = true; // prevents jump when capturing cursor
// Camera
// we need to find relative (local?) axes for the camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
// Deltatime
// this is used to adjust for the speed of different computers so that higher FPS doesn't make things faster
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main() {
	/*\\\\\\\\\\\\\\\\\\******************** Initialization ********************\\\\\\\\\\\\\\\\\\*/
	////////////////////--------------------------------------------------------////////////////////

	// GLFW //
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // sets minimum requirement for OpenGL 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // sets minimum requirement for OpenGL x.3
	// GLFW //

	// Window //
	GLFWwindow* window = glfwCreateWindow(windowX, windowY, windowName, NULL, NULL); // creates window
	if (!window)
	{
		std::cout << "Window/OpenGL context creation failed" << std::endl;
	}

	glfwMakeContextCurrent(window); // sets current opengl context to generated window
	glfwSwapInterval(1); // v-sync
	// Window //

	// GLAD //
	if (!gladLoadGL(glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	// GLAD //

	// Shaders //
	Shader defaultShader("assets/shaders/default.vert", "assets/shaders/default.frag");
	glUseProgram(defaultShader.ID);
	// Shaders // 

	// Textures // 
	glEnable(GL_BLEND); // blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	// texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Textures // 
	
	// Misc //
	glEnable(GL_DEPTH_TEST); // z layers
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures the cursor
	glfwSetCursorPosCallback(window, mouse_callback); // monitors mouse input
	// Misc //

	/*\\\\\\\\\\\\\\\\\\******************** Initialization ********************\\\\\\\\\\\\\\\\\\*/
	////////////////////--------------------------------------------------------////////////////////

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Buffer //
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
	unsigned int VBO, VAO;
	// generate
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind & set data
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Each of those big VertexAttributePointer statements below have a lot of params.
	The second is the size of that attribute, for example there are 3 floats in the
	3D position attribute. The fifth is the space from the start of one vertex to the
	start of another. The last parameter is the offset before that specific attribute
	begins in that vertex. For example, the color data starts after the 3 position
	attributes at the beginning so it is 3*sizeof(float).*/
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Buffer //

	// Texture //
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // flips it the right way up since GL expects y = 0.0 to be bottom, images tend to have it top!
	unsigned char* data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0); // loads image path relative to exe
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

	// Coordinate Systems //
	glm::mat4 model = glm::mat4(1.0f); // declare
	model = glm::scale(model, glm::vec3(7.5f, 2.5f, 7.5f)); // scale the cube
	model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f)); // move it up
	
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	// Coordinate Systems //

	/*\\\\\\\\\\\\\\\\\\********************   Render Loop  ********************\\\\\\\\\\\\\\\\\\*/
	////////////////////--------------------------------------------------------////////////////////
	float rValue = 0.2; // will be used to make background a pulsing blue color
	float gValue = 0.3;
	float bValue = 0.4;
	bool rise = true;

	while (!glfwWindowShouldClose(window)) 
	{
		// Misc //
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ensures z-order is drawn right
		processInput(window); // monitors key input
		// Misc //

		// Viewport //
		int width, height;
		glfwGetFramebufferSize(window, &width, &height); // auto-adjusts if user resizes window
		glViewport(0, 0, width, height); // a single viewport filling the window
		// Viewport //

		// Background //
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
		// Background //
	
		// Delta Time //	
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Delta Time //	
	
		// Send Coordinate Systems to Shaders //
		int projectionLoc = glGetUniformLocation(defaultShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); // changed by mouse/key input
		int viewLoc = glGetUniformLocation(defaultShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int modelLoc = glGetUniformLocation(defaultShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Send Coordinate Systems to Shaders //
		
		// Draw //
		glDrawArrays(GL_TRIANGLES, 0, 36); // draw that rectangle that has a texture mapped onto it!
		// Draw //
		
		// Update //
		glfwSwapBuffers(window);
		glfwPollEvents();
		// Update //
	}
	/*\\\\\\\\\\\\\\\\\\********************   Render Loop  ********************\\\\\\\\\\\\\\\\\\*/
	////////////////////--------------------------------------------------------////////////////////

	glfwDestroyWindow(window);
	return EXIT_SUCCESS;
}

void processInput(GLFWwindow* window) {
	const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	glm::vec3 limit = {cameraFront.x, 0.0f, cameraFront.z}; // limits y movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * limit; // cameraFront is the direction vector
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * limit;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // the vectors for strafing are normalized so that the camera orientation doesn't affect speed
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) { // automatically run each time the mouse moves
	
	if (firstMouse) // initially set to true 
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) // constraints the pitch so you can look directly up and directly down but not break your neck
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// by using triangles, we can calculate angles from coordinates by treating them as triangles
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}