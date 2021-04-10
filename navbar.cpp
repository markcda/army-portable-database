#include "navbar.h"

NavBar::NavBar() {
  auto *lt = new QHBoxLayout();
  backBtn = new QToolButton(this);
  backBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  backBtn->setArrowType(Qt::LeftArrow);
  backBtn->setText("Назад");
  lt->addWidget(backBtn);
  mainBtn = new QToolButton(this);
  mainBtn->setText("На главную");
  lt->addWidget(mainBtn);
  lbl = new QLabel(this);
  QFont font;
  font.setBold(true);
  font.setPixelSize(18);
  lbl->setFont(font);
  lt->addWidget(lbl);
  editNodeBtn = new QToolButton(this);
  editNodeBtn->setText("Редактировать");
  editNodeBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  editNodeBtn->setIcon(QIcon(":/arts/16/edit.svg"));
  lt->addWidget(editNodeBtn);
  archiveBtn = new QToolButton(this);
  archiveBtn->setText("Архив");
  lt->addWidget(archiveBtn);
  setLayout(lt);
}

void NavBar::update(int length, XMLDataBase *db, DataBrick *last) {
  if (length == 1) {
    backBtn->setHidden(true);
    mainBtn->setHidden(true);
  } else if (length < 3) {
    backBtn->setHidden(false);
    mainBtn->setHidden(true);
  } else {
    backBtn->setHidden(false);
    mainBtn->setHidden(false);
  }
  if (last == db->getArchiveDataBrick()) {
    archiveBtn->setEnabled(false);
    editNodeBtn->setHidden(true);
  } else {
    archiveBtn->setEnabled(true);
    editNodeBtn->setHidden(last == db->getRootDataBrick());
  }
}
