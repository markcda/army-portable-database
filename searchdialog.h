#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "data.h"
#include "databrickwidget.h"
#include "docs/databrick.h"
#include "documentwidget.h"
#include "nodescollection.h"
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class SearchDialog : public QDialog {
  Q_OBJECT
public:
  // Конструкторы:
  SearchDialog(Data *_data, DataBrick *_brick, QMutex *mutex, int *changeNum, QWidget *parent = nullptr);
  // Методы:
  void searchAndDraw(QString ask);

signals:
  DataBrick *openNodeInDW(DataBrick *brick);
  
private:
  // Объекты:
  Data *data = nullptr;
  DataBrick *brick = nullptr;
  QScrollArea *area = nullptr;
  QMutex *mx = nullptr;
  int *cn = nullptr;
  // Методы:
  void removeDocument(Document *doc);
  void openDataBrick(DataBrick *brick);
};

#endif
