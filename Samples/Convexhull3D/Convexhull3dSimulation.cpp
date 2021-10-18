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
#include "GraphicUtils\camera.h"

#include "GraphicUtils\Geometry\GFace.h"
#include "GraphicUtils\Geometry\GPoint.h"
#include "GraphicUtils\Geometry\GLine.h"

#include "Jyamithika\Core\Base\Vector.h"
#include "Jyamithika\Core\Primitives\Point.h"
#include "Jyamithika\Convexhull.h"
#include "Jyamithika\Core\Primitives\Polyhedron.h"

#include <iostream>
#include <cstdio>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 1080;
const unsigned int SCREEN_HEIGHT = 720;

bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

int option = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 12.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void setup_pointcloud(std::vector<jmk::Point3d>& vertices)
{
	jmk::Point3d p1(-1, -1, 1);
	jmk::Point3d p2(-1, -1, -1);
	jmk::Point3d p3(-1, 1, -1);
	jmk::Point3d p4(-1, 1, 1);
	jmk::Point3d p5(1, -1, 1);
	jmk::Point3d p6(1, -1, -1);
	jmk::Point3d p7(1, 1, -1);
	jmk::Point3d p8(1, 1, 1);

	jmk::Point3d p9(-2, -2, 2);
	jmk::Point3d p10(-3, -1, -4);
	jmk::Point3d p11(-0.1, 1.2, -0.1);
	jmk::Point3d p12(-0.7, 1.8, -0.8);
	jmk::Point3d p13(2, -2, 4);
	jmk::Point3d p14(-2, 4, -0.1);
	jmk::Point3d p15(0.5, 0.5, 0.5);
	jmk::Point3d p16(-1, -2, 4);
	//jmk::Point3d p17(-3, -5, 7);

	vertices.push_back(p9);
	vertices.push_back(p10);
	vertices.push_back(p11);
	vertices.push_back(p12);
	vertices.push_back(p13);
	vertices.push_back(p14);
	vertices.push_back(p15);
	vertices.push_back(p16);
	
	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	vertices.push_back(p4);
	vertices.push_back(p5);
	vertices.push_back(p6);
	vertices.push_back(p7);
	vertices.push_back(p8);


	//vertices.push_back(p17);
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
		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Convexhull 3D", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		glEnable(GL_DEPTH_TEST);
	}

	//Set up ImGui
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplOpenGL3_Init("#version 130");
		ImGui_ImplGlfw_InitForOpenGL(window, true);
	}

	std::vector<jmk::Face*> faces;
	std::vector<jmk::Point3d> point_cloud;

	setup_pointcloud(point_cloud);

	jmk::convexhull3D(point_cloud, faces);

	std::vector<GLine*> lines;
	for (size_t i = 0; i < faces.size(); i++)
	{
		getLinesFromAFace(faces[i], lines);
	}

	float cube_vertices[] = {
		-0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f 
	};
	
	VertexArray VAO_cube;
	VertexBuffer VBO_cube(cube_vertices, 108);
	VAO_cube.addVertexLayout(0, 3, GL_FALSE, 3 * sizeof(float), 0);

	std::vector<float> data;
	getDataFromFaceList(faces, data);

	std::vector<jmk::Point3d> unq_points;
	getUniquePoints(faces, unq_points);

	VertexArray VAO_poly;
	VertexBuffer VBO_poly(data.data(), data.size());
	VAO_poly.addVertexLayout(0, 3, GL_FALSE, 6 * sizeof(float), 0);
	VAO_poly.addVertexLayout(1, 3, GL_FALSE, 6 * sizeof(float), (3 * sizeof(float)));

	ShaderProgram testShader("C:/Users/intellect/source/repos/Jyamithika/Graphics/GraphicUtils/Shaders/basic_poly.shader");
	testShader.activeAsCurrentShader();

	unsigned int model_loc = testShader.getUniformId("model");
	unsigned int view_loc = testShader.getUniformId("view");
	unsigned int projection_loc = testShader.getUniformId("projection");

	glm::mat4 model = glm::mat4(1);
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

	glm::mat4 view = glm::mat4(1.0f);

	const float radius = 10.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

	ShaderProgram cubeShader("C:/Users/intellect/source/repos/Jyamithika/Graphics/GraphicUtils/Shaders/cube.shader");
	cubeShader.activeAsCurrentShader();

	glm::vec3 all_point_color_vec(0.0, 0.0, 0.5);
	glm::vec3 convex_point_color_vec(1.0, 0.0, 0.0);

	unsigned int cube_model_loc = cubeShader.getUniformId("model");
	unsigned int cube_view_loc = cubeShader.getUniformId("view");
	unsigned int cube_projection_loc = cubeShader.getUniformId("projection");
	unsigned int cube_color = cubeShader.getUniformId("cubeColor");

	glUniformMatrix4fv(cube_model_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(cube_projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool show_planes = false, show_all_pnt = true, show_vertices = false;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		processInput(window);
		/* Render here */

		glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.GetViewMatrix();


		VAO_cube.bindVertexArray();
		cubeShader.activeAsCurrentShader();
	
		if (show_all_pnt)
		{
			for (int i = 0; i < point_cloud.size(); i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(point_cloud[i][X], point_cloud[i][Y], point_cloud[i][Z]));
				model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
				glUniformMatrix4fv(cube_model_loc, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(cube_view_loc, 1, GL_FALSE, glm::value_ptr(view));
				glUniform3fv(cube_color, 1, glm::value_ptr(all_point_color_vec));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		if (show_vertices)
		{
			for (int i = 0; i < unq_points.size(); i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(unq_points[i][X], unq_points[i][Y], unq_points[i][Z]));
				model = glm::scale(model, glm::vec3(0.11, 0.11, 0.11));
				glUniformMatrix4fv(cube_model_loc, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(cube_view_loc, 1, GL_FALSE, glm::value_ptr(view));
				glUniform3fv(cube_color, 1, glm::value_ptr(convex_point_color_vec));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		if (show_planes)
		{
			VAO_poly.bindVertexArray();
			testShader.activeAsCurrentShader();
			glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
			glDrawArrays(GL_TRIANGLES, 0, data.size() / 6);

		}

		ImGui::Begin(" Sample : Convexhull3d");		
		ImGui::Checkbox("All Points", &show_all_pnt);
		ImGui::Checkbox("Convexhull Vertices", &show_vertices);
		ImGui::Checkbox("Convexhull Planes", &show_planes);
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
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

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}