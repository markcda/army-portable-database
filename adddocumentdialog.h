#ifndef ADDDOCUMENTDIALOG_H
#define ADDDOCUMENTDIALOG_H

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

class AddDocumentDialog : public QDialog {
  Q_OBJECT
public:
  AddDocumentDialog(QWidget *parent = nullptr);
signals:
  Document *sendResult(Document *document);

private:
  Document *getDocument();
  QLineEdit *nameLine = nullptr;
  QPushButton *pathBtn = nullptr;
  QComboBox *colorBox = nullptr;
  QComboBox *tcolorBox = nullptr;
  QCheckBox *hasExpireDate = nullptr;
  QDateTimeEdit *dateOfExpire = nullptr;
  void selectFile();
  QString docPath;
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
};

#endif
