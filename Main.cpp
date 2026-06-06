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
glm::vec3 cam_pos = glm::vec3(0, 2, 20);
glm::vec3 cam_dir = glm::vec3(0, 0, -1); // Hướng camera đang nhìn
glm::vec3 temp_cam_dir = glm::vec3(0, 0, 1); // Vector phụ trợ dùng cho tích có hướng
glm::vec3 cam_up = glm::vec3(0, 1, 0); // Hướng lên của camera

// Cấu hình Model (Mô hình 3D)
glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));
glm::vec3 modl_move = glm::vec3(0, 0, 0); // Vector dùng cho phép tịnh tiến

// Cấu hình cờ trạng thái Màu sắc và Ánh sáng
bool flag = false;
bool lights = false;
bool normalcol = false;
bool greyscale = false;
bool red = false;
bool green = false;
bool blue = false;
bool colour = false;
bool wireframe = false; 

// Hàm xử lý sự kiện nhấn phím từ GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
	
	// Nhấn ESC để thoát chương trình
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GL_TRUE); 

	// -----------------------------------------------------
	// Cụm phím W/A/S/D: Tịnh tiến Camera
	// -----------------------------------------------------
	if (key == GLFW_KEY_W) // Tiến tới
		cam_pos += cam_dir;

	if (key == GLFW_KEY_S) // Lùi lại
		cam_pos -= cam_dir;

	if (key == GLFW_KEY_D) // Sang phải
		cam_pos -= glm::cross(cam_up, cam_dir);

	if (key == GLFW_KEY_A) // Sang trái
		cam_pos += glm::cross(cam_up, cam_dir);

	// -----------------------------------------------------
	// Cụm phím Mũi tên: Xoay góc nhìn Camera
	// -----------------------------------------------------
	if (key == GLFW_KEY_RIGHT) // Xoay camera sang trái quanh trục Y
		cam_dir = glm::mat3(glm::rotate(glm::radians(0.8f), -cam_up)) * cam_dir;

	if (key == GLFW_KEY_LEFT) // Xoay camera sang phải quanh trục Y
		cam_dir = glm::mat3(glm::rotate(glm::radians(0.8f), cam_up)) * cam_dir;

	glm::vec3 rotate_around_x = glm::cross(cam_up, cam_dir); // Trục X nội bộ của camera
	
	if (key == GLFW_KEY_UP) { // Xoay camera ngước lên
		glm::mat4 rotation_matrix(1);
		rotation_matrix = glm::rotate(rotation_matrix, -(8 / 180.0f), rotate_around_x);
		cam_dir = glm::mat3(rotation_matrix) * cam_dir;
		cam_up = glm::mat3(rotation_matrix) * cam_up;
	}

	if (key == GLFW_KEY_DOWN) { // Xoay camera cúi xuống
		glm::mat4 rotation_matrix(1);
		rotation_matrix = glm::rotate(rotation_matrix, (8 / 180.0f), rotate_around_x);
		cam_dir = glm::mat3(rotation_matrix) * cam_dir;
		cam_up = glm::mat3(rotation_matrix) * cam_up;
	}

	// -----------------------------------------------------
	// Cụm phím I/J/K/L/PgUp/PgDn: Tịnh tiến Mô hình
	// -----------------------------------------------------
	if (key == GLFW_KEY_I) // Di chuyển lên theo trục +Y
		modl_move.y += 1;

	if (key == GLFW_KEY_K) // Di chuyển xuống theo trục -Y
		modl_move.y -= 1;

	if (key == GLFW_KEY_J) // Di chuyển sang trái theo trục -X
		modl_move.x -= 1;

	if (key == GLFW_KEY_L) // Di chuyển sang phải theo trục +X
		modl_move.x += 1;

	if (key == GLFW_KEY_PAGE_UP) // Di chuyển gần lại theo trục +Z
		modl_move.z += 1;

	if (key == GLFW_KEY_PAGE_DOWN) // Di chuyển xa ra theo trục -Z
		modl_move.z -= 1;

	// -----------------------------------------------------
	// Cụm phím B/N/E: Xoay Mô hình
	// -----------------------------------------------------
	if (key == GLFW_KEY_B) // Xoay quanh trục X
		model = glm::rotate(model, glm::radians(5.f), glm::vec3(1, 0, 0));

	if (key == GLFW_KEY_N) // Xoay quanh trục Y
		model = glm::rotate(model, glm::radians(5.f), glm::vec3(0, 1, 0));

	if (key == GLFW_KEY_E) // Xoay quanh trục Z
		model = glm::rotate(model, glm::radians(5.f), glm::vec3(0, 0, 1));

	// -----------------------------------------------------
	// Cụm phím O/P: Thu phóng Mô hình (Scale)
	// -----------------------------------------------------
	if (key == GLFW_KEY_O) // Phóng to 10%
		model = glm::scale(model, glm::vec3(1.1f));

	if (key == GLFW_KEY_P) // Thu nhỏ 10%
		model = glm::scale(model, glm::vec3(0.9f));

	// -----------------------------------------------------
	// Cụm phím 1->6, M, G: Điều khiển Kênh màu và Ánh sáng
	// -----------------------------------------------------
	
	// Phím 1: Bật/tắt kênh màu Đỏ (Red)
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		if (red != true) red = true;
		else red = false;
	}

	// Phím 2: Bật/tắt kênh màu Xanh lá (Green)
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		if (green != true) green = true;
		else green = false;
	}

	// Phím 3: Bật/tắt kênh màu Xanh dương (Blue)
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		if (blue != true) blue = true;
		else blue = false;
	}

	// Phím 4: Bật/tắt hiển thị màu sắc đầy đủ (Full RGB)
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		if (colour != true) {
			colour = true;
			// Tắt các màu đơn sắc khi bật Full RGB
			red = false;
			green = false;
			blue = false;
		}
		else {
			colour = false;
		}
	}

	// Phím 5: Chuyển đổi qua lại giữa Gouraud (Vertex) và Phong (Fragment) Shading
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		if (flag == false) flag = true;
		else flag = false;
	}

	// Phím 6: Bật/tắt hệ thống Ánh sáng
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		if (lights == false) lights = true;
		else lights = false;
	}

	// Phím M: Bật/tắt chế độ hiển thị màu sắc theo vector Pháp tuyến (Normal map)
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		if (normalcol == false) normalcol = true;
		else normalcol = false;
	}

	// Phím G: Bật/tắt chế độ hiển thị Đen trắng (Greyscale)
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		if (greyscale == false) greyscale = true;
		else greyscale = false;
	}
}

double last_y_pos = 0;

// Hàm xử lý sự kiện di chuyển chuột: Giữ chuột trái kéo lên/xuống để Zoom in/out
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	bool lbutton_pressed;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		lbutton_pressed = true;
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		lbutton_pressed = false;

	if (lbutton_pressed) {
		if (last_y_pos - ypos > 0) { // Chuột kéo lên -> Camera lùi lại
			cam_pos -= cam_dir;
			last_y_pos = ypos;
		}

		if (last_y_pos - ypos < 0) { // Chuột kéo xuống -> Camera tiến tới
			cam_pos += cam_dir;
			last_y_pos = ypos;
		}
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

	// Khởi tạo cửa sổ hiển thị
	window = glfwCreateWindow(WIDTH, HEIGHT, "102240088 - 102240113 - 102240118", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window); 
	
	glEnable(GL_DEPTH_TEST);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}

// Hàm MAIN: Điểm bắt đầu của ứng dụng và vòng lặp đồ họa
int main()
{
	if (init() != 0)
		return EXIT_FAILURE;
	
	// Khai báo các hàm callback
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Biên dịch và sử dụng Shader
	GLuint shader = loadSHADER("vertex.shader", "fragment.shader");
	glUseProgram(shader);

	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	
	// Nạp mô hình 3D
	loadOBJ("pokeball.obj", indices, vertices, normals, UVs);

	// Khởi tạo Vertex Array Object (VAO)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); 

	// Khởi tạo Vertex Buffer Object (VBO) cho tọa độ đỉnh
	GLuint vertices_VBO;
	glGenBuffers(1, &vertices_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);  
	glEnableVertexAttribArray(0);

	// Khởi tạo VBO cho vector pháp tuyến (Normals)
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Khởi tạo Element Buffer Object (EBO)
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW);
	
	glBindVertexArray(0); 

	// Plane VAO/VBO cho Shadow Mapping
	float planeVertices[] = {
		// positions            // normals         
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f
	};
	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// Cấu hình Framebuffer (FBO) cho Depth Map
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint depthShader = loadSHADER("depth_vertex.shader", "depth_fragment.shader");

	// Khởi tạo các ma trận chuyển đổi không gian
	glm::mat4 modl_matrix = glm::translate(glm::mat4(1.f), glm::vec3(3, 0, 0));
	glm::mat4 view_matrix = glm::lookAt(cam_pos, cam_pos + cam_dir, cam_up);
	glm::mat4 proj_matrix = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, 0.1f, 200.f);
	glEnable(GL_DEPTH_TEST); 

	// Lấy địa chỉ các biến Uniform từ Shader
	GLuint vm_loc = glGetUniformLocation(shader, "vm");
	GLuint pm_loc = glGetUniformLocation(shader, "pm");
	GLuint mm_loc = glGetUniformLocation(shader, "mm");
	GLuint flag_id = glGetUniformLocation(shader, "flag");
	GLuint lights_id = glGetUniformLocation(shader, "lights");
	GLuint normalcol_id = glGetUniformLocation(shader, "normalcol");
	GLuint greyscale_id = glGetUniformLocation(shader, "greyscale");
	GLuint red_id = glGetUniformLocation(shader, "red");
	GLuint green_id = glGetUniformLocation(shader, "green");
	GLuint blue_id = glGetUniformLocation(shader, "blue");
	GLuint colour_id = glGetUniformLocation(shader, "colour");

	// Truyền giá trị ban đầu xuống Shader
	glUniformMatrix4fv(vm_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(pm_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix));
	glUniformMatrix4fv(mm_loc, 1, GL_FALSE, glm::value_ptr(modl_matrix));

	glUniform3fv(glGetUniformLocation(shader, "light_color"), 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
	glUniform3fv(glGetUniformLocation(shader, "light_position"), 1, glm::value_ptr(glm::vec3(0.0, 20.0, 5.0)));
	glUniform3fv(glGetUniformLocation(shader, "object_color"), 1, glm::value_ptr(glm::vec3(1.0, 0.5, 0.2)));
	glUniform3fv(glGetUniformLocation(shader, "view_position"), 1, glm::value_ptr(glm::vec3(cam_pos)));


	// Vòng lặp Game (Render Loop)
	while (!glfwWindowShouldClose(window))
	{
		// Trong vòng lặp while (!glfwWindowShouldClose(window))
		glUniform3fv(glGetUniformLocation(shader, "light_position"), 1, glm::value_ptr(cam_pos));
		glUniform3fv(glGetUniformLocation(shader, "view_position"), 1, glm::value_ptr(cam_pos));
		// Cập nhật các sự kiện đầu vào
		glfwPollEvents();

		// Cập nhật ma trận View (Camera)
		view_matrix = glm::lookAt(cam_pos, glm::vec3(0, 0, 0), cam_up);
		glUniformMatrix4fv(vm_loc, 1, 0, glm::value_ptr(view_matrix));

		// Áp dụng phép tịnh tiến và cập nhật ma trận Model
		glm::mat4 translator = glm::translate(glm::mat4(1.0f), modl_move);
		modl_matrix = translator * model;
		glUniformMatrix4fv(mm_loc, 1, 0, glm::value_ptr(modl_matrix));
		
		// Render khung hình
		// 1. Render depth of scene to texture (from light's perspective)
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 70.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(cam_pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		glUseProgram(depthShader);
		glUniformMatrix4fv(glGetUniformLocation(depthShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Vẽ sàn (Floor) cho depth pass
		glm::mat4 floorModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(depthShader, "mm"), 1, GL_FALSE, glm::value_ptr(floorModel));
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Vẽ object cho depth pass
		glUniformMatrix4fv(glGetUniformLocation(depthShader, "mm"), 1, GL_FALSE, glm::value_ptr(modl_matrix));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. Render scene as normal using the generated depth/shadow map  
		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shader, "shadowMap"), 0);

		glUniform3fv(glGetUniformLocation(shader, "light_position"), 1, glm::value_ptr(cam_pos));
		glUniform3fv(glGetUniformLocation(shader, "view_position"), 1, glm::value_ptr(cam_pos));
		glUniformMatrix4fv(vm_loc, 1, 0, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(pm_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix));

		// Truyền trạng thái các phím điều khiển màu/đèn xuống Shader
		glUniform1i(flag_id, flag);
		glUniform1i(lights_id, lights);
		glUniform1i(normalcol_id, normalcol);
		glUniform1i(greyscale_id, greyscale);
		glUniform1i(red_id, red);
		glUniform1i(green_id, green);
		glUniform1i(blue_id, blue);
		glUniform1i(colour_id, colour);

		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Vẽ sàn (Floor) cho rendering pass
		glUniformMatrix4fv(mm_loc, 1, GL_FALSE, glm::value_ptr(floorModel));
		glUniform3fv(glGetUniformLocation(shader, "object_color"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Vẽ mô hình dựa trên các chỉ số (Indices)
		glUniformMatrix4fv(mm_loc, 1, 0, glm::value_ptr(modl_matrix));
		glUniform3fv(glGetUniformLocation(shader, "object_color"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.2f)));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		// Đẩy bộ đệm màu ra màn hình
		glfwSwapBuffers(window);
	}

	// Kết thúc GLFW, dọn dẹp bộ nhớ
	glfwTerminate();
	return 0;
}
