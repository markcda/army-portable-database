#include "searchdialog.h"

SearchDialog::SearchDialog(Data *_data, DataBrick *_brick, QWidget *parent)
    : QDialog(parent) {
  setModal(true);
  resize(1000, 560);
  data = _data;
  brick = _brick;
  setWindowTitle("Поиск документов в разделе \"" + brick->name + "\"");
  auto *lt = new QGridLayout();
  auto *searcher = new QLineEdit(this);
  searcher->setPlaceholderText("Поиск документов...");
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
  setWindowTitle("Поиск документов, включающих в названии \"" + ask + "\"");
  auto *w = new QWidget(area);
  auto *wlt = new QVBoxLayout(w);
  auto docs = data->db->searchDocuments(ask, brick);
  for (auto *doc : docs) {
    auto *d = new DocumentWidget(doc, brick, data, w);
    connect(d, &DocumentWidget::removed, this, [this, doc, d]() {
      removeDocument(doc);
      d->hide();
    });
    connect(d, &DocumentWidget::edited, this,
            [this, ask]() { searchAndDraw(ask); });
    wlt->addWidget(d);
  }
  wlt->addItem(
      new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
  w->setLayout(wlt);
  if (area->widget())
    area->widget()->hide();
  area->setWidget(w);
}

void SearchDialog::removeDocument(Document *doc) {
  auto *b = data->db->findParentByDocument(doc, brick);
  if (not b)
    return;
  for (int i = 0; i < b->brickDocuments.length(); i++)
    if (b->brickDocuments.at(i) == doc) {
      b->brickDocuments.removeAt(i);
      break;
    }
}
