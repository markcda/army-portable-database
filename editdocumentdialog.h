#ifndef EDITDOCUMENTDIALOG_H
#define EDITDOCUMENTDIALOG_H

#include "docs/databrick.h"
#include "docs/document.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class EditDocumentDialog : public QDialog {
  Q_OBJECT
public:
  EditDocumentDialog(Document *_document, QWidget *parent = nullptr);
signals:
  void ready();

private:
  Document *document = nullptr;
  void edit();
  QLineEdit *nameLine = nullptr;
  QPushButton *pathBtn = nullptr;
  QComboBox *colorBox = nullptr;
  QComboBox *tcolorBox = nullptr;
  QCheckBox *hasExpireDate = nullptr;
  QDateTimeEdit *dateOfExpire = nullptr;
  void selectFile();
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
  }
};

#endif
