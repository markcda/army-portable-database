#include "data.h"

Data::Data(QObject *parent) : QObject(parent) {
  st = new QSettings(QSettings::IniFormat, QSettings::UserScope, "CCLC",
                     "ArmyDB", this);
  if (not settingsFileExists(st))
    firstSetup(st);
  db = new XMLDataBase(getSettingsFileDirectoryPath(st));
}

bool Data::settingsFileExists(QSettings *settings) {
  QFileInfo settingsFileInfo(settings->fileName());
  return settingsFileInfo.exists();
}

void Data::firstSetup(QSettings *settings) {
  createSettingsFile(settings);
  createDataBaseFile(settings);
}

void Data::createSettingsFile(QSettings *settings) {
  settings->setValue("tmp", "null");
  settings->sync();
  settings->remove("tmp");
}

void Data::createDataBaseFile(QSettings *settings) {
  XMLDataBase::createXMLDataBase(getSettingsFileDirectoryPath(settings));
}

QString Data::getSettingsFileDirectoryPath(QSettings *settings) {
  QFileInfo settingsFileInfo(settings->fileName());
  return settingsFileInfo.absolutePath();
}
