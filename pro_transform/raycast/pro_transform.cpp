#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ctime>
#include <memory>
#include "pro_transform.h"
#include "shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <QTime>
#include <QKeyEvent>
#include <QDebug>
#include "common.h"

pro_transform::pro_transform(QWidget *parent)
	: QOpenGLWidget(parent)
{

}
const int volumn_w = 256;
const int volumn_h = 256;
const int volumn_d = 256;
float myoff = 0;
pro_transform::~pro_transform()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ebo);
}
//const glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
//camera transform variables
int state = 0, oldX = 0, oldY = 0;
float rX = 4, rY = 50, dist = -2;
glm::mat4 MV, P;
void pro_transform::initializeGL()
{
	glewInit();
	pShader = new Shader("transform.vs", "transform.fs");;

glm::vec3 vertices[8] = { glm::vec3(-0.5f,-0.5f,-0.5f),    // 左下后
        glm::vec3(0.5f,-0.5f,-0.5f),             // 右下后
        glm::vec3(0.5f, 0.5f,-0.5f),              // 右上后
        glm::vec3(-0.5f, 0.5f,-0.5f),             // 左上后
        glm::vec3(-0.5f,-0.5f, 0.5f),             // 左下前
        glm::vec3(0.5f,-0.5f, 0.5f),               //  右下前
        glm::vec3(0.5f, 0.5f, 0.5f),                // 右上前
        glm::vec3(-0.5f, 0.5f, 0.5f) };             // 左上前

    //unit cube indices
    GLushort cubeIndices[36] = { 0,5,4,
        5,0,1,
        3,7,6,
        3,6,2,
        7,4,6,
        6,4,5,
        2,1,3,
        3,1,0,
        3,0,7,
        7,0,4,
        6,5,2,
        2,5,1 };

	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &(vertices[0].x), GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

// 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
// 	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices[0], GL_STATIC_DRAW);

	
#if 1
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_3D, texture0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);

    const std::string filename = "Engine256.raw";
    const int size = volumn_w * volumn_h * volumn_d;
    auto mydata = std::make_unique<GLubyte[]>(size);
    //ReviewSmartPtr<plfPixelObject1D<short>> pData = plfPixelObject1D<short>::New();
    //pData->CreatePixel(size);
#if 0
    std::ifstream istrm(filename, std::ios::binary);
    istrm.read((char*)mydata.get(), sizeof(short) * size);
#else
    auto pFile = std::fopen(filename.c_str(), "rb");
    std::fread(mydata.get(), sizeof mydata[0], size, pFile);
    std::fclose(pFile);
#endif
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(texture0);
    // GL_LUMINANCE
    //glTexImage3D(GL_TEXTURE_3D, 0, GL_R16_SNORM, volumn_w, volumn_h, volumn_d, 0, GL_RED, GL_SHORT, mydata.get());
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, volumn_w, volumn_h, volumn_d, 0, GL_RED, GL_UNSIGNED_BYTE, mydata.get());
    glGenerateMipmap(GL_TEXTURE_3D);

    pShader->use();
    //glUniform1i(glGetUniformLocation(pShader->ID, "texture0"), 0);
    glUniform3f(glGetUniformLocation(pShader->ID, "step_size"), 1.0f / volumn_w, 1.0f / volumn_h, 1.0f / volumn_d);
    glUniform1i(glGetUniformLocation(pShader->ID, "volume"), 0);
#else
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
#endif
  //  glShadeModel(GL_FLAT);
    glm::vec4 bg = glm::vec4(0.5, 0.5, 1, 1);
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void pro_transform::paintGL()
{
    //set the camera transform
    glm::mat4 Tr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
    glm::mat4 Rx = glm::rotate(Tr, glm::radians(rX), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 MV = glm::rotate(Rx, glm::radians(rY), glm::vec3(0.0f, 1.0f, 0.0f));
    //MV = glm::scale(MV, glm::vec3{ 2,2,2 });

    //get the camera position
    glm::vec3 camPos = glm::vec3(glm::inverse(MV)*glm::vec4(0, 0, 0, 1));

    //clear colour and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //get the combined modelview projection matrix
    glm::mat4 MVP = P*MV;

    //render grid
    //grid->Render(glm::value_ptr(MVP));

    //enable blending and bind the cube vertex array object
    glBindVertexArray(vao);
    //bind the raycasting shader
    pShader->use();
    //pass shader uniforms
   // pShader->setMat4("MVP", MVP);
    //pShader->setVec3("camPos",  camPos);
    glUniformMatrix4fv(glGetUniformLocation(pShader->ID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform3fv(glGetUniformLocation(pShader->ID, "camPos"), 1, &(camPos.x));
    glUniform1f(glGetUniformLocation(pShader->ID, "myoffset"), myoff);
    //render the cube
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    //unbind the raycasting shader
    pShader->unUse();
    //disable blending
    //glDisable(GL_BLEND);
}

void pro_transform::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
    P = glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 100.0f);
}

void pro_transform::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    oldX = x;
    oldY = y;

    if (event->button() == Qt::MiddleButton)
        state = 0;
    else
        state = 1;

    QOpenGLWidget::mousePressEvent(event);
}

void pro_transform::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    if (state == 0) {
        dist += (y - oldY) / 50.0f;
    }
    else {
        rX += (y - oldY) / 5.0f;
        rY += (x - oldX) / 5.0f;
    }
    oldX = x;
    oldY = y;

	update();
	QOpenGLWidget::mouseMoveEvent(event);
}

void pro_transform::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Left == event->key())
    {
        myoff -= 0.01;
        m_rotateY -= 1;
    }
    if (Qt::Key_Right == event->key())
    {
        myoff += 0.01;
        m_rotateY += 1;
    }
    if (Qt::Key_Up == event->key())
    {
        m_rotateX -= 1;
    }
    if (Qt::Key_Down == event->key())
    {
        m_rotateX += 1;
    }
    if (Qt::Key_PageUp == event->key())
    {
        m_rotateZ -= 1;
    }
    if (Qt::Key_PageDown == event->key())
    {
        m_rotateZ += 1;
    }

    qBound<float>(-1, myoff, 1);
    std::cout << myoff << std::endl;

    update();
}
