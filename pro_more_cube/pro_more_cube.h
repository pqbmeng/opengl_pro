#pragma once

#include <QtWidgets/QOpenGLWidget>
class Shader;
class pro_more_cube : public QOpenGLWidget
{
    Q_OBJECT

    using base_t = QOpenGLWidget;
    using my_t = pro_more_cube;

public:
	pro_more_cube(QWidget *parent = Q_NULLPTR);
	~pro_more_cube();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
	int shaderProgram;
	unsigned int vao, vbo, ebo;;
	unsigned int texture0,texture1;
	Shader* pShader;
    float angle;
};
