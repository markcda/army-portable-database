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
  lbl->setObjectName(DBW_OBJNAME);
  QFont font;
  font.setBold(true);
  font.setPixelSize(14);
  lbl->setFont(font);
  lbl->setWordWrap(true);
  connect(this, &DataBrickWidget::clicked, this,
          [this]() { emit openDataBrick(dataBrick); });
  lt->addWidget(lbl);
  setLayout(lt);
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void DataBrickWidget::mouseReleaseEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
    emit clicked();
    e->accept();
  } else
    e->ignore();
}
