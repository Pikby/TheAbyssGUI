#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gui.h"

int main()
{
	glfwInit();
  const int numbBuildThreads = 2;
  int winWidth = 1920;
  int winHeight = 1080;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(winWidth,winHeight,"Gui Testing",NULL,NULL);

  glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
 	glewInit();
	glViewport(0, 0, winWidth, winHeight);

	GUI::initGUI(glm::vec2(1920,1080));

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f,0.7f,0.7f,1.0f);
		glfwPollEvents();

		GUI::drawQuad(glm::vec2(0.0,0.0),glm::vec2(0.5,0.5));
		GUI::drawTriangle(glm::vec2(0.5,0.5),glm::vec2(1,1),glm::vec2(0.9f,0.5f));
		glfwSwapBuffers(window);
	}
}
