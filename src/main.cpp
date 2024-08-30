#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    // Enable high DPI scaling
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Set the scale factor manually (optional, if automatic scaling does not work)
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", QByteArray("0"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}