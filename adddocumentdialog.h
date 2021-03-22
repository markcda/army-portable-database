#ifndef ADDDOCUMENTDIALOG_H
#define ADDDOCUMENTDIALOG_H

#include "docs/document.h"
#include <QDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

class AddDocumentDialog : public QDialog {
public:
  AddDocumentDialog(QWidget *parent = nullptr);
signals:
  Document *sendResult(Document *document);

private:
  Document *getDocument();
};

#endif
