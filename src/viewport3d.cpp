#include "viewport3d.h"
#include <QWidget>
#include <QVBoxLayout>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>

Viewport3D::~Viewport3D()
{
    // Add any cleanup code here
}

Viewport3D::Viewport3D(QWidget *parent)
    : Qt3DExtras::Qt3DWindow()
{
    QWidget *container = QWidget::createWindowContainer(this, parent);
    auto *rootEntity = createScene();

    setRootEntity(rootEntity);

    auto *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(camera());

    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->addWidget(container);
}

Qt3DCore::QEntity *Viewport3D::createScene()
{
    auto *rootEntity = new Qt3DCore::QEntity;

    auto *mesh = new Qt3DExtras::QCuboidMesh(rootEntity);
    auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);

    auto *shapeEntity = new Qt3DCore::QEntity(rootEntity);
    shapeEntity->addComponent(mesh);
    shapeEntity->addComponent(material);

    return rootEntity;
}

