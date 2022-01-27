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
    void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
	int shaderProgram;
	unsigned int vao, vbo;
	unsigned int texture0,texture1;
	Shader* pShader;
    float angleH{};
    float angleV{};
    QPoint m_pressedPoint{};
    GLfloat fov{ 45.f };
};
