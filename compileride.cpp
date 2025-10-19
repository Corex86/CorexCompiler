#include "compileride.h"
#include "codeeditor.h"
#include "syntaxhighlighter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QProcess>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QTextStream>
#include <QFontDialog>
#include <QSettings>
#include <QTextCursor>

CompilerIDE::CompilerIDE(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    loadSettings();
    setWindowTitle("CodeIDE - Multi-Language Compiler");
    resize(1200, 800);

    // Dark theme
    setStyleSheet("QMainWindow { background-color: #252526; }"
                 "QMenuBar { background-color: #2d2d30; color: #cccccc; }"
                 "QMenuBar::item:selected { background-color: #3e3e42; }"
                 "QMenu { background-color: #252526; color: #cccccc; border: 1px solid #454545; }"
                 "QMenu::item:selected { background-color: #094771; }"
                 "QToolBar { background-color: #2d2d30; border: none; spacing: 5px; }"
                 "QPushButton { background-color: #0e639c; color: white; border: none; "
                 "padding: 6px 12px; border-radius: 3px; }"
                 "QPushButton:hover { background-color: #1177bb; }"
                 "QPushButton:pressed { background-color: #094771; }"
                 "QComboBox { background-color: #3c3c3c; color: #cccccc; border: 1px solid #454545; "
                 "padding: 5px; border-radius: 3px; }"
                 "QComboBox::drop-down { border: none; }"
                 "QComboBox::down-arrow { image: none; border: none; }"
                 "QStatusBar { background-color: #007acc; color: white; }");
}

void CompilerIDE::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Top toolbar
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->setContentsMargins(10, 5, 10, 5);

    languageCombo = new QComboBox();
    languageCombo->addItems({"C++", "C", "Python", "JavaScript"});
    languageCombo->setMinimumWidth(150);
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CompilerIDE::onLanguageChanged);

    runButton = new QPushButton("▶ Çalıştır (F5)");
    runButton->setShortcut(QKeySequence(Qt::Key_F5));
    connect(runButton, &QPushButton::clicked, this, &CompilerIDE::runCode);

    clearButton = new QPushButton("🗑 Temizle");
    connect(clearButton, &QPushButton::clicked, this, &CompilerIDE::clearOutput);

    topBar->addWidget(new QLabel("Dil:"));
    topBar->addWidget(languageCombo);
    topBar->addWidget(runButton);
    topBar->addWidget(clearButton);
    topBar->addStretch();

    mainLayout->addLayout(topBar);

    // Splitter for code editor and output
    QSplitter *splitter = new QSplitter(Qt::Vertical);

    codeEditor = new CodeEditor();
    codeEditor->setPlaceholderText("Kodunuzu buraya yazın...");

    highlighter = new SyntaxHighlighter(codeEditor->document());
    highlighter->setLanguage("C++");

    outputEdit = new QTextEdit();
    outputEdit->setReadOnly(true);
    outputEdit->setStyleSheet("QTextEdit { background-color: #1e1e1e; color: #d4d4d4; "
                             "border: none; }");
    outputEdit->setFont(QFont("Consolas", 10));

    splitter->addWidget(codeEditor);
    splitter->addWidget(outputEdit);
    splitter->setSizes({600, 200});

    mainLayout->addWidget(splitter);
    setCentralWidget(centralWidget);

    loadTemplate("C++");
}

void CompilerIDE::setupMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);

    // File menu
    QMenu *fileMenu = menuBar->addMenu("Dosya");

    QAction *newAct = new QAction("Yeni", this);
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &CompilerIDE::newFile);
    fileMenu->addAction(newAct);

    QAction *openAct = new QAction("Aç", this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &CompilerIDE::openFile);
    fileMenu->addAction(openAct);

    QAction *saveAct = new QAction("Kaydet", this);
    saveAct->setShortcut(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &CompilerIDE::saveFile);
    fileMenu->addAction(saveAct);

    QAction *saveAsAct = new QAction("Farklı Kaydet", this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAct, &QAction::triggered, this, &CompilerIDE::saveFileAs);
    fileMenu->addAction(saveAsAct);

    fileMenu->addSeparator();

    QAction *exitAct = new QAction("Çıkış", this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAct);

    // Edit menu
    QMenu *editMenu = menuBar->addMenu("Düzenle");

    QAction *undoAct = new QAction("Geri Al", this);
    undoAct->setShortcut(QKeySequence::Undo);
    connect(undoAct, &QAction::triggered, codeEditor, &QPlainTextEdit::undo);
    editMenu->addAction(undoAct);

    QAction *redoAct = new QAction("Yinele", this);
    redoAct->setShortcut(QKeySequence::Redo);
    connect(redoAct, &QAction::triggered, codeEditor, &QPlainTextEdit::redo);
    editMenu->addAction(redoAct);

    editMenu->addSeparator();

    QAction *cutAct = new QAction("Kes", this);
    cutAct->setShortcut(QKeySequence::Cut);
    connect(cutAct, &QAction::triggered, codeEditor, &QPlainTextEdit::cut);
    editMenu->addAction(cutAct);

    QAction *copyAct = new QAction("Kopyala", this);
    copyAct->setShortcut(QKeySequence::Copy);
    connect(copyAct, &QAction::triggered, codeEditor, &QPlainTextEdit::copy);
    editMenu->addAction(copyAct);

    QAction *pasteAct = new QAction("Yapıştır", this);
    pasteAct->setShortcut(QKeySequence::Paste);
    connect(pasteAct, &QAction::triggered, codeEditor, &QPlainTextEdit::paste);
    editMenu->addAction(pasteAct);

    editMenu->addSeparator();

    QAction *selectAllAct = new QAction("Tümünü Seç", this);
    selectAllAct->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAct, &QAction::triggered, codeEditor, &QPlainTextEdit::selectAll);
    editMenu->addAction(selectAllAct);

    // View menu
    QMenu *viewMenu = menuBar->addMenu("Görünüm");

    QAction *fontAct = new QAction("Yazı Tipi", this);
    connect(fontAct, &QAction::triggered, this, &CompilerIDE::changeFont);
    viewMenu->addAction(fontAct);

    QAction *zoomInAct = new QAction("Yakınlaştır", this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAct, &QAction::triggered, codeEditor, &QPlainTextEdit::zoomIn);
    viewMenu->addAction(zoomInAct);

    QAction *zoomOutAct = new QAction("Uzaklaştır", this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAct, &QAction::triggered, codeEditor, &QPlainTextEdit::zoomOut);
    viewMenu->addAction(zoomOutAct);

    // Run menu
    QMenu *runMenu = menuBar->addMenu("Çalıştır");

    QAction *runAct = new QAction("Kodu Çalıştır", this);
    runAct->setShortcut(Qt::Key_F5);
    connect(runAct, &QAction::triggered, this, &CompilerIDE::runCode);
    runMenu->addAction(runAct);

    QAction *clearOutputAct = new QAction("Çıktıyı Temizle", this);
    connect(clearOutputAct, &QAction::triggered, this, &CompilerIDE::clearOutput);
    runMenu->addAction(clearOutputAct);

    // Help menu
    QMenu *helpMenu = menuBar->addMenu("Yardım");

    QAction *aboutAct = new QAction("Hakkında", this);
    connect(aboutAct, &QAction::triggered, this, &CompilerIDE::about);
    helpMenu->addAction(aboutAct);

    setMenuBar(menuBar);
}

void CompilerIDE::setupToolBar() {
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(20, 20));

    QAction *newAct = toolbar->addAction("📄 Yeni");
    connect(newAct, &QAction::triggered, this, &CompilerIDE::newFile);

    QAction *openAct = toolbar->addAction("📂 Aç");
    connect(openAct, &QAction::triggered, this, &CompilerIDE::openFile);

    QAction *saveAct = toolbar->addAction("💾 Kaydet");
    connect(saveAct, &QAction::triggered, this, &CompilerIDE::saveFile);

    toolbar->addSeparator();

    QAction *runAct = toolbar->addAction("▶ Çalıştır");
    connect(runAct, &QAction::triggered, this, &CompilerIDE::runCode);

    QAction *clearAct = toolbar->addAction("🗑 Temizle");
    connect(clearAct, &QAction::triggered, this, &CompilerIDE::clearOutput);

    addToolBar(toolbar);
}

void CompilerIDE::setupStatusBar() {
    statusLabel = new QLabel("Hazır");
    statusBar()->addWidget(statusLabel);

    lineColLabel = new QLabel("Satır: 1, Sütun: 1");
    statusBar()->addPermanentWidget(lineColLabel);

    langLabel = new QLabel("C++");
    statusBar()->addPermanentWidget(langLabel);

    connect(codeEditor, &QPlainTextEdit::cursorPositionChanged, this, &CompilerIDE::updateStatusBar);
}

void CompilerIDE::updateStatusBar() {
    QTextCursor cursor = codeEditor->textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.columnNumber() + 1;
    lineColLabel->setText(QString("Satır: %1, Sütun: %2").arg(line).arg(col));

    langLabel->setText(languageCombo->currentText());

    int charCount = codeEditor->toPlainText().length();
    int lineCount = codeEditor->document()->blockCount();
    statusLabel->setText(QString("Karakter: %1 | Satır: %2").arg(charCount).arg(lineCount));
}

void CompilerIDE::onLanguageChanged(int index) {
    Q_UNUSED(index);
    QString lang = languageCombo->currentText();
    highlighter->setLanguage(lang);
    updateStatusBar();

    if (codeEditor->toPlainText().isEmpty()) {
        loadTemplate(lang);
    }
}

void CompilerIDE::runCode() {
    QString lang = languageCombo->currentText();
    QString code = codeEditor->toPlainText();

    if (code.isEmpty()) {
        outputEdit->setPlainText("Hata: Kod boş olamaz!");
        return;
    }

    outputEdit->clear();
    outputEdit->append("=== Derleniyor ve Çalıştırılıyor ===\n");

    if (lang == "C++") {
        compileCpp(code);
    } else if (lang == "C") {
        compileC(code);
    } else if (lang == "Python") {
        runPython(code);
    } else if (lang == "JavaScript") {
        runJavaScript(code);
    }
}

void CompilerIDE::compileCpp(const QString &code) {
    QFile file("temp.cpp");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        outputEdit->append("Hata: Dosya oluşturulamadı!");
        return;
    }

    QTextStream out(&file);
    out << code;
    file.close();

    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_UNUSED(exitStatus);
        if (exitCode == 0) {
            outputEdit->append("✓ Derleme başarılı!\n");
            outputEdit->append("=== Program Çıktısı ===\n");

            QProcess *runProcess = new QProcess(this);
            connect(runProcess, &QProcess::readyReadStandardOutput, [this, runProcess]() {
                outputEdit->append(runProcess->readAllStandardOutput());
            });
            connect(runProcess, &QProcess::readyReadStandardError, [this, runProcess]() {
                outputEdit->append("Hata: " + runProcess->readAllStandardError());
            });

#ifdef Q_OS_WIN
            runProcess->start("temp.exe");
#else
            runProcess->start("./temp");
#endif
        } else {
            outputEdit->append("✗ Derleme hatası:\n");
            outputEdit->append(process->readAllStandardError());
        }
        process->deleteLater();
    });

#ifdef Q_OS_WIN
    process->start("g++", QStringList() << "temp.cpp" << "-o" << "temp.exe");
#else
    process->start("g++", QStringList() << "temp.cpp" << "-o" << "temp");
#endif
}

void CompilerIDE::compileC(const QString &code) {
    QFile file("temp.c");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        outputEdit->append("Hata: Dosya oluşturulamadı!");
        return;
    }

    QTextStream out(&file);
    out << code;
    file.close();

    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_UNUSED(exitStatus);
        if (exitCode == 0) {
            outputEdit->append("✓ Derleme başarılı!\n");
            outputEdit->append("=== Program Çıktısı ===\n");

            QProcess *runProcess = new QProcess(this);
            connect(runProcess, &QProcess::readyReadStandardOutput, [this, runProcess]() {
                outputEdit->append(runProcess->readAllStandardOutput());
            });
            connect(runProcess, &QProcess::readyReadStandardError, [this, runProcess]() {
                outputEdit->append("Hata: " + runProcess->readAllStandardError());
            });

#ifdef Q_OS_WIN
            runProcess->start("temp.exe");
#else
            runProcess->start("./temp");
#endif
        } else {
            outputEdit->append("✗ Derleme hatası:\n");
            outputEdit->append(process->readAllStandardError());
        }
        process->deleteLater();
    });

#ifdef Q_OS_WIN
    process->start("gcc", QStringList() << "temp.c" << "-o" << "temp.exe");
#else
    process->start("gcc", QStringList() << "temp.c" << "-o" << "temp");
#endif
}

void CompilerIDE::runPython(const QString &code) {
    QFile file("temp.py");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        outputEdit->append("Hata: Dosya oluşturulamadı!");
        return;
    }

    QTextStream out(&file);
    out << code;
    file.close();

    QProcess *process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [this, process]() {
        outputEdit->append(process->readAllStandardOutput());
    });
    connect(process, &QProcess::readyReadStandardError, [this, process]() {
        outputEdit->append("Hata: " + process->readAllStandardError());
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [process]() { process->deleteLater(); });

    process->start("python", QStringList() << "temp.py");
}

void CompilerIDE::runJavaScript(const QString &code) {
    QFile file("temp.js");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        outputEdit->append("Hata: Dosya oluşturulamadı!");
        return;
    }

    QTextStream out(&file);
    out << code;
    file.close();

    QProcess *process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [this, process]() {
        outputEdit->append(process->readAllStandardOutput());
    });
    connect(process, &QProcess::readyReadStandardError, [this, process]() {
        outputEdit->append("Hata: " + process->readAllStandardError());
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [process]() { process->deleteLater(); });

    process->start("node", QStringList() << "temp.js");
}

void CompilerIDE::clearOutput() {
    outputEdit->clear();
}

void CompilerIDE::newFile() {
    if (!codeEditor->toPlainText().isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Yeni Dosya",
            "Kaydedilmemiş değişiklikler var. Devam etmek istiyor musunuz?",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) return;
    }
    codeEditor->clear();
    currentFile.clear();
    setWindowTitle("CodeIDE - Yeni Dosya");
}

void CompilerIDE::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Dosya Aç", "",
        "C++ Files (*.cpp *.h);;C Files (*.c *.h);;Python Files (*.py);;JavaScript Files (*.js);;All Files (*.*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            codeEditor->setPlainText(file.readAll());
            currentFile = fileName;
            setWindowTitle("CodeIDE - " + fileName);
            file.close();

            // Auto-detect language
            if (fileName.endsWith(".cpp") || fileName.endsWith(".h")) {
                languageCombo->setCurrentText("C++");
            } else if (fileName.endsWith(".c")) {
                languageCombo->setCurrentText("C");
            } else if (fileName.endsWith(".py")) {
                languageCombo->setCurrentText("Python");
            } else if (fileName.endsWith(".js")) {
                languageCombo->setCurrentText("JavaScript");
            }
        }
    }
}

void CompilerIDE::saveFile() {
    if (currentFile.isEmpty()) {
        saveFileAs();
    } else {
        QFile file(currentFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << codeEditor->toPlainText();
            file.close();
            statusBar()->showMessage("Dosya kaydedildi: " + currentFile, 3000);
        }
    }
}

void CompilerIDE::saveFileAs() {
    QString filter;
    QString lang = languageCombo->currentText();

    if (lang == "C++") filter = "C++ Files (*.cpp);;Header Files (*.h)";
    else if (lang == "C") filter = "C Files (*.c);;Header Files (*.h)";
    else if (lang == "Python") filter = "Python Files (*.py)";
    else if (lang == "JavaScript") filter = "JavaScript Files (*.js)";

    QString fileName = QFileDialog::getSaveFileName(this, "Farklı Kaydet", "", filter);

    if (!fileName.isEmpty()) {
        currentFile = fileName;
        saveFile();
        setWindowTitle("CodeIDE - " + fileName);
    }
}

void CompilerIDE::changeFont() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, codeEditor->font(), this);
    if (ok) {
        codeEditor->setFont(font);
        outputEdit->setFont(font);
    }
}

void CompilerIDE::about() {
    QMessageBox::about(this, "Hakkında",
        "<h2>CodeIDE v1.0</h2>"
        "<p>Multi-Language Compiler & IDE</p>"
        "<p>Desteklenen Diller:</p>"
        "<ul><li>C++</li><li>C</li><li>Python</li><li>JavaScript</li></ul>"
        "<p>© 2025 CodeIDE & Onur Lexa</p>"
        "<p>Duyuru: 21.10.2025 tarihinden itibaren ingilizce desteği gelecektir!");
}

void CompilerIDE::loadTemplate(const QString &lang) {
    QString code;

    if (lang == "C++") {
        code = "#include <iostream>\n"
               "#include <vector>\n"
               "#include <string>\n\n"
               "using namespace std;\n\n"
               "int main() {\n"
               "    cout << \"Merhaba C++!\" << endl;\n"
               "    \n"
               "    // Kodunuzu buraya yazın\n"
               "    \n"
               "    return 0;\n"
               "}\n";
    } else if (lang == "C") {
        code = "#include <stdio.h>\n"
               "#include <stdlib.h>\n"
               "#include <string.h>\n\n"
               "int main() {\n"
               "    printf(\"Merhaba C!\\n\");\n"
               "    \n"
               "    // Kodunuzu buraya yazın\n"
               "    \n"
               "    return 0;\n"
               "}\n";
    } else if (lang == "Python") {
        code = "#!/usr/bin/env python3\n"
               "# -*- coding: utf-8 -*-\n\n"
               "def main():\n"
               "    print(\"Merhaba Python!\")\n"
               "    \n"
               "    # Kodunuzu buraya yazın\n"
               "    \n\n"
               "if __name__ == \"__main__\":\n"
               "    main()\n";
    } else if (lang == "JavaScript") {
        code = "// JavaScript Code\n\n"
               "function main() {\n"
               "    console.log(\"Merhaba JavaScript!\");\n"
               "    \n"
               "    // Kodunuzu buraya yazın\n"
               "    \n"
               "}\n\n"
               "main();\n";
    }

    codeEditor->setPlainText(code);
}

void CompilerIDE::loadSettings() {
    QSettings settings("CodeIDE", "Compiler");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    QString lastLang = settings.value("language", "C++").toString();
    int index = languageCombo->findText(lastLang);
    if (index >= 0) languageCombo->setCurrentIndex(index);
}

void CompilerIDE::saveSettings() {
    QSettings settings("CodeIDE", "Compiler");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("language", languageCombo->currentText());
}

void CompilerIDE::closeEvent(QCloseEvent *event) {
    if (!codeEditor->toPlainText().isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Çıkış",
            "Kaydedilmemiş değişiklikler olabilir. Çıkmak istediğinizden emin misiniz?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            event->ignore();
            return;
        }
    }

    saveSettings();
    event->accept();
}