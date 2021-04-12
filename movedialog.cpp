#include "movedialog.h"

MoveDialog::MoveDialog(Data *_data, Document *curr, DataBrick *brickParent,
                       QWidget *parent)
    : QDialog(parent) {
  data = _data;
  setModal(true);
  auto *glt = new QGridLayout();
  tree = new QTreeWidget(this);
  tree->setColumnCount(1);
  tree->setHeaderLabel("База данных");
  tree->addTopLevelItem(
      getItemFromDataBrick(data->db->getRootDataBrick(), nullptr));
  glt->addWidget(tree, 0, 0, 2, 1);
  auto *okBtn = new QPushButton("Переместить", this);
  connect(okBtn, &QPushButton::clicked, this,
          [this, curr, brickParent]() { move(curr, brickParent); });
  glt->addWidget(okBtn, 1, 0);
  auto *cancelBtn = new QPushButton("Отмена", this);
  connect(cancelBtn, &QPushButton::clicked, this, &MoveDialog::reject);
  glt->addWidget(cancelBtn, 1, 1);
  setLayout(glt);
}

MoveDialog::MoveDialog(Data *_data, DataBrick *curr, DataBrick *brickParent,
                       QWidget *parent)
    : QDialog(parent) {
  data = _data;
  setModal(true);
  auto *glt = new QGridLayout();
  tree = new QTreeWidget(this);
  tree->setColumnCount(1);
  tree->setHeaderLabel("База данных");
  tree->addTopLevelItem(
      getItemFromDataBrick(data->db->getRootDataBrick(), curr));
  glt->addWidget(tree, 0, 0, 0, 1);
  auto *okBtn = new QPushButton("Переместить", this);
  connect(okBtn, &QPushButton::clicked, this,
          [this, curr, brickParent]() { move(curr, brickParent); });
  glt->addWidget(okBtn, 1, 0);
  auto *cancelBtn = new QPushButton("Отмена", this);
  connect(cancelBtn, &QPushButton::clicked, this, &MoveDialog::reject);
  glt->addWidget(cancelBtn, 1, 1);
  setLayout(glt);
}

QTreeWidgetItem *MoveDialog::getItemFromDataBrick(DataBrick *dataBrick,
                                                  DataBrick *curr) {
  if (dataBrick == curr)
    return nullptr;
  auto *treeWidgetItem = new QTreeWidgetItem();
  for (auto *childBrick : dataBrick->brickNodes) {
    auto *item = getItemFromDataBrick(childBrick, curr);
    if (item)
      treeWidgetItem->addChild(item);
  }
  treeWidgetItem->setText(0, dataBrick->name);
  treeWidgetItem->setText(1, "node");
  treeWidgetItem->setText(2, dataBrick->brickUUID.toString());
  return treeWidgetItem;
}

void MoveDialog::move(DataBrick *dataBrick, DataBrick *parent) {
  QList<QTreeWidgetItem *> selectedItems = tree->selectedItems();
  if (not selectedItems.length()) {
    reject();
    return;
  } else if (selectedItems.length() != 1)
    return;
  else if (selectedItems.at(0)->text(1) != "node")
    return;
  for (int i = 0; i < parent->brickNodes.length(); i++)
    if (parent->brickNodes.at(i) == dataBrick) {
      parent->brickNodes.removeAt(i);
      break;
    }
  auto *newParent =
      getNewParent(selectedItems.first(), data->db->getRootDataBrick());
  if (not newParent)
    return;
  newParent->brickNodes.append(dataBrick);
  dataBrick->parent = newParent;
  accept();
}

void MoveDialog::move(Document *doc, DataBrick *parent) {
  QList<QTreeWidgetItem *> selectedItems = tree->selectedItems();
  if (not selectedItems.length()) {
    reject();
    return;
  } else if (selectedItems.length() != 1)
    return;
  else if (selectedItems.at(0)->text(1) != "node")
    return;
  for (int i = 0; i < parent->brickDocuments.length(); i++)
    if (parent->brickDocuments.at(i) == doc) {
      parent->brickDocuments.removeAt(i);
      break;
    }
  auto *newParent =
      getNewParent(selectedItems.first(), data->db->getRootDataBrick());
  if (not newParent)
    return;
  newParent->brickDocuments.append(doc);
  accept();
}

DataBrick *MoveDialog::getNewParent(QTreeWidgetItem *our, DataBrick *curr) {
  if ((our->text(0) == curr->name) and
      (our->text(2) == curr->brickUUID.toString()))
    return curr;
  for (int i = 0; i < curr->brickNodes.length(); i++) {
    auto *par = getNewParent(our, curr->brickNodes.at(i));
    if (par)
      return par;
  }
  return nullptr;
}
