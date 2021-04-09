#include "documentwidget.h"

DocumentWidget::DocumentWidget(Document *_document, Data *_data,
                               QWidget *parent)
    : QWidget(parent) {
  document = _document;
  data = _data;
  setObjectName(DW_OBJNAME);
  setStyleSheet("#" + DW_OBJNAME + " { color: rgb(" +
                QString::number(document->textColor.red()) + ", " +
                QString::number(document->textColor.green()) + ", " +
                QString::number(document->textColor.blue()) +
                "); background-color: rgb(" +
                QString::number(document->backgroundColor.red()) + ", " +
                QString::number(document->backgroundColor.green()) + ", " +
                QString::number(document->backgroundColor.blue()) + ");}");
  auto *lt = new QHBoxLayout();
  auto *lbl = new QLabel(this);
  QFont font;
  font.setBold(true);
  lbl->setFont(font);
  lbl->setText(document->name);
  lbl->setIndent(10);
  lbl->setObjectName(DW_OBJNAME);
  lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  lt->addWidget(lbl);
  auto *openBtn = new QToolButton(this);
  openBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  openBtn->setIcon(QIcon(":/arts/16/view-hidden.svg"));
  openBtn->setText("Открыть");
  connect(openBtn, &QToolButton::clicked, this,
          &DocumentWidget::openDocumentInApp);
  lt->addWidget(openBtn);
  auto *editBtn = new QToolButton(this);
  editBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  editBtn->setIcon(QIcon(":/arts/16/edit.svg"));
  editBtn->setText("Изменить");
  connect(editBtn, &QToolButton::clicked, this, &DocumentWidget::editDocument);
  lt->addWidget(editBtn);
  auto *archiveBtn = new QToolButton(this);
  archiveBtn->setEnabled(false);
  archiveBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  archiveBtn->setIcon(QIcon(":/arts/16/archive.svg"));
  archiveBtn->setText("В архив");
  lt->addWidget(archiveBtn);
  auto *removeBtn = new QToolButton(this);
  removeBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  removeBtn->setIcon(QIcon(":/arts/16/edit-delete.svg"));
  removeBtn->setText("Удалить");
  connect(removeBtn, &QToolButton::clicked, this, [this]() { emit removed(document); });
  lt->addWidget(removeBtn);
  setLayout(lt);
}

void DocumentWidget::openDocumentInApp() {
  QStringList args;
  auto *process = new QProcess(parent());
  if (document->filePath.toLower().endsWith(".doc") or
      document->filePath.toLower().endsWith(".docx"))
    process->setProgram(data->st->value(data->wordPath).toString());
  else if (document->filePath.toLower().endsWith(".xls") or
           document->filePath.toLower().endsWith(".xlsx"))
    process->setProgram(data->st->value(data->excelPath).toString());
  else if (document->filePath.toLower().endsWith(".ppt") or
           document->filePath.toLower().endsWith(".pptx"))
    process->setProgram(data->st->value(data->pptPath).toString());
  else if (document->filePath.toLower().endsWith(".vsd"))
    process->setProgram(data->st->value(data->visioPath).toString());
#ifdef Q_OS_WINDOWS
  QString path = document->filePath.replace("/", "\\");
  process->setNativeArguments("/f \"" + path + "\"");
#else
  process->setArguments({document->filePath});
#endif
  process->start();
}

void DocumentWidget::editDocument() {
  auto *editDialog = new EditDocumentDialog(document, this);
  editDialog->exec();
  emit edited();
}
