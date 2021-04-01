#ifndef DOCUMENTSWINDOW_H
#define DOCUMENTSWINDOW_H

#include "addnodedialog.h"
#include "data.h"
#include "documentwidget.h"
#include "firsttimesetupdialog.h"
#include "nodescollection.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

class DocumentsWindow : public QMainWindow {
  Q_OBJECT
public:
  // Конструкторы:
  DocumentsWindow(QWidget *parent = nullptr);

private:
  // Объекты:
  Data *data = nullptr;
  QList<DataBrick *> history;
  QWidget *navBar = nullptr;
  QWidget *bottomToolBar = nullptr;
  QWidget *cw = nullptr;
  QVBoxLayout *docsLt = nullptr;
  // Методы:
  void drawNode();
  QWidget *getNavBar();
  QWidget *getArchiveNavBar();
  QWidget *getBottomToolBar();
  void goBack();
  void goFirst();
  void goArchive();
  void addNode();
  void addDocument();
  void processNode(DataBrick *dataBrick);
  void processDocument(Document *doc);
};
#endif
