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
#include <Qt3DRender/QRenderStateSet>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QCullFace>
#include <QLineWidth>

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
    mCamController->setLookSpeed(180.0f);

    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->addWidget(mContainer);
}

void Viewport3D::createScene(Qt3DCore::QEntity* rootEntity) {
    // Enable depth testing
    auto *renderStateSet = new Qt3DRender::QRenderStateSet(rootEntity);
    auto *depthTest = new Qt3DRender::QDepthTest();
    depthTest->setDepthFunction(Qt3DRender::QDepthTest::Less);
    renderStateSet->addRenderState(depthTest);

    // Enable face culling
    auto *cullFace = new Qt3DRender::QCullFace();
    cullFace->setMode(Qt3DRender::QCullFace::Back);
    renderStateSet->addRenderState(cullFace);

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

void Viewport3D::createEntity(Object3D object) {
    auto *newObject = new Qt3DCore::QEntity(object.getParent());
    for (int i = 0; i < (object.getIndices().size()/3); i++) {
        auto *face = new Qt3DCore::QEntity(newObject);
        auto *geometry = new Qt3DCore::QGeometry(face);

        // Create vertex data for the current face
        QByteArray vertexData;
        vertexData.resize(3 * 3 * sizeof(float)); // 3 vertices per face, 3 coordinates per vertex
        float *vertexDataPtr = reinterpret_cast<float*>(vertexData.data());
        for (int j = 0; j < 3; j++) {
            const QVector3D &vertex = object.getVertices().at(object.getIndices().at((i*3) + j));
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
        positionAttribute->setCount(3);

        // Create index data for the current face
        QByteArray indexData;
        indexData.resize(3 * sizeof(unsigned int)); // 3 indices per face
        unsigned int *indexDataPtr = reinterpret_cast<unsigned int*>(indexData.data());
        for (int j = 0; j < 3; j++) {
            *indexDataPtr++ = j;
        }

        auto *indexBuffer = new Qt3DCore::QBuffer(geometry);
        indexBuffer->setData(indexData);

        auto *indexAttribute = new Qt3DCore::QAttribute();
        indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
        indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
        indexAttribute->setBuffer(indexBuffer);
        indexAttribute->setCount(3);

        geometry->addAttribute(positionAttribute);
        geometry->addAttribute(indexAttribute);

        auto *geometryRenderer = new Qt3DRender::QGeometryRenderer(face);
        geometryRenderer->setGeometry(geometry);
        geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);

        auto *material = new Qt3DExtras::QPhongMaterial(face);
        material->setAmbient(object.getCurrentColor(i));  // Set ambient color
        material->setDiffuse(object.getCurrentColor(i));
        material->setSpecular(Qt::gray); // Set specular color
        material->setShininess(50.0f);   // Set shininess for specular highlights

        face->addComponent(geometryRenderer);
        face->addComponent(material);
    }
    mObjects.push_back(object);
}

void Viewport3D::createAxisLines(Qt3DCore::QEntity* parent) {
    auto *newObject = new Qt3DCore::QEntity(parent);
    // Define the vertices for the axis lines
    QVector<QVector3D> vertices = {
        {0.0f, 0.0f, 0.0f}, {3.0f, 0.0f, 0.0f}, // X-axis
        {0.0f, 0.0f, 0.0f}, {0.0f, 3.0f, 0.0f}, // Y-axis
        {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 3.0f}  // Z-axis
    };

    // Define the colors for the axis lines
    QVector<QColor> colors = {
        Qt::red, Qt::red,   // X-axis
        Qt::green, Qt::green, // Y-axis
        Qt::blue, Qt::blue  // Z-axis
    };

    // Create a geometry object
    auto *geometry = new Qt3DCore::QGeometry(newObject);

    // Create a buffer for the vertices
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

    // Create a buffer for the colors
    QByteArray colorData;
    colorData.resize(colors.size() * 3 * sizeof(float));
    float *colorDataPtr = reinterpret_cast<float*>(colorData.data());
    for (const QColor &color : colors) {
        *colorDataPtr++ = color.redF();
        *colorDataPtr++ = color.greenF();
        *colorDataPtr++ = color.blueF();
    }

    auto *colorBuffer = new Qt3DCore::QBuffer(geometry);
    colorBuffer->setData(colorData);

    // Create the position attribute
    auto *positionAttribute = new Qt3DCore::QAttribute();
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(vertices.size());

    // Create the color attribute
    auto *colorAttribute = new Qt3DCore::QAttribute();
    colorAttribute->setName(Qt3DCore::QAttribute::defaultColorAttributeName());
    colorAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    colorAttribute->setVertexSize(3);
    colorAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    colorAttribute->setBuffer(colorBuffer);
    colorAttribute->setByteStride(3 * sizeof(float));
    colorAttribute->setCount(colors.size());

    // Add the attributes to the geometry
    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(colorAttribute);

    // Create a geometry renderer
    auto *geometryRenderer = new Qt3DRender::QGeometryRenderer(newObject);
    geometryRenderer->setGeometry(geometry);
    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // Create a material
    auto *material = new Qt3DExtras::QPhongMaterial(newObject);

    // Create an entity and add the components
    auto *entity = new Qt3DCore::QEntity(newObject);
    entity->addComponent(geometryRenderer);
    entity->addComponent(material);
}
