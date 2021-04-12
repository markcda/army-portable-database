#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include "data.h"
#include "docs/databrick.h"
#include "docs/document.h"
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class MoveDialog : public QDialog {
  Q_OBJECT
public:
  MoveDialog(Data *_data, Document *curr, DataBrick *brickParent,
             QWidget *parent = nullptr);
  MoveDialog(Data *_data, DataBrick *curr, DataBrick *brickParent,
             QWidget *parent = nullptr);

private:
  Data *data = nullptr;
  QTreeWidget *tree = nullptr;
  QTreeWidgetItem *getItemFromDataBrick(DataBrick *dataBrick, DataBrick *curr);
  void move(DataBrick *dataBrick, DataBrick *parent);
  void move(Document *doc, DataBrick *parent);
  DataBrick *getNewParent(QTreeWidgetItem *our, DataBrick *curr);
};

#endif
