#ifndef DATABRICKWIDGET_H
#define DATABRICKWIDGET_H

#include "docs/databrick.h"
#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>

class DataBrickWidget : public QWidget {
  Q_OBJECT
public:
  // Конструкторы:
  DataBrickWidget(DataBrick *_dataBrick, QWidget *parent = nullptr);

signals:
  void clicked();
  DataBrick *openDataBrick(DataBrick *dataBrick);

private:
  // Константы:
  const QString DBW_OBJNAME = "databrick";
  // Объекты:
  DataBrick *dataBrick;
  // Методы:
  virtual void mouseReleaseEvent(QMouseEvent *e);
};

#endif
