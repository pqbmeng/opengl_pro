#pragma once

#include <QtWidgets/QOpenGLWidget>

class pro3 : public QOpenGLWidget
{
	Q_OBJECT

public:
	pro3(QWidget *parent = Q_NULLPTR);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

private:
	int shaderProgram;
	unsigned int vao;
};
