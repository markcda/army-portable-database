#include "documentswindow.h"

DocumentsWindow::DocumentsWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowIcon(QIcon(":/arts/64/datamodeler.svg"));
  setMinimumWidth(850);
  setMinimumHeight(500);
  setObjectName("docwindow");
  setStyleSheet(D2DACCBackgroundStyleSheet + " }");
  auto flags = windowFlags();
  flags = flags ^ Qt::WindowCloseButtonHint;
  setWindowFlags(flags);
  resize(1000, 560);
  data = new Data(this);
  exporter = new Exporter(data, this);
#ifdef Q_OS_WINDOWS
  if ((data->st->value("notFirstTime") != "not first time") or
      (not data->st->value(data->wordPath)
               .toString()
               .toLower()
               .contains("exe") or
       not data->st->value(data->excelPath)
               .toString()
               .toLower()
               .contains("exe") or
       not data->st->value(data->pptPath)
               .toString()
               .toLower()
               .contains("exe") or
       not data->st->value(data->archivesPath)
               .toString()
               .toLower()
               .contains("exe") or
       not data->st->value(data->pdfPath)
               .toString()
               .toLower()
               .contains("exe") or
       not data->st->value(data->imagesPath)
               .toString()
               .toLower()
               .contains("dll") or
       not data->st->value(data->videosPath)
               .toString()
               .toLower()
               .contains("exe"))) {
    (new FirstTimeSetupDialog(data, this))->exec();
    data->st->setValue("notFirstTime", "not first time");
  }
#else
  if ((data->st->value("notFirstTime") != "not first time") or
      (data->st->value(data->wordPath).toString() != libreOfficePath or
       data->st->value(data->excelPath).toString() != libreOfficePath or
       data->st->value(data->pptPath).toString() != libreOfficePath or
       data->st->value(data->archivesPath).toString() != arkPath or
       data->st->value(data->pdfPath).toString() != okularPath or
       data->st->value(data->imagesPath).toString() != gwenviewPath or
       data->st->value(data->videosPath).toString() != vlcPath)) {
    data->st->setValue("notFirstTime", "not first time");
    data->st->setValue(data->wordPath, libreOfficePath);
    data->st->setValue(data->excelPath, libreOfficePath);
    data->st->setValue(data->pptPath, libreOfficePath);
    data->st->setValue(data->archivesPath, arkPath);
    data->st->setValue(data->pdfPath, okularPath);
    data->st->setValue(data->imagesPath, gwenviewPath);
    data->st->setValue(data->videosPath, vlcPath);
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
  connect(navBar->searchBtn, &QToolButton::clicked, this,
          &DocumentsWindow::openSearchDialog);
  connect(navBar->archiveBtn, &QToolButton::clicked, this,
          &DocumentsWindow::goArchive);
  connect(navBar->editNode, &QAction::triggered, this,
          &DocumentsWindow::editNode);
  connect(navBar->moveNode, &QAction::triggered, this,
          &DocumentsWindow::moveNode);
  connect(navBar->removeNodeBtn, &QToolButton::clicked, this,
          &DocumentsWindow::removeNode);
  connect(navBar->saveDb, &QAction::triggered, this, &DocumentsWindow::saveDb);
  connect(navBar->exportDb, &QAction::triggered, this,
          &DocumentsWindow::exportDb);
  connect(navBar->importDb, &QAction::triggered, this,
          &DocumentsWindow::importDb);
  mainLt->addWidget(navBar);
  sa = new QScrollArea(cw);
  mainLt->addWidget(sa);
  bottomToolBar = getBottomToolBar();
  mainLt->addWidget(bottomToolBar);
  cw->setLayout(mainLt);
  setCentralWidget(cw);
  drawNode();
  autosaveLoop();
}

DocumentsWindow::~DocumentsWindow() {
  data->db->generateData();
  data->db->syncDataBase();
}

void DocumentsWindow::autosaveLoop() {
  QTimer::singleShot(60000, this, [this]() {
    saveDb();
    autosaveLoop();
  });
}

void DocumentsWindow::drawNode() {
  auto *nsa = new QScrollArea(this);
  nsa->setStyleSheet("QScrollArea, #dw { background: transparent; }");
  auto *brick = history.last();
  if (brick == data->db->getArchiveDataBrick() or
      brick == data->db->getRootDataBrick())
    setStyleSheet(D2DACCBackgroundStyleSheet + " }");
  else
    setStyleSheet(D2DACCBackgroundStyleSheet + "background-color: rgba(" +
                  QString::number(brick->brickColor.red()) + ", " +
                  QString::number(brick->brickColor.green()) + ", " +
                  QString::number(brick->brickColor.blue()) + ", 0.2); }");
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
  dw->setObjectName("dw");
  dw->setAttribute(Qt::WA_NoSystemBackground);
  auto *docsLt = new QVBoxLayout();
  auto *nodes = new NodesCollection(brick->brickNodes, this);
  connect(nodes, &NodesCollection::openDataBrick, this,
          &DocumentsWindow::goNode);
  docsLt->addWidget(nodes);
  for (auto *doc : brick->brickDocuments) {
    auto *d =
        new DocumentWidget(doc, brick, data, autosaveMutex, changeNum, nsa);
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
  if (not autosaveMutex->tryLock(3000))
    return;
  auto *addNodeDialog = new AddNodeDialog(data, this);
  connect(addNodeDialog, &AddNodeDialog::sendResult, this,
          &DocumentsWindow::processNode);
  addNodeDialog->exec();
  if (addNodeDialog->result() == QDialog::Accepted)
    *changeNum += 1;
  autosaveMutex->unlock();
}

void DocumentsWindow::editNode() {
  if (not autosaveMutex->tryLock(3000))
    return;
  auto *editNode = new EditNodeDialog(history.last(), this);
  editNode->exec();
  if (editNode->result() == QDialog::Accepted)
    *changeNum += 1;
  autosaveMutex->unlock();
  drawNode();
}

void DocumentsWindow::moveNode() {
  if (not autosaveMutex->tryLock(3000))
    return;
  auto *moveNode =
      new MoveDialog(data, history.last(), history.last()->parent, this);
  moveNode->exec();
  if (moveNode->result() == QDialog::Accepted)
    *changeNum += 1;
  autosaveMutex->unlock();
  drawNode();
}

void DocumentsWindow::removeNode() {
  if (not autosaveMutex->tryLock(3000))
    return;
  auto *node = history.last();
  auto *parentBrick = node->parent;
  for (int i = 0; i < parentBrick->brickNodes.length(); i++)
    if (parentBrick->brickNodes.at(i) == node) {
      parentBrick->brickNodes.removeAt(i);
      *changeNum += 1;
      break;
    }
  history.removeLast();
  drawNode();
  if (node)
    delete node;
  autosaveMutex->unlock();
}

void DocumentsWindow::processNode(DataBrick *dataBrick) {
  DataBrick *curr = history.last();
  curr->brickNodes.append(dataBrick);
  dataBrick->parent = curr;
  drawNode();
}

void DocumentsWindow::addDocument() {
  if (not autosaveMutex->tryLock(3000))
    return;
  auto *addDocumentDialog =
      new AddDocumentDialog(data->st->value(lastPathKey).toString(), this);
  connect(addDocumentDialog, &AddDocumentDialog::sendResult, this,
          &DocumentsWindow::processDocument);
  connect(addDocumentDialog, &AddDocumentDialog::sendLastDir, this,
          &DocumentsWindow::saveLastDir);
  addDocumentDialog->exec();
  if (addDocumentDialog->result() == QDialog::Accepted)
    *changeNum += 1;
  autosaveMutex->unlock();
}

void DocumentsWindow::saveLastDir(QString lastDir) {
  data->st->setValue(lastPathKey, lastDir);
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
  if (not autosaveMutex->tryLock(3000))
    return;
  DataBrick *curr = history.last();
  for (int i = 0; i < curr->brickDocuments.length(); i++) {
    if (curr->brickDocuments.at(i) == doc) {
      curr->brickDocuments.removeAt(i);
      *changeNum += 1;
      break;
    }
  }
  autosaveMutex->unlock();
  drawNode();
}

void DocumentsWindow::saveDb() {
  if (changeNum == 0)
    return;
  if (not autosaveMutex->tryLock(3000))
    return;
  navBar->setEnabled(false);
  data->db->generateData();
  data->db->syncDataBase();
  navBar->setEnabled(true);
  *changeNum = 0;
  autosaveMutex->unlock();
}

void DocumentsWindow::exportDb() {
  navBar->setEnabled(false);
  auto fileName = QFileDialog::getSaveFileName(
      this, "Экспортировать в файл", QDir::homePath(), "База данных (*.xml)");
  if (not fileName.isEmpty())
    exporter->exportDataBase(fileName);
  navBar->setEnabled(true);
}

void DocumentsWindow::importDb() {
  if (not autosaveMutex->tryLock(3000))
    return;
  navBar->setEnabled(false);
  auto fileName = QFileDialog::getOpenFileName(
      this, "Импортировать из файла", QDir::homePath(), "База данных (*.xml)");
  auto docsDirNameDialog = QFileDialog(this);
  docsDirNameDialog.setFileMode(QFileDialog::Directory);
  docsDirNameDialog.setOption(QFileDialog::ShowDirsOnly, true);
  QString docsDirName = "";
  if (docsDirNameDialog.exec())
    if (not docsDirNameDialog.selectedFiles().isEmpty())
      docsDirName = docsDirNameDialog.selectedFiles()[0];
  if (fileName.isEmpty() or docsDirName.isEmpty()) {
    autosaveMutex->unlock();
    navBar->setEnabled(true);
    return;
  }
  exporter->importDataBase(fileName, docsDirName);
  navBar->setEnabled(true);
  history.clear();
  history.append(data->db->getRootDataBrick());
  *changeNum = 0;
  autosaveMutex->unlock();
  drawNode();
}

void DocumentsWindow::closeEvent(QCloseEvent *event) { event->ignore(); }

void DocumentsWindow::openSearchDialog() {
  auto *sd =
      new SearchDialog(data, history.last(), autosaveMutex, changeNum, this);
  connect(sd, &SearchDialog::openNodeInDW, this, &DocumentsWindow::goNode);
  sd->exec();
  drawNode();
}
