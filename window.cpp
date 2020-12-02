#include "window.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)//, control(nullptr)
{
    int x, y, z;
    std::cout<<"0. run default. "<<std::endl
             <<"1. run castom."<<std::endl;
    std::cin>>x;
    if(x!=0){
        std::cout<<"Input 4 3d points for the first curv"<<std::endl;
        for(int i=0; i<4; ++i){
            std::cin>>x>>y>>z;
            S1.push_back({x%10/10.0,y%10/10.0,z%10/10.0, 1.0});
            std::cout<<"-----------------"<<std::endl;
        }
        std::cout<<"Input 4 3d points for the second curv"<<std::endl;
        for(int i=0; i<4; ++i){
            std::cin>>x>>y>>z;
            S2.push_back({x%10/10.0,y%10/10.0,z%10/10.0, 1.0});
            std::cout<<"-----------------"<<std::endl;
        }
    }else{
        S1.push_back({-4/6.0, -5/6.0, 3/6.0, 1});
        S1.push_back({-2/6.0, -2/6.0, -2/6.0, 1});
        S1.push_back({-2/6.0, 2/6.0, -2/6.0, 1});
        S1.push_back({-3/6.0, 3/6.0, 3/6.0, 1});

        S2.push_back({ 4/6.0, -4/6.0,  1/6.0, 1});
        S2.push_back({ 2/6.0, -2/6.0, -3/6.0, 1});
        S2.push_back({ 2/6.0, 2/6.0, -4/6.0, 1});
        S2.push_back({ 5/6.0, 2/6.0,  5/6.0, 1});
    }
    concentrate();
    setScale(600);
    prop=1200;
    resize(scale, scale);

    std::cout<<"controls:"<<std::endl<<
               "up, down arrows - rotate around ox"<<std::endl<<
               "left, right - around oy"<<std::endl<<
               "k,l - rotate around oz "<<std::endl<<
               "n,m - change number of lines"<<std::endl<<
               "o, p - change scale"<<std::endl;
}

void Window::resizeEvent(QResizeEvent*){
    QMatrix4x4 scal=E;
    double sc=std::min(height(), width())/prop;
    scal.scale(sc,sc,sc);
    for(auto& v: S1)
         v = v*scal;
    for(auto& v: S2)
         v = v*scal;
    prop=std::min(height(), width());
}


void Window::concentrate(){
    QVector4D av = {0,0,0,0};
    for(auto v: S1)
        av+=v;
    for(auto v: S2)
        av+=v;
    av/=(S1.size()*S2.size());
    for(auto& v: S1)
        v-=av;
    for(auto& v: S2)
        v-=av;

}

void Window::setScale(float nscale){
   QMatrix4x4 scal=E;
   scal.scale(nscale/scale,nscale/scale,nscale/scale);
   for(auto& v: S1)
        v = v*scal;
   for(auto& v: S2)
        v = v*scal;
   scale=nscale;

}
Window::~Window(){
}

QVector4D Window::Bezie(double t, const std::vector<QVector4D>& vec){
    QVector4D ans = vec[0]*(1-t)*(1-t)*(1-t)
                + 3*vec[1]*t*(1-t)*(1-t)
                + 3*vec[2]*t*t*(1-t)
                +   vec[3]*t*t*t;
    ans[3]=1;
    return ans;
}

void Window::drawBezieSurf(QPainter& pntr, unsigned n){
  //  setScale(std::min(height(), width()));
    for(int i=1; i<=n; ++i){
        QVector4D s1=Bezie((double)(i-1)/n, S1),
                s2=Bezie((double)  (i-1)/n, S2),
                s3=Bezie((double)    (i)/n, S1),
                s4=Bezie((double)    (i)/n, S2);
        for(int j=1; j<=n; ++j){
            QVector4D v1=s1*(1-(double)(j-1)/n)+ s2*((double)(j-1)/n),
                      v2=s1*(1-(double)(j)/n)+ s2*((double)(j)/n),
                      v3=s3*(1-(double)(j-1)/n)+ s4*((double)(j-1)/n),
                      v4=s3*(1-(double)(j)/n)+ s4*((double)(j)/n);
            pntr.drawLine(v1.x()+width()/2, v1.y()+height()/2, v2.x()+width()/2, v2.y()+height()/2);
            pntr.drawLine(v1.x()+width()/2, v1.y()+height()/2, v3.x()+width()/2, v3.y()+height()/2);
            pntr.drawLine(v2.x()+width()/2, v2.y()+height()/2, v4.x()+width()/2, v4.y()+height()/2);
            pntr.drawLine(v3.x()+width()/2, v3.y()+height()/2, v4.x()+width()/2, v4.y()+height()/2);

        }
    }

}

void Window::keyPressEvent(QKeyEvent* k){
    QMatrix4x4 todo= E;
    if(k->key()==Qt::Key_Right){
        todo.rotate(2, 0, 1, 0);
    }
    if(k->key()==Qt::Key_Left){
        todo.rotate(-2, 0, 1, 0);
    }

    if(k->key()==Qt::Key_Down)
        todo.rotate(2, 1, 0, 0);
    if(k->key()==Qt::Key_Up)
        todo.rotate(-2, 1, 0, 0);

    if(k->key()==Qt::Key_K)
        todo.rotate(2, 0, 0, 1);
    if(k->key()==Qt::Key_L)
        todo.rotate(-2, 0, 0, 1);

    if(k->key() == Qt::Key_M)
        n+=1;
    if(k->key() == Qt::Key_N && n>1)
        n-=1;

    if(k->key() == Qt::Key_P)
        setScale((scale*1.1<(2*prop))?scale*1.1:scale);
    if(k->key() == Qt::Key_O)
        setScale((scale/1.1>prop/2)?scale/1.1:scale);
    if(todo!=E){
        for(auto& v: S1)
            v = v*todo;
        for(auto& v: S2)
            v = v*todo;
    }
    update();
}

void Window::paintEvent(QPaintEvent*){
    QPainter pntr(this);
    drawBezieSurf(pntr, n);
}
