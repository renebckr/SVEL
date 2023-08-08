/**
 * @file main.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief The entrypoint of the engine.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Internal
#include <svel/detail/app.h>

// GLFW
#include <GLFW/glfw3.h>

// STL
#include <cstdlib>
#include <iostream>
#include <stdexcept>

/**
 * @brief Entrypoint.
 *
 * @param argc Given by the OS to the method.
 * @param argv Given by the OS to the method.
 * @return int Error code.
 */
int main(int argc, char *argv[]) {
  try {
    // Initialize glfw
    if (glfwInit() != GLFW_TRUE)
      throw std::runtime_error("Could not initialize GLFW.");

    // Check vulkan support
    if (glfwVulkanSupported() != GLFW_TRUE)
      throw std::runtime_error("Vulkan not supported.");

    // Run the application of the user
    try {
      sv::SharedIApplication app = svel_make_app(argc, argv);
      app->Run();
    } catch (const std::exception &e) {
      std::cout << "An exception occured:\n\t" << e.what() << std::endl;
    } catch (...) {
      std::cout << "Unknown exception occured." << std::endl;
    }

    glfwTerminate();
  } catch (...) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}