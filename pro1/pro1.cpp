#include "GL/glew.h"
#include <cassert>
#include <complex>
#include <cmath>
#include <ctime>
#include <random>
#include "pro1.h"

#if 0
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);\n"
"}\0";
#endif
#if 0
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 aPos;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0, 1.f, 0, 1.0f);\n"
"}\n\0";
#endif
static auto vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexColor;
void main()
{
    gl_Position = vec4(aPos, 1);
    if (aPos.x > 0)
    {
        vertexColor = vec4(1f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        vertexColor = vec4(0, 1, 0, 1.0f);
    }
}
)";
static auto fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
uniform vec4 ourColor;
void main()
{
    FragColor = ourColor;
}
)";

pro1::pro1(QWidget *parent)
	: QOpenGLWidget(parent)
{
	
}

void pro1::initializeGL()
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
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top
	};

	unsigned int vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void pro1::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);

#if 1 // 获取支持的定点属性个数
    GLint nrAttributes{};
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
#endif

#if 1 // 反锯齿
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

#if 1 // 设置颜色
    auto vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    std::random_device rd;
    std::uniform_real_distribution<> dist(0, 1);
    glUniform4f(vertexColorLocation, dist(rd), dist(rd), dist(rd), 1);
#endif

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void pro1::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}
