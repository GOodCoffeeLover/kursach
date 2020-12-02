#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <vector>
#include <iostream>
#include <QVector4D>
#include <QPainter>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <cmath>

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();
     void paintEvent(QPaintEvent*);
     void keyPressEvent(QKeyEvent*);
     void resizeEvent(QResizeEvent*);
     void updateCurve();


private:
     QVector4D Bezie(double, const std::vector<QVector4D>&);
     void drawBezieSurf(QPainter&, unsigned);
     void setScale(float);
     void concentrate();

     std::vector<QVector4D> S1, S2;
     float scale=1.0;
     float prop=1.0;
     unsigned n=20;
     const QMatrix4x4 E={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
};
#endif // WINDOW_H
