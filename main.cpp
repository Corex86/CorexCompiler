#include "compileride.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("CodeIDE");
    app.setOrganizationName("CodeIDE");
    app.setApplicationVersion("1.0");

    CompilerIDE window;
    window.show();

    return app.exec();
}

