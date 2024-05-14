#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  if (DoingInteractive == ROTATE && not isortho) {
  makeCurrent();
  factorAngleX += ((e -> x() - xClick)*M_PI)/ample;
  factorAngleY += ((e -> y() - yClick)*M_PI)/ample;
  


  xClick = e->x();
  yClick = e->y();

  update ();
  viewTransform();
  }
}

void MyGLWidget::iniCamera(){

  obs = glm::vec3(0, 8, -30);
  vrp = glm::vec3(0, 0, 0);
  up = glm::vec3(0, 1, 0);
  fov = 1.047f;
  radiEscena = sqrt(15*15 + 15*15 + 3*3);
  znear =  radiEscena;
  zfar  = 3*radiEscena;
  factorAngleX = 0;
  factorAngleY = 0;



  isortho = false;
  viewTransform();
  projectTransform();
  
}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // SnakeHead
  glBindVertexArray (VAO_models[SNAKE_HEAD]);
  SnakeHeadTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_HEAD].faces().size()*3);
  
  // SnakeBody
  glBindVertexArray (VAO_models[SNAKE_BODY]);
  for (unsigned int i = 0; i < bodyPos.size(); i++)
  {  
	  SnakeBodyTransform(bodyPos[i]);
	  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_BODY].faces().size()*3);
  }

  // SnakeTail
  glBindVertexArray (VAO_models[SNAKE_TAIL]);
  SnakeTailTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_TAIL].faces().size()*3);
  
  // Marble
  glBindVertexArray (VAO_models[MARBLE]);
  MarbleTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MARBLE].faces().size()*3);

  // Pipes
  for(int i = 0; i < 30; ++i) {
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(14.5-i,0,14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(14.5-i,0, -14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }
  for(int i = 1; i < 29; ++i) {
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(14.5,0,14.5-i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(-14.5,0,14.5-i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }
  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::TerraTransform() {
  //LL2GLWidget::TerraTransform();
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(3,0,3));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
/*
void MyGLWidget::viewTransform () {
    glm::mat4 VM(1.0f);
    if(!isortho) {
    VM = glm::translate(VM, glm::vec3(0., 0., -60));
    VM = glm::rotate(VM, float(M_PI)/4, glm::vec3(1., 0., 0.));
    VM = glm::rotate(VM, angle_cam.y, glm::vec3(1., 0., 0.));
    VM = glm::rotate(VM, angle_cam.x, glm::vec3(0., 1., 0.));
    }
    else {
    VM = glm::translate(VM, glm::vec3(0,0,-50));
    VM = glm::rotate(VM, 1.5708f, glm::vec3(1, 0, 0));
    }
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &VM[0][0]);
}
*/
void MyGLWidget::viewTransform() {
    glm::mat4 VM(1.0f);
    VM = glm::translate(VM, glm::vec3(0., 0., -radiEscena*2));
    VM = glm::rotate(VM, float(M_PI)/4, glm::vec3(1., 0., 0.));
    VM = glm::rotate(VM, factorAngleY, glm::vec3(1., 0., 0.));
    VM = glm::rotate(VM, factorAngleX, glm::vec3(0., 1., 0.));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &VM[0][0]);
}
void MyGLWidget::VMOrtho() {
    glm::mat4 VM(1.0f);
    VM = glm::translate(VM, glm::vec3(0,0,-20));
    VM = glm::rotate(VM, float(M_PI)/2, glm::vec3(1, 0, 0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &VM[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if(!isortho)Proj = glm::perspective (fov, ra, znear, zfar);
  else {
    float rav = float(ample)/float(alt);
    if(rav >= 1)Proj = glm::ortho(-radiEscena*rav, radiEscena*rav, -radiEscena, radiEscena, 0.f, 100.f); 
    else Proj = glm::ortho(-radiEscena, radiEscena, -radiEscena/rav, radiEscena/rav, 0.f, 100.f);
  }
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::PipeTransform (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]));
  TG = glm::scale(TG, glm::vec3(1,3,1));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MarbleTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, marblePos);
  TG = glm::scale(TG, glm::vec3(escalaModels[MARBLE]));
  TG = glm::scale(TG, glm::vec3(0.5,0.5,0.5));
  TG = glm::translate(TG, -centreBaseModels[MARBLE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::iniEscena() {
  LL2GLWidget::iniEscena();
  headPos = glm::vec3(0,0,0);
  tailPos = glm::vec3(-2,0,0);
}

/*
void MyGLWidget::viewTransform() {

}
*/

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{
}
/*
void MyGLWidget::mousePressEvent (QMouseEvent *event) {
  
}
*/

bool MyGLWidget::checkPosition(glm::vec3 pos)
{
	bool valid = true;
  if (pos.x >= 13.5 || pos.x <= -13.5) valid = false;
  else if(pos.z >= 13.5 || pos.z <= -13.5) valid = false;
        // Aquí cal que completeu el codi de manera adient
  else {
    for (int i = 0; i < int(bodyPos.size()); ++i) {
      if (bodyPos[i] == pos) return false;
    }
    if (tailPos == pos) return false;
  }

	return valid; 
}

void MyGLWidget::SnakeHeadTransform ()
{
  glm::mat4 TG(1.0f);  
  TG = glm::translate(TG, headPos);
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_HEAD]));
  TG = glm::rotate(TG, (headAngle-90)/57.2958f, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_HEAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::SnakeTailTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, tailPos);
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_TAIL]));
  TG = glm::rotate(TG, (tailAngle)/57.2958f, glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_TAIL]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      direction = glm::vec3(0,0,-1);
      updateSnakeGame();
      }
      break;
    case Qt::Key_Down: {
      direction = glm::vec3(0,0,1);
      updateSnakeGame();
      break;
        }
    case Qt::Key_Left: {
      direction = glm::vec3(-1,0,0);
      updateSnakeGame();
      break;
        }  
    case Qt::Key_Right: {
      direction = glm::vec3(1,0,0);
      updateSnakeGame();
      break;
        }
    case Qt::Key_C: { 
      if(!isortho)isortho = true;
      else isortho = false;

      if(isortho) VMOrtho();
      else viewTransform();
      projectTransform ();
      break;
    }           
    case Qt::Key_R: { 
      iniEscena();
      iniCamera();
      break;
        }  
    default: event->ignore(); break;
  }
  update();
}




