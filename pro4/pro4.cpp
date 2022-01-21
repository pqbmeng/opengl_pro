#include "GL/glew.h"
#include <QDebug>
#include "pro4.h"

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 1.0f, 0, 1.0f);\n"
"}\n\0";

pro4::pro4(QWidget *parent)
	: QGLWidget(parent)
	,r_u(0.5f, 0.5f, 0.0f)
	,r_d(0.5f, -0.5f, 0.0f)
	,l_d(-0.5f, -0.5f, 0.0f)
	,l_u(-0.5f, 0.5f, 0.0f)
{

}

void pro4::initializeGL()
{
	glewInit();
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success{};
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	assert(GL_TRUE == success);
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	assert(GL_TRUE == success);
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	assert(GL_TRUE == success);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,  // first Triangle
		3
	};

	unsigned int vbo,ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void pro4::paintGL()
{
#if 0
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glLineWidth(3);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(5.0f);
	glBegin(GL_LINE_LOOP);
	glColor4ub(255, 0, 0, 255); glVertex3f(r_u.x, r_u.y, 0.0f);//right up red
	glColor4ub(0, 0, 255, 255); glVertex3f(r_d.x, r_d.y, 0.0f);// right down blue
	glColor4ub(0, 255, 0, 255); glVertex3f(l_d.x, l_d.y, 0.0f);// left down green
	glColor4ub(255, 255, 255, 255); glVertex3f(l_u.x, l_u.y, 0.0f);// left up white
	glEnd();
	glFlush();

// 	glBegin(GL_LINE_LOOP);
// 	glColor4ub(255, 0, 0, 255); glVertex3f(r_u.x+0.2, r_u.y + 0.2, 0.0f);//right up red
// 	glColor4ub(0, 0, 255, 255); glVertex3f(r_d.x + 0.2, r_d.y + 0.2, 0.0f);// right down blue
// 	glColor4ub(0, 255, 0, 255); glVertex3f(l_d.x + 0.2, l_d.y + 0.2, 0.0f);// left down green
// 	glColor4ub(255, 255, 255, 255); glVertex3f(l_u.x + 0.2, l_u.y + 0.2, 0.0f);// left up white
// 	glEnd();
// 	glFlush();
// 	int nrAttributes=0;
// 	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor4ub(0, 255, 0, 255);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(-1, 0, 0);
	glVertex3f(1, 0, 0);
	//for (auto i = 0.0f; i <1.0f;)
	const float w = width();
	auto v = 1.0f / 100;
	for (int i = 0; i < 100; i++)
	{
		glVertex3f(v*i, 0, 0);
		if (i % 5 == 0)
		{
			glVertex3f(v*i, 0.02, 0);
		}
		else if (i % 10 == 0)
		{
			glVertex3f(v*i, 0.03, 0);
		}
		else
		{
			glVertex3f(v*i, 0.01, 0);
		}
	}
	for (int i = 0; i < 100; i++)
	{
		glVertex3f(-v*i, 0, 0);
		if (i % 5 == 0)
		{
			glVertex3f(-v*i, 0.02, 0);
		}
		else if (i % 10 == 0)
		{
			glVertex3f(-v*i, 0.03, 0);
		}
		else
		{
			glVertex3f(-v*i, 0.01, 0);
		}
	}
	glEnd();
	glFlush();


	

// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
// 	gluOrtho2D(0, 512, 512, 0);
// 
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// 	glScalef(512.0 / (float)width(), 512.0 / (float)height(), 1);
// 
// 	glDisable(GL_LIGHTING);
// 	glColor3f(1.0, 1.0, 0.0);
// 	glLineWidth(10);
// 	glBegin(GL_LINES);
// 	glVertex2f(0, 0);
// 	glVertex2f(0, 100);
// 	glVertex2f(0, 100);
// 	glVertex2f(100, 100);
// 	glVertex2f(100, 100);
// 	glVertex2f(100, 0);
// 	glVertex2f(100, 0);
// 	glVertex2f(0, 0);
// 	glEnd();
// 
// 	glFlush();
	//glutSwapBuffers();

#endif
}

void pro4::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void pro4::mouseMoveEvent(QMouseEvent *event)
{
	r_u.x += 0.01;
	r_u.y += 0.01;
	r_d.x += 0.01;
	r_d.y += 0.01;
	l_d.x += 0.01;
	l_d.y += 0.01;
	l_u.x += 0.01;
	l_u.y += 0.01;
	for (auto i : { r_u, r_d, l_u,l_d })
	{
		qDebug() << i.x << " " << i.y << " " << i.z;
	}
	updateGL();
	QGLWidget::mouseMoveEvent(event);
}
