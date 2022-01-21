#pragma once

#include <QtWidgets/QOpenGLWidget>

class pro1 : public QOpenGLWidget
{
	Q_OBJECT

public:
	pro1(QWidget *parent = Q_NULLPTR);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

private:
	int shaderProgram;
	unsigned int vao;
};
