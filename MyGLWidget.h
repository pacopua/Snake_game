#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
  
    //virtual void viewTransform ();

    //virtual void mousePressEvent (QMouseEvent *event);

    virtual void VMOrtho();

    virtual void viewTransform ();

    virtual void projectTransform();

    virtual void PipeTransform (glm::vec3 pos);

    virtual void MarbleTransform();

    virtual void SnakeTailTransform();

    virtual void SnakeHeadTransform ();

    virtual void mouseMoveEvent(QMouseEvent *e);

    virtual void paintGL ();

    virtual void TerraTransform();

    virtual void keyPressEvent (QKeyEvent *event);

    virtual void iniEscena();

    // funció per comprovar si una nova posició de la serp és vàlida
    virtual bool checkPosition(glm::vec3 pos);

    virtual void iniCamera();

    bool isortho = false;

    glm::vec3 angle_cam = glm::vec3(0, 0, 0);

    int lastheadAngle = 0;

    int n_balas = 0;
  public slots:


  private:


    int printOglError(const char file[], int line, const char func[]);
   
};
