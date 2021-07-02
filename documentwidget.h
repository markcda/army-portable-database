#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include "data.h"
#include "docs/document.h"
#include "editdocumentdialog.h"
#include "movedialog.h"
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMutex>
#include <QProcess>
#include <QToolButton>
#include <QWidget>

class DocumentWidget : public QWidget {
  Q_OBJECT
public:
  DocumentWidget(Document *_document, DataBrick *_brickParent, Data *_data,
                 QMutex *mutex, int *changeNum, QWidget *parent = nullptr);

signals:
  Document *removed(Document *document);
  void edited();

private:
  Document *document = nullptr;
  DataBrick *brickParent = nullptr;
  Data *data = nullptr;
  void openDocumentInApp();
  void editDocument();
  void moveDocument();
  void archiveDocument();
  QMutex *mx = nullptr;
  int *cn = nullptr;
  const QString DW_OBJNAME = "document";
  const QString LR_OBJNAME = "background-document";
};

#endif
