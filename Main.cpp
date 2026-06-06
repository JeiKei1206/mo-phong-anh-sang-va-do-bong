#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "objloaderIndex.h"
#include "shaderloader.h"

using namespace std;

// Cấu hình kích thước cửa sổ
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow *window;

// Cấu hình Camera
glm::vec3 cam_pos = glm::vec3(0, 0, 20);
glm::vec3 cam_dir = glm::vec3(0, 0, -1);
glm::vec3 temp_cam_dir = glm::vec3(0, 0, 1);
glm::vec3 cam_up = glm::vec3(0, 1, 0);

// Cấu hình Model (Mô hình 3D)
glm::mat4 model = glm::mat4(1.0f);
glm::vec3 modl_move = glm::vec3(0, 0, 0);

// Cấu hình cờ trạng thái Màu sắc và Ánh sáng
bool wireframe = false; 
bool flag = false;

// Hàm xử lý sự kiện nhấn phím từ GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
	if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE); 

	if (key == GLFW_KEY_W) cam_pos.z -= 1.0f;
	if (key == GLFW_KEY_S) cam_pos.z += 1.0f;
	if (key == GLFW_KEY_A) cam_pos.x -= 1.0f;
	if (key == GLFW_KEY_D) cam_pos.x += 1.0f;

	if (key == GLFW_KEY_RIGHT) cam_dir = glm::mat3(glm::rotate(glm::radians(0.8f), -cam_up)) * cam_dir;
	if (key == GLFW_KEY_LEFT) cam_dir = glm::mat3(glm::rotate(glm::radians(0.8f), cam_up)) * cam_dir;

	glm::vec3 rotate_around_x = glm::cross(cam_up, cam_dir); 
	if (key == GLFW_KEY_UP) {
		glm::mat4 rotation_matrix(1);
		rotation_matrix = glm::rotate(rotation_matrix, -(8 / 180.0f), rotate_around_x);
		cam_dir = glm::mat3(rotation_matrix) * cam_dir;
		cam_up = glm::mat3(rotation_matrix) * cam_up;
	}
	if (key == GLFW_KEY_DOWN) {
		glm::mat4 rotation_matrix(1);
		rotation_matrix = glm::rotate(rotation_matrix, (8 / 180.0f), rotate_around_x);
		cam_dir = glm::mat3(rotation_matrix) * cam_dir;
		cam_up = glm::mat3(rotation_matrix) * cam_up;
	}

	if (key == GLFW_KEY_I) modl_move.y += 1;
	if (key == GLFW_KEY_K) modl_move.y -= 1;
	if (key == GLFW_KEY_J) modl_move.x -= 1;
	if (key == GLFW_KEY_L) modl_move.x += 1;
	if (key == GLFW_KEY_U) modl_move.z += 1;
	if (key == GLFW_KEY_O) modl_move.z -= 1;

	if (key == GLFW_KEY_B) model = glm::rotate(model, glm::radians(5.f), glm::vec3(1, 0, 0));
	if (key == GLFW_KEY_N) model = glm::rotate(model, glm::radians(5.f), glm::vec3(0, 1, 0));
	if (key == GLFW_KEY_M) model = glm::rotate(model, glm::radians(5.f), glm::vec3(0, 0, 1));

	if (key == GLFW_KEY_Z) model = glm::scale(model, glm::vec3(1.1f));
	if (key == GLFW_KEY_X) model = glm::scale(model, glm::vec3(0.9f));
	
	if (key == GLFW_KEY_E && action == GLFW_PRESS) flag = !flag;
}

double last_y_pos = 0;
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	bool lbutton_pressed;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) lbutton_pressed = true;
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) lbutton_pressed = false;

	if (lbutton_pressed) {
		if (last_y_pos - ypos > 0) cam_pos -= cam_dir;
		if (last_y_pos - ypos < 0) cam_pos += cam_dir;
		last_y_pos = ypos;
	}
}

int init() {
	std::cout << "Starting GLFW context, OpenGL 4.3" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Neural Rendering - 2 Models", nullptr, nullptr);
	if (nullptr == window) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window); 
	glEnable(GL_DEPTH_TEST);
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}

int main()
{
	if (init() != 0) return EXIT_FAILURE;
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	GLuint shader = loadSHADER("vertex.shader", "fragment.shader");
	glUseProgram(shader);

	// Load Model 1: Pokeball
	std::vector<int> ind1;
	std::vector<glm::vec3> vert1, norm1;
	std::vector<glm::vec2> uv1;
	loadOBJ("pokeball.obj", ind1, vert1, norm1, uv1);

	GLuint VAO1, VBO1_v, VBO1_n, EBO1;
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1); 
	glGenBuffers(1, &VBO1_v);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1_v);
	glBufferData(GL_ARRAY_BUFFER, vert1.size() * sizeof(glm::vec3), &vert1.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);  
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &VBO1_n);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1_n);
	glBufferData(GL_ARRAY_BUFFER, norm1.size() * sizeof(glm::vec3), &norm1.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind1.size() * sizeof(int), &ind1.front(), GL_STATIC_DRAW);
	glBindVertexArray(0); 

	// Load Model 2: Heart
	std::vector<int> ind2;
	std::vector<glm::vec3> vert2, norm2;
	std::vector<glm::vec2> uv2;
	loadOBJ("heart.obj", ind2, vert2, norm2, uv2);

	GLuint VAO2, VBO2_v, VBO2_n, EBO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2); 
	glGenBuffers(1, &VBO2_v);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2_v);
	glBufferData(GL_ARRAY_BUFFER, vert2.size() * sizeof(glm::vec3), &vert2.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);  
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &VBO2_n);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2_n);
	glBufferData(GL_ARRAY_BUFFER, norm2.size() * sizeof(glm::vec3), &norm2.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind2.size() * sizeof(int), &ind2.front(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	glm::mat4 proj_matrix = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, 0.1f, 200.f);
	glm::vec3 light_pos = glm::vec3(5.0f, 10.0f, 5.0f);

	GLuint vm_loc = glGetUniformLocation(shader, "vm");
	GLuint pm_loc = glGetUniformLocation(shader, "pm");
	GLuint mm_loc = glGetUniformLocation(shader, "mm");

	glUniformMatrix4fv(pm_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix));
	glUniform3fv(glGetUniformLocation(shader, "light_color"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glUniform3fv(glGetUniformLocation(shader, "light_position"), 1, glm::value_ptr(light_pos));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 view_matrix = glm::lookAt(cam_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(vm_loc, 1, 0, glm::value_ptr(view_matrix));
		glUniform3fv(glGetUniformLocation(shader, "view_position"), 1, glm::value_ptr(cam_pos));
		glUniform1i(glGetUniformLocation(shader, "flag"), flag);

		if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Global transformations from user input
		glm::mat4 global_transform = glm::translate(glm::mat4(1.0f), modl_move) * model;

		// 1. Draw Pokeball (Left)
		glm::mat4 poke_model = glm::translate(global_transform, glm::vec3(-4.0f, 0.0f, 0.0f));
		poke_model = glm::scale(poke_model, glm::vec3(0.03f)); // Scale down the pokeball further to match heart
		glUniformMatrix4fv(mm_loc, 1, 0, glm::value_ptr(poke_model));
		glUniform3fv(glGetUniformLocation(shader, "object_color"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.2f))); // Orange
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, ind1.size(), GL_UNSIGNED_INT, 0);

		// 2. Draw Heart (Right)
		glm::mat4 heart_model = glm::translate(global_transform, glm::vec3(4.0f, 0.0f, 0.0f));
		heart_model = glm::scale(heart_model, glm::vec3(0.05f)); // Scale down the heart, adjust as necessary if sizes differ wildly
		heart_model = glm::rotate(heart_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		heart_model = glm::scale(heart_model, glm::vec3(3.0f));
		glUniformMatrix4fv(mm_loc, 1, 0, glm::value_ptr(heart_model));
		glUniform3fv(glGetUniformLocation(shader, "object_color"), 1, glm::value_ptr(glm::vec3(0.9f, 0.2f, 0.3f))); // Red/Pink
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, ind2.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
