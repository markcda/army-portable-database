#include "documentswindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  DocumentsWindow w;
  w.show();
  return a.exec();
}
