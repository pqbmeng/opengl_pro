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
const int volumn_w = 651;
const int volumn_h = 651;
const int volumn_d = 384;
pro_transform::~pro_transform()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ebo);
}

void pro_transform::initializeGL()
{
	glewInit();
	pShader = new Shader("transform.vs", "transform.fs");;
    float vertices[] = {
#if 1 // back
        - 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
#endif

#if 1 // front
        - 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
#endif

#if 1 // left
        - 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,
#endif

#if 1 // right
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,
#endif

#if 1 // bottom
        - 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
#endif

#if 1 // top
        - 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f
#endif
    };

	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	//glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	
#if 1
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_3D, texture0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    const std::string filename = "ReconData_CT_0_384_651_651_20210316151257.dat";
    const int size = volumn_w * volumn_h * volumn_d;
    auto mydata = std::make_unique<short[]>(size);
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
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(texture0);
    // GL_LUMINANCE
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16_SNORM, volumn_w, volumn_h, volumn_d, 0, GL_RED, GL_SHORT, mydata.get());
    glGenerateMipmap(GL_TEXTURE_3D);

    pShader->use();
    glUniform1i(glGetUniformLocation(pShader->ID, "texture0"), 0);
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

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void pro_transform::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture0);

    glm::mat4 matMolde = glm::mat4(1.0f);
    matMolde = glm::rotate(matMolde, glm::radians(m_rotateY), glm::vec3(0.0, 1.0, 0.0));
    matMolde = glm::rotate(matMolde, glm::radians(m_rotateX), glm::vec3(1.0, 0.0, 0.0));
    matMolde = glm::rotate(matMolde, glm::radians(m_rotateZ), glm::vec3(0.0, 0.0, 1.0));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)width() / height(), 0.1f, 100.0f);

	pShader->use();
    unsigned int transformLoc = glGetUniformLocation(pShader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projection*view*matMolde));

    {
        glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
        camPos = glm::inverse(matMolde)*glm::uvec4{ camPos , 1 };

        qDebug() << "camera in world:";
        gl_ns::print(camPos);

        camPos = { camPos.x / 1/*volumn_w*/ + 0.5,
            camPos.y / 1/*volumn_h*/ + 0.5,
            camPos.z / 1/*volumn_d*/ + 0.5 };

        qDebug() << "camera in texture:";
        gl_ns::print(camPos);

        unsigned int transformLoc7 = glGetUniformLocation(pShader->ID, "camPos");
        glUniformMatrix4fv(transformLoc7, 1, GL_FALSE, glm::value_ptr(camPos));
    }

	glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 3 * 2);
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

void pro_transform::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Left == event->key())
    {
        m_rotateY -= 1;
    }
    if (Qt::Key_Right == event->key())
    {
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

    update();
}
