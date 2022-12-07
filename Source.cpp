#define GLFW_DLL // needed for window to use the GLFW library dynamically
#include <GLFW/glfw3.h> // The GLFW library is used to access OS-specific tasks such as opening windows, reading keyboard input, rendering, etc.
#include <iostream>



int main() {
	if (!glfwInit()) // checks to make sure GLFW initialized successfully
	{
		return EXIT_FAILURE;
		// Initialization failed
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "test window", NULL, NULL);
	if (!window)
	{
		return EXIT_FAILURE;
		// Window or OpenGL context creation failed
	}

	std::cout << "Application window opened! :)";
	std::cout << std::endl << "Testing pull request...";
	std::cout << std::endl << "CODEOWNERS...";
	std::cin.get();

	glfwDestroyWindow(window);
	return EXIT_SUCCESS;
}
