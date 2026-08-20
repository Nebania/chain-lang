---

# 📖 ChainLang Official Syntax Guide

ברוכים הבאים ל CHAINLANG,שפה מודרנית ודינאמית לשימוש קל,החל מ scripting פשוט, חישוב מדעי וריבוי-תהליכונים עד לאפליקציות ורישות


מציוד עם איתור שגיאות כמו של שפת rust ,chainlang עוזרת לתאר באגים בקלות ובדיוק רב

Here is the comprehensive guide to writing syntax in ChainLang.
הינה המדריך מקיף על כתיבר תחביר בchainlang
---

## 1. הערות ופלט

שתמש ב`//` לכתיבת הערות וב`print()` לפלט במסוף

```javascript
// זאת הערה חד-שורתית
print("Hello, World!") 
print("ChainLang is easy to learn.")

```

## 2. משתנים,סוגי נתונים ושרשור רב-שורתי

 כדי להכריז על משתנה הישתמש ב`set`,משתנים ב הם דטנאמים, כלמור לא צריך להכריז איזה סוג הם היו והמנוע כבר ידע איזה סוג הם לפי ערכם.chain תומך בשרשור רב-שורתי היכול להחסן HTML וJSON 



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

## 3. מבני נתונים(מילונים ורשימות)

chain תומך במערכים,רשימות ובאובייקטי JSON


**רשימה:**

```javascript
set fruits = ["Apple", "Mango", "Orange"]
print(fruits[0])           // פלט: Apple

list.add(fruits, "Banana") // מוסיף נתון
list.pop(fruits)           // מוחק את הפריט האחרון

```

**מילונים**

```javascript
set user = {
    "name": "Pilot0253",
    "role": "Admin",
    "level": 99
}
print(user.name)        // Output: Pilot0253
set user.level = 100    // Mutate value

```

## 4. זרימת בקרה

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

### B. התאמת תבניות (Switch-Case מודרני)

השתמש ב-`match` לפיצול מבני נקי יותר. השתמש ב-`_` עבור מקרה ברירת המחדל.

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

## 5. לולאות

### A. For Loop (עם שימוש ב Range)

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

## 6. פונקציות ולמבדה

השתמש ב-`func` כדי להגדיר פונקציות. נתמך שימוש אופציונלי ברמזי-טיפוס (type hinting) לשיפור קריאות הקוד.

**Basic Function:**

```javascript
func greet(name) {
    return "Hello, " + name
}
print(greet("Chain"))

```

**פונקציה עם Type Hinting (Gradual Typing):**

```javascript
func add(a: Int, b: Int) -> Int {
    return a + b
}

```

**למבדה (Anonymous Function):**
פונקציות בחיות בתוך משתנה וניתנם לשימוש.

```javascript
set double = func(x) { return x * 2 }
print(double(10)) // Output: 20

```

## 7. תכנות מוחנה עצמים (OOP)

ChainLang מספקת תמיכה מלאה במחלקות, ירושה ושיטות קסם (העמסת אופרטורים).

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

## 8. טיפול בשגיאות ומעקב ויזואלי

ChainLang כוללת מנגנון **מעקב ויזואלי אחר שגיאות בסגנון Rust**. במקרה של שגיאות תחביר או שגיאות זמן-ריצה, הביצוע נעצר בצורה בטוחה, תוך הצבעה ישירה על השורה והעמודה המדויקות באמצעות סימני חיווי (`^^^`).

ניתן ללכוד שגיאות באופן ידני כדי למנוע סיום בלתי-צפוי של התוכנית:

```javascript
try {
    // Attempting to read a non-existent file
    set data = io.read("missing.txt")
} catch (e) {
    print("An error occurred: " + e)
}

```

## 9. מקביליות (Multithreading / Goroutines)

העברת משימות רקע כבדות (כגון שרתי רשת או לולאות עיבוד עתירות-משאבים) לביצוע נפרד, מבלי לעצור את תהליך הביצוע הראשי או את לולאת ממשק המשתמש (UI).

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

## 10.יישומי שולחן עבודה מקוריים (Webview)

בנו ממשקי שולחן עבודה מקוריים באמצעות **HTML, CSS ו-JavaScript**, תוך הרצת לוגיקת צד-שרת (backend) בתוך ChainLang. פתרון קל-משקל ומודרני, התומך באופן מלא בתקשורת דו-כיוונית (IPC).

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

## 11. אינטראופרביליות (Interop) ישירה עם C++ (Extern C++)

ניתן להטמיע קוד C++ טהור ישירות בתוך סקריפטים של ChainLang. משתנים משותפים באופן אוטומטי בין הקשרים (contexts) באמצעות זיכרון משותף *(נתמך ב-POSIX,‏ Linux ו-macOS)*.

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

## 📚 מדריך לספרייה הסטנדרטית

ChainLang כוללת מודולים מובנים הזמינים ב־scope הגלובלי, ללא צורך בייבוא מפורש.

### 🧮 מתמטיקה (חישובים מדעיים)

**ליבה:** math.pi(), math.e(), math.abs(x), math.sqrt(x), math.pow(b, e)
**טריגונומטריה:** math.sin(x), math.cos(x), math.tan(x), math.asin(x), math.acos(x), math.atan2(y, x)
**אקספוננטים ולוגריתמים:** math.exp(x), math.log(x), math.log10(x), math.log2(x)
**כלי עזר:** math.ceil(x), math.floor(x), math.round(x), math.min(a, b), math.max(a, b), math.random(), math.randint(min, max)

### 🔤 מחרוזות ואוספים

**מחרוזות:** str.upper(s), str.lower(s), str.trim(s), str.split(s, ","), str.replace(s, "a", "b"), str.substr(s, start, length), str.contains(s, "x")
**רשימות:** list.add(l, x), list.remove(l, idx), list.insert(l, idx, x), list.pop(l)
**מילונים:** dict.keys(d), dict.has(d, "key"), dict.remove(d, "key")
**כללי:** len(obj) (תומך במחרוזות, רשימות ומילונים)

### 🖥️ מערכת ו־File I/O

**קבצים:** io.read("file.txt"), io.write("file.txt", "Text"), io.append("file.txt", "Text"), io.exists("file.txt"), io.remove("file.txt")
**מערכת ההפעלה:** os.exec("ls -la"), os.getenv("PATH"), os.setenv("K", "V"), os.cwd(), os.chdir("dir"), os.date()
**תהליכונים:** thread.spawn(func)
**זמן:** time.sleep(ms)

### 🌐 תקשורת רשת

net.server(port) : יוצר שרת TCP.
net.connect(ip, port) : מתחבר למארח TCP מרוחק.
net.accept(sock) : מקבל חיבור Socket נכנס (חוסם עד לקבלת חיבור).
net.send(sock, data) & net.recv(sock) : שולח ומקבל נתוני Socket.
net.close(sock) : סוגר Socket רשת.

### 🎨 ממשק גרפי וגרפיקה (מנוע Raylib)

**ניהול חלון:** gui_setup(w, h, "Title"), gui_running(), gui_close(), gui.width(), gui.height()
**צינור הרינדור:** gui_start(), gui_present(), gui_clear("color")
**פרימיטיבים:** gui_rect(x, y, w, h, c), gui_line(x1, y1, x2, y2, thick, c), gui_text(x, y, txt, c, size)
**רינדור מתקדם:** gui_begin_clip(x, y, w, h), gui_end_clip() (תמיכה ב־Scissor/Masking)
**טיפול בקלט:** gui.mouse_x(), gui.mouse_y(), gui.get_mouse_wheel(), gui_click(), gui_is_key_pressed("K"), gui_get_char()

### 🎵 מנוע אודיו

audio.init(), audio.close()
audio.play("track.mp3"), audio.pause(), audio.stop(), audio.volume(0.5)
audio.update() (יש לקרוא לפונקציה בתוך לולאת הפריימים כדי להזרים את מאגר האודיו)
audio.get_eq(band) (מחזירה תחומי תדר FFT עבור ויזואליזציות אודיו)

### 🌍 Webview (מנוע Desktop מודרני)

webview.show(title, width, height, html_string) : יוצר חלון Native של מערכת ההפעלה.
webview.eval(js_string) : מריץ מחרוזות JavaScript מתוך סביבת ה־Backend של C++.