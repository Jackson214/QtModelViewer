#include "viewport3d.h"
#include <QWidget>
#include <QVBoxLayout>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QCamera>

Viewport3D::~Viewport3D() {}

Viewport3D::Viewport3D(QWidget *parent) : Qt3DExtras::Qt3DWindow() {
    QWidget *container = QWidget::createWindowContainer(this, parent);
    auto *rootEntity = createScene();

    setRootEntity(rootEntity);

    // Set up the camera
    Qt3DRender::QCamera *camera = this->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 5.0f));  // Adjust the Z value to zoom out
    camera->setViewCenter(QVector3D(0, 0, 0));

    // Set up the camera controller
    auto *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(camera);

    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->addWidget(container);
}

Qt3DCore::QEntity *Viewport3D::createScene() {
    auto *rootEntity = new Qt3DCore::QEntity;

    auto *mesh = new Qt3DExtras::QCuboidMesh(rootEntity);
    auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);

    auto *shapeEntity = new Qt3DCore::QEntity(rootEntity);
    shapeEntity->addComponent(mesh);
    shapeEntity->addComponent(material);

    return rootEntity;
}

