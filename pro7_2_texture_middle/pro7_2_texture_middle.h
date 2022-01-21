﻿#pragma once

#include <QtWidgets/QOpenGLWidget>
class Shader;
class pro7_2_texture_middle : public QOpenGLWidget
{
	Q_OBJECT

public:
	pro7_2_texture_middle(QWidget *parent = Q_NULLPTR);
	~pro7_2_texture_middle();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	int shaderProgram;
	unsigned int vao, vbo, ebo;;
	unsigned int texture0,texture1;
	Shader* p;
};
