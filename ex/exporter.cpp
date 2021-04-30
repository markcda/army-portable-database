#include "exporter.h"
#include <iostream>

Exporter::Exporter(Data *_data, QObject *parent) : QObject(parent) {
  data = _data;
}

void Exporter::exportDataBase(QString path) {
  auto file = QFile(data->getSettingsFileDirectoryPath(data->st) +
                    QDir::separator() + "ArmyDB.xml");
  auto newFile = QFile(path);
  if (newFile.exists())
    newFile.remove();
  file.copy(path);
  path.truncate(path.length() - 4);
  auto *dir = new QDir();
  dir->mkpath(path);
  getFolderFromDataBrick(data->db->getRootDataBrick(), path, dir);
  getFolderFromDataBrick(data->db->getArchiveDataBrick(), path, dir);
  delete dir;
}

void Exporter::getFolderFromDataBrick(DataBrick *dataBrick, QString parentPath,
                                      QDir *dirObj) {
  QString dataBrickFolderPath =
      parentPath + QDir::separator() + dataBrick->brickUUID.toString();
  dirObj->mkpath(dataBrickFolderPath);
  for (auto *childBrick : dataBrick->brickNodes)
    getFolderFromDataBrick(childBrick, dataBrickFolderPath, dirObj);
  for (auto *document : dataBrick->brickDocuments) {
    auto newFilePath = dataBrickFolderPath + QDir::separator() +
                       document->filePath.split("/").last();
    auto newFile = QFile(newFilePath);
    if (newFile.exists())
      newFile.remove();
    auto docFile = QFile(document->filePath);
    docFile.copy(newFilePath);
  }
}

void Exporter::importDataBase(QString path, QString savePath) {
  auto oldDb = QFile(data->getSettingsFileDirectoryPath(data->st) +
                     QDir::separator() + "ArmyDB.xml");
  if (oldDb.exists())
    oldDb.remove();
  auto file = QFile(path);
  file.copy(data->getSettingsFileDirectoryPath(data->st) + QDir::separator() +
            "ArmyDB.xml");
  data->db->loadData();
  QString importFolderPath = path;
  importFolderPath.truncate(importFolderPath.length() - 4);
  auto *dir = new QDir();
  copyFolder(importFolderPath, savePath, dir);
  getDataBrickFromFolder(data->db->getRootDataBrick(),
                         savePath + QDir::separator() +
                             data->db->getRootDataBrick()->brickUUID.toString(),
                         dir);
  getDataBrickFromFolder(
      data->db->getArchiveDataBrick(),
      savePath + QDir::separator() +
          data->db->getArchiveDataBrick()->brickUUID.toString(),
      dir);
  delete dir;
}

void Exporter::copyFolder(QString fromPath, QString toPath, QDir *dirObj) {
  dirObj->setPath(fromPath);
  for (QString d : dirObj->entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
    QString dst_path = toPath + QDir::separator() + d;
    dirObj->mkpath(dst_path);
    copyFolder(fromPath + QDir::separator() + d, dst_path, dirObj);
  }
  dirObj->setPath(fromPath);
  for (QString f : dirObj->entryList(QDir::Files)) {
    QFile::copy(fromPath + QDir::separator() + f,
                toPath + QDir::separator() + f);
  }
}

void Exporter::getDataBrickFromFolder(DataBrick *dataBrick, QString parentPath,
                                      QDir *dirObj) {
  dirObj->setPath(parentPath);
  for (auto *brick : dataBrick->brickNodes)
    for (auto folder : dirObj->entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
      if (brick->brickUUID.toString() == folder) {
        getDataBrickFromFolder(brick, parentPath + QDir::separator() + folder,
                               dirObj);
      }
  dirObj->setPath(parentPath);
  for (auto *doc : dataBrick->brickDocuments)
    for (auto fileName : dirObj->entryList(QDir::Files))
      if (doc->filePath.split("/").last() == fileName)
        doc->filePath = parentPath + "/" + fileName;
}
