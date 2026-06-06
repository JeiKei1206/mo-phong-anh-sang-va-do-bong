import torch
import torch.nn as nn

class NeuralShader(nn.Module):
    def __init__(self):
        super(NeuralShader, self).__init__()
        self.network = nn.Sequential(
            nn.Linear(6, 64),
            nn.ReLU(),
            nn.Linear(64, 64),
            nn.ReLU(),
            nn.Linear(64, 3), 
            nn.Sigmoid()      
        )

    def forward(self, normal, view_dir):
        x = torch.cat([normal, view_dir], dim=-1)
        return self.network(x)

if __name__ == "__main__":
    ai_shader = NeuralShader()
    surface_normal = torch.tensor([[0.0, 1.0, 0.0]]) 
    camera_view = torch.tensor([[0.5, 0.5, 0.0]])
    predicted_color = ai_shader(surface_normal, camera_view)
    print("Màu RGB do AI dự đoán (R, G, B):", predicted_color.detach().numpy())