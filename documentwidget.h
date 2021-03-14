#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include "data.h"
#include "docs/document.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QWidget>

class DocumentWidget : public QWidget {
public:
  DocumentWidget(Document *_document, Data *_data, QWidget *parent = nullptr);

private:
  Document *document = nullptr;
  Data *data = nullptr;
};

#endif
