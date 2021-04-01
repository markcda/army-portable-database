#include "addnodedialog.h"

AddNodeDialog::AddNodeDialog(Data *_data, QWidget *parent) : QDialog(parent) {
  data = _data;
  setModal(true);
  setWindowTitle("Добавить раздел");
  auto *lt = new QGridLayout();
  auto *nodeNameLbl = new QLabel(this);
  nodeNameLbl->setText("Название раздела: ");
  lt->addWidget(nodeNameLbl, 0, 0);
  nodeNameLine = new QLineEdit(this);
  lt->addWidget(nodeNameLine, 0, 1);
  auto *bgLbl = new QLabel(this);
  bgLbl->setText("Цвет раздела: ");
  lt->addWidget(bgLbl, 1, 0);
  bgColorComboBox = new QComboBox(this);
  bgColorComboBox->addItems(bgColors);
  lt->addWidget(bgColorComboBox, 1, 1);
  auto *textLbl = new QLabel(this);
  textLbl->setText("Цвет текста в разделе: ");
  lt->addWidget(textLbl, 2, 0);
  textColorComboBox = new QComboBox(this);
  textColorComboBox->addItems(textColors);
  textColorComboBox->setCurrentIndex(1);
  lt->addWidget(textColorComboBox, 2, 1);
  auto *okBtn = new QPushButton(this);
  okBtn->setText("Добавить");
  connect(okBtn, &QPushButton::clicked, this, [this]() {
    emit sendResult(getDataBrick());
    close();
  });
  lt->addWidget(okBtn, 3, 0);
  auto *cancelBtn = new QPushButton(this);
  cancelBtn->setText("Отменить");
  connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
  lt->addWidget(cancelBtn, 3, 1);
  setLayout(lt);
}

DataBrick *AddNodeDialog::getDataBrick() {
  auto *brick = new DataBrick;
  brick->brickUUID = data->db->generateUUID();
  brick->name = nodeNameLine->text();
  brick->textColor = getColorFromIndex(textColorComboBox->currentIndex());
  brick->brickColor = getColorFromIndex(bgColorComboBox->currentIndex());
  return brick;
}
