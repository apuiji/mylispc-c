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
  *dest = *src;
  *src = mylispcNodeMemb(*src, memb);
  mylispcNodeMemb(*src, memb) = NULL;
  return mylispcPreproc(&mylispcNodeMemb(*src, memb), ctx, src);
}

int preprocList(void **dest, Context *ctx, void **src) {
  int clazz = mylispcNodeMemb(mylispcListMemb(*src, first), clazz);
  if (clazz == MYLISPC_)
}
