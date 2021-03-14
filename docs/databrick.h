#ifndef DATABRICK_H
#define DATABRICK_H

#include "docs/document.h"
#include <QColor>
#include <QList>
#include <QUuid>

/*!
 * @struct DataBrick
 * @brief Узел данных (раздел) базы
 */
struct DataBrick {
  QString name;
  // Идентификаторы раздела:
  QUuid brickUUID;
  DataBrick *parent = nullptr;
  // Цвет кирпичика:
  QColor brickColor;
  QColor textColor;
  // Дочерние разделы и документы раздела:
  QList<DataBrick *> brickNodes;
  QList<Document *> brickDocuments;
};

#endif
