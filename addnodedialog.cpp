#include "addnodedialog.h"

AddNodeDialog::AddNodeDialog(Data *_data) {
  data = _data;
  setWindowTitle("Добавить раздел");
  auto *lt = new QGridLayout();
  auto *nodeNameLbl = new QLabel(this);
  nodeNameLbl->setText("Название раздела: ");
  lt->addWidget(nodeNameLbl, 0, 0);
  auto *nodeNameLine = new QLineEdit(this);
  lt->addWidget(nodeNameLine, 0, 1);
  auto *bgLbl = new QLabel(this);
  bgLbl->setText("Цвет раздела: ");
  lt->addWidget(bgLbl, 1, 0);
  auto *bgColorComboBox = new QComboBox(this);
  bgColorComboBox->addItems(bgColors);
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
//  brick->brickUUID =
}
