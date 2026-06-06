# 🌟 Mô Phỏng Chiếu Sáng 3D: Phong vs. Gouraud Shading & Neural Rendering

[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-5586A4?style=for-the-badge&logo=opengl&logoColor=white)](https://www.opengl.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

Dự án Đồ họa Máy tính tập trung vào việc nghiên cứu và thực nghiệm các kỹ thuật chiếu sáng kinh điển (Traditional Shading) và ứng dụng trí tuệ nhân tạo (Neural Rendering). Điểm nổi bật của dự án là khả năng so sánh trực quan giữa các phương pháp đổ bóng thông qua kiến trúc mã nguồn phân tách rõ ràng.

---

## 👥 Thông tin Nhóm thực hiện

| STT | Họ và Tên | MSSV | Vai trò |
| :--- | :--- | :--- | :--- |
| 1 | **Đinh Nguyễn Anh Khoa** | 102240088 | Trưởng nhóm, Cấu hình Shader |
| 2 | **Võ Thị Kim Tiền** | 102240113 | Xây dựng Model & Normal |
| 3 | **Nguyễn Trần Thắng Trung** | 102240118 | Nghiên cứu Neural Rendering |

---

## 🚀 Tính năng Nổi bật

### 1. Kỹ thuật Đổ bóng Kép (Dual Shading Architecture)
Dự án không chỉ thay đổi công thức tính toán trong Shader mà còn thay đổi cả **cấu trúc dữ liệu Mesh** đầu vào để minh họa rõ nét sự khác biệt:
- **Phong Shading (Smooth Mode):** Sử dụng hàm `loadPhong` để thực hiện *Vertex Normal Averaging* (trung bình cộng pháp tuyến đỉnh). Kết quả mang lại bề mặt trơn nhẵn, mượt mà hoàn hảo.
- **Gouraud Shading (Faceted Mode):** Sử dụng hàm `loadGouraud` để tạo ra *Face Normals* (pháp tuyến mặt). Kết quả hiển thị rõ các cạnh đa giác, mang lại vẻ ngoài góc cạnh (faceted), giúp người học hiểu rõ bản chất của việc tính toán ánh sáng tại đỉnh.

### 2. Hệ thống Chuyển đổi Mô hình Linh hoạt
Hỗ trợ nạp và hiển thị thời gian thực 3 loại mô hình phổ biến trong đồ họa máy tính:
- **Quả cầu (Pokeball):** Minh họa đổ bóng trên bề mặt cong đơn giản.
- **Trái tim (Heart):** Minh họa đổ bóng trên bề mặt hữu cơ phức tạp.
- **Ấm trà (Teapot):** Mô hình kinh điển trong đồ họa 3D.

---

## 🎮 Hệ thống Điều khiển

Chương trình cung cấp bộ phím tắt mạnh mẽ để tương tác và so sánh:

### 🔄 Chuyển đổi Chế độ & Mô hình
| Phím | Hành động |
| :--- | :--- |
| **1** | Hiển thị mô hình **Pokeball** |
| **2** | Hiển thị mô hình **Heart** |
| **3** | Hiển thị mô hình **Teapot** |
| **E** | Chuyển đổi kỹ thuật Shading (**Phong Mịn ↔ Gouraud Gãy**) |

### 🕹️ Điều khiển Camera (View Matrix)
| Phím | Hành động |
| :--- | :--- |
| **W / S** | Zoom Tiến / Lùi |
| **A / D** | Di chuyển Camera sang Trái / Phải |
| **Mũi tên** | Xoay hướng nhìn của Camera |
| **Chuột trái** | Kéo thả để di chuyển Camera theo trục nhìn |

### 📦 Tương tác Vật thể (Model Matrix)
| Phím | Hành động |
| :--- | :--- |
| **I / K** | Di chuyển vật thể Lên / Xuống |
| **J / L** | Di chuyển vật thể sang Trái / Phải |
| **U / O** | Di chuyển vật thể Tiến / Lùi |
| **B / N / M** | Xoay vật thể quanh các trục X / Y / Z |
| **Z / X** | Phóng to / Thu nhỏ tỷ lệ vật thể |

---

## 🖼️ Demo Hình ảnh
**1. Phong Shading quả bóng:**

![Phong Shading Ball](images/Ball.png)

**2. Phong Shading trái tim:**

![Phong Shading Heart](images/Heart.png)

**3. Phong Shading ấm trà:**

![Phong Shading Teapot](images/Teapot.png)

---

## 🛠️ Cài đặt & Biên dịch

1. **Yêu cầu:** Visual Studio 2019 trở lên, hỗ trợ C++11/14.
2. **Thư viện tích hợp:** 
   - **GLEW & GLFW:** Đã bao gồm trong thư mục `packages` hoặc nạp qua NuGet.
   - **GLM:** Thư viện toán học cho Matrix/Vector.
3. **Cấu hình:** 
   - Đảm bảo các file `.dll` (`glew32.dll`, `glfw3.dll`) nằm cùng thư mục với file `.exe`.
   - Các file `.shader` và `.obj` phải nằm trong thư mục làm việc của ứng dụng.

---
*Bản quyền © 2026 Nhóm Đồ họa Máy tính - Trường Công nghệ Thông tin & Truyền thông - ĐH Bách Khoa - ĐHĐN*
---