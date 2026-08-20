---

# 📖 ChainLang Official Syntax Guide

Welcome to **ChainLang**! A modern, dynamic, and robust programming language designed for ease of use. Ranging from simple scripting, scientific computing, and multithreading to **Native Desktop Applications** and Networking.

Equipped with **Rust-like Visual Error Tracing**, ChainLang helps you locate bugs with high precision.

Here is the comprehensive guide to writing syntax in ChainLang.

---

## 1. Comments & Output

Use `//` to write comments (ignored by the execution engine). Use `print()` to output text to the terminal.

```javascript
// This is a single-line comment
print("Hello, World!") 
print("ChainLang is easy to learn.")

```

## 2. Variables, Data Types & Multiline Strings

Use the `set` keyword to declare variables. ChainLang is **dynamically typed**. ChainLang natively supports **Multiline Strings** for storing long formatted text such as HTML or JSON.

```javascript
set age = 25              // Integer
set pi = 3.14             // Float
set is_active = true       // Boolean (true / false)

// Multiline String (Supports newline breaks directly)
set html_template = "
<html>
    <body>
        <h1>Hello from ChainLang!</h1>
    </body>
</html>
"

```

## 3. Data Structures (List & Dictionary)

ChainLang natively supports Arrays (Lists) and JSON Objects (Dictionaries).

**List (Array):**

```javascript
set fruits = ["Apple", "Mango", "Orange"]
print(fruits[0])           // Output: Apple

list.add(fruits, "Banana") // Append data
list.pop(fruits)           // Remove last item

```

**Dictionary (Key-Value):**

```javascript
set user = {
    "name": "Pilot0253",
    "role": "Admin",
    "level": 99
}
print(user.name)        // Output: Pilot0253
set user.level = 100    // Mutate value

```

## 4. Control Flow

### A. If, Elif, Else

```javascript
set score = 85

if score >= 90 {
    print("Grade A")
} elif score >= 80 {
    print("Grade B")
} else {
    print("Grade C")
}

```

### B. Pattern Matching (Modern Switch-Case)

Use `match` for cleaner structural branching. Use `_` for the default condition.

```javascript
set status = 404

match status {
    200 => print("OK")
    404 => {
        print("Not Found")
        print("Please check your URL")
    }
    _ => print("Unknown Status")
}

```

## 5. Loops

### A. For Loop (Using Range)

```javascript
// Prints numbers 0 through 4
for i in range(5) {
    print(i)
}

```

### B. While Loop

```javascript
set x = 0
while x < 5 {
    print(x)
    set x = x + 1
    // Use 'break' to stop, or 'continue' to skip an iteration
}

```

## 6. Functions & Lambdas

Use `func` to define functions. Optional type hinting is supported for enhanced code readability.

**Basic Function:**

```javascript
func greet(name) {
    return "Hello, " + name
}
print(greet("Chain"))

```

**Function with Type Hinting (Gradual Typing):**

```javascript
func add(a: Int, b: Int) -> Int {
    return a + b
}

```

**Lambda (Anonymous Function):**
Functions can be stored inside variables and passed around as arguments.

```javascript
set double = func(x) { return x * 2 }
print(double(10)) // Output: 20

```

## 7. Object-Oriented Programming (OOP)

ChainLang provides full support for Classes, Inheritance, and Magic Methods (Operator Overloading).

```javascript
// Parent Class
class Engine {
    func init() {
        set this.status = "Running"
    }
}

// Child Class (Inherits Engine)
class Robot : Engine {
    // Constructor
    func init(name, power) {
        set this.name = name
        set this.power = power
    }
    
    // Regular Method
    func info() {
        print(this.name + " power: " + this.power)
    }
    
    // Magic Method (Operator Overloading for '+')
    func __add__(other) {
        return new Robot("Combined", this.power + other.power)
    }
}

set r1 = new Robot("Alpha", 50)
set r2 = new Robot("Beta", 30)

set r3 = r1 + r2  // Automatically calls __add__!
r3.info()         // Output: Combined power: 80

```

## 8. Error Handling & Visual Tracing

ChainLang features **Rust-like Visual Error Tracing**. On Syntax or Runtime Errors, execution halts safely while pointing directly to the exact line and column using caret markers (`^^^`).

Catch errors manually to prevent unexpected termination:

```javascript
try {
    // Attempting to read a non-existent file
    set data = io.read("missing.txt")
} catch (e) {
    print("An error occurred: " + e)
}

```

## 9. Concurrency (Multithreading / Goroutines)

Delegate heavy background tasks (such as networking servers or intense processing loops) without freezing the main execution thread or UI loop.

```javascript
func heavyTask() {
    while true {
        print("Running in background...")
        time.sleep(1000) // Sleep 1 second
    }
}

// Execute in a separate thread (Non-blocking)
thread.spawn(heavyTask)

// Main thread continues execution immediately
print("Main thread finished execution!")

```

## 10. Native Desktop Apps (Webview)

Build Native Desktop Interfaces using **HTML, CSS, and JavaScript**, while executing backend application logic inside ChainLang. Light, modern, and fully supports bi-directional IPC.

```javascript
set ui = "
    <html>
        <body style='background: #1e1e2e; color: white;'>
            <h1>Hello from ChainLang!</h1>
            <button onclick='chain_print(\"UI Event Fired\")'>Send to Backend</button>
        </body>
    </html>
"

// Open Native Window
webview.show("Native Application", 800, 600, ui)

// Execute JavaScript from C++ Backend
webview.eval("alert('Message from Backend!')")

```

## 11. Native C++ Interop (Extern C++)

Directly embed pure C++ inside your ChainLang scripts. Variables are automatically shared across contexts via Shared Memory *(POSIX/Linux/macOS supported)*.

```javascript
set num = 10

extern "c" "-O3" {
    #include <iostream>
    
    // Access 'num' from ChainLang using the LINK_ prefix
    LINK_num = LINK_num * 5;
    std::cout << "Calculated via Native C++: " << LINK_num << std::endl;
}

print(num) // Output: 50

```

---

## 📚 Standard Library Reference

ChainLang includes built-in modules available in global scope without needing explicit imports.

### 🧮 Math (Scientific Computation)

* **Core:** `math.pi()`, `math.e()`, `math.abs(x)`, `math.sqrt(x)`, `math.pow(b, e)`
* **Trigonometry:** `math.sin(x)`, `math.cos(x)`, `math.tan(x)`, `math.asin(x)`, `math.acos(x)`, `math.atan2(y, x)`
* **Exponential & Logarithmic:** `math.exp(x)`, `math.log(x)`, `math.log10(x)`, `math.log2(x)`
* **Utilities:** `math.ceil(x)`, `math.floor(x)`, `math.round(x)`, `math.min(a, b)`, `math.max(a, b)`, `math.random()`, `math.randint(min, max)`

### 🔤 Strings & Collections

* **String:** `str.upper(s)`, `str.lower(s)`, `str.trim(s)`, `str.split(s, ",")`, `str.replace(s, "a", "b")`, `str.substr(s, start, length)`, `str.contains(s, "x")`
* **List:** `list.add(l, x)`, `list.remove(l, idx)`, `list.insert(l, idx, x)`, `list.pop(l)`
* **Dict:** `dict.keys(d)`, `dict.has(d, "key")`, `dict.remove(d, "key")`
* **General:** `len(obj)` *(Supports Strings, Lists, and Dictionaries)*

### 🖥️ System & File I/O

* **File:** `io.read("file.txt")`, `io.write("file.txt", "Text")`, `io.append("file.txt", "Text")`, `io.exists("file.txt")`, `io.remove("file.txt")`
* **OS:** `os.exec("ls -la")`, `os.getenv("PATH")`, `os.setenv("K", "V")`, `os.cwd()`, `os.chdir("dir")`, `os.date()`
* **Thread:** `thread.spawn(func)`
* **Time:** `time.sleep(ms)`

### 🌐 Networking

* `net.server(port)` : Creates TCP Server.
* `net.connect(ip, port)` : Connects to remote TCP host.
* `net.accept(sock)` : Accepts incoming socket connection (Blocking).
* `net.send(sock, data)` & `net.recv(sock)` : Sends and receives socket payloads.
* `net.close(sock)` : Closes network socket.

### 🎨 GUI & Graphics (Raylib Engine)

* **Window Management:** `gui_setup(w, h, "Title")`, `gui_running()`, `gui_close()`, `gui.width()`, `gui.height()`
* **Render Pipeline:** `gui_start()`, `gui_present()`, `gui_clear("color")`
* **Primitives:** `gui_rect(x, y, w, h, c)`, `gui_line(x1, y1, x2, y2, thick, c)`, `gui_text(x, y, txt, c, size)`
* **Advanced Rendering:** `gui_begin_clip(x, y, w, h)`, `gui_end_clip()` *(Scissor/Masking support)*
* **Input Handling:** `gui.mouse_x()`, `gui.mouse_y()`, `gui.get_mouse_wheel()`, `gui_click()`, `gui_is_key_pressed("K")`, `gui_get_char()`

### 🎵 Audio Engine

* `audio.init()`, `audio.close()`
* `audio.play("track.mp3")`, `audio.pause()`, `audio.stop()`, `audio.volume(0.5)`
* `audio.update()` *(Must be called within frame loop to stream audio buffer)*
* `audio.get_eq(band)` *(Returns FFT frequency bands for audio visualizers)*

### 🌍 Webview (Modern Desktop Engine)

* `webview.show(title, width, height, html_string)` : Instantiates Native OS Window.
* `webview.eval(js_string)` : Executes JavaScript strings from C++ Backend runtime.
