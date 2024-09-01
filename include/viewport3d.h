#pragma once
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <QColor>
#include <Qt3DRender/QRayCaster>
#include <Qt3DCore/QEntity>
#include <QMouseEvent>
#include "Object3D.h"

class Viewport3D : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT

public:
    Viewport3D(QWidget *parent = nullptr);
    ~Viewport3D();

    void moveCameraForward(float distance);
    void moveCameraBackward(float distance);

    Object3D* getObjectByEntity(Qt3DCore::QEntity* entity);

    void triggerRaycast(const QPointF &mousePosition);


private:
    void createScene(Qt3DCore::QEntity* parent);
    void createPyramidEntity(Qt3DCore::QEntity* parent);
    
    // Indices should be defined in counter-clockwise order to determine which direction the face is facing
    void createEntity(Object3D object);
    void createAxisLines(Qt3DCore::QEntity* parent);

    Qt3DCore::QEntity* mRootEntity;
    Qt3DRender::QCamera *mCamera;
    Qt3DExtras::QOrbitCameraController *mCamController;
    QWidget *mContainer;
    QVector<Object3D> mObjects;
    Qt3DRender::QRayCaster *mRayCaster;

private slots:
    void processRaycastHits(Qt3DRender::QRayCaster::Hits hits);
};