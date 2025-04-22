#pragma once
#include "../UIManager.h"
#include "../../Camera.h"

class StatsWindow : public IImGuiWindow {
    Camera* m_Camera;
public:
    StatsWindow(Camera* cam) : m_Camera(cam) {}
    
    const char* GetName() const override { return "Statistics"; }
    
    void Render() override {
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Camera Pos: (%.2f, %.2f, %.2f)", 
                   m_Camera->Position.x, m_Camera->Position.y, m_Camera->Position.z);
    }
};
