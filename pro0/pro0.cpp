#include "GL/glew.h"
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "pro0.h"
#include "common.h"

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

pro0::pro0(QWidget *parent)
	: QOpenGLWidget(parent)
{
	ui.setupUi(this);

	setGeometry(0, 0, 640, 480);

	
}
//#include <QOpenGLFunctions> 
void pro0::initializeGL()
{
	//initializeOpenGLFunctions();
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void pro0::paintGL()
{
    {
        {
            qDebug() << "沿着x轴平移：";

            auto myMat = glm::mat4{ 1.f };
            //glm::mat4 view = glm::mat4(1.0f);
            //glm::mat4 projection = glm::mat4(1.0f);
            //model = glm::rotate(model, glm::radians(angleH), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(angleV), glm::vec3(1.0f, 0.0f, 0.0f));
            gl_ns::print(myMat);
            myMat = glm::translate(myMat, glm::vec3{ 0.1, 0, 0 });
            gl_ns::print(myMat);
            glm::vec4 mypos{ 1,2,3,1 };
            gl_ns::print(mypos);
            gl_ns::print(myMat * mypos);
        }

        {
            qDebug() << "先向左平移再向右平移：";

            auto myMat = glm::mat4{ 1.f };
            gl_ns::print(myMat);
            myMat = glm::translate(myMat, glm::vec3{ 0.1, 0, 0 });
            gl_ns::print(myMat);
            myMat = glm::translate(myMat, glm::vec3{ -0.1, 0, 0 });
            gl_ns::print(myMat);
            glm::vec4 mypos{ 1,2,3,1 };
            gl_ns::print(mypos);
            gl_ns::print(myMat * mypos);
        }

        {
            qDebug() << "缩放：";

            auto myMat = glm::mat4{ 1.f };
            //glm::mat4 view = glm::mat4(1.0f);
            //glm::mat4 projection = glm::mat4(1.0f);
            //model = glm::rotate(model, glm::radians(angleH), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(angleV), glm::vec3(1.0f, 0.0f, 0.0f));
            gl_ns::print(myMat);
            myMat = glm::scale(myMat, glm::vec3{ 2 });
            gl_ns::print(myMat);
            glm::vec4 mypos{ 1,2,3,1 };
            gl_ns::print(mypos);
            gl_ns::print(myMat * mypos);
        }

        {
            qDebug() << "先放大再缩小：";

            auto myMat = glm::mat4{ 1.f };
            gl_ns::print(myMat);
            myMat = glm::scale(myMat, glm::vec3{ 2 });
            gl_ns::print(myMat);
            myMat = glm::scale(myMat, glm::vec3{ 0.5 });
            gl_ns::print(myMat);
            glm::vec4 mypos{ 1,2,3,1 };
            gl_ns::print(mypos);
            gl_ns::print(myMat * mypos);
        }

        {
            qDebug() << "先平移再缩放：";

            auto myMat = glm::mat4{ 1.f };
            gl_ns::print(myMat);
            myMat = glm::translate(myMat, glm::vec3{ 0.1, 0, 0 });
            gl_ns::print(myMat);
            myMat = glm::scale(myMat, glm::vec3{ 2 });
            gl_ns::print(myMat);
            glm::vec4 mypos{ 1,2,3,1 };
            gl_ns::print(mypos);
            gl_ns::print(myMat * mypos);
        }

        {
            qDebug() << "先缩放再平移：";

            auto myMat = glm::mat4{ 1.f };
            gl_ns::print(myMat);
            myMat = glm::scale(myMat, glm::vec3{ 2 });
            gl_ns::print(myMat);
            myMat = glm::translate(myMat, glm::vec3{ 0.1, 0, 0 });
            gl_ns::print(myMat);
            glm::vec4 mypos{ 1,2,3,1 };
            gl_ns::print(mypos);
            gl_ns::print(myMat * mypos);
        }

        {
            qDebug() << "矩阵相乘：";
            glm::mat4 matTrans{ 1,0,0,0,  0,1,0,0,  0,0,1,0,  1,2,3,1 };
            glm::mat4 matScale{ 2,0,0,0,  0,2,0,0,  0,0,2,0,  0,0,0,1 };
            gl_ns::print(matTrans);
            gl_ns::print(matScale);
            gl_ns::print(matTrans * matScale);
            gl_ns::print(matScale * matTrans); // 矩阵相乘不遵守交换律(Commutative)

            glm::mat4 matTrans_{ 1,0,0,0,  0,1,0,0,  0,0,1,0,  10,20,30,1 };
            gl_ns::print(matTrans * matTrans_);
            gl_ns::print(matTrans_ * matTrans); // 俩矩阵都是平移结果相同

            gl_ns::print(matScale*matTrans * matTrans_);
            gl_ns::print(matScale*(matTrans_ * matTrans));
            gl_ns::print((matTrans_ * matTrans)*matScale);
            gl_ns::print(matTrans_ * matTrans*matScale); // 改变优先级不影响结果？？
            gl_ns::print(glm::inverse(matTrans_ * matTrans*matScale));

            glm::mat4 matRotate{1.f};
            matRotate = glm::rotate(matRotate, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
            gl_ns::print(matRotate);
            gl_ns::print(matScale * matTrans * matRotate);
            gl_ns::print((matScale * matTrans) * matRotate);
            gl_ns::print(matScale * (matTrans * matRotate)); // 改变优先级不影响结果？？
        }
    }


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-5, -5, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(5, -5, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 5, 0);
	glEnd();
}

void pro0::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}
