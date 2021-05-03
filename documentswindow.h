#ifndef DOCUMENTSWINDOW_H
#define DOCUMENTSWINDOW_H

#include "adddocumentdialog.h"
#include "addnodedialog.h"
#include "data.h"
#include "documentwidget.h"
#include "editnodedialog.h"
#include "ex/exporter.h"
#include "firsttimesetupdialog.h"
#include "movedialog.h"
#include "navbar.h"
#include "nodescollection.h"
#include "searchdialog.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QSpacerItem>
#include <QToolButton>
#include <QWidget>

class DocumentsWindow : public QMainWindow {
  Q_OBJECT
public:
  // Конструкторы:
  DocumentsWindow(QWidget *parent = nullptr);
  ~DocumentsWindow();

private:
  // Константы:
  const QString wordPath = "wordPath";
  const QString excelPath = "excelPath";
  const QString pptPath = "pptPath";
  const QString visioPath = "visioPath";
  const QString libreOfficePath = "/usr/bin/libreoffice";
  // Объекты:
  Data *data = nullptr;
  Exporter *exporter = nullptr;
  QList<DataBrick *> history;
  QVBoxLayout *mainLt = nullptr;
  NavBar *navBar = nullptr;
  QWidget *bottomToolBar = nullptr;
  QScrollArea *sa = nullptr;
  // Методы:
  void drawNode();
  QWidget *getBottomToolBar();
  void goBack();
  void goFirst();
  void goArchive();
  void goNode(DataBrick *dataBrick);
  void addNode();
  void editNode();
  void moveNode();
  void removeNode();
  void addDocument();
  void processNode(DataBrick *dataBrick);
  void processDocument(Document *doc);
  void removeDocument(Document *doc);
  void saveDb();
  void exportDb();
  void importDb();
  void openSearchDialog();
  virtual void closeEvent(QCloseEvent *event);
};
#endif
