#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <ctime>

#include <QTime>
#include <QWheelEvent>
#include <QDebug>
#include <QApplication>

#include "pro_transform_perspective_depth.h"
#include "shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "common.h"

#define PRIENT_VAL(v) qDebug() << #v":" << v;

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
#if 1 // back
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
#endif

#if 1 // front
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
#endif

#if 0 // left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
#endif

#if 0 // right
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
#endif

#if 0 // bottom
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
#endif

#if 1 // top
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
    model = glm::translate(model, glm::vec3{ local_translation_x, 0, 0 });
    model = glm::translate(model, glm::vec3{ 0, local_translation_y, 0 });
    model = glm::translate(model, glm::vec3{ 0, 0, local_translation_z });
    model = glm::scale(model, glm::vec3{ m_scale });

#if 0 // magical
    glm::vec4 myvec4{ 123,0,40,31 };
    gl_ns::print(myvec4);
    gl_ns::print(model);
    auto r = model * myvec4;
    gl_ns::print(r);
    gl_ns::print(glm::inverse(model) * r);
#endif

#if 0 // 使用控制变量
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, m_viewZ)); // m_vieZ的初始值设置为-3.f
#elif 1
    //view = glm::lookAt(glm::vec3(0.0f, 0.0f, m_viewZ), glm::vec3{ 0, 0, 0 }, glm::vec3{ 0,1,0 });
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, m_cameraPosZ), glm::vec3(0.0f, 0.0f, m_cameraPosZ)+glm::vec3{ 0, 0, -1 }, glm::vec3{ 0,1,0 }); // m_vieZ的初始值设置为3.f
#elif 1 // 相机看向Z轴的正方向
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
#elif 1 // 从顶部看
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    view = glm::rotate(view, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
#else // 相机看向Z轴的负方向
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));
    view = glm::rotate(view, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 6.0f));
#endif
    //projection = glm::perspective(glm::radians(fov), (float)width() / height(), 0.1f, 100.0f);
#if 1
    projection = glm::perspective(glm::radians(m_fov), (float)width() / height(), m_nearPlane, m_farPlane);
#else
    projection = glm::ortho(-1.f, 1.f, -1.f, 1.f, m_nearPlane, m_farPlane);
#endif
    unsigned int modelLoc = glGetUniformLocation(pShader->ID, "model");
    unsigned int viewLoc = glGetUniformLocation(pShader->ID, "view");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    pShader->setMat4("projection", projection);

#if 0 // 打印0点变换后的位置
    const auto originPos = glm::vec4{ 0.f, 0.f, 0.f, 1.f };
    qDebug() << "model:";
    gl_ns::print(model*originPos);
    qDebug() << "view:";
    gl_ns::print(view*model*originPos);
    qDebug() << "projection:";
    gl_ns::print(projection*view*model*originPos);
#endif

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

            PRIENT_VAL(angleH);
        }
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

            PRIENT_VAL(angleV);
        }
    }

    m_pressedPoint.emplace(event->pos());
	update();
	QOpenGLWidget::mouseMoveEvent(event);
}

void pro_transform_perspective_depth::wheelEvent(QWheelEvent *event)
{
    //qDebug() <<"----------" << event->delta() << " - " << event->angleDelta() << " - " << event->pixelDelta();

    // near plane 和 far plane控制
    if (QGuiApplication::keyboardModifiers().testFlag(Qt::AltModifier))
    {
        if (QGuiApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
        {
            if (event->delta() > 0)
            {
                m_farPlane += 0.1f;
            }
            else
            {
                m_farPlane -= 0.1f;
            }
            m_farPlane = qBound(0.1f, m_farPlane, 100.f);
        }
        else
        {
            if (event->delta() > 0)
            {
                m_nearPlane += 0.1f;
            }
            else
            {
                m_nearPlane -= 0.1f;
            }
            m_nearPlane = qBound(0.1f, m_nearPlane, 100.f);
        }
        qDebug() << "m_nearPlane-" << m_nearPlane << "| m_farPlane-" << m_farPlane;
    }
    // fov控制
    else if (QGuiApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
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
        PRIENT_VAL(m_fov);
    }
    // 缩放控制
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
        qDebug() << "scale-" << m_scale;
    }

    update();
}

void pro_transform_perspective_depth::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Q:
        local_translation_x -= 0.1;
        PRIENT_VAL(local_translation_x);
        break;
    case Qt::Key_W:
        local_translation_x += 0.1;
        PRIENT_VAL(local_translation_x);
        break;
    case Qt::Key_A:
        local_translation_y -= 0.1;
        PRIENT_VAL(local_translation_y);
        break;
    case Qt::Key_S:
        local_translation_y += 0.1;
        PRIENT_VAL(local_translation_y);
        break;
    case Qt::Key_Z:
        local_translation_z -= 0.1;
        PRIENT_VAL(local_translation_z);
        break;
    case Qt::Key_X:
        local_translation_z += 0.1;
        PRIENT_VAL(local_translation_z);
        break;
    default:
        break;
    }

    if (Qt::Key_Left == event->key())
    {
        if (QGuiApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
        {
            m_cameraPosZ -= 0.1;
            PRIENT_VAL(m_cameraPosZ);
        }
    }
    if (Qt::Key_Right == event->key())
    {
        if (QGuiApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
        {
            m_cameraPosZ += 0.1;
            PRIENT_VAL(m_cameraPosZ);
        }
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
    local_translation_x = 0;
    local_translation_y = 0;
    local_translation_z = 0;
    m_fov = 45.f;
    m_nearPlane = 0.1f;
    m_farPlane = 5.f;
    m_cameraPosZ = 3.f;
}

void pro_transform_perspective_depth::print() const
{
    qDebug() <<"angleH:"<< angleH 
        << "|angleV:" << angleV 
        << "|m_scale:" << m_scale 
        << "|m_transparentH:" << local_translation_x 
        << "|m_transparentV:" << local_translation_y 
        << "|m_fov:" << m_fov 
        << "|m_nearPlane:" << m_nearPlane 
        << "|m_farPlane:" << m_farPlane 
        << "|m_viewZ:" << m_cameraPosZ;
}
