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
  DataBrick *parent;
  // Цвет кирпичика:
  QColor brickColor;
  QColor textColor;
  // Дочерние разделы и документы раздела:
  QList<DataBrick *> brickNodes;
  QList<Document *> brickDocuments;
};

inline static const QStringList bgColors = {"Белый",   "Чёрный",  "Синий",
                                            "Красный", "Зелёный", "Жёлтый"};
inline static const QStringList textColors = {bgColors[0], bgColors[1]};

#endif
