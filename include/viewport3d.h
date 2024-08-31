#pragma once
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <QColor>

class Viewport3D : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT

public:
    Viewport3D(QWidget *parent = nullptr);
    ~Viewport3D();

    void moveCameraForward(float distance);
    void moveCameraBackward(float distance);


private:
    void createScene(Qt3DCore::QEntity* parent);
    void createPyramidEntity(Qt3DCore::QEntity* parent);
    
    // Indices should be defined in counter-clockwise order to determine which direction the face is facing
    void createEntity(Qt3DCore::QEntity* parent, const QVector<QVector3D>& vertices, const QVector<unsigned int>& indices, const QVector<QColor>& colors);
    void createAxisLines(Qt3DCore::QEntity* parent);

    Qt3DRender::QCamera *mCamera;
    Qt3DExtras::QOrbitCameraController *mCamController;
    QWidget *mContainer;
};