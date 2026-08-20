release_notes: |
  # 🚀 ChainLang v0.6.1-beta (The "Robustness & Ecosystem" Update)

  This update marks one of the largest milestones in ChainLang’s development history. We have completely overhauled the core architecture of the *Parser* and *Runtime*, resolved critical bugs, and introduced industry-grade features.

  ChainLang is no longer just a prototype scripting language; it is now a **production-ready programming language** designed for Scientific Computing, Interactive GUI Development, and Native Desktop Application Development!

  Here is the full list of new features, upgrades, and fixes included in this release:

  ## ✨ Major Features

  ### 1. 🛡️ Visual Error Handling 

  Say goodbye to confusing crash logs! ChainLang now features an informative visual error-tracing system. When a *Syntax Error* or *Runtime Error* occurs, the engine does not silently fail; instead, it renders the exact line of code alongside caret indicators (`^^^`) pointing directly to the origin of the typo or error.

  ### 2. 🧵 Concurrency (Multithreading)

  ChainLang now supports Asynchronous Programming! Using the new `thread.spawn(func)` module, long-running processes (such as intensive processing loops or network servers) can run on background threads. This ensures your primary thread (such as Raylib UI or Webview loops) remains smooth and responsive without freezing or lagging.

  ### 3. 🖥️ Native Desktop Apps (Webview Engine)

  Introducing our latest feature: native desktop application development (similar to VSCode or Discord architectures) directly within ChainLang!

  * Uses a modern architectural split: HTML/CSS for the UI, ChainLang/C++ for the backend.
  * Supports bi-directional IPC: Invoke C++ functions from HTML, and inject dynamic JavaScript from C++ using `webview.eval()`.
  * Lightweight runtime footprint by binding directly to native OS WebKit engines (avoiding bundled Chromium/Electron overhead).

  ### 4. 🧬 Advanced Object-Oriented Programming (OOP)

  ChainLang's OOP model has been upgraded:

  * **Inheritance:** Classes can now inherit properties and methods from parent classes using the `class Robot : Engine` syntax.
  * **Magic Methods (Operator Overloading):** Implement special methods like `__add__` to allow native operator evaluation on objects (e.g., `obj1 + obj2`).

  ### 5. 🧩 Pattern Matching & Lambdas

  * **Match Statement:** A cleaner, modern alternative to nested `if/elif` blocks for structural pattern branching.
  * **First-Class Functions (Lambdas):** Functions can be assigned to variables and passed around as arguments via the `set myFunc = func(x) { ... }` syntax.
