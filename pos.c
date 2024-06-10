#include"pos.h"

int mylispcPosTreeCmpForFind(const void *pos, const void *tree) {
  const mylispcPos *pos1 = &mylispcPosTreeMemb(tree, pos);
  return zltStrCmp(pos->file, pos1->file) < 0 || pos->li < pos1->li || pos->prev < pos1->prev;
}

const mylispcPos *mylispcPosTreeInsert(void **tree, const mylispcPos *pos) {
  void *parent;
  void **ins = mylispcPosTreeFindForInsert(&parent, tree, pos);
  mylispcPosTree *node = zltPointTo(ins, mylispcPosTree *);
  if (node) {
    goto A;
  }
  node = zltTypeAlloc(mylispcPosTree);
  if (!node) {
    return zltInvPtr;
  }
  *node = mylispcPosTreeMake(parent, pos);
  *ins = node;
  zltRBTreeAfterInsert(tree, node);
  return &mylispcPosTreeMemb(node, pos);
}
