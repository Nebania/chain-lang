#ifndef LINK_GUI_H
#define LINK_GUI_H

#include <string>

namespace SysGui {
    void enableDebug(); 
    
    // Window Management
    void setup(int width, int height, const std::string& title);
    void close();
    bool running();
    
    // Utilities (Resize & Input Helpers)
    int getScreenWidth();
    int getScreenHeight();
    int measureText(const std::string& text, int fontSize);
    int measureTextHeight(const std::string& text, int fontSize);

    // Rendering Cycle
    void start();
    void stop(); 
    void present();
    void clear(const std::string& colorName);
    
    // Resources
    void loadFont(const std::string& path, int fontSize);
    void loadImage(const std::string& path, const std::string& name);
    
    // Drawing
    void drawRect(int x, int y, int w, int h, const std::string& color);
    void drawText(int x, int y, const std::string& text, const std::string& color, int size);
    void drawImage(const std::string& name, int x, int y, int width, int height);

    // INPUT MOUSE ( 
    int getMouseX();
    int getMouseY();
    float getMouseWheel();
    bool isMousePressed();  
    bool isMouseDown();     
    // INPUT KEYBOARD
    int getKeyPressed();
    int getCharPressed();
    bool isKeyDown(const std::string& key);     
    bool isKeyPressed(const std::string& key);  
    double getTime(); 
}

#endif
