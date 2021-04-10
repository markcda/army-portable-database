#ifndef EDITNODEDIALOG_H
#define EDITNODEDIALOG_H

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

class EditNodeDialog : public QDialog {
  Q_OBJECT
public:
  // Конструкторы:
  EditNodeDialog(DataBrick *_dataBrick, QWidget *parent = nullptr);

signals:
  void edited();

private:
  // Объекты:
  QLineEdit *nodeNameLine = nullptr;
  QComboBox *bgColorComboBox = nullptr;
  QComboBox *textColorComboBox = nullptr;
  DataBrick *dataBrick = nullptr;
  // Методы:
  void save();
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
    else
      return QColor::fromRgb(255, 255, 0);
  };
  int getIndexFromColor(QColor color) {
    if (color == QColor::fromRgb(255, 255, 255))
      return 0;
    else if (color == QColor::fromRgb(0, 0, 0))
      return 1;
    else if (color == QColor::fromRgb(0, 0, 255))
      return 2;
    else if (color == QColor::fromRgb(255, 0, 0))
      return 3;
    else if (color == QColor::fromRgb(0, 170, 0))
      return 4;
    else
      return 5;
  };
};

#endif
