#pragma once

#include <QtWidgets/QOpenGLWidget>
class Shader;
class pro7 : public QOpenGLWidget
{
	Q_OBJECT

public:
	pro7(QWidget *parent = Q_NULLPTR);
	~pro7();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	int shaderProgram;
	unsigned int vao, vbo, ebo;;
	unsigned int texture;
	Shader* p;
};
