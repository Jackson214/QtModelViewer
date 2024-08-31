#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewport3d.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Ensure this macro is present

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Viewport3D *mViewport;
};

#endif // MAINWINDOW_H

