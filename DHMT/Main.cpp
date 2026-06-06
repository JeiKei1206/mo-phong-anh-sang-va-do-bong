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
int activeModel = 1; // 1: Pokeball, 2: Heart, 3: Teapot

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

	// Chuyển đổi mô hình
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) activeModel = 1;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) activeModel = 2;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) activeModel = 3;
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

	window = glfwCreateWindow(WIDTH, HEIGHT, "Neural Rendering - 3 Models Switching", nullptr, nullptr);
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

struct MeshData {
	GLuint VAO, VBO_v, VBO_n, EBO;
	int indexCount;
};

MeshData setupPhongMesh(const char* path) {
	std::vector<int> ind;
	std::vector<glm::vec3> vert, norm;
	std::vector<glm::vec2> uv;
	loadPhong(path, ind, vert, norm, uv);

	MeshData mesh;
	mesh.indexCount = (int)ind.size();
	glGenVertexArrays(1, &mesh.VAO);
	glBindVertexArray(mesh.VAO);
	glGenBuffers(1, &mesh.VBO_v);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO_v);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &mesh.VBO_n);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO_n);
	glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &mesh.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(int), &ind.front(), GL_STATIC_DRAW);
	glBindVertexArray(0);
	return mesh;
}

MeshData setupGouraudMesh(const char* path) {
	std::vector<int> ind;
	std::vector<glm::vec3> vert, norm;
	std::vector<glm::vec2> uv;
	loadGouraud(path, ind, vert, norm, uv);

	MeshData mesh;
	mesh.indexCount = (int)ind.size();
	glGenVertexArrays(1, &mesh.VAO);
	glBindVertexArray(mesh.VAO);
	glGenBuffers(1, &mesh.VBO_v);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO_v);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &mesh.VBO_n);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO_n);
	glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &mesh.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(int), &ind.front(), GL_STATIC_DRAW);
	glBindVertexArray(0);
	return mesh;
}

int main()
{
	if (init() != 0) return EXIT_FAILURE;
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	GLuint shader = loadSHADER("vertex.shader", "fragment.shader");
	glUseProgram(shader);

	MeshData pokePhong = setupPhongMesh("pokeball.obj");
	MeshData pokeGouraud = setupGouraudMesh("pokeball.obj");
	MeshData heartPhong = setupPhongMesh("heart.obj");
	MeshData heartGouraud = setupGouraudMesh("heart.obj");
	MeshData teapotPhong = setupPhongMesh("teapot.obj");
	MeshData teapotGouraud = setupGouraudMesh("teapot.obj");

	glm::mat4 proj_matrix = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, 0.1f, 200.f);
	glm::vec3 light_pos = glm::vec3(5.0f, 10.0f, 5.0f);

	GLuint vm_loc = glGetUniformLocation(shader, "vm");
	GLuint pm_loc = glGetUniformLocation(shader, "pm");
	GLuint mm_loc = glGetUniformLocation(shader, "mm");
	GLuint is_poke_loc = glGetUniformLocation(shader, "is_pokeball");

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

		glm::mat4 global_transform = glm::translate(glm::mat4(1.0f), modl_move) * model;

		if (activeModel == 1) { // Pokeball
			glUniform1i(is_poke_loc, true);
			glm::mat4 poke_model = glm::rotate(global_transform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			poke_model = glm::scale(poke_model, glm::vec3(0.03f)); 
			glUniformMatrix4fv(mm_loc, 1, 0, glm::value_ptr(poke_model));
			if (flag) {
				glBindVertexArray(pokeGouraud.VAO);
				glDrawElements(GL_TRIANGLES, pokeGouraud.indexCount, GL_UNSIGNED_INT, 0);
			} else {
				glBindVertexArray(pokePhong.VAO);
				glDrawElements(GL_TRIANGLES, pokePhong.indexCount, GL_UNSIGNED_INT, 0);
			}
		} 
		else if (activeModel == 2) { // Heart
			glUniform1i(is_poke_loc, false);
			glm::mat4 heart_model = glm::rotate(global_transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			heart_model = glm::scale(heart_model, glm::vec3(0.15f)); 
			glUniformMatrix4fv(mm_loc, 1, 0, glm::value_ptr(heart_model));
			glUniform3fv(glGetUniformLocation(shader, "object_color"), 1, glm::value_ptr(glm::vec3(0.9f, 0.2f, 0.3f))); 
			if (flag) {
				glBindVertexArray(heartGouraud.VAO);
				glDrawElements(GL_TRIANGLES, heartGouraud.indexCount, GL_UNSIGNED_INT, 0);
			} else {
				glBindVertexArray(heartPhong.VAO);
				glDrawElements(GL_TRIANGLES, heartPhong.indexCount, GL_UNSIGNED_INT, 0);
			}
		}
		else if (activeModel == 3) { // Teapot
			glUniform1i(is_poke_loc, false);
			glm::mat4 teapot_model = glm::rotate(global_transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			teapot_model = glm::scale(teapot_model, glm::vec3(1.5f)); 
			glUniformMatrix4fv(mm_loc, 1, 0, glm::value_ptr(teapot_model));
			glUniform3fv(glGetUniformLocation(shader, "object_color"), 1, glm::value_ptr(glm::vec3(0.2f, 0.6f, 0.9f))); 
			if (flag) {
				glBindVertexArray(teapotGouraud.VAO);
				glDrawElements(GL_TRIANGLES, teapotGouraud.indexCount, GL_UNSIGNED_INT, 0);
			} else {
				glBindVertexArray(teapotPhong.VAO);
				glDrawElements(GL_TRIANGLES, teapotPhong.indexCount, GL_UNSIGNED_INT, 0);
			}
		}

		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}