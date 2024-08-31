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
    // else if (event->type() == QEvent::MouseButtonPress) {
    //     QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
    //     if (mouseEvent) {
    //         if (mouseEvent->button() == Qt::LeftButton) {
    //             mHoldingLeftClick = true;
    //             return true;  // Indicate that the event has been handled
    //         }
    //         else if (mouseEvent->button() == Qt::RightButton) {
    //             mHoldingRightClick = true;
    //             return true;  // Indicate that the event has been handled
    //         }
    //     }
    // }
    // else if (event->type() == QEvent::MouseButtonRelease) {
    //     QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
    //     if (mouseEvent) {
    //         if (mouseEvent->button() == Qt::LeftButton) {
    //             mHoldingLeftClick = false;
    //             return true;  // Indicate that the event has been handled
    //         }
    //         else if (mouseEvent->button() == Qt::RightButton) {
    //             mHoldingRightClick = false;
    //             return true;  // Indicate that the event has been handled
    //         }
    //     }
    // }
    // else if (event->type() == QEvent::MouseMove)
    // {
    //     QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
    //     if (mouseEvent && mHoldingLeftClick) {
    //         static QPoint lastMousePosition = mouseEvent->pos();

    //         // Calculate the delta movement
    //         QPoint delta = mouseEvent->pos() - lastMousePosition;
    //         lastMousePosition = mouseEvent->pos();

    //         // Get the current view center
    //         QVector3D viewCenter = mViewport->camera()->viewCenter();

    //         // Adjust the view center based on the mouse movement
    //         float sensitivity = 0.01f; // Adjust sensitivity as needed
    //         viewCenter.setX(viewCenter.x() + delta.x() * sensitivity);
    //         viewCenter.setY(viewCenter.y() - delta.y() * sensitivity); // Invert Y axis if needed

    //         // Update the camera's view center
    //         mViewport->camera()->setViewCenter(viewCenter);

    //         qDebug() << "New View Center: " << viewCenter.x() << ", " << viewCenter.y() << ", " << viewCenter.z();
    //         return true;  // Indicate that the event has been handled
    //     }
    // }
    return QObject::eventFilter(obj, event);
}

