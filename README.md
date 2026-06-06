# OpenGL 3D Model Viewer: Phong & Gouraud Lighting Simulation

Dự án mô phỏng hệ thống chiếu sáng và tương tác với mô hình 3D trong không gian ba chiều sử dụng ngôn ngữ C++ và thư viện đồ họa OpenGL. Chương trình cho phép người dùng nạp mô hình từ file `.obj`, tùy chỉnh các chế độ Shading (Phong/Gouraud), điều khiển kênh màu và tương tác Camera linh hoạt.

## 🚀 Các tính năng chính

- **Nạp mô hình 3D:** Hỗ trợ đọc dữ liệu từ file định dạng `.obj` (Vertices, Normals, Indices).
- **Hệ thống chiếu sáng (Local Illumination):** 
    - Chuyển đổi linh hoạt giữa **Phong/Blinn-Phong Shading** (tính toán tại từng pixel) và **Gouraud Shading** (tính toán tại đỉnh).
    - Mô phỏng đầy đủ các thành phần ánh sáng: Ambient, Diffuse và Specular (sử dụng Halfway Vector của Blinn-Phong để xử lý ánh sáng chói mượt mà hơn).
- **Tương tác thời gian thực:** 
    - Điều khiển Camera theo phong cách FPS (di chuyển và xoay góc nhìn).
    - Biến đổi mô hình (Transformation): Tịnh tiến, xoay 3 trục và thu phóng.
- **Hiệu ứng hình ảnh & Màu sắc:**
    - Bật/tắt hệ thống ánh sáng.
    - Chế độ hiển thị đa dạng: Wireframe, Normal Mapping, Greyscale (Đen trắng).
    - Tùy chỉnh các kênh màu đơn sắc (Red, Green, Blue) hoặc Full RGB.
- **Demo Mở rộng (Neural Rendering):** 
    - Tích hợp bản mẫu mô phỏng Shader bằng AI (PyTorch - `neural_shader.py`) để dự đoán màu sắc RGB thay cho công thức quang học truyền thống.

## 🎮 Phím tắt điều khiển

Hệ thống điều khiển được thiết kế tối ưu cho việc quan sát và tùy chỉnh mô hình:

### 1. Điều khiển Camera (Góc nhìn)
| Phím | Chức năng |
| :--- | :--- |
| **W / S** | Di chuyển Camera Tiến / Lùi |
| **A / D** | Di chuyển Camera sang Trái / Phải |
| **Mũi tên Trái / Phải** | Xoay góc nhìn sang Trái / Phải (Yaw) |
| **Mũi tên Lên / Xuống** | Xoay góc nhìn Ngước lên / Cúi xuống (Pitch) |
| **Chuột trái + Kéo dọc** | Thu nhỏ / Phóng to khoảng cách quan sát (Zoom) |

### 2. Tương tác Mô hình (Transformation)
| Phím | Chức năng |
| :--- | :--- |
| **I / K** | Di chuyển Model Lên / Xuống (Trục Y) |
| **J / L** | Di chuyển Model sang Trái / Phải (Trục X) |
| **Page Up / Page Down** | Di chuyển Model Gần lại / Xa ra (Trục Z) |
| **B / N / E** | Xoay Model quanh trục X / Y / Z |
| **O / P** | Phóng to / Thu nhỏ tỷ lệ mô hình (Scale) |

### 3. Hiệu ứng Shading & Màu sắc
| Phím | Chức năng |
| :--- | :--- |
| **1 / 2 / 3** | Bật/tắt kênh màu Đỏ (Red) / Xanh lá (Green) / Xanh dương (Blue) |
| **4** | Chế độ hiển thị màu sắc đầy đủ (Full RGB) |
| **5** | Chuyển đổi kỹ thuật Shading (**Phong** ↔ **Gouraud**) |
| **6** | Bật / Tắt hệ thống chiếu sáng |
| **M** | Hiển thị màu theo Vector pháp tuyến (Normal Mapping) |
| **G** | Bật / Tắt chế độ đen trắng (Greyscale) |
| **ESC** | Thoát chương trình |

## 🛠 Môi trường & Thư viện sử dụng

Để biên dịch và chạy dự án, hệ thống cần được cài đặt các thành phần sau:
- **Ngôn ngữ:** C++ (chuẩn C++11 trở lên) và Python (cho thử nghiệm Neural Rendering).
- **Đồ họa:** OpenGL 4.3 (Core Profile).
- **Thư viện quản lý cửa sổ:** GLFW 3.
- **Thư viện nạp hàm OpenGL:** GLEW.
- **Thư viện toán học:** GLM (OpenGL Mathematics).

## 📂 Cấu trúc dự án

- `Main.cpp`: Chứa mã nguồn chính, vòng lặp Render, xử lý sự kiện đầu vào và thiết lập VAO/VBO.
- `vertex.shader`: Xử lý biến đổi tọa độ đỉnh và tính toán ánh sáng cho chế độ Gouraud.
- `fragment.shader`: Xử lý màu sắc pixel, tính toán ánh sáng cho chế độ Blinn-Phong và các bộ lọc màu.
- `objloaderIndex.h`: Bộ nạp dữ liệu từ file `.obj` tùy chỉnh, hỗ trợ Indexing để tối ưu bộ nhớ.
- `shaderloader.h`: Tiện ích hỗ trợ đọc, biên dịch và liên kết các file shader.
- `neural_shader.py`: Tập lệnh Python minh họa Proof of Concept về Neural Rendering.

## 🔨 Hướng dẫn cài đặt & Chạy

1. **Chuẩn bị:** Cài đặt Visual Studio (khuyến nghị 2019 hoặc mới hơn).
2. **Cấu hình thư viện:**
   - Tải và cấu hình các thư viện `glfw`, `glew`, `glm` vào thư mục `include` và `lib` của dự án.
   - Thêm các file `.dll` (`glew32.dll`, `glfw3.dll`) vào thư mục chứa file thực thi (.exe).
3. **Thiết lập dự án:**
   - Mở Solution trong Visual Studio.
   - Chỉnh cấu hình build sang **x64** và chế độ **Release** (hoặc Debug).
   - Trong `Project Properties > Linker > Input`, thêm: `opengl32.lib`, `glew32.lib`, `glfw3.lib`.
4. **Biên dịch dự án C++:** Nhấn `Ctrl + F5` để biên dịch và chạy chương trình.
5. **Chạy thử Neural Rendering:** Mở Terminal/Command Prompt, di chuyển đến thư mục dự án và chạy lệnh: `python neural_shader.py`

---
*Dự án được phát triển nhằm mục đích nghiên cứu các nguyên lý cơ bản của đồ họa máy tính 3D.*