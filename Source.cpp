#define GLFW_DLL // needed for window to use the GLFW library dynamically
#include <GLFW/glfw3.h> // The GLFW library is used to access OS-specific tasks such as opening windows, reading keyboard input, rendering, etc.
#include <iostream>

// Constants //
const unsigned short windowX = 640;
const unsigned short windowY = 480;
const char* const windowName = "C++ Game"; //   c-string as GLFW doesn't like stl strings
// Constants //


int main() {
	if (!glfwInit()) // checks to make sure GLFW initialized successfully
	{
		return EXIT_FAILURE;
		// Initialization failed
	}

	GLFWwindow* window = glfwCreateWindow(windowX, windowY, windowName, NULL, NULL);
	if (!window)
	{
		return EXIT_FAILURE;
		// Window or OpenGL context creation failed
	}

	bool isRunning = true;
	while (isRunning) {



	}

	glfwDestroyWindow(window);
	return EXIT_SUCCESS;
}