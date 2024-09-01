#pragma once

#include <QEntity>
#include <QColor>
#include <QVector>
#include <QVector3D>

class Object3D
{
    public:
        Object3D(Qt3DCore::QEntity* parent, const QVector<QVector3D> vertices, const QVector<unsigned int> indices, const QVector<QColor> colors);
        ~Object3D();

        Qt3DCore::QEntity* getParent() const;
        QVector<QVector3D> getVertices() const;
        QVector<unsigned int> getIndices() const;
        QVector<QColor> getOriginalColors() const;
        QVector<std::pair<unsigned long, QColor>> getColorOverrides() const;
        void addColorOverride(unsigned long index, QColor color);
        void removeColorOverride(unsigned long index);
        QColor getCurrentColor(unsigned long index) const;


    private:
        QVector<QVector3D> mVertices;
        QVector<unsigned int> mIndices;
        QVector<QColor> mColors;
        Qt3DCore::QEntity* mEntityPtr;
        Qt3DCore::QEntity* mParent;
        QVector<std::pair<unsigned long, QColor>> mColorOverrides;
};
