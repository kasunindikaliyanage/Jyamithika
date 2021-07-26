#include "glad\glad.h"
#include <GLFW\glfw3.h>
#include <array>

#include "GLM\glm.hpp"
#include "GLM\gtc\matrix_transform.hpp"
#include "GLM\gtc\type_ptr.hpp"

#include "GraphicUtils\Imgui\imgui.h"
#include "GraphicUtils\Imgui\imgui_impl_glfw.h"
#include "GraphicUtils\Imgui\imgui_impl_opengl3.h"

#include "GraphicUtils\ShaderProgram.h"
#include "GraphicUtils\VertexArray.h"
#include "GraphicUtils\VertexBuffer.h"
#include "GraphicUtils\GraphicUtils.h"

#include "GraphicUtils\Geometry\GFace.h"
#include "GraphicUtils\Geometry\GPoint.h"
#include "GraphicUtils\Geometry\GLine.h"

#include "Jyamithika\Core\Primitives\Point.h"
#include "Jyamithika\Core\Primitives\Bounds.h"
#include "Jyamithika\Voronoi.h"

#include <iostream>
#include <cstdio>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 900;
const unsigned int SCREEN_HEIGHT = 900;

bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

int option = 0;

//Camera camera(glm::vec3(0.0f, 0.0f, 12.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void setup_pointcloud(std::vector<jmk::Point2d>& points)
{
	//jmk::Point2d p1(-1, -1);
	//jmk::Point2d p2(-1, -1);
	//jmk::Point2d p3(-1, 1 );
	//jmk::Point2d p4(-1, 1 );
	//jmk::Point2d p5(1, -1 );
	//jmk::Point2d p6(1, -1 );
	//jmk::Point2d p7(1, 1  );
	//jmk::Point2d p8(1, 1  );

	points.push_back(jmk::Point2d(0.2, 0.1));
	points.push_back(jmk::Point2d(0.6, 0.2));
	points.push_back(jmk::Point2d(0.8, 0.6));
	points.push_back(jmk::Point2d(0.4, 0.5));
	points.push_back(jmk::Point2d(0.5, 0.7));
}

int main(void)
{
	GLFWwindow* window;

	//Set up glfw context and window
	{
		/* Initialize the library */
		if (!glfwInit())
			return -1;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Voronoi 2d Simulation", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetCursorPosCallback(window, mouse_callback);
		//glfwSetScrollCallback(window, scroll_callback);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		glEnable(GL_DEPTH_TEST);
	}

	std::vector<jmk::Point2d> points;
	std::vector<float> point_data;
	std::vector<jmk::Edge2d> edges;
	std::vector<float> edge_data;

	setup_pointcloud(points);
	getReactanglePointClouds(points, point_data);

	jmk::BoundRectangle rect{ 0.0,1.0,1.0,0.0 };
	jmk::constructVoronoiDiagram_fortunes(points, edges, rect);

	get2DLinePointsFromEdgeList(edges, edge_data);

	VertexArray VAO_points;
	VertexBuffer VBO_points(point_data.data(), point_data.size());
	VAO_points.addVertexLayout(0, 2, GL_FALSE, 2 * sizeof(float), 0);

	VertexArray VAO_edges;
	VertexBuffer VBO_edge(edge_data.data(), edge_data.size());
	VAO_edges.addVertexLayout(0, 2, GL_FALSE, 2 * sizeof(float), 0);

	ShaderProgram shader("C:/Users/intellect/source/repos/Jyamithika/Graphics/GraphicUtils/Shaders/triangle2d.shader");
	shader.activeAsCurrentShader();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		/* Render here */

		glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		VAO_points.bindVertexArray();
		shader.activeAsCurrentShader();

		glDrawArrays(GL_TRIANGLES, 0, point_data.size()/2);

		VAO_edges.bindVertexArray();
		glDrawArrays(GL_LINE, 0, edge_data.size() / 2);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		option = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		option = 2;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		option = 3;
	}
	else
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		//	camera.ProcessKeyboard(FORWARD, deltaTime);
		//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		//	camera.ProcessKeyboard(BACKWARD, deltaTime);
		//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//	camera.ProcessKeyboard(LEFT, deltaTime);
		//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		//	camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	return;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	//camera.ProcessMouseMovement(xoffset, yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}