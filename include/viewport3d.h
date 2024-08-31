#pragma once
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>

class Viewport3D : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT

public:
    Viewport3D(QWidget *parent = nullptr);
    ~Viewport3D();

    void moveCameraForward(float distance);
    void moveCameraBackward(float distance);


private:
    Qt3DCore::QEntity *createScene();
    Qt3DCore::QEntity* createPyramidEntity(Qt3DCore::QEntity *rootEntity);
    Qt3DRender::QCamera *mCamera;
    Qt3DExtras::QOrbitCameraController *mCamController;
    QWidget *mContainer;
};