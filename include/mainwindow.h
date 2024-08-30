#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Ensure this macro is present

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H

