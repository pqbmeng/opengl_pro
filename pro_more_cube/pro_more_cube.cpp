#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ctime>
#include "pro_more_cube.h"
#include "shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <QTime>
#include <QKeyEvent>
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

namespace
{
    // camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    int lastFrame = 0.0f;

    void printVec3(const glm::vec3 &v)
    {
        std::cout << "x: " << v.x << " y: " << v.y << " z: " << v.z << std::endl;
    }
}

pro_more_cube::pro_more_cube(QWidget *parent)
	: QOpenGLWidget(parent)
    , angle{}
{
    startTimer(10);
}

pro_more_cube::~pro_more_cube()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void pro_more_cube::initializeGL()
{
	glewInit();
	pShader = new Shader("pro_more_cube.vs", "pro_more_cube.fs");;
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

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

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
//     unsigned int indices[] = {
//         0, 1, 3, // first triangle
//         1, 2, 3  // second triangle
//     };

	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	
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

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //float borderColor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("awesomeface.png", &width, &height, &nrchannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	pShader->use();
	glUniform1i(glGetUniformLocation(pShader->ID, "texture0"), 0);
	pShader->setInt("texture1", 1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void pro_more_cube::paintGL()
{
	//float greenvalue = (sin(std::time(nullptr)) / 2.0f) + 0.5f;
	//glUniform4f(glGetUniformLocation(shaderProgram, "ourColor"), 0.0, greenvalue, 0.0f, 1.0f);

    auto currentFrame = std::time(nullptr);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;


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

    //glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    //model = glm::rotate(model, QTime::currentTime().second()*glm::radians(-50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
#if 0
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
#elif 0// 使用camera变换观察角度
    static float sss = 0;
    float radius = 10.0f;
    float camX = std::sin(sss) * radius;
    float camZ = std::cos(sss) * radius;
    sss+=0.01f;
    view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
#elif 1 // 通过键盘控制camera角度
    static int interval = 0;
    if (0 == interval++ % 10)
    {
        std::cout << "cameraPos:" << std::endl;
        printVec3(cameraPos);
        std::cout << "target:" << std::endl;
        printVec3(cameraPos + cameraFront);
    }
//     std::cout << "up:" << std::endl;
//     printVec3(cameraUp);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
#endif
    projection = glm::perspective(glm::radians(45.0f), (float)width() / height(), 0.1f, 100.0f);
    unsigned int modelLoc = glGetUniformLocation(pShader->ID, "model");
    unsigned int viewLoc = glGetUniformLocation(pShader->ID, "view");
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    pShader->setMat4("projection", projection);

	glBindVertexArray(vao);
    for (unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        //float angle_ = angle*(i + 1);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        pShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void pro_more_cube::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void pro_more_cube::mouseMoveEvent(QMouseEvent *event)
{
	update();
	QOpenGLWidget::mouseMoveEvent(event);
}

void pro_more_cube::timerEvent(QTimerEvent *event)
{
    angle += 0.5;
    if (angle > 360)
    {
        angle = 0;
    }
    //repaint();
    update();
    QOpenGLWidget::timerEvent(event);
}

void pro_more_cube::keyPressEvent(QKeyEvent *event)
{
    //float cameraSpeed = 2.5 * deltaTime;
    float cameraSpeed = 1.2;
    if (Qt::Key_W == event->key())
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (Qt::Key_S == event->key())
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (Qt::Key_A == event->key())
    {
        auto a = glm::cross(cameraFront, cameraUp);
        auto b = glm::normalize(a);
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (Qt::Key_D == event->key())
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    base_t::keyPressEvent(event);
}
