#include "Object3D.h"

Object3D::Object3D(Qt3DCore::QEntity* parent, const QVector<QVector3D>& vertices, const QVector<unsigned int>& indices, const QVector<QColor>& colors, const QVector<std::pair<unsigned long, QColor>>& colorOverrides) {
	mVertices = vertices;
	mIndices = indices;
	mColors = colors;
	mColorOverrides = colorOverrides;
	mParent = parent;
}

Object3D::~Object3D() {}

Qt3DCore::QEntity* getParent() {
	return mParent;
}

QVector<QVector3D> Object3D::getVertices() const {
	return mVertices;
}

QVector<unsigned int> Object3D::getIndices() const {
	return mIndices;
}

QVector<QColor> Object3D::getOriginalColors() const {
	return mColors;
}

QVector<std::pair<unsigned long, QColor>> Object3D::getColorOverrides() const {
	return mColorOverrides;
}

void Object3D::addColorOverride(unsigned long index, QColor color) {
	mColorOverrides.push_back(std::make_pair(index, color));
}

void Object3D::removeColorOverride(unsigned long index) {
	for (int i = 0; i < mColorOverrides.size(); i++) {
		if (mColorOverrides[i].first == index) {
			mColorOverrides.erase(mColorOverrides.begin() + i);
			break;
		}
	}
}

QColor Object3D::getCurrentColor(unsigned long index) const {
	for (int i = 0; i < mColorOverrides.size(); i++) {
		if (mColorOverrides[i].first == index) {
			return mColorOverrides[i].second;
		}
	}
	return mColors[index];
}
