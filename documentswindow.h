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
#include <QMutex>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QSpacerItem>
#include <QTimer>
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
  const QString arkPath = "/usr/bin/ark";
  const QString okularPath = "/usr/bin/okular";
  const QString D2DACCBackgroundStyleSheet = "#docwindow { background-image: url(:/arts/480/background.png); background-position: center center; background-attachment: fixed; background-origin: content; background-repeat: no-repeat;";
  // Объекты:
  Data *data = nullptr;
  Exporter *exporter = nullptr;
  QList<DataBrick *> history;
  QVBoxLayout *mainLt = nullptr;
  NavBar *navBar = nullptr;
  QWidget *bottomToolBar = nullptr;
  QScrollArea *sa = nullptr;
  QMutex *autosaveMutex = new QMutex();
  int *changeNum = new int(0);
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
  void autosaveLoop();
  virtual void closeEvent(QCloseEvent *event);
};
#endif
