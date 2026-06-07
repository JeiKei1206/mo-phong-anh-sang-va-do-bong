# Đồ án Đồ họa Máy tính - Phong & Gouraud Lighting

Dự án này thực hiện mô phỏng chiếu sáng Phong và Gouraud trên các mô hình 3D (Pokeball, Heart, Teapot) sử dụng OpenGL.

## Cấu trúc thư mục SOURCE

Thư mục **SOURCE** là thư mục gốc của đồ án, chứa tất cả các thành phần cần thiết:

- **DATA/**: Chứa các tài nguyên Shader (.shader) và mô hình 3D (.obj).
- **BaoCao/**: Chứa các file báo cáo đồ án (.docx, .pdf, .tex).
- **DHMT.slnx**: File Solution chính để mở dự án bằng Visual Studio.
- **DHMT.vcxproj**: File cấu hình dự án C++.
- **Main.cpp**: Mã nguồn chính của chương trình.
- **shaderloader.h, objloaderIndex.h...**: Các file header hỗ trợ tải shader và mô hình.
- **glew32.dll, glfw3.dll**: Các thư viện liên kết động cần thiết để chạy ứng dụng.
- **packages/**: Chứa các thư viện phụ thuộc (GLM, NuGet packages).

## Hướng dẫn chạy chương trình

1. **Mở dự án**: Mở file `DHMT.slnx` bằng Visual Studio (Khuyên dùng bản 2022).
2. **Cấu hình**: Đảm bảo chế độ build là `Debug` hoặc `Release` với nền tảng `x64` hoặc `x86`.
3. **Chạy**: Nhấn `F5` hoặc nút `Start` trong Visual Studio để biên dịch và chạy.

## Điều khiển trong chương trình

- **Phím 1, 2, 3**: Chuyển đổi giữa các mô hình (Pokeball, Heart, Teapot).
- **Phím E**: Bật/Tắt chế độ Gouraud Shading (Mặc định là Phong Shading).
- **Phím W, A, S, D**: Di chuyển Camera.
- **Phím mũi tên**: Xoay Camera.
- **Phím I, K, J, L, U, O**: Di chuyển Mô hình.
- **Phím B, N, M**: Xoay Mô hình.
- **Phím Z, X**: Phóng to/Thu nhỏ Mô hình.
- **Chuột trái**: Giữ và di chuyển dọc để Zoom Camera.
- **ESC**: Thoát chương trình.

---
**Nhóm thực hiện:**
- sv102240088 - Khoa
- sv102240118 - Trung
- sv102240113 - Tiền
