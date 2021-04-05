#include "firsttimesetupdialog.h"

FirstTimeSetupDialog::FirstTimeSetupDialog(Data *_data, QWidget *parent)
    : QDialog(parent) {
  data = _data;
  // TODO пути для Word, Visio, PowerPoint и Excel
  setWindowTitle("Первый запуск");
  setModal(true);
  auto *vblt = new QVBoxLayout();
  auto *group = new QGroupBox(this);
  group->setTitle("Пути к приложениям Microsoft Office");
  auto *glt = new QGridLayout();
  // 1
  auto *wordLbl = new QLabel(this);
  wordLbl->setText("Путь до Microsoft Word:");
  wordLbl->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  glt->addWidget(wordLbl, 0, 0);
  wordBtn = new QPushButton(this);
  wordBtn->setText(noPath);
  connect(wordBtn, &QPushButton::clicked, this,
          &FirstTimeSetupDialog::updateAcceptEnabled);
  connect(wordBtn, &QPushButton::clicked, this, [this]() {
    MSWord = getPath(DocumentType::MSWord);
    wordBtn->setText(QFontMetrics(wordBtn->font())
                         .elidedText(MSWord, Qt::ElideLeft, wordBtn->width()));
  });
  glt->addWidget(wordBtn, 0, 1);
  // 2
  auto *excelLbl = new QLabel(this);
  excelLbl->setText("Путь до Microsoft Excel:");
  excelLbl->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  glt->addWidget(excelLbl, 1, 0);
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
  glt->addWidget(excelBtn, 1, 1);
  // 3
  auto *ppLbl = new QLabel(this);
  ppLbl->setText("Путь до Microsoft PowerPoint:");
  ppLbl->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  glt->addWidget(ppLbl, 2, 0);
  ppBtn = new QPushButton(this);
  ppBtn->setText(noPath);
  connect(ppBtn, &QPushButton::clicked, this,
          &FirstTimeSetupDialog::updateAcceptEnabled);
  connect(ppBtn, &QPushButton::clicked, this, [this]() {
    MSPP = getPath(DocumentType::MSPowerPoint);
    ppBtn->setText(QFontMetrics(ppBtn->font())
                       .elidedText(MSPP, Qt::ElideLeft, ppBtn->width()));
  });
  glt->addWidget(ppBtn, 2, 1);
  // 4
  auto *visioLbl = new QLabel(this);
  visioLbl->setText("Путь до Microsoft Visio (необязательно):");
  visioLbl->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  glt->addWidget(visioLbl, 3, 0);
  visioBtn = new QPushButton(this);
  visioBtn->setText(noPath);
  connect(visioBtn, &QPushButton::clicked, this,
          &FirstTimeSetupDialog::updateAcceptEnabled);
  connect(visioBtn, &QPushButton::clicked, this, [this]() {
    MSVisio = getPath(DocumentType::MSVisio);
    visioBtn->setText(
        QFontMetrics(visioBtn->font())
            .elidedText(MSVisio, Qt::ElideLeft, visioBtn->width()));
  });
  glt->addWidget(visioBtn, 3, 1);
  // Конец
  group->setLayout(glt);
  vblt->addWidget(group);
  acceptBtn = new QPushButton("Сохранить", this);
  connect(acceptBtn, &QPushButton::clicked, this,
          &FirstTimeSetupDialog::saveAll);
  vblt->addWidget(acceptBtn);
  setLayout(vblt);
}

void FirstTimeSetupDialog::updateAcceptEnabled() {
  if (MSWord.isEmpty() or MSPP.isEmpty() or MSExcel.isEmpty())
    acceptBtn->setEnabled(false);
  else
    acceptBtn->setEnabled(true);
}

QString FirstTimeSetupDialog::getPath(DocumentType docType) {
  QString fileExt;
  switch (docType) {
  case DocumentType::MSWord: {
    fileExt = "Программа MS Word (*.exe)";
    break;
  }
  case DocumentType::MSExcel: {
    fileExt = "Программа MS Excel (*.exe)";
    break;
  }
  case DocumentType::MSVisio: {
    fileExt = "Программа MS Visio (*.exe)";
    break;
  }
  case DocumentType::MSPowerPoint: {
    fileExt = "Программа MS PowerPoint (*.exe)";
    break;
  }
  }
  return QFileDialog::getOpenFileName(this, "Открыть программу", "", fileExt);
}

void FirstTimeSetupDialog::saveAll() {
  data->st->setValue(data->wordPath, MSWord);
  data->st->setValue(data->excelPath, MSExcel);
  data->st->setValue(data->pptPath, MSPP);
  data->st->setValue(data->visioPath, MSVisio);
  close();
}
