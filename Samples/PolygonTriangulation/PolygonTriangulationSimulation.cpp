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
#include "Jyamithika\MonotonePartition.h"
#include "Jyamithika\Triangulation.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::milli;

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
	points.push_back(jmk::Point2d(0.7, 0.4));
	points.push_back(jmk::Point2d(0.4, 0.1));
	points.push_back(jmk::Point2d(0.3, 0.8));
	points.push_back(jmk::Point2d(0.05, 0.7));
	points.push_back(jmk::Point2d(-0.05, 0.8));
	points.push_back(jmk::Point2d(-0.5, 0.5));
	points.push_back(jmk::Point2d(-0.2, 0.2));
	points.push_back(jmk::Point2d(-0.4, 0.05));
	points.push_back(jmk::Point2d(-0.6, 0.15));
	points.push_back(jmk::Point2d(-0.7, -0.2));
	points.push_back(jmk::Point2d(-0.3, -0.5));
	points.push_back(jmk::Point2d(-0.1, -0.3));
	points.push_back(jmk::Point2d(0.2, -0.8));
	points.push_back(jmk::Point2d(0.1, -0.05));
	points.push_back(jmk::Point2d(0.5, -0.1));
}

void setup_pointcloud_2(std::vector<jmk::Point2d>& points)
{
	points.push_back(jmk::Point2d(0.2, 0.4));
	points.push_back(jmk::Point2d(0.6, 0.2));
	points.push_back(jmk::Point2d(0.8, 0.5));
	points.push_back(jmk::Point2d(0.9, 0.7));
	points.push_back(jmk::Point2d(0.5, 0.9));
	points.push_back(jmk::Point2d(0.4, 0.6));
}

int main(void)
{
	std::srand(std::time(nullptr));
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
		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Polygon triagulation with monotone partitioning", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		//glEnable(GL_DEPTH_TEST);
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

	std::vector<jmk::Point2d> points;
	std::vector<float> point_data;
	std::vector<float> original_poly_edge_data;
	std::vector<float> monotone_poly_edge_data;
	std::vector<float> trigulation_data;
	std::vector<float> trigulation_face_data;
	std::vector<float> trigulation_ear_clipping;

	setup_pointcloud(points);
	getReactanglePointClouds(points, point_data);
	
	jmk::Polygon2d polygon(points);
	auto edge_list = polygon.getEdgeList();
	get2DLinePointsFromDCEL2d(edge_list, original_poly_edge_data);

	std::vector<jmk::Polygon2d*> poly_peices;
	jmk::get_monotone_polygons(&polygon, poly_peices);
	for (auto poly_ptr : poly_peices) {
		edge_list = poly_ptr->getEdgeList();
		get2DLinePointsFromDCEL2d(edge_list, monotone_poly_edge_data);
	}

	for (auto polygon : poly_peices) {
		triangulate_monotone(polygon);
	}

	for (auto polygon : poly_peices) {
		for (auto face_ptr : polygon->getFaceList()){
			edge_list = face_ptr->getEdgeList();
			get2DLinePointsFromDCEL2d(edge_list, trigulation_data);
		}
	}

	for (auto polygon : poly_peices) {
		for (auto face_ptr : polygon->getFaceList()){
			auto points = face_ptr->getPoints();
			getGraphicDataFromPointsList(points, trigulation_face_data);
		}
	}

	//Triangulation ear clipping
	jmk::Polygon2dSimple poly_simple(points);
	std::vector<jmk::Edge2dSimple> edge_list_ear;
	triangulate_earclipping(&poly_simple, edge_list_ear);
	get2DLinePointsFromEdgeList(edge_list_ear,trigulation_ear_clipping);


	VertexArray VAO_points;
	VertexBuffer VBO_points(point_data.data(), point_data.size());
	VAO_points.addVertexLayout(0, 2, GL_FALSE, 2 * sizeof(float), 0);

	VertexArray VAO_original_poly_edges;
	VertexBuffer VBO_edge(original_poly_edge_data.data(), original_poly_edge_data.size());
	VAO_original_poly_edges.addVertexLayout(0, 2, GL_FALSE, 2 * sizeof(float), 0);

	VertexArray VAO_monotone_poly_edges;
	VertexBuffer VBO_mono_edge(monotone_poly_edge_data.data(), monotone_poly_edge_data.size());
	VAO_monotone_poly_edges.addVertexLayout(0, 2, GL_FALSE, 2 * sizeof(float), 0);

	VertexArray VAO_triangulation_edges;
	VertexBuffer VBO_tri_edge(trigulation_data.data(), trigulation_data.size());
	VAO_triangulation_edges.addVertexLayout(0, 2, GL_FALSE, 2 * sizeof(float), 0);

	VertexArray VAO_triangulation_ear_edges;
	VertexBuffer VBO_tri_ear_edge(trigulation_ear_clipping.data(), trigulation_ear_clipping.size());
	VAO_triangulation_ear_edges.addVertexLayout(0, 2, GL_FALSE, 2 * sizeof(float), 0);

	VertexArray VAO_triangulation_faces;
	VertexBuffer VBO_tri_faces(trigulation_face_data.data(), trigulation_face_data.size());
	VAO_triangulation_faces.addVertexLayout(0, 3, GL_FALSE, 6 * sizeof(float), 0);
	VAO_triangulation_faces.addVertexLayout(1, 3, GL_FALSE, 6 * sizeof(float), (3 * sizeof(float)));

	ShaderProgram poly2dshader("C:/Users/intellect/source/repos/Jyamithika/Graphics/GraphicUtils/Shaders/basic_poly_2d.shader");
	ShaderProgram shader("C:/Users/intellect/source/repos/Jyamithika/Graphics/GraphicUtils/Shaders/triangle2d.shader");
	ShaderProgram line_shader("C:/Users/intellect/source/repos/Jyamithika/Graphics/GraphicUtils/Shaders/generic_line.shader");
	unsigned int line_color_loc = line_shader.getUniformId("line_color");
	
	glm::vec3 red = glm::vec3(0.95f, 0.02f, 0.03f);
	glm::vec3 blue = glm::vec3(0.55f, 0.42f, 0.83f);
	glm::vec3 green = glm::vec3(0.15f, 0.92f, 0.13f);
	glm::vec3 yellow = glm::vec3(0.9f, 0.95f, 0.2f);

	bool show_points = true, show_monotone_partitioning = false, show_triangulation = false,
		show_tri_faces = false, show_tri_ear = false;

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

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (show_tri_faces)
		{
			poly2dshader.activeAsCurrentShader();
			VAO_triangulation_faces.bindVertexArray();
			glDrawArrays(GL_TRIANGLES, 0, trigulation_face_data.size()/3);
		}

		if (show_triangulation)
		{
			line_shader.activeAsCurrentShader();
			glUniform3fv(line_color_loc, 1, glm::value_ptr(green));
			VAO_triangulation_edges.bindVertexArray();
			glLineWidth(2);
			glDrawArrays(GL_LINES, 0, trigulation_data.size() / 2);
		}

		if (show_tri_ear)
		{
			line_shader.activeAsCurrentShader();
			glUniform3fv(line_color_loc, 1, glm::value_ptr(yellow));
			VAO_triangulation_ear_edges.bindVertexArray();
			glLineWidth(2);
			glDrawArrays(GL_LINES, 0, trigulation_data.size() / 2);
		}
		
		if (show_monotone_partitioning)
		{
			line_shader.activeAsCurrentShader();
			glUniform3fv(line_color_loc, 1, glm::value_ptr(blue));
			VAO_monotone_poly_edges.bindVertexArray();
			glLineWidth(4);
			glDrawArrays(GL_LINES, 0, monotone_poly_edge_data.size() / 2);
		}

		if (show_points)
		{
			shader.activeAsCurrentShader();
			VAO_points.bindVertexArray();
			glDrawArrays(GL_TRIANGLES, 0, point_data.size() / 2);
			glLineWidth(4);
			line_shader.activeAsCurrentShader();
			glUniform3fv(line_color_loc, 1, glm::value_ptr(red));
			VAO_original_poly_edges.bindVertexArray();
			glDrawArrays(GL_LINES, 0, original_poly_edge_data.size() / 2);
		}

		ImGui::Begin(" Sample : Polygon triangulation to monotone pieces and Polygon triangulation");
		ImGui::Checkbox("Original Polygon", &show_points);
		ImGui::Checkbox("Monotone partitions", &show_monotone_partitioning);
		ImGui::Checkbox("Triangulation", &show_triangulation);
		ImGui::Checkbox("Triangulation faces", &show_tri_faces);
		ImGui::Checkbox("Triangulation Ear clipping", &show_tri_ear);

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