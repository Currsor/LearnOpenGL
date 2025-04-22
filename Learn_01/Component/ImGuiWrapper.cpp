// ImGuiWrapper.cpp
#include "ImGuiWrapper.h"
#include <iostream>

ImGuiWrapper::ImGuiWrapper(GLFWwindow* window) 
    : m_Window(window), m_MultiViewportEnabled(false), m_MainFont(nullptr) {
    Initialize();
}

ImGuiWrapper::~ImGuiWrapper() {
    Shutdown();
}

void ImGuiWrapper::Initialize() {
    // 检查GLFW窗口
    if (!m_Window) {
        std::cerr << "Error: 无效的 GLFW 窗口句柄!" << std::endl;
        return;
    }

    // 初始化ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // 启用多视口和停靠功能 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_MultiViewportEnabled = (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0;
    
    // 设置默认样式
    SetClassicTheme();
    
    // 加载字体
    LoadCustomFont(16);
    
    // 初始化平台和渲染器后端 
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiWrapper::Shutdown() {
    // 清理ImGui资源 
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiWrapper::BeginFrame() {
    // 开始新帧 
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiWrapper::EndFrame() {
    // 渲染ImGui 
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // 处理多视口 
    if (m_MultiViewportEnabled) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}

void ImGuiWrapper::RenderDockSpace(unsigned int processedTexture, Camera* camera) {
    // 设置全屏停靠空间 
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // 提交停靠空间
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    // 菜单栏
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Fullscreen", nullptr, &opt_fullscreen);
            ImGui::MenuItem("Padding", nullptr, &opt_padding);
            ImGui::Separator();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // 渲染主窗口
    RenderMainWindows(processedTexture, camera);
    
    ImGui::End();
}

void ImGuiWrapper::RenderMainWindows(unsigned int processedTexture, Camera* camera) {
    // 3D视图窗口 
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);
    ImGui::Begin("3D View");
    {
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        static bool bTemporary = true;
        if (bTemporary)
        {
            glBindTexture(GL_TEXTURE_2D, processedTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportSize.x, viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
            bTemporary = false;
        }
        ImTextureID tex_id = (ImTextureID)(intptr_t)processedTexture;
        ImGui::Image(tex_id, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
        
        bool isHovered = ImGui::IsWindowHovered();
        bool rightDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);
        camera->RightClickIn3DView = isHovered && rightDown;
    }
    ImGui::End();
}

void ImGuiWrapper::SetDarkTheme() {
    ImGui::StyleColorsDark();
}

void ImGuiWrapper::SetClassicTheme() {
    ImGui::StyleColorsClassic();
}

void ImGuiWrapper::SetCustomTheme(float* colors) {
    if (colors) {
        ImGuiStyle& style = ImGui::GetStyle();
        for (int i = 0; i < ImGuiCol_COUNT; i++) {
            style.Colors[i] = ImVec4(colors[i*4+0], colors[i*4+1], 
                                    colors[i*4+2], colors[i*4+3]);
        }
    } else {
        // 默认自定义主题
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.30f, 0.35f, 0.80f);
    }
}

void ImGuiWrapper::LoadDefaultFont(float fontSize) {
    ImGuiIO& io = ImGui::GetIO();
    m_MainFont = io.Fonts->AddFontDefault();
    io.FontDefault = m_MainFont;
}

bool ImGuiWrapper::LoadCustomFont(float fontSize) {
    ImGuiIO& io = ImGui::GetIO();
    m_MainFont = io.Fonts->AddFontFromFileTTF
    (
        "C:/Windows/Fonts/msyh.ttc",  // 微软雅黑字体路径
        16.0f,
        nullptr,
        io.Fonts->GetGlyphRangesChineseFull()
    );
    return false;
}

bool ImGuiWrapper::IsMultiViewportEnabled() const {
    return m_MultiViewportEnabled;
}
