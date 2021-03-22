#ifndef ADDNODEDIALOG_H
#define ADDNODEDIALOG_H

#include "data.h"
#include "docs/databrick.h"
#include "docs/xmldatabase.h"
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QUuid>

class AddNodeDialog : public QDialog {
  Q_OBJECT
public:
  // Конструкторы:
  AddNodeDialog(Data *_data, QWidget *parent = nullptr);

signals:
  DataBrick *sendResult(DataBrick *dataBrick);

private:
  // Объекты:
  QLineEdit *nodeNameLine = nullptr;
  QComboBox *bgColorComboBox = nullptr;
  QComboBox *textColorComboBox = nullptr;
  Data *data = nullptr;
  // Методы:
  DataBrick *getDataBrick();
};

#endif
