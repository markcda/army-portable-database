#ifndef DATABASE_H
#define DATABASE_H

#include "docs/database.h"

struct DataBase {
  DataBrick *rootBrick = nullptr;
  DataBrick *archiveBrick = nullptr;
};

#endif
