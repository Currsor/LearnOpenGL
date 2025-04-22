// ImGuiWrapper.h
#pragma once
#include <memory>
#include <vector>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_internal.h"

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "UI/UIManager.h"

class ImGuiWrapper {
public:
    ImGuiWrapper(GLFWwindow* window);
    ~ImGuiWrapper();
    
    void BeginFrame();
    void EndFrame();
    
    void RenderDockSpace(unsigned int processedTexture, Camera* camera);
    void RenderMainWindows(unsigned int processedTexture, Camera* camera);
    
    // 样式设置
    void SetDarkTheme();
    void SetClassicTheme();
    void SetCustomTheme(float* colors = nullptr);
    
    // 字体管理
    void LoadDefaultFont(float fontSize = 16.0f);
    bool LoadCustomFont(float fontSize);
    
    // 多视口支持
    bool IsMultiViewportEnabled() const;

    template<typename T, typename... Args>
    void AddWindow(Args&&... args) {
        m_Windows.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }
    
    void RenderWindows()
    {
        for (auto& window : m_Windows) {
            ImGui::Begin(window->GetName());
            window->Render();
            ImGui::End();
        }
    }
    
private:
    void Initialize();
    void Shutdown();
    
    GLFWwindow* m_Window;
    std::vector<std::unique_ptr<IImGuiWindow>> m_Windows;
    bool m_MultiViewportEnabled;
    ImFont* m_MainFont;
};
