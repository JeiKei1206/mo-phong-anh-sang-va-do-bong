#include <cstring>
#include <vector>
#include <string>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <map>

// Cấu trúc so sánh để gộp các đỉnh trùng vị trí
struct Vec3Less {
	bool operator()(const glm::vec3& v1, const glm::vec3& v2) const {
		if (v1.x < v2.x - 0.0001f) return true;
		if (v1.x > v2.x + 0.0001f) return false;
		if (v1.y < v2.y - 0.0001f) return true;
		if (v1.y > v2.y + 0.0001f) return false;
		if (v1.z < v2.z - 0.0001f) return true;
		return false;
	}
};

// Hàm nạp dữ liệu cho Phong Shading (Mịn - Smooth)
// Tính toán trung bình pháp tuyến đỉnh (Vertex Normal Averaging) bằng cách gộp đỉnh trùng vị trí
bool loadPhong(
	const char * path,
	std::vector<int> & vertexIndices,
	std::vector<glm::vec3> & temp_vertices,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec2> & out_uvs){

	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE * file;
    int _errno = fopen_s(&file, path, "r");
	if (_errno != 0) return false;

	while (1){
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader,128);
		if (res == EOF) break;

		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "f") == 0){
			int vertexIndex[3];
            char line [128]; fgets(line, 128, file);
            int matches = sscanf_s(line, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
            if (matches != 3){
                matches = sscanf_s(line, "%d//%*d %d//%*d %d//%*d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                if (matches != 3){
                    matches = sscanf_s(line, "%d/%*d %d/%*d %d/%*d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                    if (matches != 3) sscanf_s(line, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                }
            }
            vertexIndices.push_back(abs(vertexIndex[0])-1);
            vertexIndices.push_back(abs(vertexIndex[1])-1);
            vertexIndices.push_back(abs(vertexIndex[2])-1);
		}
		else { char clear[1000]; fgets(clear, 1000, file); }
	}
	fclose(file);

	// Gộp các đỉnh trùng tọa độ để tính trung bình pháp tuyến
	std::map<glm::vec3, int, Vec3Less> posToUniqueIdx;
	std::vector<int> mapping(temp_vertices.size());
	std::vector<glm::vec3> unique_vertices;
	for (int i = 0; i < (int)temp_vertices.size(); ++i) {
		glm::vec3 p = temp_vertices[i];
		if (posToUniqueIdx.find(p) == posToUniqueIdx.end()) {
			posToUniqueIdx[p] = (int)unique_vertices.size();
			unique_vertices.push_back(p);
		}
		mapping[i] = posToUniqueIdx[p];
	}

	std::vector<glm::vec3> unique_normals(unique_vertices.size(), glm::vec3(0.0f));
	for (unsigned int i = 0; i < vertexIndices.size(); i += 3) {
		int v0 = vertexIndices[i]; int v1 = vertexIndices[i + 1]; int v2 = vertexIndices[i + 2];
		glm::vec3 p0 = temp_vertices[v0]; glm::vec3 p1 = temp_vertices[v1]; glm::vec3 p2 = temp_vertices[v2];
		glm::vec3 normal = glm::cross(p1 - p0, p2 - p0);
		unique_normals[mapping[v0]] += normal;
		unique_normals[mapping[v1]] += normal;
		unique_normals[mapping[v2]] += normal;
	}

	out_normals.resize(temp_vertices.size());
	for (unsigned int i = 0; i < temp_vertices.size(); i++) {
		glm::vec3 n = unique_normals[mapping[i]];
		out_normals[i] = (glm::length(n) > 0.0001f) ? glm::normalize(n) : glm::vec3(0, 1, 0);
	}
	return true;
}

// Hàm nạp dữ liệu cho Gouraud Shading (Gãy khúc - Faceted)
// Tính toán pháp tuyến mặt (Face Normals) và gán cho từng đỉnh của tam giác
bool loadGouraud(
	const char * path,
	std::vector<int> & vertexIndices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec2> & out_uvs){

	std::vector<glm::vec3> temp_vertices;
	std::vector<int> temp_indices;

	FILE * file;
    int _errno = fopen_s(&file, path, "r");
	if (_errno != 0) return false;

	while (1){
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader,128);
		if (res == EOF) break;

		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "f") == 0){
			int vIdx[3];
            char line [128]; fgets(line, 128, file);
            int matches = sscanf_s(line, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d\n", &vIdx[0], &vIdx[1], &vIdx[2]);
            if (matches != 3) {
                matches = sscanf_s(line, "%d//%*d %d//%*d %d//%*d\n", &vIdx[0], &vIdx[1], &vIdx[2]);
                if (matches != 3) {
                    matches = sscanf_s(line, "%d/%*d %d/%*d %d/%*d\n", &vIdx[0], &vIdx[1], &vIdx[2]);
                    if (matches != 3) sscanf_s(line, "%d %d %d\n", &vIdx[0], &vIdx[1], &vIdx[2]);
                }
            }
            temp_indices.push_back(abs(vIdx[0])-1);
            temp_indices.push_back(abs(vIdx[1])-1);
            temp_indices.push_back(abs(vIdx[2])-1);
		}
		else { char clear[1000]; fgets(clear, 1000, file); }
	}
	fclose(file);

	// Tạo các đỉnh độc lập cho mỗi mặt để đạt hiệu ứng gãy khúc
	for (unsigned int i = 0; i < temp_indices.size(); i += 3) {
		glm::vec3 p0 = temp_vertices[temp_indices[i]];
		glm::vec3 p1 = temp_vertices[temp_indices[i+1]];
		glm::vec3 p2 = temp_vertices[temp_indices[i+2]];
		glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

		int startIdx = (int)out_vertices.size();
		out_vertices.push_back(p0);
		out_vertices.push_back(p1);
		out_vertices.push_back(p2);
		out_normals.push_back(normal);
		out_normals.push_back(normal);
		out_normals.push_back(normal);
		vertexIndices.push_back(startIdx);
		vertexIndices.push_back(startIdx + 1);
		vertexIndices.push_back(startIdx + 2);
	}
	return true;
}