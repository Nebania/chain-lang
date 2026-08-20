---

# ChainLang Webview UI Framework Tutorial

Learn how to build desktop applications with **ChainLang** using an external HTML/CSS frontend, background worker threads, and dual IPC communication.

---

## 💡 Key Concepts

1. **Separation of Concerns:** Keep your layout/styles inside `index.html` and application logic inside your `.chain` script.
2. **Non-Blocking UI:** Heavy compute or long-running tasks belong inside background functions using `thread.spawn()`.
3. **Bi-Directional IPC:**
* **Frontend $\rightarrow$ Backend:** JS calls `chain_print(data)` to deliver messages to C++/ChainLang.
* **Backend $\rightarrow$ Frontend:** ChainLang calls `webview.eval("jsFunction()")` to run dynamic JavaScript inside the window.

---

## 🛠️ Step-by-Step Usage Guide

### Step 1: Design the Frontend (`index.html`)

Create an `index.html` file in your project root. Define your UI elements and set up the JavaScript handlers.

---

### Step 2: Write the Backend Logic (`main.chain`)

Create your `main.chain` script to load the UI file, start background workers, and trigger JavaScript methods.

```python
print("=== Starting Application ===")

# 1. Load the UI markup into memory
set ui = io.read("index.html")

# 2. Define background task logic
func logicBackend() {
    # Wait for the webview window to complete initialization
    time.sleep(2000) 
    
    # Push status update from Backend to Frontend UI
    webview.eval("updateStatus('ChainLang Backend Ready!')")
}

# 3. Spawn background logic thread so it doesn't freeze the UI loop
thread.spawn(logicBackend)

# 4. Render native webview window (Blocking Main Loop)
# Usage: webview.show(window_title, width, height, html_content)
webview.show("Clean Architecture Application", 800, 600, ui)

```

---

## ⚡ API Quick Reference

| ChainLang Command | Description | Example |
| --- | --- | --- |
| `io.read(path)` | Reads external file into a string variable. | `set content = io.read("index.html")` |
| `webview.show(title, w, h, html)` | Creates and renders native OS window. | `webview.show("App", 800, 600, ui)` |
| `webview.eval(js_string)` | Executes JavaScript in the active UI context. | `webview.eval("alert('Hello')")` |
| `thread.spawn(function)` | Runs a function in a non-blocking background thread. | `thread.spawn(workerFunc)` |
| `time.sleep(ms)` | Pauses execution for a specified duration in milliseconds. | `time.sleep(1000)` |
