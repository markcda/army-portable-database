#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "data.h"
#include "docs/databrick.h"
#include "documentwidget.h"
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class SearchDialog : public QDialog {
public:
  SearchDialog(Data *_data, DataBrick *_brick, QWidget *parent = nullptr);
  void searchAndDraw(QString ask);

private:
  Data *data = nullptr;
  DataBrick *brick = nullptr;
  QScrollArea *area = nullptr;
  void removeDocument(Document *doc);
};

#endif
