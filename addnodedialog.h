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
  AddNodeDialog(Data *_data);

signals:
  DataBrick *sendResult(DataBrick *dataBrick);

private:
  // Константы:
  inline static const QStringList bgColors = {"Белый",   "Чёрный",  "Синий",
                                             "Красный", "Зелёный", "Жёлтый"};
  inline static const QStringList textColors = {bgColors[0], bgColors[1]};
  // Объекты:
  QUuid UUID;
  QColor background_color;
  QColor text_color;
  QString name;
  Data *data = nullptr;
  // Методы:
  DataBrick *getDataBrick();
};

#endif
