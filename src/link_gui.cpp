#include "../include/link_gui.h"
#include "raylib.h"
#include <iostream>
#include <map>
#include <string>

namespace SysGui {
    
    Font currentFont;
    bool isFontLoaded = false;
    std::map<std::string, Texture2D> imageRegistry;
    
    bool _shouldClose = false; // Gunakan variabel internal
    bool _guiDebug = false; // Debug Flag

    void enableDebug() {
        _guiDebug = true;
        // Kembalikan log Raylib ke tingkat normal (INFO, WARNING, ERROR, dll)
        SetTraceLogLevel(LOG_INFO); 
        std::cout << "[CHAIN-LANG] GUI Debug Mode Enabled.\n";
    }

    void stop() {
		std::cout << " [DEBUG] : \n "; 
        _shouldClose = true;
    }

    bool running() {
        if (WindowShouldClose() || _shouldClose) {
            return false;
        }
        return true;
    }

    // --- HELPER TRANSLATOR KEYBOARD ---
    int getKeyFromName(const std::string& keyName) {
        if (keyName == "SPACE") return KEY_SPACE;
        if (keyName == "ENTER") return KEY_ENTER;
        if (keyName == "BACKSPACE") return KEY_BACKSPACE;
        if (keyName == "ESCAPE") return KEY_ESCAPE;
        if (keyName == "UP") return KEY_UP;
        if (keyName == "DOWN") return KEY_DOWN;
        if (keyName == "LEFT") return KEY_LEFT;
        if (keyName == "RIGHT") return KEY_RIGHT;
        
        if (keyName.length() == 1) {
            char c = keyName[0];
            if (c >= 'A' && c <= 'Z') return (int)c;
            if (c >= '0' && c <= '9') return (int)c;
        }
        return 0;
    }

    // --- WINDOW ---
    void setup(int width, int height, const std::string& title) {
        if (!_guiDebug) {
            SetTraceLogLevel(LOG_NONE); 
        }
        
        // FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT; // Debug Features HighDPI from C++
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); 

        InitWindow(width, height, title.c_str());
        // --- Force RayLib for Resize  ---
        SetWindowMinSize(300, 200); 
        SetWindowSize(width, height); 
        // --------------------------------------------------------
        SetTargetFPS(60);
    }

    void close() {
        for (auto const& [name, tex] : imageRegistry) UnloadTexture(tex);
        if (isFontLoaded) UnloadFont(currentFont);
        CloseWindow();
    }
	
    // --- UTILITIES ---
    int getScreenWidth() { return GetScreenWidth(); }
    int getScreenHeight() { return GetScreenHeight(); }

    int measureText(const std::string& text, int fontSize) {
        if (isFontLoaded) return (int)MeasureTextEx(currentFont, text.c_str(), fontSize, 1.0f).x;
        return MeasureText(text.c_str(), fontSize);
    }

    int measureTextHeight(const std::string& text, int fontSize) {
        if (isFontLoaded) return (int)MeasureTextEx(currentFont, text.c_str(), fontSize, 1.0f).y;
        return fontSize;
    }

    // --- RENDER & RESOURCES ---
    void start() { BeginDrawing(); }
    void present() { EndDrawing(); }

    void clear(const std::string& colorName) {
        Color c = BLACK;
        if (colorName == "white") c = WHITE;
        else if (colorName == "red") c = RED;
        else if (colorName == "green") c = GREEN;
        else if (colorName == "blue") c = BLUE;
        else if (colorName == "gray") c = Fade(GRAY, 0.8f);
        ClearBackground(c);
    }

    void loadFont(const std::string& path, int fontSize) {
        Font temp = LoadFontEx(path.c_str(), fontSize, 0, 0);
        if (temp.texture.id == 0) return;
        if (isFontLoaded) UnloadFont(currentFont);
        currentFont = temp;
        SetTextureFilter(currentFont.texture, TEXTURE_FILTER_TRILINEAR);
        isFontLoaded = true;
    }

    void loadImage(const std::string& path, const std::string& name) {
        Image img = LoadImage(path.c_str());
        if (img.data == NULL) return;
        Texture2D tex = LoadTextureFromImage(img);
        UnloadImage(img);
        imageRegistry[name] = tex;
    }

    // --- DRAWING ---
    void drawRect(int x, int y, int w, int h, const std::string& color) {
        Color c = BLACK;
        if (color == "white") c = WHITE;
        else if (color == "red") c = RED;
        else if (color == "green") c = GREEN;
        else if (color == "blue") c = BLUE;
        else if (color == "cyan") c = {0, 255, 255, 255}; // 🔥 TAMBAHKAN INI
        else if (color == "gray") c = Fade(GRAY, 0.8f);
        DrawRectangle(x, y, w, h, c);
    }

    void drawText(int x, int y, const std::string& text, const std::string& color, int size) {
        Color c = BLACK;
        if (color == "white") c = WHITE;
        else if (color == "red") c = RED;
        else if (color == "green") c = GREEN;
        else if (color == "blue") c = BLUE;
        else if (color == "cyan") c = {0, 255, 255, 255}; // 🔥 TAMBAHKAN INI JUGA
        
        if (isFontLoaded) DrawTextEx(currentFont, text.c_str(), {(float)x, (float)y}, size, 1.0f, c);
        else DrawText(text.c_str(), x, y, size, c);
    }

    void drawImage(const std::string& name, int x, int y, int width, int height) {
        if (imageRegistry.find(name) != imageRegistry.end()) {
            Texture2D tex = imageRegistry[name];
            Rectangle src = {0.0f, 0.0f, (float)tex.width, (float)tex.height};
            Rectangle dst = {(float)x, (float)y, (float)width, (float)height};
            DrawTexturePro(tex, src, dst, {0,0}, 0.0f, WHITE);
        }
    }

    // --- INPUT HANDLING (INI YANG SEBELUMNYA ERROR/HILANG) ---
    int getMouseX() { return GetMouseX(); }
    int getMouseY() { return GetMouseY(); }
    float getMouseWheel() { return GetMouseWheelMove(); }
    
    bool isMousePressed() { return IsMouseButtonPressed(MOUSE_BUTTON_LEFT); }
    bool isMouseDown() { return IsMouseButtonDown(MOUSE_BUTTON_LEFT); } // <- INI YANG HILANG DI ERROR LOG KAMU

    int getKeyPressed() { return GetKeyPressed(); }
    int getCharPressed() { return GetCharPressed(); }
    
    // Ini juga sempat error di log kamu (undefined reference)
    bool isKeyDown(const std::string& key) {
        int k = getKeyFromName(key);
        return (k != 0) && IsKeyDown(k);
    }

    bool isKeyPressed(const std::string& key) {
        int k = getKeyFromName(key);
        return (k != 0) && IsKeyPressed(k);
    }
    double getTime() {
        return GetTime();
    }

} // end namespace
