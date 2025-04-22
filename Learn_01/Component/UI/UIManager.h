#pragma once

class IImGuiWindow {
public:
    virtual ~IImGuiWindow() = default;
    virtual void Render() = 0;
    virtual const char* GetName() const = 0;
};
