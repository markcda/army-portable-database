#ifndef EXPORTER_H
#define EXPORTER_H

#include "data.h"
#include "docs/databrick.h"
#include "docs/document.h"
#include "docs/xmldatabase.h"
#include <QDir>
#include <QFile>
#include <QObject>
#include <QString>

class Exporter : public QObject {
public:
  Exporter(Data *_data, QObject *parent);
  void exportDataBase(QString path);
  void importDataBase(QString path, QString savePath);

private:
  Data *data = nullptr;
  void getFolderFromDataBrick(DataBrick *dataBrick, QString parentPath,
                              QDir *dirObj);
  void copyFolder(QString fromPath, QString toPath, QDir *dirObj);
  void getDataBrickFromFolder(DataBrick *dataBrick, QString parentPath,
                              QDir *dirObj);
};

#endif
