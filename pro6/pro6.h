#pragma once

#include <QtWidgets/QOpenGLWidget>

class pro6 : public QOpenGLWidget
{
	Q_OBJECT

public:
	pro6(QWidget *parent = Q_NULLPTR);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	int shaderProgram;
	unsigned int vao;
};
