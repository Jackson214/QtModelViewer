#include "viewport3d.h"
#include <QWidget>
#include <QVBoxLayout>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QCamera>
#include <QKeyEvent>
#include <iostream>

Viewport3D::~Viewport3D() {}

Viewport3D::Viewport3D(QWidget *parent) : Qt3DExtras::Qt3DWindow() {
    mContainer = QWidget::createWindowContainer(this, parent);
    mContainer->setFocusPolicy(Qt::StrongFocus);
    auto *rootEntity = createScene();

    setRootEntity(rootEntity);

    // Set up the camera
    mCamera = this->camera();
    mCamera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    mCamera->setPosition(QVector3D(0, 0, 10.0f));  // Adjust the Z value to zoom out
    mCamera->setViewCenter(QVector3D(0, 0, 0));
    
    // Set up the camera controller
    mCamController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    mCamController->setCamera(mCamera);
    mCamController->setLinearSpeed(10.0f);
    mCamController->setLookSpeed(90.0f);

    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->addWidget(mContainer);
}

Qt3DCore::QEntity *Viewport3D::createScene() {
    auto *rootEntity = new Qt3DCore::QEntity;

    auto *mesh = new Qt3DExtras::QCuboidMesh(rootEntity);
    auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);

    auto *shapeEntity = new Qt3DCore::QEntity(rootEntity);
    shapeEntity->addComponent(mesh);
    shapeEntity->addComponent(material);

    // Name the cube entity - so we can reference it elsewhere
    shapeEntity->setObjectName("cubeEntity");

    return rootEntity;
}

void Viewport3D::moveCameraForward(float distance) {
    // Get the current position and orientation of the camera
    QVector3D position = mCamera->position();
    QQuaternion orientation = mCamera->transform()->rotation();

    // Calculate the forward direction vector
    QVector3D forwardDirection = orientation.rotatedVector(QVector3D(0, 0, -1));

    // Move the camera forward in the direction it is facing
    position += forwardDirection * distance;

    // Set the new position of the camera
    mCamera->setPosition(position);

    std::cout << "Setting View Center" << std::endl;
    std::cout << "Camera position: " << position.x() << ", " << position.y() << ", " << position.z() << std::endl;
    mCamera->setViewCenter(QVector3D(0, 0, 0));
}

void Viewport3D::moveCameraBackward(float distance) {
    // Get the current position and orientation of the camera
    QVector3D position = mCamera->position();
    QQuaternion orientation = mCamera->transform()->rotation();

    // Calculate the forward direction vector
    QVector3D forwardDirection = orientation.rotatedVector(QVector3D(0, 0, -1));

    // Move the camera backward in the direction it is facing
    position -= forwardDirection * distance;

    // Set the new position of the camera
    mCamera->setPosition(position);

    std::cout << "Setting View Center" << std::endl;
    std::cout << "Camera position: " << position.x() << ", " << position.y() << ", " << position.z() << std::endl;
    mCamera->setViewCenter(QVector3D(0, 0, 0));
}

