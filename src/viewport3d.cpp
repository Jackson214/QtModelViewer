#include "viewport3d.h"
#include <QWidget>
#include <QVBoxLayout>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QCamera>
#include <QKeyEvent>
#include <Qt3DCore/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QAttribute>
#include <Qt3DCore/QBuffer>
#include <QVector3D>
#include <QByteArray>
#include <iostream>
#include <QColor>

Viewport3D::~Viewport3D() {}

Viewport3D::Viewport3D(QWidget *parent) : Qt3DExtras::Qt3DWindow() {
    auto *rootEntity = new Qt3DCore::QEntity;
    mContainer = QWidget::createWindowContainer(this, parent);
    mContainer->setFocusPolicy(Qt::StrongFocus);
    createScene(rootEntity);

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

void Viewport3D::createScene(Qt3DCore::QEntity* rootEntity) {
    // Default cube
    // Qt3DCore::QEntity* cubeEntity = new Qt3DCore::QEntity(rootEntity);
    // auto *mesh = new Qt3DExtras::QCuboidMesh(cubeEntity);
    // auto *material = new Qt3DExtras::QPhongMaterial(cubeEntity);
    // auto *shapeEntity = new Qt3DCore::QEntity(cubeEntity);
    // shapeEntity->addComponent(mesh);
    // shapeEntity->addComponent(material);
    // shapeEntity->setObjectName("cubeEntity");

    createPyramidEntity(rootEntity);
    // createAxisLines(rootEntity);

}

void Viewport3D::createPyramidEntity(Qt3DCore::QEntity* parent) {
    // Define the vertices of the pyramid
    QVector<QVector3D> vertices = {
        { 0.0f,  1.0f,  0.0f}, // Top vertex
        {-1.0f, -1.0f,  1.0f}, // Front-left vertex
        { 1.0f, -1.0f,  1.0f}, // Front-right vertex
        { 1.0f, -1.0f, -1.0f}, // Back-right vertex
        {-1.0f, -1.0f, -1.0f}  // Back-left vertex
    };

    // Define the indices for the pyramid faces
    QVector<unsigned int> indices = {
        0, 1, 2, // Front face
        0, 2, 3, // Right face
        0, 3, 4, // Back face
        0, 4, 1, // Left face
        1, 3, 2, // Base face (first triangle)
        1, 4, 3  // Base face (second triangle)
    };

    // Define the colors for each face
    QVector<QColor> colors = {
        Qt::red,    // Front face
        Qt::green,  // Right face
        Qt::blue,   // Back face
        Qt::yellow, // Left face
        Qt::cyan,   // Base face (first triangle)
        Qt::magenta // Base face (second triangle)
    };

    // Create the pyramid entity
    createEntity(parent, vertices, indices, colors);
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


void Viewport3D::createEntity(Qt3DCore::QEntity* parent, const QVector<QVector3D>& vertices, const QVector<unsigned int>& indices, const QVector<QColor>& colors) {
    Qt3DCore::QEntity* createdEntity = new Qt3DCore::QEntity(parent);
    auto *geometry = new Qt3DCore::QGeometry(createdEntity);

    QByteArray vertexData;
    vertexData.resize(vertices.size() * 3 * sizeof(float));
    float *vertexDataPtr = reinterpret_cast<float*>(vertexData.data());
    for (const QVector3D &vertex : vertices) {
        *vertexDataPtr++ = vertex.x();
        *vertexDataPtr++ = vertex.y();
        *vertexDataPtr++ = vertex.z();
    }

    auto *vertexBuffer = new Qt3DCore::QBuffer(geometry);
    vertexBuffer->setData(vertexData);

    auto *positionAttribute = new Qt3DCore::QAttribute();
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(vertices.size());

    QByteArray indexData;
    indexData.resize(indices.size() * sizeof(unsigned int));
    unsigned int *indexDataPtr = reinterpret_cast<unsigned int*>(indexData.data());
    for (unsigned int index : indices) {
        *indexDataPtr++ = index;
    }

    auto *indexBuffer = new Qt3DCore::QBuffer(geometry);
    indexBuffer->setData(indexData);

    auto *indexAttribute = new Qt3DCore::QAttribute();
    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(indices.size());

    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(indexAttribute);

    auto *geometryRenderer = new Qt3DRender::QGeometryRenderer(createdEntity);
    geometryRenderer->setGeometry(geometry);
    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);

    for (int i = 0; i < indices.size(); i += 3) {
        auto *material = new Qt3DExtras::QPhongMaterial(createdEntity);
        material->setDiffuse(colors[i / 3]);

        auto *entity = new Qt3DCore::QEntity(createdEntity);
        entity->addComponent(geometryRenderer);
        entity->addComponent(material);
    }
}

void Viewport3D::createAxisLines(Qt3DCore::QEntity* parent) {
//     struct Line {
//         QVector3D start;
//         QVector3D end;
//         QColor color;
//     };

//     QVector<Line> lines = {
//         {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, Qt::red},   // X-axis
//         {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, Qt::green}, // Y-axis
//         {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, Qt::blue}   // Z-axis
//     };

//     for (const Line& line : lines) {
//         QVector<QVector3D> vertices = {line.start, line.end};
//         QVector<unsigned int> indices = {0, 1};

//         createEntity(parent, vertices, indices, line.color);
//     }
}
