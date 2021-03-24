#include "firsttimesetupdialog.h"

FirstTimeSetupDialog::FirstTimeSetupDialog(Data *_data, QWidget *parent)
    : QDialog(parent) {
  data = _data;
  // TODO пути для Word, Visio, PowerPoint и Excel
  // 1
  auto *wordLbl = new QLabel(this);
  wordLbl->setText("Путь до Microsoft Word:");
  wordBtn = new QPushButton(this);
  wordBtn->setText(noPath);
  connect(wordBtn, &QPushButton::clicked, this,
          &FirstTimeSetupDialog::updateAcceptEnabled);
  connect(wordBtn, &QPushButton::clicked, this, [this]() {
    MSWord = getPath(DocumentType::MSWord);
    wordBtn->setText(QFontMetrics(wordBtn->font())
                         .elidedText(MSWord, Qt::ElideLeft, wordBtn->width()));
  });
  // 2
  auto *excelLbl = new QLabel(this);
  excelLbl->setText("Путь до Microsoft Excel:");
  excelBtn = new QPushButton(this);
  excelBtn->setText(noPath);
  connect(excelBtn, &QPushButton::clicked, this,
          &FirstTimeSetupDialog::updateAcceptEnabled);
  connect(excelBtn, &QPushButton::clicked, this, [this]() {
    MSExcel = getPath(DocumentType::MSExcel);
    excelBtn->setText(
        QFontMetrics(excelBtn->font())
            .elidedText(MSExcel, Qt::ElideLeft, excelBtn->width()));
  });
  // 3

  // 4

}

void FirstTimeSetupDialog::updateAcceptEnabled() {}
