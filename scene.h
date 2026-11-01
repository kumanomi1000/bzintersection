#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit Scene(QObject *parent = nullptr);

signals:
	void mouseMove(QGraphicsSceneMouseEvent *mouseEvent);
	void mousePress(QGraphicsSceneMouseEvent *mouseEvent);

protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
};

#endif // SCENE_H
