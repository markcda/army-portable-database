#ifndef NODESCOLLECTION_H
#define NODESCOLLECTION_H

#include "docs/databrick.h"
#include "databrickwidget.h"
#include <QList>
#include <QWidget>

class NodesCollection : public QWidget {
public:
  // Конструкторы:
  NodesCollection(QList<DataBrick *> dataBricks, QWidget *parent = nullptr);

private:
  // Константы:
  const int NC_NODESINLINE = 4;
  const QString DBW_OBJNAME = "databrick";
  const QString NC_BORDER = "border-width: 2px; border-style: solid; ";
  // Объекты:
  QList<QWidget *> widgetCollection;
  // Методы:
  QWidget *getEmptyNode();
  virtual void resizeEvent(QResizeEvent *event);
};

#endif
