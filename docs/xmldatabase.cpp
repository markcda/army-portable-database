#include "xmldatabase.h"
#include <iostream>

XMLDataBase::XMLDataBase(QString directoryPath) {
  dbDirectory = directoryPath;
  QDir::setCurrent(dbDirectory);
  QFile xmlDataBaseFile(DATABASE_FILENAME);
  if (not xmlDataBaseFile.exists())
    XMLDataBase::createXMLDataBase(dbDirectory);
  xmlDataBaseFile.open(QIODevice::ReadOnly);
  xmlDataBase = new QDomDocument();
  xmlDataBase->setContent(xmlDataBaseFile.readAll());
  xmlDataBaseFile.close();
  loadData();
}

XMLDataBase::~XMLDataBase() {
  syncDataBase();
  delete xmlDataBase;
  destroyDataBrick(rootDataBrick);
  destroyDataBrick(archiveDataBrick);
}

void XMLDataBase::createXMLDataBase(QString directoryPath) {
  QDomDocument xmlDataBase(DATABASE_DOCTYPE);
  auto root = createRootElement(&xmlDataBase);
  auto archive = createArchiveElement(&xmlDataBase);
  xmlDataBase.appendChild(root);
  xmlDataBase.appendChild(archive);
  std::cout << xmlDataBase.toString().toStdString() << std::endl;
  // Запись в файл:
  QDir::setCurrent(directoryPath);
  QFile xmlDataBaseFile(DATABASE_FILENAME);
  xmlDataBaseFile.open(QIODevice::WriteOnly);
  xmlDataBaseFile.write(xmlDataBase.toByteArray());
  xmlDataBaseFile.close();
}

DataBrick *XMLDataBase::getRootDataBrick() { return rootDataBrick; }

DataBrick *XMLDataBase::getArchiveDataBrick() { return archiveDataBrick; }

QDomElement XMLDataBase::createRootElement(QDomDocument *doc, QString rootElementName) {
  QDomElement rootNode = doc->createElement(DATABASE_NODE_TAGNAME);
  rootNode.setAttribute(DATABASE_NODEATTR_NAME, rootElementName);
  rootNode.setAttribute(DATABASE_NODEATTR_UUID, generateUUID().toString());
  rootNode.setAttribute(DATABASE_NODEATTR_COLOR, QColor("white").rgb());
  rootNode.setAttribute(DATABASE_NODEATTR_TEXTCOLOR, QColor("black").rgb());
  return rootNode;
}

QDomElement XMLDataBase::createArchiveElement(QDomDocument *doc, QString archiveElementName) {
  QDomElement archiveNode = doc->createElement(DATABASE_NODE_TAGNAME);
  archiveNode.setAttribute(DATABASE_NODEATTR_NAME, archiveElementName);
  archiveNode.setAttribute(DATABASE_NODEATTR_UUID, generateUUID().toString());
  archiveNode.setAttribute(DATABASE_NODEATTR_COLOR, QColor("red").rgb());
  archiveNode.setAttribute(DATABASE_NODEATTR_TEXTCOLOR, QColor("white").rgb());
  return archiveNode;
}

QUuid XMLDataBase::generateUUID() {
  QRandomGenerator randomGenerator(QDateTime::currentSecsSinceEpoch());
  return QUuid(
      randomGenerator.bounded(UINT_MAX), randomGenerator.bounded(USHRT_MAX),
      randomGenerator.bounded(USHRT_MAX), randomGenerator.bounded(UCHAR_MAX),
      randomGenerator.bounded(UCHAR_MAX), randomGenerator.bounded(UCHAR_MAX),
      randomGenerator.bounded(UCHAR_MAX), randomGenerator.bounded(UCHAR_MAX),
      randomGenerator.bounded(UCHAR_MAX), randomGenerator.bounded(UCHAR_MAX),
      randomGenerator.bounded(UCHAR_MAX));
}

void XMLDataBase::syncDataBase() {
  QDir::setCurrent(dbDirectory);
  QFile xmlDataBaseFile(DATABASE_FILENAME);
  if (not xmlDataBaseFile.exists())
    return;
  xmlDataBaseFile.open(QIODevice::WriteOnly);
  xmlDataBaseFile.write(xmlDataBase->toByteArray());
  xmlDataBaseFile.close();
}

void XMLDataBase::generateData() {
  auto *newXmlDataBase = new QDomDocument(DATABASE_DOCTYPE);
  newXmlDataBase->appendChild(generateDomElementFromDataBrick(rootDataBrick));
  newXmlDataBase->appendChild(generateDomElementFromDataBrick(archiveDataBrick));
  if (xmlDataBase)
    delete xmlDataBase;
  xmlDataBase = newXmlDataBase;
}

void XMLDataBase::loadData() {
  QDomElement rootElement = xmlDataBase->childNodes().at(0).toElement();
  rootDataBrick = generateDataBrickFromDomElement(rootElement);
  QDomElement archiveElement = xmlDataBase->childNodes().at(1).toElement();
  archiveDataBrick = generateDataBrickFromDomElement(archiveElement);
}

DataBrick *XMLDataBase::generateDataBrickFromDomElement(QDomElement element) {
  auto *dataBrick = new DataBrick();
  dataBrick->name = element.attribute(DATABASE_NODEATTR_NAME);
  dataBrick->brickUUID =
      QUuid::fromString(element.attribute(DATABASE_NODEATTR_UUID));
  dataBrick->brickColor =
      QColor::fromRgb(element.attribute(DATABASE_NODEATTR_COLOR).toUInt());
  dataBrick->textColor =
      QColor::fromRgb(element.attribute(DATABASE_NODEATTR_TEXTCOLOR).toUInt());
  auto childNodes = element.childNodes();
  for (int i = 0; i < childNodes.length(); i++) {
    auto childNode = childNodes.at(i).toElement();
    if (childNode.tagName() == DATABASE_NODE_TAGNAME) {
      auto *childDataBrick = generateDataBrickFromDomElement(childNode);
      childDataBrick->parent = dataBrick;
      dataBrick->brickNodes.append(childDataBrick);
    } else if (childNode.tagName() == DATABASE_DOCUMENT_TAGNAME) {
      auto *document = generateDocumentFromDomElement(childNode);
      dataBrick->brickDocuments.append(document);
    }
  }
  return dataBrick;
}

Document *XMLDataBase::generateDocumentFromDomElement(QDomElement element) {
  auto *document = new Document();
  document->name = element.attribute(DATABASE_DOCATTR_NAME);
  document->filePath = element.attribute(DATABASE_DOCATTR_FILEPATH);
  document->addingDateTime = QDateTime::fromSecsSinceEpoch(
      element.attribute(DATABASE_DOCATTR_ADDDATETIME).toInt());
  document->hasDateOfExpire =
      element.attribute(DATABASE_DOCATTR_EXPIRES).toInt();
  document->expiringDateTime = QDateTime::fromSecsSinceEpoch(
      element.attribute(DATABASE_DOCATTR_EXPIREDATETIME).toInt());
  return document;
}

QDomElement XMLDataBase::generateDomElementFromDataBrick(DataBrick *dataBrick) {
  QDomElement current;
  current.setTagName(DATABASE_NODE_TAGNAME);
  current.setAttribute(DATABASE_NODEATTR_NAME, dataBrick->name);
  current.setAttribute(DATABASE_NODEATTR_UUID, dataBrick->brickUUID.toString());
  current.setAttribute(DATABASE_NODEATTR_COLOR, dataBrick->brickColor.rgb());
  current.setAttribute(DATABASE_NODEATTR_TEXTCOLOR, dataBrick->textColor.rgb());
  for (auto *childDataBrick : dataBrick->brickNodes)
    current.appendChild(generateDomElementFromDataBrick(childDataBrick));
  for (auto *document : dataBrick->brickDocuments) {
    QDomElement documentDomElement;
    documentDomElement.setTagName(DATABASE_DOCUMENT_TAGNAME);
    documentDomElement.setAttribute(DATABASE_DOCATTR_NAME, document->name);
    documentDomElement.setAttribute(DATABASE_DOCATTR_FILEPATH,
                                    document->filePath);
    documentDomElement.setAttribute(
        DATABASE_DOCATTR_ADDDATETIME,
        document->addingDateTime.toSecsSinceEpoch());
    documentDomElement.setAttribute(DATABASE_DOCATTR_EXPIRES,
                                    document->hasDateOfExpire);
    documentDomElement.setAttribute(
        DATABASE_DOCATTR_EXPIREDATETIME,
        document->expiringDateTime.toSecsSinceEpoch());
    current.appendChild(documentDomElement);
  }
  return current;
}

void XMLDataBase::destroyDataBrick(DataBrick *dataBrick) {
  for (auto childDataBrick : dataBrick->brickNodes)
    if (childDataBrick) {
      destroyDataBrick(childDataBrick);
      delete childDataBrick;
    }
  dataBrick->brickNodes.clear();
  for (auto document : dataBrick->brickDocuments)
    if (document)
      delete document;
  dataBrick->brickDocuments.clear();
}
