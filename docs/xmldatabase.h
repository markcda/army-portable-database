#ifndef XMLDATABASE_H
#define XMLDATABASE_H

#include "docs/databrick.h"
#include <QColor>
#include <QDateTime>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QFile>
#include <QRandomGenerator>
#include <QRgb>

class XMLDataBase {
public:
  // Конструкторы:
  XMLDataBase(QString directoryPath);
  ~XMLDataBase();

  // Методы:
  static void createXMLDataBase(QString directoryPath);
  DataBrick *getRootDataBrick();
  DataBrick *getArchiveDataBrick();

  //Статические методы:
  static QUuid generateUUID();

private:
  // Константы:
  inline static const QString DATABASE_FILENAME = "ArmyDB.xml";
  inline static const QString DATABASE_DOCTYPE = "cclc-xml-db";

  inline static const QString DATABASE_NODES_TAGNAME = "nodes";

  inline static const QString DATABASE_NODE_TAGNAME = "document-node";
  inline static const QString DATABASE_NODEATTR_NAME = "name";
  inline static const QString DATABASE_NODEATTR_UUID = "uuid";
  inline static const QString DATABASE_NODEATTR_COLOR = "color";
  inline static const QString DATABASE_NODEATTR_TEXTCOLOR = "text-color";

  inline static const QString DATABASE_DOCUMENT_TAGNAME = "document";
  inline static const QString DATABASE_DOCATTR_NAME = "name";
  inline static const QString DATABASE_DOCATTR_FILEPATH = "path";
  inline static const QString DATABASE_DOCATTR_ADDDATETIME = "from";
  inline static const QString DATABASE_DOCATTR_EXPIRES = "expires";
  inline static const QString DATABASE_DOCATTR_EXPIREDATETIME = "to";

  // Статические методы:
  static QDomElement createRootElement(QDomDocument *doc,
                                       QString rootElementName = "База данных");
  static QDomElement
  createArchiveElement(QDomDocument *doc,
                       QString archiveElementName = "Архив документов");

  // Объекты:
  QString dbDirectory;
  QDomDocument *xmlDataBase = nullptr;
  DataBrick *rootDataBrick = nullptr;
  DataBrick *archiveDataBrick = nullptr;

  // Методы:
  void syncDataBase();
  void generateData();
  void loadData();
  QDomElement generateDomElementFromDataBrick(DataBrick *dataBrick);
  DataBrick *generateDataBrickFromDomElement(QDomElement element);
  Document *generateDocumentFromDomElement(QDomElement element);
  void destroyDataBrick(DataBrick *dataBrick);
};

#endif
