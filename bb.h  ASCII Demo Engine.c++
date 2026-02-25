#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "Renderer.h"     // GPU/Terminal rendering abstraction
#include "AudioEngine.h"  // Modern audio library
#include "Font.h"
#include "Image.h"

namespace BB2036 {

struct Color {
    float r, g, b, a;
};

class Scene {
public:
    Scene(std::shared_ptr<Renderer> renderer, std::shared_ptr<AudioEngine> audio);
    virtual ~Scene() = default;

    virtual void load() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void render() = 0;

protected:
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<AudioEngine> audio;
};

class BBEngine {
public:
    BBEngine(std::shared_ptr<Renderer> renderer, std::shared_ptr<AudioEngine> audio);
    void addScene(std::shared_ptr<Scene> scene);
    void start();
    void stop();
    void update(double deltaTime);
    void render();

private:
    std::vector<std::shared_ptr<Scene>> scenes;
    size_t currentScene = 0;
    bool running = false;
};

// Function to draw text
inline void drawText(std::shared_ptr<Renderer> renderer, const std::string& text, int x, int y, Color color) {
    renderer->drawText(text, x, y, color);
}

// Function to draw image
inline void drawImage(std::shared_ptr<Renderer> renderer, const Image& img, int x, int y) {
    renderer->drawImage(img, x, y);
}

} // namespace BB2036
