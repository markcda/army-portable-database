#ifndef NODESCOLLECTION_H
#define NODESCOLLECTION_H

#include "databrickwidget.h"
#include "docs/databrick.h"
#include <QList>
#include <QWidget>

class NodesCollection : public QWidget {
  Q_OBJECT
public:
  // Конструкторы:
  NodesCollection(QList<DataBrick *> dataBricks, QWidget *parent = nullptr);
  // Константы:
  inline static const QString DBW_OBJNAME = "databrick";
  inline static const QString NC_BORDER =
      "border-width: 2px; border-style: solid; border-radius: 5px; ";
signals:
  DataBrick *openDataBrick(DataBrick *dataBrick);

private:
  // Константы:
  const int NC_NODESINLINE = 4;
  // Объекты:
  QList<QWidget *> widgetCollection;
  // Методы:
  QWidget *getEmptyNode();
  virtual void resizeEvent(QResizeEvent *event);
};

#endif
