#include "mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("BATorrent");
    setWindowIcon(QIcon(":/images/logo1.png"));
}

MainWindow::~MainWindow() {}