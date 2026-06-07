<div align="center">

# 🌟 Mô Phỏng Chiếu Sáng 3D
### Phong Shading & Neural Rendering

[![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl)](https://www.opengl.org/)
[![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54)](https://www.python.org/)
[![Visual Studio](https://img.shields.io/badge/Visual%20Studio-5C2D91.svg?style=for-the-badge&logo=visual-studio&logoColor=white)](https://visualstudio.microsoft.com/)

*Đồ án môn học: Đồ họa máy tính - Đại học Bách khoa, Đại học Đà Nẵng (DUT)*

</div>

## 👥 Nhóm Thực Hiện

| STT | Họ và Tên | Mã Sinh Viên |
| :---: | :--- | :---: |
| 1 | **Đinh Nguyễn Anh Khoa** | 102240088 |
| 2 | **Võ Thị Kim Tiền** | 102240113 |
| 3 | **Nguyễn Trần Thắng Trung** | 102240118 |

---

## 🎯 Giới Thiệu & Các Tính Năng Chính

Dự án tập trung vào việc mô phỏng ánh sáng chiếu lên vật thể 3D, ứng dụng cả các kỹ thuật truyền thống trên nền tảng OpenGL và kỹ thuật kết xuất đồ họa nâng cao bằng trí tuệ nhân tạo.

* 💡 **Kỹ thuật Truyền thống (C++/OpenGL):**
  * Cài đặt mô hình chiếu sáng **Phong Shading**.
  * Cài đặt mô hình chiếu sáng **Gouraud Shading**.
  * Đã tối ưu hóa tính toán pháp tuyến mặt (**Smooth Normals**) để bề mặt vật thể hiển thị mượt mà và chân thực hơn.

---

## 🎮 Hệ Thống Điều Khiển

Dự án hỗ trợ tương tác trực tiếp qua bàn phím để thao tác với Camera và mô hình 3D:

| Nhóm | Phím tắt | Mô tả thao tác |
| :--- | :---: | :--- |
| 🎥 **Camera (Di chuyển)** | <kbd>W</kbd> / <kbd>A</kbd> / <kbd>S</kbd> / <kbd>D</kbd> | Di chuyển Camera (Tiến / Trái / Lùi / Phải) |
| 🎥 **Camera (Góc nhìn)** | <kbd>←</kbd> <kbd>↑</kbd> <kbd>→</kbd> <kbd>↓</kbd> | Xoay Camera (Lên / Xuống / Trái / Phải) |
| 📦 **Chọn vật thể** | <kbd>1</kbd> / <kbd>2</kbd> / <kbd>3</kbd> | Chuyển đổi giữa các mô hình (Pokeball / Heart / Teapot) |
| 🏎️ **Dịch chuyển vật thể** | <kbd>I</kbd> / <kbd>J</kbd> / <kbd>K</kbd> / <kbd>L</kbd> | Di chuyển vật thể theo trục X (Trái/Phải) và Y (Lên/Xuống) |
| 🏎️ **Dịch chuyển vật thể** | <kbd>U</kbd> / <kbd>O</kbd> | Di chuyển vật thể theo trục Z (Ra xa / Lại gần) |
| 🔄 **Xoay vật thể** | <kbd>B</kbd> / <kbd>N</kbd> / <kbd>M</kbd> | Xoay vật thể quanh các trục không gian tương ứng (X / Y / Z) |
| 🔍 **Thu phóng** | <kbd>Z</kbd> / <kbd>X</kbd> | Tỉ lệ kích thước mô hình (Phóng to / Thu nhỏ) |
| ✨ **Đổi Shading**| <kbd>E</kbd> | Chuyển đổi qua lại giữa kỹ thuật **Phong Shading** và **Gouraud Shading** |
| ❌ **Thoát** | <kbd>ESC</kbd> | Đóng ứng dụng |

---

## 🚀 Hướng Dẫn Chạy Dự Án (Dành cho Giáo viên)

Để đảm bảo dự án chạy ổn định trên Visual Studio, Thầy vui lòng thực hiện theo các bước sau:

### 1. Giải nén
* Giải nén toàn bộ thư mục bài làm vào một đường dẫn không chứa ký tự tiếng Việt có dấu (để tránh lỗi đọc file `.obj`).

### 2. Mở dự án
* Khởi động **Visual Studio** (khuyên dùng phiên bản 2019 hoặc 2022).
* Vào thư mục vừa giải nén, tìm và mở file **`DHMT.slnx`** (hoặc `DHMT.sln` nếu có).

### 3. Cấu hình & Chạy
* Đảm bảo cấu hình Solution là **`Debug`** hoặc **`Release`** và nền tảng là **`x64`**.
* Nhấn phím **`F5`** hoặc nhấn vào nút **Local Windows Debugger** để biên dịch và khởi chạy.

> [!IMPORTANT]
> **Lưu ý quan trọng:**
> * Các thư viện (OpenGL, GLFW, GLEW, GLM) đã được cấu hình sẵn qua **NuGet Packages**. Nếu Visual Studio báo thiếu thư viện, Thầy vui lòng chuột phải vào **Solution** và chọn **Restore NuGet Packages**.
> * Các file thư viện liên kết động (`.dll`) và thư mục `DATA` (chứa model, shader) đã được đặt sẵn trong thư mục gốc của Solution để chương trình có thể đọc trực tiếp.

---

<br>

<div align="center">
  <i>Được thực hiện bởi Nhóm sinh viên Đại học Bách khoa - ĐH Đà Nẵng</i>
</div>
