#pragma once

#include <QGLWidget>
#include "glm/glm.hpp"

class pro4 : public QGLWidget
{
	Q_OBJECT

public:
	pro4(QWidget *parent = Q_NULLPTR);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	int shaderProgram;
	unsigned int vao;
	glm::vec3 r_u;
	glm::vec3 r_d;
	glm::vec3 l_d;
	glm::vec3 l_u;
};
