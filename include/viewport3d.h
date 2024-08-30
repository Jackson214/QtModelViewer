#include <Qt3DExtras/Qt3DWindow>

class Viewport3D : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT

public:
    Viewport3D(QWidget *parent = nullptr);
    ~Viewport3D();

    // Add declarations for any other methods here

private:
    Qt3DCore::QEntity *createScene();  // Declare createScene here
};