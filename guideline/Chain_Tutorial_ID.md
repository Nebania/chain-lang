---

# 📖 Buku Panduan ChainLang (Official Syntax Guide)

Selamat datang di **ChainLang**! Bahasa pemrograman modern, dinamis, dan tangguh yang dirancang untuk kemudahan penggunaan. Mulai dari *scripting* sederhana, komputasi sains, *multithreading*, hingga pembuatan **Aplikasi Desktop Native** dan Jaringan (*Networking*).

Dilengkapi dengan **Rust-like Visual Error Tracing**, ChainLang akan membantu Anda menemukan *bug* dengan presisi tinggi.

Berikut adalah panduan lengkap cara penulisan sintaks di ChainLang.

---

## 1. Komentar & Menampilkan Teks
Gunakan `//` untuk menulis komentar (tidak akan dieksekusi oleh program). Gunakan `print()` untuk menampilkan teks ke layar.

```javascript
// Ini adalah komentar satu baris
print("Halo, Dunia!") 
print("ChainLang sangat mudah dipelajari.")
```

## 2. Variabel, Tipe Data & Multiline String
Gunakan kata kunci `set` untuk membuat variabel. ChainLang adalah bahasa *dynamically typed*. ChainLang juga mendukung **Multiline String** secara bawaan untuk menyimpan teks panjang seperti HTML atau JSON.

```javascript
set umur = 25              // Integer
set phi = 3.14             // Float
set is_active = true       // Boolean (true / false)

// Multiline String (Bisa menggunakan Enter di dalamnya)
set html_template = "
<html>
    <body>
        <h1>Halo dari ChainLang!</h1>
    </body>
</html>
"
```

## 3. Struktur Data (List & Dictionary)
ChainLang mendukung Array (List) dan Objek JSON (Dictionary) secara bawaan.

**List (Array):**
```javascript
set buah = ["Apel", "Mangga", "Jeruk"]
print(buah[0])           // Output: Apel

list.add(buah, "Pisang") // Menambah data
list.pop(buah)           // Menghapus data terakhir
```

**Dictionary (Key-Value):**
```javascript
set user = {
    "nama": "Pilot0253",
    "role": "Admin",
    "level": 99
}
print(user.nama)        // Output: Pilot0253
set user.level = 100    // Mengubah nilai
```

## 4. Percabangan (Control Flow)
### A. If, Elif, Else
```javascript
set nilai = 85

if nilai >= 90 {
    print("Grade A")
} elif nilai >= 80 {
    print("Grade B")
} else {
    print("Grade C")
}
```

### B. Pattern Matching (Modern Switch-Case)
Gunakan `match` untuk percabangan yang lebih rapi dan canggih. Gunakan `_` untuk kondisi *default*.
```javascript
set status = 404

match status {
    200 => print("OK")
    404 => {
        print("Not Found")
        print("Periksa kembali URL Anda")
    }
    _ => print("Unknown Status")
}
```

## 5. Perulangan (Loops)
### A. For Loop (Menggunakan Range)
```javascript
// Mencetak angka 0 sampai 4
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
    // Gunakan 'break' untuk berhenti, atau 'continue' untuk melompati iterasi
}
```

## 6. Fungsi (Functions & Lambdas)
Gunakan `func` untuk mendefinisikan fungsi. Anda juga bisa menggunakan *Type Hinting* opsional untuk membuat kode lebih mudah dibaca.

**Fungsi Dasar:**
```javascript
func sapa(nama) {
    return "Halo, " + nama
}
print(sapa("Chain"))
```

**Fungsi dengan Type Hinting (Gradual Typing):**
```javascript
func tambah(a: Int, b: Int) -> Int {
    return a + b
}
```

**Lambda (Anonymous Function):**
Fungsi dapat disimpan di dalam variabel dan di-*passing* sebagai argumen!
```javascript
set kaliDua = func(x) { return x * 2 }
print(kaliDua(10)) // Output: 20
```

## 7. Pemrograman Berorientasi Objek (OOP)
ChainLang mendukung penuh *Class*, *Inheritance* (Pewarisan), dan *Magic Methods* (Operator Overloading).

```javascript
// Class Induk
class Mesin {
    func init() {
        set this.status = "Menyala"
    }
}

// Class Anak (Mewarisi Mesin)
class Robot : Mesin {
    // Constructor
    func init(nama, power) {
        set this.nama = nama
        set this.power = power
    }
    
    // Method biasa
    func info() {
        print(this.nama + " power: " + this.power)
    }
    
    // Magic Method (Operator Overloading untuk '+')
    func __add__(other) {
        return new Robot("Gabungan", this.power + other.power)
    }
}

set r1 = new Robot("Alpha", 50)
set r2 = new Robot("Beta", 30)

set r3 = r1 + r2  // Memanggil __add__ otomatis!
r3.info()         // Output: Gabungan power: 80
```

## 8. Error Handling & Visual Tracing
ChainLang dilengkapi dengan **Rust-like Visual Error Tracing**. Jika terjadi *Syntax Error* atau *Runtime Error*, ChainLang tidak akan asal *crash*, melainkan menunjukkan dengan tepat baris dan kolom yang salah menggunakan tanda panah `^^^`.

Anda juga bisa menangkap error secara manual agar program tidak berhenti:
```javascript
try {
    // Mencoba membaca file yang tidak ada
    set data = io.read("hilang.txt")
} catch (e) {
    print("Terjadi kesalahan: " + e)
}
```

## 9. Concurrency (Multithreading / Goroutines)
Jalankan tugas berat (seperti server jaringan atau *looping* panjang) di *background* tanpa membuat program utama (GUI/Webview) macet!

```javascript
func tugasBerat() {
    while true {
        print("Berjalan di background...")
        time.sleep(1000) // Tidur 1 detik
    }
}

// Jalankan di thread terpisah (Non-blocking)
thread.spawn(tugasBerat)

// Program utama tetap berjalan tanpa menunggu fungsi di atas selesai
print("Program utama selesai!")
```

## 10. Native Desktop Apps (Webview)
Buat Aplikasi Desktop Native dengan antarmuka (UI) menggunakan **HTML, CSS, dan JavaScript**, sementara logika *Backend* menggunakan ChainLang. Sangat modern, ringan, dan mendukung komunikasi dua arah!

```javascript
set ui = "
    <html>
        <body style='background: #1e1e2e; color: white;'>
            <h1>Halo dari ChainLang!</h1>
            <button onclick='chain_print(\"Data dari UI\")'>Kirim ke Backend</button>
        </body>
    </html>
"

// Buka Jendela Aplikasi Desktop
webview.show("Aplikasi Native", 800, 600, ui)

// Mengeksekusi JavaScript dari Backend (C++)
webview.eval("alert('Pesan dari Backend!')")
```

## 11. Native C++ Interop (Extern C++)
Fitur "Killer" dari ChainLang! Tulis kode C++ murni langsung di dalam skrip ChainLang Anda. Variabel akan dibagikan secara otomatis melalui *Shared Memory*. *(Hanya untuk OS POSIX/Linux/Mac)*.

```javascript
set angka = 10

extern "c" "-O3" {
    #include <iostream>
    
    // Akses variabel 'angka' dari ChainLang menggunakan prefix LINK_
    LINK_angka = LINK_angka * 5;
    std::cout << "Dihitung menggunakan C++ Native: " << LINK_angka << std::endl;
}

print(angka) // Output: 50
```

---

## 📚 Standard Library (Modul Bawaan)
ChainLang dilengkapi dengan berbagai modul siap pakai. Anda tidak perlu meng-*import* apa pun, cukup panggil namanya!

### 🧮 Math (Komputasi Sains)
*   **Dasar:** `math.pi()`, `math.e()`, `math.abs(x)`, `math.sqrt(x)`, `math.pow(b, e)`
*   **Trigonometri:** `math.sin(x)`, `math.cos(x)`, `math.tan(x)`, `math.asin(x)`, `math.acos(x)`, `math.atan2(y, x)`
*   **Eksponensial & Logaritma:** `math.exp(x)`, `math.log(x)`, `math.log10(x)`, `math.log2(x)`
*   **Utilitas:** `math.ceil(x)`, `math.floor(x)`, `math.round(x)`, `math.min(a, b)`, `math.max(a, b)`, `math.random()`, `math.randint(min, max)`

### 🔤 String & Collections
*   **String:** `str.upper(s)`, `str.lower(s)`, `str.trim(s)`, `str.split(s, ",")`, `str.replace(s, "a", "b")`, `str.substr(s, start, length)`, `str.contains(s, "x")`
*   **List:** `list.add(l, x)`, `list.remove(l, idx)`, `list.insert(l, idx, x)`, `list.pop(l)`
*   **Dict:** `dict.keys(d)`, `dict.has(d, "key")`, `dict.remove(d, "key")`
*   **Umum:** `len(obj)` *(Bisa untuk String, List, dan Dict)*

### 🖥️ Sistem & File I/O
*   **File:** `io.read("file.txt")`, `io.write("file.txt", "Teks")`, `io.append("file.txt", "Teks")`, `io.exists("file.txt")`, `io.remove("file.txt")`
*   **OS:** `os.exec("ls -la")`, `os.getenv("PATH")`, `os.setenv("K", "V")`, `os.cwd()`, `os.chdir("dir")`, `os.date()`
*   **Thread:** `thread.spawn(func)`
*   **Waktu:** `time.sleep(ms)`

### 🌐 Jaringan (Networking)
*   `net.server(port)` : Membuat TCP Server.
*   `net.connect(ip, port)` : Terhubung ke Server.
*   `net.accept(sock)` : Menerima koneksi masuk (Blocking).
*   `net.send(sock, data)` & `net.recv(sock)` : Mengirim dan menerima data.
*   `net.close(sock)` : Menutup koneksi.

### 🎨 GUI & Grafis (Raylib)
*   **Window:** `gui_setup(w, h, "Title")`, `gui_running()`, `gui_close()`, `gui.width()`, `gui.height()`
*   **Render:** `gui_start()`, `gui_present()`, `gui_clear("color")`
*   **Drawing:** `gui_rect(x, y, w, h, c)`, `gui_line(x1, y1, x2, y2, thick, c)`, `gui_text(x, y, txt, c, size)`
*   **Advanced:** `gui_begin_clip(x, y, w, h)`, `gui_end_clip()` *(Untuk Scissor/Masking)*
*   **Input:** `gui.mouse_x()`, `gui.mouse_y()`, `gui.get_mouse_wheel()`, `gui_click()`, `gui_is_key_pressed("K")`, `gui_get_char()`

### 🎵 Audio Engine
*   `audio.init()`, `audio.close()`
*   `audio.play("lagu.mp3")`, `audio.pause()`, `audio.stop()`, `audio.volume(0.5)`
*   `audio.update()` *(Wajib dipanggil di dalam loop agar musik terus berjalan)*
*   `audio.get_eq(band)` *(Untuk Audio Visualizer / Spectrum Analysis)*

### 🌍 Webview (Aplikasi Desktop Modern)
*   `webview.show(title, width, height, html_string)` : Membuka Native Window.
*   `webview.eval(js_string)` : Mengeksekusi JavaScript dari Backend C++.

---
**Selamat Berkarya dengan ChainLang! 🚀**
