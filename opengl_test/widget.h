#ifndef WIDGET_H
#define WIDGET_H

#include <QGLWidget>

class Widget : public QGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initializeGL() override;
    void loadGLTexture();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    GLfloat rTri;
    GLfloat rQuad;
    GLfloat xRot, yRot, zRot;
    GLuint texture[3];
    GLuint filter;
    bool light;
    bool blend;
    GLfloat zoom;
    GLfloat xSpeed, ySpeed;
};

#endif // WIDGET_H
