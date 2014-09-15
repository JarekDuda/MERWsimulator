#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = QImage(3*w,3*h,QImage::Format_RGB32);

    merw = new MERW(w,h,0.1,0,0);
    timer = new QTimer(this);
    timer->setInterval(10);
    timer->start();


    QObject::connect(ui->pushButton,SIGNAL(clicked()),this, SLOT(newdef()));
    QObject::connect(ui->swMode,SIGNAL(toggled(bool)),this, SLOT(swMode(bool)));
    QObject::connect(ui->stationary,SIGNAL(toggled(bool)),this, SLOT(stationary(bool)));
    QObject::connect(ui->defects,SIGNAL(toggled(bool)),this, SLOT(chdef(bool)));
    QObject::connect(ui->cych,SIGNAL(toggled(bool)),this, SLOT(cych(bool)));
    QObject::connect(ui->cycv,SIGNAL(toggled(bool)),this, SLOT(cycv(bool)));
    QObject::connect(ui->gradh,SIGNAL(valueChanged(int)),this, SLOT(chGradh(int)));
    QObject::connect(ui->gradv,SIGNAL(valueChanged(int)),this, SLOT(chGradv(int)));
    QObject::connect(ui->defp,SIGNAL(valueChanged(int)),this, SLOT(chDefp(int)));
    QObject::connect(ui->walkers,SIGNAL(valueChanged(int)),this, SLOT(walkers(int)));
    QObject::connect(ui->width,SIGNAL(valueChanged(int)),this, SLOT(setwdth(int)));
    QObject::connect(ui->height,SIGNAL(valueChanged(int)),this, SLOT(setheight(int)));
    QObject::connect(ui->deltaT,SIGNAL(valueChanged(int)),this, SLOT(setdT(int)));
    QObject::connect(ui->pointden,SIGNAL(clicked()),this, SLOT(pointden()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    ui->label->setScaledContents(true);    
    draw();

}
void MainWindow::draw(){
    float c= 255/merw->maxv;
    int ps=0;
    for(int j=0;j<merw->h;j++) for(int i=0;i<merw->w;i++) {
        //qDebug()<<"d: "<<i<<j<<ps<<(*merw->display)[ps];
        int g=255-(int)(c*(*merw->display)[ps++]); col = qRgb(g,g,g);
        for(int y=3*j; y<3*j+3; y++) for(int x=3*i; x<3*i+3; x++)
                img.setPixel(x,y,col);
    }
    if(shdef){
        ps=0; col=qRgb(255,0,0);
        for(int y=0;y<merw->h;y++)
            for(int x=0;x<merw->w;x++)
                if(merw->self[ps++] == 0){
                    img.setPixel(3*x+1, 3*y+1, col);
                    img.setPixel(3*x, 3*y, col); img.setPixel(3*x+2, 3*y, col);
                    img.setPixel(3*x, 3*y+2, col); img.setPixel(3*x+2, 3*y+2, col);
        }

    }

    for(int i=0; i<merw->walkers.size(); i++)
    {auto t=merw->walkers[i]; col=qRgb(t.r,t.g,t.b);
        img.setPixel(3*t.x+1,3*t.y+1,col);}

    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->lambda->setText(QString::number(merw->lam));
    ui->nb_walk->setText(QString::number(merw->numbwalk));
    ui->gradx->setText(QString::number(merw->gradx));
    ui->grady->setText(QString::number(merw->grady));
    ui->defpr->setText(QString::number(merw->defp));
    ui->widthv->setText(QString::number(w));
    ui->heightv->setText(QString::number(h));   
}

MainWindow::~MainWindow()
{
    delete ui, merw;
}


void MainWindow::update(){
    merw->make_step();
    draw();
}
void MainWindow::swMode(bool mode){
    merw->MG=!mode;
    merw->update();
    draw();
}
void MainWindow::stationary(bool cont){
    merw->cont=!cont;
    merw->update();
    draw();
}
void MainWindow::cych(bool v){
    merw->cych=v;
    merw->init();
    draw();
}
void MainWindow::cycv(bool v){
    merw->cycv=v;
    merw->init();
    draw();
}
void MainWindow::chGradh(int v){
    merw->gradx = -(float)v/10;
    merw->update();
    draw();
}
void MainWindow::chGradv(int v){
    merw->grady = -(float)v/10;
    merw->update();
    draw();
}
void MainWindow::newdef(){
    merw->gen_def();
    merw->update();
    draw();
}
void MainWindow::chDefp(int v){
    merw->defp=(float)v/100;
    merw->init();
    draw();
}
void MainWindow::walkers(int v){
    merw->numbwalk=v;
    merw->update_walkers();
    draw();
}
void MainWindow::setwdth(int v){
    w=v;
    img = QImage(3*w,3*h,QImage::Format_RGB32);
    merw->w=v;
    merw->init();
    draw();
}
void MainWindow::setheight(int v){
    h=v;
    img = QImage(3*w,3*h,QImage::Format_RGB32);
    merw->h=v;
    merw->init();
    draw();
}
void MainWindow::pointden(){
    merw->point_density(qrand()%w, qrand()%h);
    merw->update();
    timer->setInterval(200);
    draw();
}
void MainWindow::setdT(int v){
    timer->setInterval(v);
}
void MainWindow::chdef(bool v){
    shdef=v;
    draw();
}
