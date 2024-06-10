#ifndef MYLISPC_POS_H
#define MYLISPC_POS_H

#include"zlt/rbtree.h"
#include"zlt/string.h"

#include"zlt/ifcpp_begin.h"

typedef struct {
  zltString file;
  int li;
  const void *prev;
} mylispcPos;

#define mylispcPosMemb(p, m) zltMemb(p, mylispcPos, m)

static inline mylispcPos mylispcPosMake(zltString file, int li, const void *prev) {
  return (mylispcPos) { .file = file, .li = li, .prev = prev };
}

typedef struct {
  zltRBTree rbTree;
  mylispcPos pos;
} mylispcPosTree;

#define mylispcPosTreeMemb(p, m) zltMemb(p, mylispcPosTree, m)

static inline mylispcPosTree mylispcPosTreeMake(const void *parent, const mylispcPos *pos) {
  return (mylispcPosTree) { .rbTree = zltRBTreeMake(parent), .pos = *pos };
}

int mylispcPosTreeCmpForFind(const void *pos, const void *tree);

static inline void *mylispcPosTreeFind(const void *tree, const mylispcPos *pos) {
  return zltBiTreeFind(tree, mylispcPosTreeCmpForFind, pos);
}

static inline void **mylispcPosTreeFindForInsert(void **parent, void **tree, const mylispcPos *pos) {
  return zltBiTreeFindForInsert(parent, tree, mylispcPosTreeCmpForFind, pos);
}

const mylispcPos *mylispcPosTreeInsert(void **tree, const mylispcPos *pos);

#include"zlt/ifcpp_end.h"

#endif
