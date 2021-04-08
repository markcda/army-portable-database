#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include "data.h"
#include "docs/document.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QProcess>
#include <QToolButton>
#include <QWidget>

class DocumentWidget : public QWidget {
  Q_OBJECT
public:
  DocumentWidget(Document *_document, Data *_data, QWidget *parent = nullptr);

signals:
  Document *removed(Document *document);

private:
  Document *document = nullptr;
  Data *data = nullptr;
  void openDocumentInApp();
  const QString DW_OBJNAME = "document";
};

#endif
