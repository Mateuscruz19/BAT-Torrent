#include <QApplication>
#include <QIcon>
#include "core/sessionmanager.h"
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("BATorrent");
    app.setWindowIcon(QIcon(":/images/logo1.png"));

    SessionManager session;
    MainWindow window(&session);
    window.show();

    return app.exec();
}
