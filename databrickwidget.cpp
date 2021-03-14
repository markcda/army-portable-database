#include "databrickwidget.h"

DataBrickWidget::DataBrickWidget(DataBrick *_dataBrick, QWidget *parent)
    : QWidget(parent) {
  dataBrick = _dataBrick;
  setAttribute(Qt::WA_StyledBackground);
  setObjectName(DBW_OBJNAME);
  setContentsMargins(0, 0, 0, 0);
  auto *lt = new QGridLayout();
  lt->setContentsMargins(0, 0, 0, 0);
  auto *lbl = new QLabel(dataBrick->name);
  QFont font;
  font.setBold(true);
  font.setPixelSize(14);
  lbl->setFont(font);
  lbl->setWordWrap(true);
  setStyleSheet("#" + DBW_OBJNAME + " { color: rgb(" +
                QString::number(dataBrick->brickColor.red()) + ", " +
                QString::number(dataBrick->brickColor.green()) + ", " +
                QString::number(dataBrick->brickColor.blue()) + "); }");
  connect(this, &DataBrickWidget::clicked, this,
          [this]() { emit openDataBrick(dataBrick); });
}

void DataBrickWidget::mouseReleaseEvent(QMouseEvent *e) {
  Q_UNUSED(e)
  emit clicked();
}
