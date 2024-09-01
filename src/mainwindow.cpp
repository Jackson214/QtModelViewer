#include "mainwindow.h"
#include "viewport3d.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QKeyEvent>
#include <iostream>
#include <Qt3DRender/QCamera>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    mViewport = new Viewport3D;
    QWidget *container = QWidget::createWindowContainer(mViewport);
    container->setFocusPolicy(Qt::StrongFocus);
    qApp->installEventFilter(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(container);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    resize(800, 600);
    setMinimumSize(640, 480);
    setMaximumSize(1280, 960);
    setWindowTitle("Qt3D Viewport");
    
}

MainWindow::~MainWindow() {}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QPointF mousePosition = mouseEvent->localPos();
            mViewport->triggerRaycast(mousePosition);
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

