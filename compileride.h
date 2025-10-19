#ifndef COMPILERIDE_H
#define COMPILERIDE_H

#include <QMainWindow>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class CodeEditor;
class SyntaxHighlighter;

class CompilerIDE : public QMainWindow {
    Q_OBJECT

public:
    explicit CompilerIDE(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onLanguageChanged(int index);
    void runCode();
    void clearOutput();
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void changeFont();
    void about();
    void updateStatusBar();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void loadTemplate(const QString &lang);
    void loadSettings();
    void saveSettings();

    void compileCpp(const QString &code);
    void compileC(const QString &code);
    void runPython(const QString &code);
    void runJavaScript(const QString &code);

    CodeEditor *codeEditor;
    QTextEdit *outputEdit;
    QComboBox *languageCombo;
    QPushButton *runButton;
    QPushButton *clearButton;
    QLabel *statusLabel;
    QLabel *lineColLabel;
    QLabel *langLabel;
    SyntaxHighlighter *highlighter;
    QString currentFile;
};

#endif // COMPILERIDE_H