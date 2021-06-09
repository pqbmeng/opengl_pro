#pragma once

#include <QtWidgets/QOpenGLWidget>
#include "ui_pro0.h"

class pro0 : public QOpenGLWidget
{
	Q_OBJECT

public:
	pro0(QWidget *parent = Q_NULLPTR);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

private:
	Ui::pro0Class ui;
};
