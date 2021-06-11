#include "GL/glew.h"
#include <ctime>
#include "pro7_2_texture_middle.h"
#include "shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

pro7_2_texture_middle::pro7_2_texture_middle(QWidget *parent)
	: QOpenGLWidget(parent)
{
	
}

pro7_2_texture_middle::~pro7_2_texture_middle()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void pro7_2_texture_middle::initializeGL()
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
		1,  1, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.5f,1.5f,// top right
		1, -1, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.5f, -0.5f,// bottom right
		-1, -1, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -0.5f,-0.5f,// bottom left
		-1,  1, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, -0.5f,1.5f // top left 
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10*sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

	
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
    float borderColor[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
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
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void pro7_2_texture_middle::paintGL()
{
	//float greenvalue = (sin(std::time(nullptr)) / 2.0f) + 0.5f;
	//glUniform4f(glGetUniformLocation(shaderProgram, "ourColor"), 0.0, greenvalue, 0.0f, 1.0f);


    {
        glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        // 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
        // 下面这行代码就需要改为:
        glm::mat4 trans = glm::mat4(1.0f);
        // 之后将不再进行提示
        //glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
        vec = trans * vec;
        std::cout << vec.x << vec.y << vec.z << std::endl;
    }

	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	p->use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// 	glUseProgram(shaderProgram);
// 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
// 	glBindVertexArray(vao);
// 	glDrawArrays(GL_TRIANGLES, 0, 3);
// 	glBindVertexArray(0);
}

void pro7_2_texture_middle::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void pro7_2_texture_middle::mouseMoveEvent(QMouseEvent *event)
{
	update();
	QOpenGLWidget::mouseMoveEvent(event);
}
