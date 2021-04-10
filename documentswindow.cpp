#include "documentswindow.h"
#include <iostream>

DocumentsWindow::DocumentsWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowIcon(QIcon(":/arts/64/datamodeler.svg"));
  resize(1000, 560);
  data = new Data(this);
#ifdef Q_OS_WINDOWS
  if ((data->st->value("notFirstTime") != "not first time") or
      (not data->st->value(data->wordPath)
               .toString()
               .toLower()
               .contains("exe") and
       not data->st->value(data->excelPath)
               .toString()
               .toLower()
               .contains("exe") and
       not data->st->value(data->pptPath)
               .toString()
               .toLower()
               .contains("exe"))) {
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
  auto *cw = new QWidget(this);
  mainLt = new QVBoxLayout();
  navBar = new NavBar();
  connect(navBar->backBtn, &QToolButton::clicked, this,
          &DocumentsWindow::goBack);
  connect(navBar->mainBtn, &QToolButton::clicked, this,
          &DocumentsWindow::goFirst);
  connect(navBar->archiveBtn, &QToolButton::clicked, this,
          &DocumentsWindow::goArchive);
  connect(navBar->editNodeBtn, &QToolButton::clicked, this,
          &DocumentsWindow::editNode);
  mainLt->addWidget(navBar);
  sa = new QScrollArea(cw);
  mainLt->addWidget(sa);
  bottomToolBar = getBottomToolBar();
  mainLt->addWidget(bottomToolBar);
  cw->setLayout(mainLt);
  setCentralWidget(cw);
  drawNode();
}

DocumentsWindow::~DocumentsWindow() {
  data->db->generateData();
  data->db->syncDataBase();
}

void DocumentsWindow::drawNode() {
  auto *nsa = new QScrollArea(this);
  auto *brick = history.last();
  setWindowTitle(brick->name);
  setUpdatesEnabled(false);
  int scrollPercentage = 0;
  if (sa)
    scrollPercentage = sa->verticalScrollBar()->value();
  navBar->update(history.length(), data->db, brick);
  navBar->lbl->setText(brick->name);
  bottomToolBar->setHidden(brick == data->db->getArchiveDataBrick());
  nsa->setWidgetResizable(true);
  nsa->setFrameShape(QFrame::NoFrame);
  nsa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  auto *dw = new QWidget(nsa);
  auto *docsLt = new QVBoxLayout();
  auto *nodes = new NodesCollection(brick->brickNodes, this);
  connect(nodes, &NodesCollection::openDataBrick, this,
          &DocumentsWindow::goNode);
  docsLt->addWidget(nodes);
  for (auto *doc : brick->brickDocuments) {
    auto *d = new DocumentWidget(doc, data, nsa);
    connect(d, &DocumentWidget::removed, this,
            &DocumentsWindow::removeDocument);
    connect(d, &DocumentWidget::edited, this, &DocumentsWindow::drawNode);
    docsLt->addWidget(d);
  }
  docsLt->addItem(
      new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
  dw->setLayout(docsLt);
  nsa->setWidget(dw);
  mainLt->replaceWidget(sa, nsa);
  nsa->verticalScrollBar()->setValue(scrollPercentage);
  if (sa)
    delete sa;
  sa = nsa;
  setUpdatesEnabled(true);
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

void DocumentsWindow::editNode() {
  auto *editNode = new EditNodeDialog(history.last(), this);
  editNode->exec();
  drawNode();
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
