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
    createAxisLines(rootEntity);

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
        1, 2, 3, // Base face (first triangle)
        1, 3, 4  // Base face (second triangle)
    };

    // Create the pyramid entity
    createEntity(parent, vertices, indices);
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


void Viewport3D::createEntity(Qt3DCore::QEntity* parent, const QVector<QVector3D>& vertices, const QVector<unsigned int>& indices, QColor color) {
    Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(parent);

    // Create geometry
    Qt3DCore::QGeometry* geometry = new Qt3DCore::QGeometry(entity);

    // Create vertex data buffer
    QByteArray vertexBufferData;
    vertexBufferData.resize(vertices.size() * 3 * sizeof(float));
    float* rawVertexArray = reinterpret_cast<float*>(vertexBufferData.data());
    for (int i = 0; i < vertices.size(); ++i) {
        *rawVertexArray++ = vertices[i].x();
        *rawVertexArray++ = vertices[i].y();
        *rawVertexArray++ = vertices[i].z();
    }

    Qt3DCore::QBuffer* vertexDataBuffer = new Qt3DCore::QBuffer(geometry);
    vertexDataBuffer->setData(vertexBufferData);

    Qt3DCore::QAttribute* positionAttribute = new Qt3DCore::QAttribute();
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexDataBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(vertices.size());

    // Create index data buffer
    QByteArray indexBufferData;
    indexBufferData.resize(indices.size() * sizeof(unsigned int));
    unsigned int* rawIndexArray = reinterpret_cast<unsigned int*>(indexBufferData.data());
    for (int i = 0; i < indices.size(); ++i) {
        *rawIndexArray++ = indices[i];
    }

    Qt3DCore::QBuffer* indexDataBuffer = new Qt3DCore::QBuffer(geometry);
    indexDataBuffer->setData(indexBufferData);

    Qt3DCore::QAttribute* indexAttribute = new Qt3DCore::QAttribute();
    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexDataBuffer);
    indexAttribute->setCount(indices.size());

    // Add attributes to geometry
    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(indexAttribute);

    // Create geometry renderer
    Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer();
    geometryRenderer->setGeometry(geometry);
    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);

    // Create material
    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0x928327)));

    // Add components to entity
    entity->addComponent(geometryRenderer);
    entity->addComponent(material);
}

void Viewport3D::createAxisLines(Qt3DCore::QEntity* parent) {
    struct Line {
        QVector3D start;
        QVector3D end;
        QColor color;
    };

    QVector<Line> lines = {
        {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, Qt::red},   // X-axis
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, Qt::green}, // Y-axis
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, Qt::blue}   // Z-axis
    };

    for (const Line& line : lines) {
        QVector<QVector3D> vertices = {line.start, line.end};
        QVector<unsigned int> indices = {0, 1};

        createEntity(parent, vertices, indices, line.color);
    }
}
