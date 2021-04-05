#include "nodescollection.h"

NodesCollection::NodesCollection(QList<DataBrick *> dataBricks, QWidget *parent)
    : QWidget(parent) {
  auto *glt = new QGridLayout();
  int i;
  if (dataBricks.length() == 0) {
    glt->addWidget(new QLabel("Подразделов нет", this));
  } else {
    for (i = 0; i < dataBricks.length(); i++) {
      auto *dbw = new DataBrickWidget(dataBricks.at(i), this);
      connect(dbw, &DataBrickWidget::openDataBrick, this, &NodesCollection::openDataBrick);
      dbw->setStyleSheet(
          "#" + DBW_OBJNAME + " { " + NC_BORDER + "border-color: rgb(" +
          QString::number(dataBricks.at(i)->brickColor.red()) + ", " +
          QString::number(dataBricks.at(i)->brickColor.green()) + ", " +
          QString::number(dataBricks.at(i)->brickColor.blue()) +
          "); color: rgb(" +
          QString::number(dataBricks.at(i)->textColor.red()) + ", " +
          QString::number(dataBricks.at(i)->textColor.green()) + ", " +
          QString::number(dataBricks.at(i)->textColor.blue()) +
          "); background-color: rgb(" +
          QString::number(dataBricks.at(i)->brickColor.red()) + ", " +
          QString::number(dataBricks.at(i)->brickColor.green()) + ", " +
          QString::number(dataBricks.at(i)->brickColor.blue()) + ");}");
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
  setLayout(glt);
}

QWidget *NodesCollection::getEmptyNode() { return new QWidget(this); }

void NodesCollection::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event)
  int maxw = (width() - 30) / 4;
  for (auto *widget : widgetCollection)
    widget->setMaximumWidth(maxw);
  if (widgetCollection.length() == 0)
    resize(width(), 40);
}
