#ifndef NAVBAR_H
#define NAVBAR_H

#include "docs/databrick.h"
#include "docs/xmldatabase.h"
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QWidget>

class NavBar : public QWidget {
public:
  NavBar();
  void update(int length, XMLDataBase *db, DataBrick *last);
  QLabel *lbl = nullptr;
  QToolButton *backBtn = nullptr;
  QToolButton *mainBtn = nullptr;
  QToolButton *editNodeBtn = nullptr;
  QAction *editNode = nullptr;
  QAction *moveNode = nullptr;
  QToolButton *removeNodeBtn = nullptr;
  QToolButton *archiveBtn = nullptr;
};

#endif
