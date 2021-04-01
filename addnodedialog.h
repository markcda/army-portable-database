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
  QColor getColorFromIndex(int index) {
      if (index == 0)
        return QColor::fromRgb(255, 255, 255);
      else if (index == 1)
        return QColor::fromRgb(0, 0, 0);
      else if (index == 2)
        return QColor::fromRgb(0, 0, 255);
      else if (index == 3)
        return QColor::fromRgb(255, 0, 0);
      else if (index == 4)
        return QColor::fromRgb(0, 170, 0);
      else return QColor::fromRgb(255, 255, 0);
    }
};

#endif
