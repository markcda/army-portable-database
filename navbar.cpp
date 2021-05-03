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
  editNodeBtn->setText("Редактировать раздел");
  editNodeBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  editNodeBtn->setIcon(QIcon(":/arts/16/edit.svg"));
  editNodeBtn->setPopupMode(QToolButton::InstantPopup);
  editNode = new QAction("Редактировать");
  editNodeBtn->addAction(editNode);
  moveNode = new QAction("Переместить");
  editNodeBtn->addAction(moveNode);
  lt->addWidget(editNodeBtn);
  removeNodeBtn = new QToolButton(this);
  removeNodeBtn->setText("Удалить раздел");
  removeNodeBtn->setIcon(QIcon(":/arts/16/edit-delete.svg"));
  removeNodeBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  lt->addWidget(removeNodeBtn);
  searchBtn = new QToolButton(this);
  searchBtn->setText("Поиск...");
  searchBtn->setIcon(QIcon(":/arts/16/edit-find.svg"));
  searchBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  lt->addWidget(searchBtn);
  archiveBtn = new QToolButton(this);
  archiveBtn->setText("Архив");
  lt->addWidget(archiveBtn);
  auto *dbBtn = new QToolButton(this);
  dbBtn->setText("Действия с БД");
  dbBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  dbBtn->setPopupMode(QToolButton::InstantPopup);
  saveDb = new QAction("Сохранить базу");
  dbBtn->addAction(saveDb);
  importDb = new QAction("Импорт...");
  dbBtn->addAction(importDb);
  exportDb = new QAction("Экспорт...");
  dbBtn->addAction(exportDb);
  lt->addWidget(dbBtn);
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
    removeNodeBtn->setHidden(true);
  } else {
    archiveBtn->setEnabled(true);
    editNodeBtn->setHidden(last == db->getRootDataBrick());
    removeNodeBtn->setHidden(last == db->getRootDataBrick());
  }
}
