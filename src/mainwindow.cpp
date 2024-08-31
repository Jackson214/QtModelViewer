#include "mainwindow.h"
#include "viewport3d.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QKeyEvent>
#include <iostream>

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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{   
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (keyEvent) {
            qDebug() << "key " << keyEvent->key() << "from" << obj;
            if (keyEvent->key() == Qt::Key_W) {
                mViewport->moveCameraForward(0.1f);
                return true;  // Indicate that the event has been handled
            }
            else if (keyEvent->key() == Qt::Key_S) {
                mViewport->moveCameraBackward(0.1f);
                return true;  // Indicate that the event has been handled
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

