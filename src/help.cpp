#include "help.h"
#include <iostream>

void printVersion() {
    std::cout << "Chain-Lang Version 0.5a (Alpha Release) \n" << std::endl;
} 

void printHelp() {
    std::cout << R"(
========================================================
           NEBULA OS CHAIN-LANG v0.5a (Alpha) HELP
========================================================
History:
Created by   : Pilot0253
Developed by : logoro17 
Our Website  : https://nebania.site

USAGE:
  ./link                  : Enter Interactive Mode (REPL).
  ./link <file.link>      : Execute a Chain-Lang script file.
  ./link --debug <file>   : Execute with AST Debug Mode.
  ./link --help           : Show this manual.
  ./link --version        : Show current version.

USAGE ON NEBULA OS PREINSTALL:
  linkrun                  : Enter Interactive Mode (REPL).
  linkrun <file.link>      : Execute a Chain-Lang script file.
  linkrun --help           : Show this manual.

DATA TYPES:
  Integer  : 10, 25, -5
  Float    : 3.14, 2.5
  String   : "Hello", 'World'
  Char     : 'A', 'x'
  Boolean  : true, false
  List     : [1, 2, "Text"]
  Dict     : {"key": "value", "ver": 1.0}
  Object   : <Instance Robot>

BASIC COMMANDS:
  set x = 10              : Variable declaration.
  print(x)                : Output to screen.
  import "file.link"      : Import other script files / libraries.
  sh "ls -la"             : Quick shell command execution.

OBJECT ORIENTED PROGRAMMING:
  class Robot {           : Define a class.
      init(name) {        : Constructor.
          set this.n = name
      }
      func walk() {       : Method definition.
          print(this.n)
      }
  }
  set r = new Robot("X")  : Create new instance.
  r.walk()                : Call method.

CONTROL FLOW:
  * Supports both Indentation (Python) and Block {} (C/Java)
  
  1. CONDITIONAL
     if x > 10 { print("Big") }
     elif x == 5 { print("Medium") }
     else { print("Small") }

  2. LOOPING
     while x < 5 {
         print(x)
         set x = x + 1
     }
     for i in range(5) {
         print(i)
     }

  3. ERROR HANDLING
     try {
         io.read("missing.txt")
     } catch (e) {
         print("Error: " + e)
     }

========================================================
               BUILT-IN LIBRARIES & MODULES
========================================================

[GUI & GRAPHICS] (Powered by Raylib)
  gui_setup(w, h, "Title") : Initialize application window.
  gui_running()            : Check if window is still open.
  gui_start()              : Begin drawing frame.
  gui_present()            : End drawing frame & swap buffers.
  gui_clear("color")       : Clear background (e.g., "black", "white").
  gui_rect(x, y, w, h, c)  : Draw a rectangle.
  gui_text(x, y, txt, c, s): Draw text with specific size.
  gui_close()              : Close window and free resources.

[INPUT HANDLING]
  gui_click()              : Check if Left Mouse Button is clicked.
  gui.mouse_x() / _y()     : Get current mouse X/Y coordinates.
  gui_is_key_pressed("K")  : Check if key is pressed once (e.g., "SPACE").
  gui_is_key_down("K")     : Check if key is being held down.

[AUDIO ENGINE]
  audio.init()             : Initialize audio device.
  audio.play("file.mp3")   : Load and play a music stream.
  audio.pause() / resume() : Pause or resume current music.
  audio.update()           : MUST be called in loop to keep music playing.
  audio.load_sound("n","p"): Load short SFX to RAM (Alias, Path).
  audio.play_sound("n")    : Play loaded SFX instantly.
  audio.close()            : Close audio device and free memory.

[SYSTEM & TIME]
  time.sleep(ms)           : Pause execution (e.g., 1000 = 1 sec).
  gui_get_time()           : Get elapsed time since window opened (seconds).
  os.exec("cmd")           : Execute shell command and return output.
  os.getenv("KEY")         : Get environment variable.

[MATH]
  math.pi()                : Returns PI (3.14159...).
  math.randint(min, max)   : Returns random integer between min and max.
  math.pow(b, e)           : Power calculation.
  math.sqrt(x)             : Square Root.

[FILE I/O]
  io.read("path")          : Read file content as string.
  io.write("p", "txt")     : Write to file (Overwrite).
  io.append("p", "txt")    : Append to file.
  io.exists("path")        : Check if file exists.

[NATIVE C++ WRAPPER]
  extern "c" "-lflags" {   : Write raw C++ code directly inside Chain-Lang!
      // C++ Code here     : Variables are shared via LINK_varname macros.
  }

========================================================
)" << std::endl;
}