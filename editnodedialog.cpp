#include "editnodedialog.h"

EditNodeDialog::EditNodeDialog(DataBrick *_dataBrick, QWidget *parent) : QDialog(parent) {
  dataBrick = _dataBrick;
  setModal(true);
  setWindowTitle("Изменить раздел");
  auto *lt = new QGridLayout();
  auto *nodeNameLbl = new QLabel(this);
  nodeNameLbl->setText("Название раздела: ");
  lt->addWidget(nodeNameLbl, 0, 0);
  nodeNameLine = new QLineEdit(this);
  nodeNameLine->setText(dataBrick->name);
  lt->addWidget(nodeNameLine, 0, 1);
  auto *bgLbl = new QLabel(this);
  bgLbl->setText("Цвет раздела: ");
  lt->addWidget(bgLbl, 1, 0);
  bgColorComboBox = new QComboBox(this);
  bgColorComboBox->addItems(bgColors);
  bgColorComboBox->setCurrentIndex(getIndexFromColor(dataBrick->brickColor));
  lt->addWidget(bgColorComboBox, 1, 1);
  auto *textLbl = new QLabel(this);
  textLbl->setText("Цвет текста в разделе: ");
  lt->addWidget(textLbl, 2, 0);
  textColorComboBox = new QComboBox(this);
  textColorComboBox->addItems(textColors);
  textColorComboBox->setCurrentIndex(getIndexFromColor(dataBrick->textColor));
  lt->addWidget(textColorComboBox, 2, 1);
  auto *okBtn = new QPushButton(this);
  okBtn->setText("Сохранить");
  connect(okBtn, &QPushButton::clicked, this, [this]() {
    if (nodeNameLine->text().isEmpty())
      return;
    save();
    emit edited();
    close();
  });
  lt->addWidget(okBtn, 3, 0);
  auto *cancelBtn = new QPushButton(this);
  cancelBtn->setText("Отменить");
  connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
  lt->addWidget(cancelBtn, 3, 1);
  setLayout(lt);
}

void EditNodeDialog::save() {
  dataBrick->brickColor = getColorFromIndex(bgColorComboBox->currentIndex());
  dataBrick->textColor = getColorFromIndex(textColorComboBox->currentIndex());
  dataBrick->name = nodeNameLine->text();
}
