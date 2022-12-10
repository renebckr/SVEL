#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <svel/detail/app.h>

int main(int argc, char *argv[]) {
  try {
    // Initialize glfw
    if (glfwInit() != GLFW_TRUE)
      throw std::runtime_error("Could not initialize GLFW.");

    // Check vulkan support
    if (glfwVulkanSupported() != GLFW_TRUE)
      throw std::runtime_error("Vulkan not supported.");

    {
      sv::SharedIApplication app = svel_make_app(argc, argv);
      app->Run();
    }

    glfwTerminate();
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    return EXIT_FAILURE;
  }
}