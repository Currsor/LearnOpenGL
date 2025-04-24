#pragma once
#include <fstream>
#include <string>

class StyleEditorWindow : public IImGuiWindow {
public:
    StyleEditorWindow() {
        LoadStyleFromFile(); // 加载模板设置
    }

    const char* GetName() const override { return "Style"; }

    void Render() override {
        ImGui::Text("ImGui 样式编辑器");
        ImGui::Separator();

        // 显示样式选择器
        if (ImGui::BeginCombo("Select Style", currentStyle.c_str())) {
            if (ImGui::Selectable("Dark", currentStyle == "Dark")) {
                currentStyle = "Dark";
                ImGui::StyleColorsDark();
                SaveToFile("Style", currentStyle);
            }
            if (ImGui::Selectable("Classic", currentStyle == "Classic")) {
                currentStyle = "Classic";
                ImGui::StyleColorsClassic();
                SaveToFile("Style", currentStyle);
            }
            if (ImGui::Selectable("Light", currentStyle == "Light")) {
                currentStyle = "Light";
                ImGui::StyleColorsLight();
                SaveToFile("Style", currentStyle);
            }
            ImGui::EndCombo();
        }

        if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
        {
            style.GrabRounding = style.FrameRounding;
            SaveToFile("FrameRounding", std::to_string(style.FrameRounding));
            SaveToFile("GrabRounding", std::to_string(style.GrabRounding));
        }

        { bool border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &border)) { style.WindowBorderSize = border ? 1.0f : 0.0f; SaveToFile("WindowBorder", std::to_string(border)); } }
        ImGui::SameLine();
        { bool border = (style.FrameBorderSize > 0.0f);  if (ImGui::Checkbox("FrameBorder",  &border)) { style.FrameBorderSize  = border ? 1.0f : 0.0f; SaveToFile("FrameBorder", std::to_string(border)); } }
        ImGui::SameLine();
        { bool border = (style.PopupBorderSize > 0.0f);  if (ImGui::Checkbox("PopupBorder",  &border)) { style.PopupBorderSize  = border ? 1.0f : 0.0f; SaveToFile("PopupBorder", std::to_string(border)); } }
    }

private:
    ImGuiStyle& style = ImGui::GetStyle();
    std::string currentStyle = "Dark"; // 存储当前模板名称
    const std::string configFilePath = "style_config.txt"; // 配置文件路径

    void SaveToFile(const std::string& key, const std::string& value) {
        std::ofstream outFile(configFilePath, std::ios::app); // 以追加模式打开文件
        if (outFile.is_open()) {
            outFile << key << "=" << value << std::endl; // 保存键值对
            outFile.close();
        }
    }

    void LoadStyleFromFile()
    {
        std::ifstream inFile(configFilePath);
        if (!inFile.is_open()) return;

        std::string line;
        while (std::getline(inFile, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) continue;

            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            if (key == "Style") {
                ApplyStyle(value);
            } else if (key == "FrameRounding") {
                style.FrameRounding = std::stof(value);
            } else if (key == "GrabRounding") {
                style.GrabRounding = std::stof(value);
            } else if (key == "WindowBorder") {
                style.WindowBorderSize = std::stof(value);
            } else if (key == "FrameBorder") {
                style.FrameBorderSize = std::stof(value);
            } else if (key == "PopupBorder") {
                style.PopupBorderSize = std::stof(value);
            }
        }
        inFile.close();
    }

    void ApplyStyle(const std::string& styleName) {
        if (styleName == "Dark") {
            ImGui::StyleColorsDark();
            return;
        }
        if (styleName == "Classic") {
            ImGui::StyleColorsClassic();
            return;
        }
        if (styleName == "Light") {
            ImGui::StyleColorsLight();
            return;
        }
        ImGui::StyleColorsDark(); // 默认样式
    }
};