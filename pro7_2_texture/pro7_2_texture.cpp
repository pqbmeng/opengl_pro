#include "GL/glew.h"
#include <ctime>
#include "pro7_2_texture.h"
#include "shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// const char *vertexShaderSource = "#version 330 core\n"
// "layout (location = 0) in vec3 aPos;\n"
// "layout (location = 1) in vec3 aColor;\n"
// "out vec3 ourColor;\n"
// "void main()\n"
// "{\n"
// "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);\n"
// "	ourColor = aColor;\n"
// "}\0";
// const char *fragmentShaderSource = "#version 330 core\n"
// "out vec4 FragColor;\n"
// "in vec3 ourColor;\n"
// "void main()\n"
// "{\n"
// "   FragColor = vec4(ourColor,1.0);\n"
// "}\n\0";

pro7_2_texture::pro7_2_texture(QWidget *parent)
	: QOpenGLWidget(parent)
{
	
}

pro7_2_texture::~pro7_2_texture()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void pro7_2_texture::initializeGL()
{
	glewInit();
	/*int vertexShader = glCreateShader(GL_VERTEX_SHADER);*/
// 	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
// 	glCompileShader(vertexShader);
// 	int success{};
// 	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
// 	assert(GL_TRUE == success);
// 	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
// 	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
// 	glCompileShader(fragmentShader);
// 	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
// 	assert(GL_TRUE == success);
// 	shaderProgram = glCreateProgram();
// 	glAttachShader(shaderProgram, vertexShader);
// 	glAttachShader(shaderProgram, fragmentShader);
// 	glLinkProgram(shaderProgram);
// 	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
// 	assert(GL_TRUE == success);
// 	glDeleteShader(vertexShader);
	/*glDeleteShader(fragmentShader);*/

	//Shader ourShader("4.1.texture.vs", "4.1.texture.fs");
	p = new Shader("4.1.texture.vs", "4.1.texture.fs");;
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("awesomeface.png", &width, &height, &nrchannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	p->use();
	glUniform1i(glGetUniformLocation(p->ID, "texture0"), 0);
	p->setInt("texture1", 1);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void pro7_2_texture::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	p->use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void pro7_2_texture::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void pro7_2_texture::mouseMoveEvent(QMouseEvent *event)
{
	update();
	QOpenGLWidget::mouseMoveEvent(event);
}
