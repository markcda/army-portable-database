#include "documentwidget.h"

DocumentWidget::DocumentWidget(Document *_document, DataBrick *_brickParent,
                               Data *_data, QMutex *mutex, int *changeNum, QWidget *parent)
    : QWidget(parent) {
  data = _data;
  brickParent = _brickParent;
  document = _document;
  mx = mutex;
  cn = changeNum;
  setAttribute(Qt::WA_StyledBackground);
  setContentsMargins(0, 0, 0, 0);
  setObjectName(LR_OBJNAME);
  setStyleSheet("#" + DW_OBJNAME + " { color: rgb(" +
                QString::number(document->textColor.red()) + ", " +
                QString::number(document->textColor.green()) + ", " +
                QString::number(document->textColor.blue()) +
                "); background-color: rgba(" +
                QString::number(document->backgroundColor.red()) + ", " +
                QString::number(document->backgroundColor.green()) + ", " +
                QString::number(document->backgroundColor.blue()) +
                ", 0.85); border-radius: 8px;} #" + LR_OBJNAME +
                " { background-color: rgba(" +
                QString::number(document->backgroundColor.red()) + ", " +
                QString::number(document->backgroundColor.green()) + ", " +
                QString::number(document->backgroundColor.blue()) +
                ", 0.6); border-radius: 8px;}");
  auto *lt = new QHBoxLayout();
  lt->setContentsMargins(0, 0, 6, 0);
  auto *lbl = new QLabel(this);
  QFont font;
  font.setBold(true);
  lbl->setFont(font);
  lbl->setText(document->name);
  lbl->setFixedHeight(
      QFontMetrics(lbl->font()).boundingRect(lbl->text()).height() + 20);
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
  if (not(brickParent == data->db->getArchiveDataBrick())) {
    auto *moveBtn = new QToolButton(this);
    moveBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    moveBtn->setIcon(QIcon(":/arts/16/archive.svg"));
    moveBtn->setText("Переместить");
    auto *moveAction = new QAction("Переместить");
    connect(moveAction, &QAction::triggered, this,
            &DocumentWidget::moveDocument);
    moveBtn->addAction(moveAction);
    auto *archiveAction = new QAction("В архив");
    connect(archiveAction, &QAction::triggered, this,
            &DocumentWidget::archiveDocument);
    moveBtn->addAction(archiveAction);
    moveBtn->setPopupMode(QToolButton::InstantPopup);
    lt->addWidget(moveBtn);
  }
  auto *removeBtn = new QToolButton(this);
  removeBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  removeBtn->setIcon(QIcon(":/arts/16/edit-delete.svg"));
  removeBtn->setText("Удалить");
  connect(removeBtn, &QToolButton::clicked, this,
          [this]() { emit removed(document); });
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
  else if (document->filePath.toLower().endsWith(".rar") or
           document->filePath.toLower().endsWith(".zip")or
           document->filePath.toLower().endsWith(".7z"))
    process->setProgram(data->st->value(data->archivesPath).toString());
  else if (document->filePath.toLower().endsWith(".pdf"))
    process->setProgram(data->st->value(data->pdfPath).toString());
#ifdef Q_OS_WINDOWS
  QString path = QDir::toNativeSeparators(document->filePath);
  process->setNativeArguments("\"" + path + "\"");
#else
  process->setArguments({document->filePath});
#endif
  process->start();
}

void DocumentWidget::editDocument() {
  if (not mx->tryLock(3000))
    return;
  auto *editDialog = new EditDocumentDialog(document, this);
  editDialog->exec();
  *cn += 1;
  mx->unlock();
  emit edited();
}

void DocumentWidget::moveDocument() {
  if (not mx->tryLock(3000))
    return;
  auto *moveDialog = new MoveDialog(data, document, brickParent, this);
  if (moveDialog->exec()) {
    *cn += 1;
    emit edited();
  }
  mx->unlock();
}

void DocumentWidget::archiveDocument() {
  if (not mx->tryLock(3000))
    return;
  for (int i = 0; i < brickParent->brickDocuments.length(); i++)
    if (brickParent->brickDocuments.at(i) == document) {
      brickParent->brickDocuments.removeAt(i);
      *cn += 1;
      break;
    }
  data->db->getArchiveDataBrick()->brickDocuments.append(document);
  mx->unlock();
  emit edited();
}
