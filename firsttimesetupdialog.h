#ifndef FIRSTTIMESETUPDIALOG_H
#define FIRSTTIMESETUPDIALOG_H

#include "data.h"
#include <QDialog>
#include <QFileDialog>
#include <QFontMetrics>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>

enum DocumentType { MSWord, MSVisio, MSPowerPoint, MSExcel };

class FirstTimeSetupDialog : public QDialog {
public:
  // Конструкторы:
  FirstTimeSetupDialog(Data *_data, QWidget *parent = nullptr);

private:
  // Константы:
  const QString noPath = "(не указан)";
  // Объекты:
  Data *data = nullptr;
  QString MSWord;
  QString MSVisio;
  QString MSPP;
  QString MSExcel;
  QPushButton *wordBtn = nullptr;
  QPushButton *ppBtn = nullptr;
  QPushButton *excelBtn = nullptr;
  QPushButton *visioBtn = nullptr;
  QPushButton *acceptBtn = nullptr;
  // Методы:
  void saveAll();
  void saveSettings(DocumentType docType, QString path);
  QString getPath(DocumentType docType);
  void updateAcceptEnabled();
};

#endif
