#include "glview.h"
#include <QOpenGLExtraFunctions>
#include <math.h>
#include <QPushButton>
#include <cstdlib>
#include <QEvent>
#include <time.h>
#include <QElapsedTimer>
#include <QTest>

bool starter = false;
bool shooter = false;


float floor_place[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};
float wall1_place[] = {0,1,1, 0,-1,1, 0,-1,-1, 0,1,-1};
float wall2_place[] = {1,0,1, -1,0,1, -1,0,-1, 1,0,-1};

float cube[] = {0,0,0, 0,1,0, 1,1,0, 1,0,0, 0,0,1, 0,1,1, 1,1,1, 1,0,1};
GLuint cubeInd[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4, 3,2,5, 6,7,3, 0,1,5, 5,4,0,
                   1,2,6, 6,5,1, 0,3,7, 7,4,0};


float gravity = 0.002;

struct {
    float x,y,z;
    float Xrot, Zrot;
} camera= {2,2,2,70,-40};

typedef struct {
    float x,y,z;
    float dx, dy, dz;
    float Xrot, Zrot;
} Tbullet;

Tbullet rocket;

float width = 900;
float height = 600;

#define pW 20
#define pH 20

typedef struct {
    float r, g, b;
} TColor;

typedef struct {
    TColor clr;
} TCell;

TCell map[pW][pH];

#define enemyCnt 0

struct {
    float x,y,z;
    bool active;
} enemy[enemyCnt];


float xAngle = 0;
float zAngle = 0;
float xpos = 0;
float ypos = 0;
int widthW, heightW;

glView::glView(QWidget *parent)
    : QOpenGLWidget(parent)
{
    ui.setupUi(this);
    setWindowTitle("Battle Turtle");
    setGeometry(250, 50, 900, 600);

    QPushButton* pcmdSTART = new QPushButton("&START", this);
    QPushButton* pcmdfullScreen = new QPushButton("&FULL SCREEN", this);
    QPushButton* pcmdQUIT = new QPushButton("&QUIT", this);
    pcmdSTART->setFixedWidth(200);
    pcmdSTART->setFixedHeight(100);

    pcmdQUIT->setFixedWidth(200);
    pcmdQUIT->setFixedHeight(100);

    pcmdfullScreen->setFixedWidth(200);
    pcmdfullScreen->setFixedHeight(100);

    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->addWidget(pcmdSTART, 1, 0);
    ptopLayout->addWidget(pcmdfullScreen, 2, 0);
    ptopLayout->addWidget(pcmdQUIT, 3, 0);
    setLayout(ptopLayout);

    connect(pcmdSTART, SIGNAL(clicked()), SLOT(slotButtonClicked()));
    connect(pcmdSTART, SIGNAL(clicked()), pcmdSTART, SLOT(hide()));
    connect(pcmdSTART, SIGNAL(clicked()), pcmdfullScreen, SLOT(hide()));
    connect(pcmdSTART, SIGNAL(clicked()), pcmdQUIT, SLOT(hide()));

    connect(pcmdfullScreen, SIGNAL(clicked()), this, SLOT(showFullScreen()));

    connect(pcmdQUIT, SIGNAL(clicked()), this, SLOT(close()));

    if(starter == false){
    pcmdSTART->show();
    update();}
}

void mapInit()
{
    for (int i = 0; i < pW; i++)
        for (int j = 0; j < pH; j++)
        {
            float dc = (rand()%20)*0.01;
            map[i][j].clr.r = 0.31 + dc;
            map[i][j].clr.g = 0.5 + dc;
            map[i][j].clr.b = 0.13 + dc;
        }
}

void enemyInit()
{
    for (int i = 0; i < enemyCnt; i++)
    {
        enemy[i].active = true;
        enemy[i].x = rand()%pW;
        enemy[i].y = rand()%pH;
        enemy[i].z = rand()%2;
    }
}

void rocketInit(Tbullet *obj, float x1, float y1, float z1, float dx1, float dy1, float dz1)
{
    obj->x = x1;
    obj->y = y1;
    obj->z = z1;

    obj->dx = dx1;
    obj->dy = dy1;
    obj->dz = dz1;
}


void glView::slotButtonClicked()
{
    starter = true;
    mapInit();
    enemyInit();
    rocketInit(&rocket, 2, 2, 4, 0, 0, 0);
}

void glView::initializeGL()
{

}

void glView::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}


void glView::paintGL()
{
    if (starter == false)
    {
    menu();
    }
    else {
        start();}
}

void cameraApply()
{
    glRotatef(-camera.Xrot, 1,0,0);
    glRotatef(-camera.Zrot, 0,0,1);
    glTranslatef(-camera.x, -camera.y, -camera.z);
}


void enemyShow()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_FLOAT, 0, cube);
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    for (int i = 0; i < enemyCnt; i++)
    {
        if(enemy[i].active) {
        glPushMatrix();
        glTranslatef(enemy[i].x, enemy[i].y, enemy[i].z);
        glColor3ub(200, 0, 43);
        gluSphere(quadric, 0.5, 360, 360);
        gluDeleteQuadric(quadric);
        glPopMatrix();}
        else continue;
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void glView::menu()
{
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-5, 5, -5, 5, 1, -1);
    update();
    glPopMatrix();
}

void glView::drawLevel(int a)
{
    glClearColor(0.6,0.8,1,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    cameraApply();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cube);
    for(int i = 0; i < pW; i++)
        for(int j = 0; j < pH; j++)
        {
            glPushMatrix();
            glTranslatef(i, j, 0);
            glColor3f(map[i][j].clr.r, map[i][j].clr.g, map[i][j].clr.b);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, cubeInd);
            glPopMatrix();
        }
    glDisableClientState(GL_VERTEX_ARRAY);
    enemyShow();

    if(GetKeyState(VK_LSHIFT)){
            shoot(shooter);
            }
    glPopMatrix();
}


void bulletMove(Tbullet *obj)
{
    obj->x += obj->dx;
    obj->y += obj->dy;
    obj->z += obj->dz;
    if(obj->z < 0){
    obj->dz *= -0.85;
    if (obj->dz < 0.0002){
    obj->dz = 0;
    obj->z = 0;
    gravity = 0;
    }
    }
    obj->dz -= gravity;
    //obj->z = 0;}
}

void glView::shoot(bool shooter)
{
    shooter = true;
    if(shooter == true){
    bulletMove(&rocket);
    glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_FLOAT, 0, cube);
    glPushMatrix();
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    glTranslatef(rocket.x, rocket.y, rocket.z);
    glColor3ub(50, 50, 50);
    gluSphere(quadric, 0.5, 360, 360);
    gluDeleteQuadric(quadric);
    QTest::qWait(10);
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);}

}

void cameraRotation(float xAngle, float zAngle)
{
    camera.Zrot += zAngle;
    if (camera.Zrot < 0) camera.Zrot += 360;
    if (camera.Zrot > 360) camera.Zrot -= 360;
    camera.Xrot += xAngle;
    if (camera.Xrot < 0) camera.Xrot += 360;
    if (camera.Xrot > 360) camera.Xrot -= 360;
}

void glView::cameraMove()
{
    /*if(GetKeyState(VK_UP)<0){
    xAngle = --xAngle > 360 ? 360 : xAngle;
    if(xAngle == 360){
    xAngle = 0;}}

    if(GetKeyState(VK_DOWN)<0){
    xAngle = ++xAngle < -360 ? -360 : xAngle;
    if(xAngle == -360){
    xAngle = 0;}}

    if(GetKeyState(VK_RIGHT)<0){
    zAngle = ++zAngle > 360 ? 360 : zAngle;
    if(zAngle == 360){
    zAngle = 0;}}

    if(GetKeyState(VK_LEFT)<0){
    zAngle = --zAngle < -360 ? -360 : zAngle;
    if(zAngle == -360){
    zAngle = 0;}}*/

    float angle = -camera.Zrot / 180 * M_PI;
    float speed = 0;
    if(GetKeyState('W')<0){
    speed = 0.1;}
    if(GetKeyState('S')<0){
    speed = -0.1;}
    if(GetKeyState('A')<0){
    speed = -0.1; angle += M_PI*0.5;}
    if(GetKeyState('D')<0){
    speed = -0.1; angle -= M_PI*0.5;}
    if(speed != 0){
        camera.x += sin(angle) * speed;
        camera.y += cos(angle) * speed;
    }


    ShowCursor(false);
    POINT cur;
    static POINT base = {400,300};
    GetCursorPos(&cur);
    cameraRotation((base.y-cur.y)/5.0, (base.x-cur.x)/5.0);
    SetCursorPos(base.x, base.y);
}

void glView::start()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1.7, 80);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glPushMatrix();
            glScalef(0.7, 1, 1);
            cameraMove();
            drawLevel(1);
            update();
            glPopMatrix();
            if(GetKeyState(VK_ESCAPE)<0)
            {
                close();
            }

}

glView::~glView()
{
}
