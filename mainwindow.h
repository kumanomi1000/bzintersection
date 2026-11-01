#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void mouseMove(QGraphicsSceneMouseEvent *mouseEvent);
	void mousePress(QGraphicsSceneMouseEvent *mouseEvent);

private:
	Scene scene;
    QGraphicsEllipseItem *rc0[4];
    QGraphicsEllipseItem *rc1[4];
    QGraphicsPathItem *ci0;
    QGraphicsPathItem *ci1;
    QList<QGraphicsEllipseItem *> ri;
    QList<QPointF> cp0;
    QList<QPointF> cp1;
    int scp;

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
