#include "mainwindow.h"
#include "viewport3d.h"
#include <QWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *container = QWidget::createWindowContainer(new Viewport3D);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(container);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    resize(800, 600);
}

MainWindow::~MainWindow() {}

