#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include "data.h"
#include "docs/document.h"
#include "editdocumentdialog.h"
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
  void edited();

private:
  Document *document = nullptr;
  Data *data = nullptr;
  void openDocumentInApp();
  void editDocument();
  const QString DW_OBJNAME = "document";
};

#endif
