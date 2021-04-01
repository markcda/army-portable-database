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

private:
  // Методы:
  bool settingsFileExists(QSettings *settings);
  void firstSetup(QSettings *settings);
  void createSettingsFile(QSettings *settings);
  QString getSettingsFileDirectoryPath(QSettings *settings);
  void createDataBaseFile(QSettings *settings);
};

#endif
