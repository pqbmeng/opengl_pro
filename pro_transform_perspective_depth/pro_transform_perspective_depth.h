#pragma once

#include "optional.hpp"

#include <QtWidgets/QOpenGLWidget>
class Shader;
class pro_transform_perspective_depth : public QOpenGLWidget
{
	Q_OBJECT

private:
    using base_t = QOpenGLWidget;
    using my_t = pro_transform_perspective_depth;

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
    void keyPressEvent(QKeyEvent *event) override;
    bool event(QEvent *e) override;

private:
    void reset();
    void print() const;

private:
	int shaderProgram;
	unsigned int vao, vbo;
	unsigned int texture0,texture1;
	Shader* pShader;
    float angleH{};
    float angleV{};
    nonstd::optional<QPoint> m_pressedPoint{};
    GLfloat m_scale{ };
    GLfloat local_translation_x{};
    GLfloat local_translation_y{};
    GLfloat local_translation_z{};
    GLfloat m_fov{};
    GLfloat m_nearPlane{};
    GLfloat m_farPlane{};
    GLfloat m_cameraPosZ{};
};
