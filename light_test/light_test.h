#pragma once

#include <QtWidgets/QOpenGLWidget>
class Shader;

class light_test : public QOpenGLWidget
{
    Q_OBJECT

    using base_t = QOpenGLWidget;
    using my_t = light_test;

public:
    light_test(QWidget *parent = Q_NULLPTR);
    ~light_test();

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
    unsigned int texture0, texture1;
    Shader* pShader;
    float angle;
};
