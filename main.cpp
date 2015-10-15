#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>

#include "serveur.h"
#include "client.h"
#include "serveurwindow.h"

using namespace std;

struct point
{
    float x, y ,z;
};

class paramCamera
{
public:
    float rotX = -45.0;
    float rotY = -45.0;
    float ss = 1.0f;
    float anim = 0.0f;

    int etat = 0;
};

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();
    TriangleWindow(int maj);
    void initialize();
    void render();
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void displayTriangles();
    void displayLines();
    void displayTrianglesC();
    void displayPoints();
    void displayTrianglesTexture();

    void displayColor(float);

    void loadMap(QString localPath);
    paramCamera* c;

private:
    bool master = false;
    int m_frame;
    QImage m_image;
    point *p;


    int carte=1;


    int maj = 20;

    QTimer *timer;

};

TriangleWindow::TriangleWindow()
{
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);
    master = true;
}
TriangleWindow::TriangleWindow(int _maj)
{

    maj = _maj;
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);
}
int main(int argc, char **argv)
{
    srand(time(NULL));
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    paramCamera* c=new paramCamera();

    TriangleWindow window;
    window.c = c;
    window.setFormat(format);
    window.resize(500,375);
    window.setPosition(0,0);
    window.show();

    TriangleWindow window2;
    window2.c = c;
    window2.setFormat(format);
    window2.resize(500,375);
    window2.setPosition(500, 0);
    window2.show();

    TriangleWindow window3;
    window3.c = c;
    window3.setFormat(format);
    window3.resize(500,375);
    window3.setPosition(0, 450);
    window3.show();

    TriangleWindow window4;
    window4.c = c;
    window4.setFormat(format);
    window4.resize(500,375);
    window4.setPosition(500,450);
    window4.show();

//    serveurwindow* serv = new serveurwindow();

//    connect(serv,SIGNAL(changeSeason(int)),cl1,SLOT(s_changeS(int)));
//    connect(serv,SIGNAL(changeSeason(int)),cl2,SLOT(s_changeS(int)));
//    connect(serv,SIGNAL(changeSeason(int)),cl3,SLOT(s_changeS(int)));

    return app.exec();
}


void TriangleWindow::initialize()
{
    const qreal retinaScale = devicePixelRatio();


    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);


    loadMap(":/heightmap-1.png");

}

void TriangleWindow::loadMap(QString localPath)
{

    if (QFile::exists(localPath)) {
        m_image = QImage(localPath);
    }


    uint id = 0;
    p = new point[m_image.width() * m_image.height()];
    QRgb pixel;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            pixel = m_image.pixel(i,j);

            id = i*m_image.width() +j;

            p[id].x = (float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width();
            p[id].y = (float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height();
            p[id].z = 0.001f * (float)(qRed(pixel));
        }
    }
}

void TriangleWindow::render()
{

    glClear(GL_COLOR_BUFFER_BIT);


    glLoadIdentity();
    glScalef(c->ss,c->ss,c->ss);

    glRotatef(c->rotX,1.0f,0.0f,0.0f);
    if(c->anim == 0.0f)
    {
        glRotatef(c->rotY,0.0f,0.0f,1.0f);
    }
    else
    {
        glRotatef(c->anim,0.0f,0.0f,1.0f);
        if(master)
            c->anim +=0.05f;
    }
    switch(c->etat)
    {
    case 0:
        displayPoints();
        break;
    case 1:
        displayLines();
        break;
    case 2:
        displayTriangles();
        break;
    case 3:
        displayTrianglesC();
        break;
    case 4:
        displayTrianglesTexture();
        break;
    case 5:

        displayTrianglesTexture();
        displayLines();
        break;
    default:
        displayPoints();
        break;
    }

    ++m_frame;
}

bool TriangleWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void TriangleWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case 'C':
        if(c->anim == 0.0f)
            c->anim = c->rotY;
        else
            c->anim = 0.0f;
        break;
    case 'Z':
        c->ss += 0.10f;
        break;
    case 'S':
        c->ss -= 0.10f;
        break;
    case 'A':
        c->rotX += 1.0f;
        break;
    case 'E':
        c->rotX -= 1.0f;
        break;
    case 'Q':
        c->rotY += 1.0f;
        break;
    case 'D':
        c->rotY -= 1.0f;
        break;
    case 'W':
        c->etat ++;
        if(c->etat > 5)
            c->etat = 0;
        break;
    case 'P':
        maj++;
        timer->stop();
        timer->start(maj);
        break;
    case 'O':
        maj--;
        if(maj < 1)
            maj = 1;
        timer->stop();
        timer->start(maj);
        break;
    case 'L':
        maj = maj - 20;
        if(maj < 1)
            maj = 1;
        timer->stop();
        timer->start(maj);
        break;
    case 'M':
        maj = maj + 20;

        timer->stop();
        timer->start(maj);
        break;
    case 'X':
        carte ++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);
        break;
    }
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
}


void TriangleWindow::displayPoints()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    uint id = 0;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

        }
    }
    glEnd();
}


void TriangleWindow::displayTriangles()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void TriangleWindow::displayTrianglesC()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);


            glColor3f(1.0f, 1.0f, 1.0f);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}


void TriangleWindow::displayLines()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +(j);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void TriangleWindow::displayTrianglesTexture()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}


void TriangleWindow::displayColor(float alt)
{
    if (alt > 0.2)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else     if (alt > 0.1)
    {
        glColor3f(alt, 1.0f, 1.0f);
    }
    else     if (alt > 0.05f)
    {
        glColor3f(1.0f, alt, alt);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 1.0f);
    }

}
