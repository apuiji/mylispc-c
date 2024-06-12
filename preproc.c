#include"mylispc.h"
#include"token.h"

typedef mylispcPos Pos;
typedef mylispcPreprocContext Context;

static int preprocList(void **dest, Context *ctx, void **src);

int mylispcPreproc(void **dest, Context *ctx, void **src) {
  if (!*src) {
    return 0;
  }
  if (mylispcNodeMemb(*src, clazz) == MYLISPC_LIST_CLASS && mylispcListMemb(*src, first)) {
    return preprocList(dest, ctx, src);
  }
  void **next = zltLinkPush(dest, zltLinkPop(src));
  return mylispcPreproc(next, ctx, src);
}

static int preprocListID(void **dest, Context *ctx, void **src, zltString id);
static int preprocListToken(void **dest, Context *ctx, void **src, int token);
static int preprocList1(void **dest, Context *ctx, void **src);

int preprocList(void **dest, Context *ctx, void **src) {
  void *first = mylispcListMemb(*src, first);
  int clazz = mylispcNodeMemb(first, clazz);
  if (clazz == MYLISPC_ID_ATOM_CLASS) {
    return preprocListID(dest, ctx, src, mylispcIDAtomMemb(first, raw));
  }
  if (clazz == MYLISPC_TOKEN_ATOM_CLASS) {
    return preprocListToken(dest, ctx, src, mylispcTokenAtomMemb(first, token));
  }
  return preprocList1(dest, ctx, src);
}

int preprocList1(void **dest, Context *ctx, void **src) {
  void *first = NULL;
  if (mylispcPreproc(&first, ctx, mylispcListMemb(*src, first))) {
    goto A;
  }
  mylispcListMemb(*src, first) = first;
  void **next = zltLinkPush(dest, zltLinkPop(src));
  return mylispcPreproc(next, ctx, src);
  A:
  mylispcNodeClean(first);
  return -1;
}
