#include "scene.h"

Scene::Scene(QObject *parent)
: QGraphicsScene{parent}
{
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	emit mouseMove(mouseEvent);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	emit mousePress(mouseEvent);
}
