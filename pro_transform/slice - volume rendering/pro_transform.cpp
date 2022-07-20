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
glm::dmat4 mdRotation(1.0f);
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
#define iWidth 16
#define iHeight 16
#define iDepth 16

static GLubyte image[iDepth][iHeight][iWidth][3]; //深度，高度，宽度+3个颜色通道
static GLuint texName;
void makeImage(void)		//造纹理
{
    int s, t, r;

    for (s = 0; s < 16; s++)
        for (t = 0; t < 16; t++)
            for (r = 0; r < 16; r++) {
                image[r][t][s][0] = (GLubyte)(s * 17);
                image[r][t][s][1] = (GLubyte)(t * 17);
                image[r][t][s][2] = (GLubyte)(r * 17);
            }
}

void pro_transform::initializeGL()
{
	glewInit();
    pShader = new Shader("transform.vs", "transform.fs");;

	
#if 1
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_3D, texture0);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    const std::string filename = "ReconData_CT_0_384_651_651_20210316151257.dat";
    const int size = volumn_w * volumn_h * volumn_d;
    auto mydata = std::make_unique<std::uint16_t[]>(size);
    //ReviewSmartPtr<plfPixelObject1D<short>> pData = plfPixelObject1D<short>::New();
    //pData->CreatePixel(size);
#if 0
    std::ifstream istrm(filename, std::ios::binary);
    istrm.read((char*)mydata.get(), sizeof(short) * size);
#else
    auto pFile = std::fopen(filename.c_str(), "rb");
    std::fread(mydata.get(), sizeof mydata[0], size, pFile);
    std::fclose(pFile);
    std::vector<std::uint16_t> rgbaBuffer(size * 4);
    const auto shortmax = std::numeric_limits<std::uint16_t>::max();
    for (unsigned int nIndx = 0; nIndx < (size); ++nIndx)
    {
        //const auto mm = shortmax - mydata[nIndx];
        std::fill_n(&rgbaBuffer[nIndx * 4], 4, mydata[nIndx]);
    }
#endif
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(texture0);
    // GL_LUMINANCE
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, volumn_w, volumn_h, volumn_d, 0,
        GL_RGBA, GL_SHORT, rgbaBuffer.data());
    //glTexImage3D(GL_TEXTURE_3D, 0, GL_R16_SNORM, volumn_w, volumn_h, volumn_d, 0, GL_RED, GL_SHORT, mydata.get());
    glGenerateMipmap(GL_TEXTURE_3D);

    //pShader->use();
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
  //  glShadeModel(GL_FLAT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}
const glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);


GLfloat dOrthoSize = 1.0f;

#define MAP_3DTEXT( TexIndex ) \
            glTexCoord3f(0.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
        glVertex3f(-dOrthoSize,-dOrthoSize,TexIndex);\
        glTexCoord3f(1.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
        glVertex3f(dOrthoSize,-dOrthoSize,TexIndex);\
        glTexCoord3f(1.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
        glVertex3f(dOrthoSize,dOrthoSize,TexIndex);\
        glTexCoord3f(0.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
        glVertex3f(-dOrthoSize,dOrthoSize,TexIndex);


void pro_transform::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // glEnable(GL_ALPHA_TEST);
   // glAlphaFunc(GL_GREATER, 0.000005f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    // Translate and make 0.5f as the center 
    // (texture co ordinate is from 0 to 1. so center of rotation has to be 0.5f)
    glTranslatef(0.5f, 0.5f, 0.5f);

    // A scaling applied to normalize the axis 
    // (Usually the number of slices will be less so if this is not - 
    // normalized then the z axis will look bulky)
    // Flipping of the y axis is done by giving a negative value in y axis.
    // This can be achieved either by changing the y co ordinates in -
    // texture mapping or by negative scaling of y axis
//     glScaled((float)data.dim.x / (float)data.dim.x,
//         -1.0f*(float)data.dim.x / (float)(float)data.dim.y,
//         (float)data.dim.x / (float)data.dim.z);

    // Apply the user provided transformations
    glMultMatrixd(glm::value_ptr(mdRotation));

    glTranslatef(-0.5f, -0.5f, -0.5f);

    glEnable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, texture0);
    for (float fIndx = -1.0f; fIndx <= 1.0f; fIndx += 0.01f)
    {
        glBegin(GL_QUADS);
        MAP_3DTEXT(fIndx);
        glEnd();
    }
}

void pro_transform::resizeGL(int w, int h)
{
    //Find the aspect ratio of the window.
    GLdouble AspectRatio = (GLdouble)(w) / (GLdouble)(h);
    //glViewport( 0, 0, cx , cy );
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Set the orthographic projection.
    if (w <= h)
    {
        glOrtho(-dOrthoSize, dOrthoSize, -(dOrthoSize / AspectRatio),
            dOrthoSize / AspectRatio, 2.0f*-dOrthoSize, 2.0f*dOrthoSize);
    }
    else
    {
        glOrtho(-dOrthoSize * AspectRatio, dOrthoSize * AspectRatio,
            -dOrthoSize, dOrthoSize, 2.0f*-dOrthoSize, 2.0f*dOrthoSize);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
QPoint mypoint;
void pro_transform::mousePressEvent(QMouseEvent *event)
{
    mypoint = event->pos();

    QOpenGLWidget::mousePressEvent(event);
}
 void Rotate(float fx_i, float fy_i, float fz_i)
{
    glm::vec3 mfRot = { fx_i,fy_i,fz_i };

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(glm::value_ptr(mdRotation));
    glRotated(mfRot.x, 1.0f, 0, 0);
    glRotated(mfRot.y, 0, 1.0f, 0);
    glRotated(mfRot.z, 0, 0, 1.0f);

    glGetDoublev(GL_MODELVIEW_MATRIX, glm::value_ptr(mdRotation));
    glLoadIdentity();
}
void pro_transform::mouseMoveEvent(QMouseEvent *event)
{
    const auto pos = event->pos();
    Rotate(mypoint.y()- pos.y() , mypoint.x()- pos.x(), 0);
    mypoint = pos;

	update();
	QOpenGLWidget::mouseMoveEvent(event);
}



void pro_transform::keyPressEvent(QKeyEvent *event)
{
    float a = 1.5f;

    if (Qt::Key_Left == event->key())
    {
        Rotate(0, a, 0);
        m_rotateY -= 1;
    }
    if (Qt::Key_Right == event->key())
    {
        Rotate(0, -a, 0);
        m_rotateY += 1;
    }
    if (Qt::Key_Up == event->key())
    {
        Rotate(a, 0, 0);
        m_rotateX -= 1;
    }
    if (Qt::Key_Down == event->key())
    {
        Rotate(-a, 0, 0);
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
