#include "editdocumentdialog.h"

EditDocumentDialog::EditDocumentDialog(Document *_document, QWidget *parent)
    : QDialog(parent) {
  document = _document;
  setWindowTitle("Изменить документ");
  setModal(true);
  auto *lt = new QGridLayout();
  auto *nameLbl = new QLabel("Название документа:", this);
  lt->addWidget(nameLbl, 0, 0);
  nameLine = new QLineEdit(this);
  nameLine->setText(document->name);
  lt->addWidget(nameLine, 0, 1);
  auto *pathLbl = new QLabel("Путь до документа:", this);
  lt->addWidget(pathLbl, 1, 0);
  pathBtn = new QPushButton(this);
  pathBtn->setText(
      QFontMetrics(pathBtn->font())
          .elidedText(document->filePath, Qt::ElideLeft, pathBtn->width()));
  connect(pathBtn, &QPushButton::clicked, this,
          &EditDocumentDialog::selectFile);
  lt->addWidget(pathBtn, 1, 1);
  auto *colorLbl = new QLabel("Подсветка документа:", this);
  lt->addWidget(colorLbl, 2, 0);
  colorBox = new QComboBox(this);
  colorBox->addItems(bgColors);
  colorBox->setCurrentIndex(getIndexFromColor(document->backgroundColor));
  lt->addWidget(colorBox, 2, 1);
  auto *textColorLbl = new QLabel("Цвет названия документа:", this);
  lt->addWidget(textColorLbl, 3, 0);
  tcolorBox = new QComboBox(this);
  tcolorBox->addItems(textColors);
  tcolorBox->setCurrentIndex(getIndexFromColor(document->textColor));
  lt->addWidget(tcolorBox, 3, 1);
  auto *hdoeLbl = new QLabel("Есть ли дата автоархивации?", this);
  lt->addWidget(hdoeLbl, 4, 0);
  hasExpireDate = new QCheckBox(this);
  hasExpireDate->setEnabled(false);
  hasExpireDate->setChecked(document->hasDateOfExpire);
  lt->addWidget(hasExpireDate, 4, 1);
  auto *doeLbl = new QLabel("Дата автоархивации:", this);
  lt->addWidget(doeLbl, 5, 0);
  dateOfExpire = new QDateTimeEdit(this);
  dateOfExpire->setEnabled(hasExpireDate->isChecked());
  dateOfExpire->setDateTime(document->expiringDateTime);
  lt->addWidget(dateOfExpire, 5, 1);
  connect(hasExpireDate, &QCheckBox::stateChanged, dateOfExpire,
          &QDateTimeEdit::setEnabled);
  auto *okBtn = new QPushButton(this);
  okBtn->setText("Сохранить");
  connect(okBtn, &QPushButton::clicked, this, [this]() {
    if (document->filePath.isEmpty() or nameLine->text().isEmpty())
      return;
    edit();
    emit ready();
    close();
  });
  lt->addWidget(okBtn, 6, 0);
  auto *cancelBtn = new QPushButton(this);
  cancelBtn->setText("Отменить");
  connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
  lt->addWidget(cancelBtn, 6, 1);
  setLayout(lt);
}

void EditDocumentDialog::selectFile() {
  auto dirPath = document->filePath;
  dirPath.truncate(dirPath.lastIndexOf(QDir::separator()));
  document->filePath = QFileDialog::getOpenFileName(
      this, "Открыть программу", dirPath,
      "Документы (*.doc *.docx *.xls *.xlsx *.ppt *.pptx *.vsd *.rar *.tar.* "
      "*.zip *.7z *.pdf)");
  pathBtn->setText(
      QFontMetrics(pathBtn->font())
          .elidedText(document->filePath, Qt::ElideLeft, pathBtn->width()));
  if (nameLine->text().isEmpty())
    nameLine->setText(document->filePath.split("/").last().split(".").first());
}

void EditDocumentDialog::edit() {
  document->addingDateTime = QDateTime::currentDateTime();
  document->backgroundColor = getColorFromIndex(colorBox->currentIndex());
  document->hasDateOfExpire = hasExpireDate->isChecked();
  if (document->hasDateOfExpire)
    document->expiringDateTime = dateOfExpire->dateTime();
  document->name = nameLine->text();
  document->textColor = getColorFromIndex(tcolorBox->currentIndex());
}
