#include <glad\gl.h>
#define GLFW_DLL // needed for window to use the GLFW library dynamically
#include <GLFW\glfw3.h> // The GLFW library is used to access OS-specific tasks such as opening windows, reading keyboard input, rendering, etc.
#include <iostream>

// Constants //
const unsigned short windowX = 640;
const unsigned short windowY = 480;
const char* const windowName = "C++ Game"; //   c-string as GLFW doesn't like stl strings
// Constants //


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

	if (!gladLoadGL(glfwGetProcAddress)) { // initializes glad  and checks for success
		std::cout << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
		// Initialization failed
	}

	while (!glfwWindowShouldClose(window)) // while the window is not set to close
	{
		bool isRunning = true;
		while (isRunning) { // the update loop that runs each frame



		}
	}

	glfwDestroyWindow(window);
	return EXIT_SUCCESS;
}