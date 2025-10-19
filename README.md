# CodeIDE - Multi-Language Compiler

Modern ve kullanıcı dostu bir IDE/Compiler uygulaması.

## Özellikler

- ✅ C++, C, Python ve JavaScript desteği
- ✅ Syntax highlighting (renkli kod vurgulama)
- ✅ Satır numaraları
- ✅ Gerçek zamanlı derleme ve çalıştırma
- ✅ Hata gösterimi
- ✅ Dosya yönetimi (aç/kaydet)
- ✅ Kod editörü (undo/redo, copy/paste)
- ✅ Zoom in/out
- ✅ Karanlık tema
- ✅ Durum çubuğu
- ✅ Klavye kısayolları

## Derleme

### Qt Creator ile:
1. CodeIDE.pro dosyasını Qt Creator'da açın
2. Build > Run tuşuna basın

### qmake ile:
```bash
qmake
make
```

### CMake ile:
```bash
mkdir build
cd build
cmake ..
make
```

### Manuel derleme:
```bash
# Linux/Mac
g++ -std=c++17 main.cpp compileride.cpp codeeditor.cpp syntaxhighlighter.cpp \
    -o CodeIDE `pkg-config --cflags --libs Qt5Widgets` -fPIC

# Windows (MinGW)
g++ -std=c++17 main.cpp compileride.cpp codeeditor.cpp syntaxhighlighter.cpp \
    -o CodeIDE.exe -I%QTDIR%\include -L%QTDIR%\lib -lQt5Widgets -lQt5Gui -lQt5Core
```

## Gereksinimler

- Qt 5.12 veya üzeri
- C++17 uyumlu derleyici
- g++/gcc (C/C++ kodu çalıştırmak için)
- python3 (Python kodu çalıştırmak için)
- node.js (JavaScript kodu çalıştırmak için)

## Kullanım

1. Dil seçin (C++, C, Python, JavaScript)
2. Kodunuzu yazın
3. F5 tuşuna basın veya "Çalıştır" butonuna tıklayın
4. Çıktıyı alt panelde görüntüleyin

## Klavye Kısayolları

- **F5**: Kodu çalıştır
- **Ctrl+N**: Yeni dosya
- **Ctrl+O**: Dosya aç
- **Ctrl+S**: Kaydet
- **Ctrl+Shift+S**: Farklı kaydet
- **Ctrl+Z**: Geri al
- **Ctrl+Y**: Yinele
- **Ctrl+X**: Kes
- **Ctrl+C**: Kopyala
- **Ctrl+V**: Yapıştır
- **Ctrl+A**: Tümünü seç
- **Ctrl++**: Yakınlaştır
- **Ctrl+-**: Uzaklaştır

## Dosya Yapısı

```
CodeIDE/
├── main.cpp                 # Ana program
├── compileride.h            # Ana pencere header
├── compileride.cpp          # Ana pencere implementation
├── codeeditor.h             # Kod editörü header
├── codeeditor.cpp           # Kod editörü implementation
├── syntaxhighlighter.h      # Syntax highlighter header
├── syntaxhighlighter.cpp    # Syntax highlighter implementation
├── CodeIDE.pro              # Qt project file
├── CMakeLists.txt           # CMake build file
└── README.md                # Bu dosya
```

## Lisans

MIT License

## Katkıda Bulunma

Pull request'ler kabul edilir. Büyük değişiklikler için lütfen önce bir issue açın.

## Yazar

CodeIDE Team - 2025