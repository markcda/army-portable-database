#include "documentwidget.h"

DocumentWidget::DocumentWidget(Document *_document, Data *_data, QWidget *parent)
    : QWidget(parent) {
  document = _document;
  data = _data;
  auto *lt = new QHBoxLayout();
  auto *lbl = new QLabel(this);
  lbl->setText(document->name);
  lt->addWidget(lbl);
  auto *openBtn = new QToolButton(this);
  openBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  openBtn->setIcon(QIcon(":/arts/16/view-hidden.svg"));
  openBtn->setText("Открыть");
  lt->addWidget(openBtn);
  auto *renameBtn = new QToolButton(this);
  renameBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  renameBtn->setIcon(QIcon(":/arts/16/edit.svg"));
  renameBtn->setText("Изменить");
  lt->addWidget(renameBtn);
  auto *archiveBtn = new QToolButton(this);
  archiveBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  archiveBtn->setIcon(QIcon(":/arts/16/archive.svg"));
  archiveBtn->setText("В архив");
  lt->addWidget(archiveBtn);
  auto *removeBtn = new QToolButton(this);
  removeBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  removeBtn->setIcon(QIcon(":/arts/16/edit-delete.svg"));
  removeBtn->setText("Удалить");
  lt->addWidget(removeBtn);
  setLayout(lt);
}
