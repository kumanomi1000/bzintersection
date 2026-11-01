#include "mainwindow.h"
#include "ui_mainwindow.h"

bool vBezierIntersection(
    QList<QPointF> &cp0,	// IN  3次ベジェ曲線#0 制御点
    QList<QPointF> &cp1,	// IN  3次ベジェ曲線#1 制御点
    QList<QPointF> &op);	// OUT 交点座標

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    scene.setSceneRect(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2);
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
#if 0
    // 例①
    cp0<<QPointF(7,8);
    cp0<<QPointF(23,20);
    cp0<<QPointF(1,1);
    cp0<<QPointF(15,11);
    cp1<<QPointF(10,11);
    cp1<<QPointF(22,5);
    cp1<<QPointF(2,20);
    cp1<<QPointF(12,7);
#elif 1
    // 例①の10倍
    cp0<<QPointF(70,80);
    cp0<<QPointF(230,200);
    cp0<<QPointF(10,10);
    cp0<<QPointF(150,110);
    cp1<<QPointF(100,110);
    cp1<<QPointF(220,50);
    cp1<<QPointF(20,200);
    cp1<<QPointF(120,70);
#elif 1
    // 2重根ではない (23)の100倍
    cp0<<QPointF(500,600);
    cp0<<QPointF(700,200);
    cp0<<QPointF(900,500);
    cp0<<QPointF(900,900);
    cp1<<QPointF(80.8,834.4);
    cp1<<QPointF(80.8,534.4);
    cp1<<QPointF(680.8,546.9);
    cp1<<QPointF(680.8,334.4);
#elif 1
    // 3重根ではない (22)の100倍
    cp0<<QPointF(100,600);
    cp0<<QPointF(300,200);
    cp0<<QPointF(500,500);
    cp0<<QPointF(500,900);
    cp1<<QPointF(156,568);
    cp1<<QPointF(156,268);
    cp1<<QPointF(756,468);
    cp1<<QPointF(756,68);
#elif 0
    // 3重根 (20)の100倍
    cp0<<QPointF(100,300);
    cp0<<QPointF(100,100);
    cp0<<QPointF(300,500);
    cp0<<QPointF(400,400);
    cp1<<QPointF(100,450);
    cp1<<QPointF(200,50);
    cp1<<QPointF(200,550);
    cp1<<QPointF(400,250);
#elif 0
    // 2次曲線 (26)の10倍
    cp0<<QPointF(120,210);
    cp0<<QPointF(130,10);
    cp0<<QPointF(140,10);
    cp0<<QPointF(150,210);
    cp1<<QPointF(110,180);
    cp1<<QPointF(260,140);
    cp1<<QPointF(10,120);
    cp1<<QPointF(160,110);
#elif 0
    // 1次 (27)の10倍
    cp0<<QPointF(120,20);
    cp0<<QPointF(130,100);
    cp0<<QPointF(140,180);
    cp0<<QPointF(150,260);
    cp1<<QPointF(110,180);
    cp1<<QPointF(260,140);
    cp1<<QPointF(10,120);
    cp1<<QPointF(160,110);
#elif 0
    // カスプvs通常 2重根 (11)の100倍
    cp0<<QPointF(300,100);
    cp0<<QPointF(300,200);
    cp0<<QPointF(400,200);
    cp0<<QPointF(200,100);
    cp1<<QPointF(100,400);
    cp1<<QPointF(200,300);
    cp1<<QPointF(300,100);
    cp1<<QPointF(400,200);
#elif 0
    // カスプvsカスプ 4重根 (10)
    cp0<<QPointF(7,1);
    cp0<<QPointF(7,2);
    cp0<<QPointF(8,2);
    cp0<<QPointF(6,1);
    cp1<<QPointF(2.1875,10.1875);
    cp1<<QPointF(8.1875,1.1875);
    cp1<<QPointF(7.1875,1.1875);
    cp1<<QPointF(7.1875,2.1875);
#else
    // カスプvsカスプ 4重根 (10)の100倍
    cp0<<QPointF(700,100);
    cp0<<QPointF(700,200);
    cp0<<QPointF(800,200);
    cp0<<QPointF(600,100);
    cp1<<QPointF(218.75,1018.75);
    cp1<<QPointF(818.75,118.75);
    cp1<<QPointF(718.75,118.75);
    cp1<<QPointF(718.75,218.75);
#endif
    for(int i=0;i<4;i++){
        rc0[i]=scene.addEllipse(cp0[i].x()-2.5,cp0[i].y()-2.5,5,5,QPen(Qt::blue));
        rc1[i]=scene.addEllipse(cp1[i].x()-2.5,cp1[i].y()-2.5,5,5,QPen(Qt::darkGreen));
    }
    {
        QPainterPath curve(cp0[0]);
        curve.cubicTo(cp0[1],cp0[2],cp0[3]);
        ci0=scene.addPath(curve,QPen(QBrush(Qt::blue),1));
    }
    {
        QPainterPath curve(cp1[0]);
        curve.cubicTo(cp1[1],cp1[2],cp1[3]);
        ci1=scene.addPath(curve,QPen(QBrush(Qt::darkGreen),1));
    }
    QList<QPointF> op;
    vBezierIntersection(cp0,cp1,op);
    foreach (QPointF p,op)
        ri<<scene.addEllipse(p.x()-2.5,p.y()-2.5,5,5,QPen(Qt::red),QBrush(Qt::red));
    scp=-1;

    connect(&scene,SIGNAL(mouseMove(QGraphicsSceneMouseEvent*)),this,SLOT(mouseMove(QGraphicsSceneMouseEvent*)));
    connect(&scene,SIGNAL(mousePress(QGraphicsSceneMouseEvent*)),this,SLOT(mousePress(QGraphicsSceneMouseEvent*)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mouseMove(QGraphicsSceneMouseEvent *mouseEvent)
{
    if((mouseEvent->buttons()&Qt::LeftButton)&&scp>=0){
        QPointF pos=mouseEvent->scenePos();
        if(scp<=3){
            cp0[scp]=pos;
            rc0[scp]->setRect(pos.x()-2.5,pos.y()-2.5,5,5);
            QPainterPath curve(cp0[0]);
            curve.cubicTo(cp0[1],cp0[2],cp0[3]);
            ci0->setPath(curve);
        }else{
            cp1[scp-4]=pos;
            rc1[scp-4]->setRect(pos.x()-2.5,pos.y()-2.5,5,5);
            QPainterPath curve(cp1[0]);
            curve.cubicTo(cp1[1],cp1[2],cp1[3]);
            ci1->setPath(curve);
        }
        foreach(QGraphicsEllipseItem *gi,ri){
            scene.removeItem(gi);
            delete gi;
        }
        ri.clear();
        QList<QPointF> op;
        vBezierIntersection(cp0,cp1,op);
        foreach(QPointF p,op)
            ri<<scene.addEllipse(p.x()-2.5,p.y()-2.5,5,5,QPen(Qt::red),QBrush(Qt::red));
    }
}

void MainWindow::mousePress(QGraphicsSceneMouseEvent *mouseEvent)
{
	if(mouseEvent->buttons() & Qt::LeftButton){
        QPointF pos = mouseEvent->scenePos();
        scp=-1;
        for(int i=0;i<4;i++){
            if(QLineF(pos,rc0[i]->rect().center()).length()<=5)
                scp=i;
        }
        for(int i=0;i<4;i++){
            if(QLineF(pos,rc1[i]->rect().center()).length()<=5)
                scp=i+4;
        }
    }
}
