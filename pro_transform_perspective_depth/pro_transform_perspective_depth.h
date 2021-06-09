#pragma once

#include <QtWidgets/QOpenGLWidget>
class Shader;
class pro_transform_perspective_depth : public QOpenGLWidget
{
	Q_OBJECT

public:
	pro_transform_perspective_depth(QWidget *parent = Q_NULLPTR);
	~pro_transform_perspective_depth();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
	int shaderProgram;
	unsigned int vao, vbo, ebo;;
	unsigned int texture0,texture1;
	Shader* pShader;
    float angle;
};
