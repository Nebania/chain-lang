```text
 ██████╗██╗  ██╗ █████╗ ██╗███╗   ██╗    ██╗      █████╗ ███╗   ██╗ ██████╗ 
██╔════╝██║  ██║██╔══██╗██║████╗  ██║    ██║     ██╔══██╗████╗  ██║██╔════╝ 
██║     ███████║███████║██║██╔██╗ ██║    ██║     ███████║██╔██╗ ██║██║  ███╗
██║     ██╔══██║██╔══██║██║██║╚██╗██║    ██║     ██╔══██║██║╚██╗██║██║   ██║
╚██████╗██║  ██║██║  ██║██║██║ ╚████║    ███████╗██║  ██║██║ ╚████║╚██████╔╝
 ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝    ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ 
                  "Link your files with Chain!"
============= A Nebania Project =============
---------------------------------------------------------------------------
Version   : 0.5 (Full Release) 
Creator   : Pilot0253
Developer : logoro17 // LoRoGo17
PROJECT   : ChainLang (Interpreter)
LANGUAGE  : C++
OS        : Linux Distro / Android (Custom Kernel) ARM-v8a
STATUS    : Active Development
```
Chain-Lang is a dynamic, interpreted programming language built for NebulaOS. It
features a clean, Python-like syntax based on indentation, designed for
simplicity and readability. Built from scratch using C++ (Lexer, Parser, AST,
Runtime).

Features

Chain-Lang has evolved from a simple math parser into a fully functional
scripting language.

Core Capabilities

  - **Dynamic Typing:** Supports Integer, Float, String, Char, Boolean, Lists
    (Arrays), and Dictionaries.
  - **Math Engine:** Full support for +, -, *, / with operator precedence (PEMDAS)
    and parentheses ().
  - **Logic & Comparison:** Support for >, <, ==, !=, and, or operators.
  - **Control Flow:**
      - if, elif, else conditionals (recursive parsing).
      - while loops.
      - for loops (basic numeric range).
      - try and catch for error handling.
  - **I/O Operations:** Built-in print() and input().
  - **Hybrid Syntax:** Blocks can be defined by whitespace (Python-style) OR by
    using {} braces (C/Java-style).
  - **Comments:** Use # for single-line comments.

Advanced Features (v0.5 Major Update)

  - **Object-Oriented Programming (OOP):** Full support for class, constructors
    (init), methods, this, and object instantiation (new).
  - **Modular System:** Use import "file.link" to split your code into multiple
    files and build reusable libraries.
  - **Native C++ Wrapper (extern "c"):** The crown jewel of Chain-Lang. Write raw C++
    code directly inside your .link scripts! Chain-Lang will automatically
    compile, cache, and execute it, sharing variables seamlessly via Shared
    Memory.

Built-in Standard Libraries

Chain-Lang now comes with powerful built-in modules:

  - **GUI Engine (gui_):** Powered by Raylib. Create windows, draw shapes, render
    text, and handle mouse/keyboard inputs natively.
  - **Audio Engine (audio.):** Stream MP3/WAV music or load multiple Sound Effects
    (SFX) into RAM for game development.
  - **Networking (net.):** Low-level TCP socket support. Create clients, servers,
    send/receive data, and scan ports — cross-platform (Linux & Windows).
  - **System & OS (os., io.):** Execute shell commands (os.exec), read/write files,
    and manage environment variables.
  - **Math & Strings (math., str.):** Random number generation, trigonometry, string
    splitting, replacing, and trimming.

The Nebania Ecosystem

Chain-Lang is the core of a growing suite of tools built for NebulaOS, collectively
known as the **Penthouse Apps**:

  - **Bellhop** — A custom shell for NebulaOS. Supports job control, command history,
    aliases, tab completion, and a Chain-Lang plugin system (`.link` scripts as shell
    extensions).
    
  - **Martini** — A terminal-based file manager. Browse the filesystem, open `.link`
    scripts in Sucrose, or run them with `chainlang` directly. The built-in Concierge
    system analyses `.link` files using a scoring algorithm (`sick.link`) to detect
    Bellhop plugins and install them on the spot — all from a TUI interface.
    
  - **Sucrose** — A terminal text editor with full syntax highlighting for Chain-Lang,
    configurable via a `syntax.link` config file. No recompile needed to add new
    language support.

Together, these tools form the NebulaOS user environment: a shell, a file manager,
and an editor all designed to work natively with Chain-Lang. The Penthouse Apps are
available for download at https://nebania.site

Runtime Modes

1.  **File Mode:** Execute .link script files.
2.  **Interactive REPL:** A smart shell that supports multi-line blocks (Shift+Enter
    logic).
3.  **AST Debug Mode:** Run with ./link --debug <file> to visualize the Abstract
    Syntax Tree.

Installation & Build

Ensure you have a C++ compiler installed (G++ recommended) and Raylib installed
on your system.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Pilot0253/chain-lang.git
    cd chain-lang
    ```
2.  **Install Dependencies (Linux):**
```bash
    sudo apt install libraylib-dev
```
3.  **Build the project:**
```bash
    make          for a standard build
    make debug    for a build with debug symbols (for development)
    make release  for an optimized build
```
4.  **Optional — install to PATH:**
```bash
    sudo make install
```
    This keeps the `chainlang` binary in this project folder and symlinks it into
    `/usr/local/bin` as both `chain` and `chainlang`, so you can run either command
    from anywhere.

Usage
Running a Script
**To run a file ending in .link:**
```bash
./chainlang examples/hello.link
```
