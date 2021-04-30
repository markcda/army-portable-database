#ifndef DATA_H
#define DATA_H

#include "docs/xmldatabase.h"
#include <QFileInfo>
#include <QSettings>

class Data : public QObject {
  Q_OBJECT
public:
  // Конструкторы:
  Data(QObject *parent = nullptr);
  // Объекты:
  XMLDataBase *db = nullptr;
  QSettings *st = nullptr;
  // Константы:
  const QString wordPath = "wordPath";
  const QString excelPath = "excelPath";
  const QString pptPath = "pptPath";
  const QString visioPath = "visioPath";
  // Методы:
  QString getSettingsFileDirectoryPath(QSettings *settings);

private:
  // Методы:
  bool settingsFileExists(QSettings *settings);
  void firstSetup(QSettings *settings);
  void createSettingsFile(QSettings *settings);
  void createDataBaseFile(QSettings *settings);
};

#endif
