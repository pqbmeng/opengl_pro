#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ctime>
#include "pro_transform.h"
#include "shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <QTime>

pro_transform::pro_transform(QWidget *parent)
	: QOpenGLWidget(parent)
    , angle{}
{
    startTimer(100);
}

pro_transform::~pro_transform()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void pro_transform::initializeGL()
{
	glewInit();
	pShader = new Shader("transform.vs", "transform.fs");;
    float vertices[] = {
        // positions          // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
}

void pro_transform::paintGL()
{
	//float greenvalue = (sin(std::time(nullptr)) / 2.0f) + 0.5f;
	//glUniform4f(glGetUniformLocation(shaderProgram, "ourColor"), 0.0, greenvalue, 0.0f, 1.0f);


	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);


    // create transformations
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
    transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
    //transform = glm::rotate(transform, (float)QTime::currentTime().second(), glm::vec3(0.0f, 0.0f, 1.0f));


	pShader->use();
    unsigned int transformLoc = glGetUniformLocation(pShader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void pro_transform::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void pro_transform::mouseMoveEvent(QMouseEvent *event)
{
	update();
	QOpenGLWidget::mouseMoveEvent(event);
}

void pro_transform::timerEvent(QTimerEvent *event)
{
    angle += 3;
    if (angle > 360)
    {
        angle = 0;
    }
    update();
    QOpenGLWidget::timerEvent(event);
}
