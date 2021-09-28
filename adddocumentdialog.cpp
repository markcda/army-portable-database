#include "adddocumentdialog.h"

AddDocumentDialog::AddDocumentDialog(QString _lastPath, QWidget *parent)
    : QDialog(parent) {
  lastPath = _lastPath;
  setWindowTitle("Добавить документ");
  setModal(true);
  auto *lt = new QGridLayout();
  auto *nameLbl = new QLabel("Название документа:", this);
  lt->addWidget(nameLbl, 0, 0);
  nameLine = new QLineEdit(this);
  lt->addWidget(nameLine, 0, 1);
  auto *pathLbl = new QLabel("Путь до документа:", this);
  lt->addWidget(pathLbl, 1, 0);
  pathBtn = new QPushButton("Выбрать файл...", this);
  connect(pathBtn, &QPushButton::clicked, this, &AddDocumentDialog::selectFile);
  lt->addWidget(pathBtn, 1, 1);
  auto *colorLbl = new QLabel("Подсветка документа:", this);
  lt->addWidget(colorLbl, 2, 0);
  colorBox = new QComboBox(this);
  colorBox->addItems(bgColors);
  lt->addWidget(colorBox, 2, 1);
  auto *textColorLbl = new QLabel("Цвет названия документа:", this);
  lt->addWidget(textColorLbl, 3, 0);
  tcolorBox = new QComboBox(this);
  tcolorBox->addItems(textColors);
  tcolorBox->setCurrentIndex(1);
  lt->addWidget(tcolorBox, 3, 1);
  auto *hdoeLbl = new QLabel("Есть ли дата автоархивации?", this);
  lt->addWidget(hdoeLbl, 4, 0);
  hasExpireDate = new QCheckBox(this);
  hasExpireDate->setEnabled(false);
  lt->addWidget(hasExpireDate, 4, 1);
  auto *doeLbl = new QLabel("Дата автоархивации:", this);
  lt->addWidget(doeLbl, 5, 0);
  dateOfExpire = new QDateTimeEdit(this);
  dateOfExpire->setEnabled(false);
  lt->addWidget(dateOfExpire, 5, 1);
  connect(hasExpireDate, &QCheckBox::stateChanged, dateOfExpire,
          &QDateTimeEdit::setEnabled);
  auto *okBtn = new QPushButton(this);
  okBtn->setText("Добавить");
  connect(okBtn, &QPushButton::clicked, this, [this]() {
    if (docPath.isEmpty() or nameLine->text().isEmpty())
      return;
    emit sendResult(getDocument());
    close();
  });
  lt->addWidget(okBtn, 6, 0);
  auto *cancelBtn = new QPushButton(this);
  cancelBtn->setText("Отменить");
  connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
  lt->addWidget(cancelBtn, 6, 1);
  setLayout(lt);
}

void AddDocumentDialog::selectFile() {
  docPath = QFileDialog::getOpenFileName(
      this, "Открыть программу",
      lastPath.isEmpty() ? QDir::homePath() : lastPath, fileDialogDocsString);
  lastPath = docPath;
  lastPath.truncate(lastPath.lastIndexOf(QDir::separator()));
  emit sendLastDir(lastPath);
  pathBtn->setText(QFontMetrics(pathBtn->font())
                       .elidedText(docPath, Qt::ElideLeft, pathBtn->width()));
  if (nameLine->text().isEmpty())
    nameLine->setText(docPath.split("/").last().split(".").first());
}

Document *AddDocumentDialog::getDocument() {
  Document *doc = new Document;
  doc->addingDateTime = QDateTime::currentDateTime();
  doc->backgroundColor = getColorFromIndex(colorBox->currentIndex());
  doc->hasDateOfExpire = hasExpireDate->isChecked();
  if (doc->hasDateOfExpire)
    doc->expiringDateTime = dateOfExpire->dateTime();
  doc->filePath = docPath;
  doc->name = nameLine->text();
  doc->textColor = getColorFromIndex(tcolorBox->currentIndex());
  return doc;
}
