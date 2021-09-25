#include "searchdialog.h"

SearchDialog::SearchDialog(Data *_data, DataBrick *_brick,
                           QMutex *mutex, int *changeNum, QWidget *parent)
    : QDialog(parent) {
  setModal(true);
  resize(1000, 560);
  data = _data;
  brick = _brick;
  mx = mutex;
  cn = changeNum;
  setWindowTitle("Поиск документов и разделов в \"" + brick->name + "\"");
  auto *lt = new QGridLayout();
  auto *searcher = new QLineEdit(this);
  searcher->setPlaceholderText("Поиск...");
  lt->addWidget(searcher, 0, 0);
  auto *searchBtn = new QPushButton(this);
  searchBtn->setText("Искать");
  searchBtn->setIcon(QIcon(":/arts/16/edit-find.svg"));
  connect(searchBtn, &QPushButton::clicked, this,
          [this, searcher]() { searchAndDraw(searcher->text()); });
  lt->addWidget(searchBtn, 0, 1);
  area = new QScrollArea(this);
  area->setWidgetResizable(true);
  area->setFrameShape(QFrame::NoFrame);
  area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  lt->addWidget(area, 1, 0, 1, 2);
  setLayout(lt);
}

void SearchDialog::searchAndDraw(QString ask) {
  setWindowTitle("Поиск документов и разделов, включающих в названии \"" + ask + "\"");
  auto *w = new QWidget(area);
  auto *wlt = new QVBoxLayout(w);
  auto nodes = data->db->searchNodes(ask, brick);
  for (int cntr = nodes.keys().length(); cntr > 0; cntr--) {
    for (auto *node : nodes[cntr]) {
      auto *nodeWidget = new DataBrickWidget(node, this);
      nodeWidget->setObjectName(NodesCollection::DBW_OBJNAME);
      nodeWidget->setStyleSheet("#" + NodesCollection::DBW_OBJNAME + 
      " { " + NodesCollection::NC_BORDER + " " + "border-color: rgb(" +
      QString::number(node->brickColor.red()) + ", " +
      QString::number(node->brickColor.green()) + ", " +
      QString::number(node->brickColor.blue()) +
      "); color: rgb(" +
      QString::number(node->textColor.red()) + ", " +
      QString::number(node->textColor.green()) + ", " +
      QString::number(node->textColor.blue()) +
      "); background-color: rgb(" +
      QString::number(node->brickColor.red()) + ", " +
      QString::number(node->brickColor.green()) + ", " +
      QString::number(node->brickColor.blue()) + ");}");
      connect(nodeWidget, &DataBrickWidget::openDataBrick, this, &SearchDialog::openDataBrick);
      wlt->addWidget(nodeWidget);
    }
  }
  auto docs = data->db->searchDocuments(ask, brick);
  for (int cntr = docs.keys().length(); cntr > 0; cntr--) {
    for (auto *doc : docs[cntr]) {
      auto *d = new DocumentWidget(doc, brick, data, mx, cn, w);
      connect(d, &DocumentWidget::removed, this, [this, doc, d]() {
        removeDocument(doc);
        d->hide();
      });
      connect(d, &DocumentWidget::edited, this,
              [this, ask]() { searchAndDraw(ask); });
      wlt->addWidget(d);
    }
  }
  wlt->addItem(
      new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
  w->setLayout(wlt);
  if (area->widget())
    area->widget()->hide();
  area->setWidget(w);
}

void SearchDialog::openDataBrick(DataBrick *brick) {
  if (!brick) return; // Если удалили раздел в базе, но не обновили результаты поиска.
  emit openNodeInDW(brick);
  close();
}

void SearchDialog::removeDocument(Document *doc) {
  if (not mx->tryLock(3000))
    return;
  auto *b = data->db->findParentByDocument(doc, brick);
  if (not b)
    return;
  for (int i = 0; i < b->brickDocuments.length(); i++)
    if (b->brickDocuments.at(i) == doc) {
      b->brickDocuments.removeAt(i);
      *cn += 1;
      break;
    }
  mx->unlock();
}
