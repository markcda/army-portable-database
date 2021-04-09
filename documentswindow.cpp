#include "documentswindow.h"
#include <iostream>

DocumentsWindow::DocumentsWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowIcon(QIcon(":/arts/64/datamodeler.svg"));
  resize(1000, 560);
  data = new Data(this);
#ifdef Q_OS_WINDOWS
  if ((data->st->value("notFirstTime") != "not first time") or
      (not data->st->value(data->wordPath).toString().toLower().contains("exe") and
       not data->st->value(data->excelPath).toString().toLower().contains("exe") and
       not data->st->value(data->pptPath).toString().toLower().contains("exe"))) {
    (new FirstTimeSetupDialog(data, this))->exec();
    data->st->setValue("notFirstTime", "not first time");
  }
#else
  if (data->st->value("notFirstTime") != "not first time") {
    data->st->setValue("notFirstTime", "not first time");
    data->st->setValue(data->wordPath, libreOfficePath);
    data->st->setValue(data->excelPath, libreOfficePath);
    data->st->setValue(data->pptPath, libreOfficePath);
  }
#endif
  history.append(data->db->getRootDataBrick());
  drawNode();
}

DocumentsWindow::~DocumentsWindow() {
  data->db->generateData();
  data->db->syncDataBase();
}

void DocumentsWindow::drawNode() {
  auto *brick = history.last();
  setWindowTitle(brick->name);
  setUpdatesEnabled(false);
  int scrollPercentage = 0;
  if (sa)
    scrollPercentage = sa->verticalScrollBar()->value();
  if (cw)
    delete cw;
  navBar = getNavBar();
  cw = new QWidget(this);
  auto *lt = new QVBoxLayout();
  lt->addWidget(navBar);
  sa = new QScrollArea(cw);
  sa->setWidgetResizable(true);
  sa->setFrameShape(QFrame::NoFrame);
  sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  auto *dw = new QWidget(sa);
  docsLt = new QVBoxLayout();
  auto *nodes = new NodesCollection(brick->brickNodes, cw);
  connect(nodes, &NodesCollection::openDataBrick, this, &DocumentsWindow::goNode);
  docsLt->addWidget(nodes);
  for (auto *doc : brick->brickDocuments) {
    auto *d = new DocumentWidget(doc, data, sa);
    connect(d, &DocumentWidget::removed, this, &DocumentsWindow::removeDocument);
    connect(d, &DocumentWidget::edited, this, &DocumentsWindow::drawNode);
    docsLt->addWidget(d);
  }
  docsLt->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
  dw->setLayout(docsLt);
  sa->setWidget(dw);
  lt->addWidget(sa);
  if (brick != data->db->getArchiveDataBrick()) {
    bottomToolBar = getBottomToolBar();
    lt->addWidget(bottomToolBar);
  }
  cw->setLayout(lt);
  setCentralWidget(cw);
  sa->verticalScrollBar()->setValue(scrollPercentage);
  setUpdatesEnabled(true);
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
  connect(addNodeDialog, &AddNodeDialog::sendResult, this,
          &DocumentsWindow::processNode);
  addNodeDialog->show();
}

void DocumentsWindow::processNode(DataBrick *dataBrick) {
  DataBrick *curr = history.last();
  curr->brickNodes.append(dataBrick);
  drawNode();
}

void DocumentsWindow::addDocument() {
  auto *addDocumentDialog = new AddDocumentDialog(this);
  connect(addDocumentDialog, &AddDocumentDialog::sendResult, this,
          &DocumentsWindow::processDocument);
  addDocumentDialog->show();
}

void DocumentsWindow::processDocument(Document *document) {
  DataBrick *curr = history.last();
  curr->brickDocuments.append(document);
  drawNode();
}

void DocumentsWindow::goNode(DataBrick *dataBrick) {
  history.append(dataBrick);
  drawNode();
}

void DocumentsWindow::removeDocument(Document *doc) {
  DataBrick *curr = history.last();
  for (int i = 0; i < curr->brickDocuments.length(); i++) {
    if (curr->brickDocuments.at(i) == doc) {
      curr->brickDocuments.removeAt(i);
      break;
    }
  }
  drawNode();
}
