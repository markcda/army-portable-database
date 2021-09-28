#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "docs/databrick.h"
#include <QColor>
#include <QDateTime>
#include <QString>
#include <QUuid>

inline const static QString fileDialogDocsString =
    "Документы (*.doc *.docx *.xls *.xlsx *.ppt *.pptx *.vsd *.rar *.tar.* "
    "*.zip *.7z *.pdf *.jpg *.jpeg *.tiff *.png *.bmp *.gif *.mp4)";

/*!
 * @struct Document
 * @brief Структура для хранения данных о документах.
 * @details Когда у документа истекает срок действия, он автоматически попадает
 * в архив.
 */
struct Document {
  QString name;
  QString filePath;
  // Дата и время добавления документа в базу:
  QDateTime addingDateTime;
  // Дата и время истечения срока действия:
  bool hasDateOfExpire;
  QDateTime expiringDateTime;
  QColor backgroundColor;
  QColor textColor;
};

#endif
