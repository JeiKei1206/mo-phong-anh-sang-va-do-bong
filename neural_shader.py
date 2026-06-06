import torch
import torch.nn as nn

class NeuralPhongShader(nn.Module):
    def __init__(self):
        super(NeuralPhongShader, self).__init__()
        # Nâng cấp đầu vào từ 6 lên 9: bao gồm Pháp tuyến (Normal), Hướng nhìn (View Dir), và Hướng sáng (Light Dir)
        # Điều này giúp mạng MLP học được đầy đủ các yếu tố cấu thành mô hình Blinn-Phong truyền thống.
        self.network = nn.Sequential(
            nn.Linear(9, 128),
            nn.ReLU(),
            nn.Linear(128, 128),
            nn.ReLU(),
            nn.Linear(128, 64),
            nn.ReLU(),
            nn.Linear(64, 3), 
            nn.Sigmoid() # Đảm bảo giá trị màu RGB luôn nằm trong khoảng [0, 1]
        )

    def forward(self, normal, view_dir, light_dir):
        # Nối các vector lại thành một tensor duy nhất làm đầu vào cho mạng
        x = torch.cat([normal, view_dir, light_dir], dim=-1)
        return self.network(x)

if __name__ == "__main__":
    ai_shader = NeuralPhongShader()
    
    # Giả lập các vector đầu vào (đã được chuẩn hóa)
    surface_normal = torch.tensor([[0.0, 1.0, 0.0]]) 
    camera_view = torch.tensor([[0.0, 0.707, 0.707]])
    light_direction = torch.tensor([[0.577, 0.577, 0.577]])
    
    predicted_color = ai_shader(surface_normal, camera_view, light_direction)
    print("Màu RGB do Neural Rendering dự đoán (R, G, B):", predicted_color.detach().numpy())