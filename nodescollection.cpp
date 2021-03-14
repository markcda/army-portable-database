#include "nodescollection.h"

NodesCollection::NodesCollection(QList<DataBrick *> dataBricks, QWidget *parent)
    : QWidget(parent) {
  setStyleSheet("#" + DBW_OBJNAME + " { " + NC_BORDER + " }");
  auto *glt = new QGridLayout();
  int i;
  for (i = 0; i < dataBricks.length(); i++) {
    auto *dbw = new DataBrickWidget(dataBricks.at(i), this);
    glt->addWidget(dbw, i / NC_NODESINLINE, i % NC_NODESINLINE);
    widgetCollection.append(dbw);
  }
  if (i % 4) {
    for (int j = 0; j < 4 - (i % 4); j++) {
      auto *w = getEmptyNode();
      glt->addWidget(w, (i + j + 1) / NC_NODESINLINE,
                     (i + j + 1) % NC_NODESINLINE);
      widgetCollection.append(w);
    }
  }
}

QWidget *NodesCollection::getEmptyNode() { return new QWidget(this); }

void NodesCollection::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event)
  int maxw = (width() - 30) / 4;
  for (auto *widget : widgetCollection)
    widget->setMaximumWidth(maxw);
}
