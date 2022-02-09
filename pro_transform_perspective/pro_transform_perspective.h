#pragma once

#include <QtWidgets/QOpenGLWidget>
#include "optional.hpp"
class Shader;
class pro_transform_perspective : public QOpenGLWidget
{
	Q_OBJECT

private:
    using base_t = QOpenGLWidget;
    using my_t = pro_transform_perspective;

public:
	pro_transform_perspective(QWidget *parent = Q_NULLPTR);
	~pro_transform_perspective();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool event(QEvent *e) override;

private:
    void reset();

private:
	int shaderProgram;
	unsigned int vao, vbo, ebo;;
	unsigned int texture0,texture1;
	Shader* pShader;
    float angleH{};
    float angleV{};
    nonstd::optional<QPoint> m_pressedPoint{};
};
