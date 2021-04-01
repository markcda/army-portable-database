#include "documentswindow.h"
#include <iostream>

DocumentsWindow::DocumentsWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowIcon(QIcon(":/arts/64/datamodeler.svg"));
  resize(1000, 560);
  data = new Data(this);
  if (data->st->value("notFirstTime") != "not first time") {
    (new FirstTimeSetupDialog(data, this))->exec();
    data->st->setValue("notFirstTime", "not first time");
  }
  history.append(data->db->getRootDataBrick());
  drawNode();
}

void DocumentsWindow::drawNode() {
  auto *brick = history.last();
  setWindowTitle(brick->name);
  if (cw)
    delete cw;
  if (brick == data->db->getArchiveDataBrick())
    navBar = getArchiveNavBar();
  else {
    navBar = getNavBar();
    bottomToolBar = getBottomToolBar();
  }
  cw = new QWidget(this);
  auto *clt = new QVBoxLayout();
  clt->addWidget(navBar);
  auto *nodes = new NodesCollection(brick->brickNodes, cw);
  clt->addWidget(nodes);
  auto *sa = new QScrollArea(cw);
  docsLt = new QVBoxLayout();
  for (auto *doc : brick->brickDocuments)
    docsLt->addWidget(new DocumentWidget(doc, data, sa));
  sa->setLayout(docsLt);
  clt->addWidget(sa);
  if (brick != data->db->getArchiveDataBrick())
    clt->addWidget(bottomToolBar);
  cw->setLayout(clt);
  setCentralWidget(cw);
}

QWidget *DocumentsWindow::getNavBar() {
  auto *w = new QWidget(this);
  auto *lt = new QHBoxLayout();
  if (history.length() > 1) {
    auto *backBtn = new QToolButton(this);
    backBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    backBtn->setArrowType(Qt::LeftArrow);
    backBtn->setText("Назад");
    connect(backBtn, &QToolButton::clicked, this, &DocumentsWindow::goBack);
    lt->addWidget(backBtn);
  }
  if (history.length() > 2) {
    auto *mainBtn = new QToolButton(this);
    mainBtn->setText("На главную");
    connect(mainBtn, &QToolButton::clicked, this, &DocumentsWindow::goFirst);
    lt->addWidget(mainBtn);
  }
  auto *lbl = new QLabel(this);
  QFont font;
  font.setBold(true);
  font.setPixelSize(18);
  lbl->setFont(font);
  lbl->setText(history.last()->name);
  lt->addWidget(lbl);
  auto *archiveBtn = new QToolButton(this);
  archiveBtn->setText("Архив");
  connect(archiveBtn, &QToolButton::clicked, this, &DocumentsWindow::goArchive);
  lt->addWidget(archiveBtn);
  w->setLayout(lt);
  return w;
}

QWidget *DocumentsWindow::getArchiveNavBar() {
  auto *w = new QWidget(this);
  auto *lt = new QHBoxLayout();
  auto *backBtn = new QToolButton(this);
  backBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  backBtn->setArrowType(Qt::LeftArrow);
  backBtn->setText("Назад");
  connect(backBtn, &QToolButton::clicked, this, &DocumentsWindow::goBack);
  lt->addWidget(backBtn);
  auto *lbl = new QLabel(this);
  QFont font;
  font.setBold(true);
  font.setPixelSize(18);
  lbl->setFont(font);
  lbl->setText("Архив");
  lt->addWidget(lbl);
  w->setLayout(lt);
  return w;
}

QWidget *DocumentsWindow::getBottomToolBar() {
  auto *w = new QWidget(this);
  auto *lt = new QHBoxLayout();
  auto *addNodeBtn = new QPushButton(this);
  addNodeBtn->setText("Добавить раздел");
  addNodeBtn->setIcon(QIcon(":/arts/16/list-add.svg"));
  connect(addNodeBtn, &QPushButton::clicked, this, &DocumentsWindow::addNode);
  lt->addWidget(addNodeBtn);
  auto *addDocumentBtn = new QPushButton(this);
  addDocumentBtn->setText("Добавить документ");
  addDocumentBtn->setIcon(QIcon(":/arts/16/list-add.svg"));
  connect(addDocumentBtn, &QPushButton::clicked, this,
          &DocumentsWindow::addDocument);
  lt->addWidget(addDocumentBtn);
  w->setLayout(lt);
  return w;
}

void DocumentsWindow::goBack() {
  history.removeLast();
  drawNode();
}

void DocumentsWindow::goFirst() {
  auto *firstBrick = history.first();
  history.clear();
  history.append(firstBrick);
  drawNode();
}

void DocumentsWindow::goArchive() {
  history.append(data->db->getArchiveDataBrick());
  drawNode();
}

void DocumentsWindow::addNode() {
  auto *addNodeDialog = new AddNodeDialog(data, this);
  connect(addNodeDialog, &AddNodeDialog::sendResult, this, &DocumentsWindow::processNode);
  addNodeDialog->show();
}

void DocumentsWindow::processNode(DataBrick *dataBrick) {
  DataBrick *curr = history.last();
  curr->brickNodes.append(dataBrick);
  drawNode();
}

void DocumentsWindow::addDocument() {}
