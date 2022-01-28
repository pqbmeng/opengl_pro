#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ctime>
#include "pro_transform_perspective_depth.h"
#include "shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <QTime>
#include <QWheelEvent>
#include <QDebug>
#include <QApplication>

pro_transform_perspective_depth::pro_transform_perspective_depth(QWidget *parent)
	: QOpenGLWidget(parent)
    , angleH{}
{
    startTimer(10);
    reset();
}

pro_transform_perspective_depth::~pro_transform_perspective_depth()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void pro_transform_perspective_depth::initializeGL()
{
	glewInit();
	pShader = new Shader("transform.vs", "transform.fs");;
    float vertices[] = {
#if 0
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
#endif

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

#if 0
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
#endif

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

#if 1
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
#endif
    };
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	
    {
        {
            glGenTextures(1, &texture0);
            glBindTexture(GL_TEXTURE_2D, texture0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            int width, height, nrchannels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char* data = stbi_load("container.jpg", &width, &height, &nrchannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(data);
        }

        {
            glGenTextures(1, &texture1);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            //float borderColor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
            //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int width, height, nrchannels;
            unsigned char* data = stbi_load("awesomeface.png", &width, &height, &nrchannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(data);
        }
    }

	pShader->use();
	glUniform1i(glGetUniformLocation(pShader->ID, "texture0"), 0);
	pShader->setInt("texture1", 1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void pro_transform_perspective_depth::paintGL()
{
	//float greenvalue = (sin(std::time(nullptr)) / 2.0f) + 0.5f;
	//glUniform4f(glGetUniformLocation(shaderProgram, "ourColor"), 0.0, greenvalue, 0.0f, 1.0f);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);

    // create transformations
   // glm::mat4 transform = glm::mat4(1.0f);
   // transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
   // transform = glm::rotate(transform, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
   // transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
    //transform = glm::rotate(transform, (float)QTime::currentTime().second(), glm::vec3(0.0f, 0.0f, 1.0f));


	pShader->use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(angleH), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angleV), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3{ m_transparentH, 0, 0 });
    model = glm::translate(model, glm::vec3{ 0, m_transparentV, 0 });
    model = glm::scale(model, glm::vec3{ m_scale,m_scale,m_scale });

#if 1 // 相机看向Z轴的正方向
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
#else // 相机看向Z轴的负方向
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));
    view = glm::rotate(view, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 6.0f));
#endif
    //projection = glm::perspective(glm::radians(fov), (float)width() / height(), 0.1f, 100.0f);
    projection = glm::perspective(glm::radians(m_fov), (float)width() / height(), 0.1f, 100.0f);
    unsigned int modelLoc = glGetUniformLocation(pShader->ID, "model");
    unsigned int viewLoc = glGetUniformLocation(pShader->ID, "view");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    pShader->setMat4("projection", projection);

#if 0 // 反锯齿
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

	glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6*3*2);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void pro_transform_perspective_depth::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void pro_transform_perspective_depth::mousePressEvent(QMouseEvent *event)
{
    m_pressedPoint.reset();
}

void pro_transform_perspective_depth::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton))
    {
        if (m_pressedPoint)
        {
            if ((event->pos().x() > m_pressedPoint->x()))
            {
                angleH += 0.5;
            }
            else
            {
                angleH -= 0.5;
            }

            if (angleH > 360)
            {
                angleH = 0;
            }
            if (angleH < 0)
            {
                angleH = 360;
            }
        }

        qDebug() << "current angleH: " << angleH;
    }
    else if (event->buttons().testFlag(Qt::RightButton))
    {
        if (m_pressedPoint)
        {
            if ((event->pos().y() > m_pressedPoint->y()))
            {
                angleV += 0.5;
            }
            else
            {
                angleV -= 0.5;
            }

            if (angleV > 360)
            {
                angleV = 0;
            }
            if (angleV < 0)
            {
                angleV = 360;
            }
        }

        qDebug() << "current angleV: " << angleV;
    }

    m_pressedPoint.emplace(event->pos());
	update();
	QOpenGLWidget::mouseMoveEvent(event);
}

void pro_transform_perspective_depth::wheelEvent(QWheelEvent *event)
{
    //qDebug() <<"----------" << event->delta() << " - " << event->angleDelta() << " - " << event->pixelDelta();

    if (QGuiApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
    {
        if (event->delta() > 0)
        {
            m_fov += 1;
        }
        else
        {
            m_fov -= 1;
        }
        m_fov = qBound(0.01f, m_fov, 100.f);
    }
    else
    {
        if (event->delta() > 0)
        {
            m_scale -= 0.1;
        }
        else
        {
            m_scale += 0.1;
        }
        m_scale = qBound(0.01f, m_scale, 100.f);
    }

    qDebug() << "current|scale-" << m_scale << "|fov-" << m_fov;
    update();
}

void pro_transform_perspective_depth::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Left == event->key())
    {
        m_transparentH -= 0.1;
    }
    if (Qt::Key_Right == event->key())
    {
        m_transparentH += 0.1;
    }
    if (Qt::Key_Up == event->key())
    {
        m_transparentV += 0.1;
    }
    if (Qt::Key_Down == event->key())
    {
        m_transparentV -= 0.1;
    }

    update();
    base_t::keyPressEvent(event);
}

bool pro_transform_perspective_depth::event(QEvent *e)
{
    if (QEvent::MouseButtonDblClick == e->type())
    {
        reset();
        update();
    }

    return base_t::event(e);
}

void pro_transform_perspective_depth::reset()
{
    angleH = 0;
    angleV = 0;
    m_pressedPoint.reset();
    m_scale = 1.f;
    m_transparentH = 0;
    m_transparentV = 0;
    m_fov = 45.f;
}
