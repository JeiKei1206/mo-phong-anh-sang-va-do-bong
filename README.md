# 🌟 Mô Phỏng Chiếu Sáng 3D: Phong Shading & Neural Rendering

[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-5586A4?style=for-the-badge&logo=opengl&logoColor=white)](https://www.opengl.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

Dự án đồ họa máy tính tập trung vào việc nghiên cứu và triển khai các kỹ thuật chiếu sáng hiện đại, từ các công thức quang học truyền thống đến ứng dụng trí tuệ nhân tạo trong việc mô phỏng bề mặt vật liệu.

---

## 👥 Thông tin Nhóm thực hiện

| STT | Họ và Tên | MSSV | Vai trò |
| :--- | :--- | :--- | :--- |
| 1 | **Đinh Nguyễn Anh Khoa** | 102240088 | Trưởng nhóm, Cấu hình Shader |
| 2 | **Võ Thị Kim Tiền** | 102240113 | Xây dựng Model & Normal |
| 3 | **Nguyễn Trần Thắng Trung** | 102240118 | Nghiên cứu Neural Rendering |

---

## 🚀 Tính năng & Kết quả đạt được

### 1. Yêu cầu Cơ bản (Traditional Rendering)
- **Lý thuyết:** Tìm hiểu sâu về các mô hình chiếu sáng **Phong**, **Blinn-Phong**, **PBR** (Physically Based Rendering) và **Global Illumination**.
- **Thực thi:** Xây dựng ứng dụng C++/OpenGL thực hiện tô bóng Phong thời gian thực.
- **Mô hình:** Tối ưu hóa việc nạp và hiển thị các mô hình phức tạp như `heart.obj` (Trái tim) và `pokeball.obj` (Quả cầu).
- **Smooth Shading:** Triển khai thuật toán tự động tính toán **Smooth Normals** (Cross Product & Normal Averaging) cho các mô hình thiếu dữ liệu pháp tuyến, giúp bề mặt trơn mượt.

### 2. Yêu cầu Nâng cao (Neural Rendering)
- **AI Integration:** Ứng dụng mô hình mạng thần kinh (Neural Network) để dự đoán sự tương tác của ánh sáng trên bề mặt vật thể thay cho các công thức shader truyền thống.
- **PoC:** Triển khai mã nguồn Python (`neural_shader.py`) minh họa việc huấn luyện và dự đoán màu sắc pixel dựa trên dữ liệu đầu vào.

---

## 📊 So sánh: Blinn-Phong vs. Neural Rendering

| Đặc tính | Blinn-Phong (Truyền thống) | Neural Rendering (AI) |
| :--- | :--- | :--- |
| **Cơ sở logic** | Công thức toán học (Lamber, Blinn) | Mạng thần kinh (MLP/CNN) |
| **Tốc độ xử lý** | Cực nhanh (Tính toán phần cứng) | Phụ thuộc vào kiến trúc mạng |
| **Độ chân thực** | Xấp xỉ vật lý | Có thể học được các hiệu ứng phức tạp |
| **Độ mượt** | Phụ thuộc mật độ lưới (Mesh) | Có khả năng nội suy mượt mà hơn |
| **Tài nguyên** | Yêu cầu GPU Shader truyền thống | Yêu cầu Tensor Core / VRAM cao |

---

## 🎮 Hệ thống Điều khiển

Chương trình được thiết kế với bộ phím tắt linh hoạt để tương tác tối đa với không gian 3D.

### 🕹️ Điều khiển Camera (View Matrix)
*Camera luôn tập trung nhìn về trung tâm (0,0,0) của hai vật thể.*

| Phím | Hành động |
| :--- | :--- |
| **W** | Zoom Tiến (Lại gần vật thể) |
| **S** | Zoom Lùi (Ra xa vật thể) |
| **A** | Di chuyển Camera sang Trái |
| **D** | Di chuyển Camera sang Phải |

### 📦 Tương tác Vật thể (Model Matrix)
| Phím | Hành động |
| :--- | :--- |
| **I / K** | Di chuyển Model Lên / Xuống |
| **J / L** | Di chuyển Model sang Trái / Phải |
| **U / O** | Di chuyển Model Tiến / Lùi |
| **B / N / M** | Xoay Model quanh trục X / Y / Z |
| **Z / X** | Phóng to / Thu nhỏ tỷ lệ mô hình |
| **E** | Chuyển đổi kỹ thuật Shading (Phong ↔ Gouraud) |

---

## 🖼️ Demo Hình ảnh
> ![Kỹ thuật Tô bóng Phong (Phong Shading)](images/PhongShading.png)
> ![Kỹ thuật Tô bóng Gouraud (Gouraud Shading)](images/GouraudShading.png)

---

## 🛠️ Cài đặt Môi trường

Để chạy dự án này trên máy địa phương, bạn cần chuẩn bị:

1. **Công cụ lập trình:** 
   - Visual Studio 2019/2022 (C++ Desktop Development).
   - Python 3.8+ (Nếu muốn chạy module Neural Rendering).

2. **Cấu hình thư viện OpenGL:**
   - **GLFW 3:** Quản lý cửa sổ và Input.
   - **GLEW:** Nạp các hàm OpenGL mở rộng.
   - **GLM:** Thư viện toán học cho đồ họa (Matrix, Vector).

3. **Cấu hình Visual Studio:**
   - Thêm đường dẫn `include` vào `Additional Include Directories`.
   - Thêm đường dẫn `lib` vào `Additional Library Directories`.
   - Khai báo các file `.lib`: `opengl32.lib`, `glew32s.lib`, `glfw3.lib`.
   - Đảm bảo các file `.dll` đi kèm nằm cùng thư mục với file `.exe` sau khi build.

---
*Bản quyền © 2026 Nhóm Đồ họa Máy tính - Đại học Bách Khoa - ĐHĐN*
