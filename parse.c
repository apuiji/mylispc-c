#include"mylispc.h"
#include"nodes.h"
#include"token.h"

typedef mylispcParseContext Context;
typedef const char *It;
typedef mylispcPos Pos;

static It nodes(void **dest, Context *ctx, It end0, It end);

void mylispcParse(void **dest, Context *ctx, It it, It end) {
  It start1 = nodes(dest, ctx, it, end);
  if (start1 == zltInvPtr) {
    goto A;
  }
  int token1;
  mylispcLexer1(&token1, ctx.err, &ctx.pos, start1, end);
  if (token1 != MYLISPC_EOF_TOKEN) {
    goto A;
  }
  return 0;
  A:
  mylispcNodeClean(*dest);
  return -1;
}

static It node(void **dest, Context *ctx, It start0, It end);

It nodes(void **dest, Context *ctx, It end0, It end) {
  It start1 = mylispcHit(pos, end0, end);
  It end1 = node(dest, ctx, start1, end);
  if (end1 == zltInvPtr) {
    return zltInvPtr;
  }
  if (!end1) {
    return start1;
  }
  return nodes(&mylispcNodeMemb(*dest, next), ctx, end1, end);
}

It node(void **dest, Context *ctx, It start0, It end) {
  mylispcLexerDest ld0;
  It end0 = mylispcLexer(&ld0, ctx.err, &ctx.pos, start0, end);
  if (end0 == zltInvPtr) {
    return zltInvPtr;
  }
  if (ld0.token == MYLISPC_EOF_TOKEN) {
    return NULL;
  }
  if (ld0.token == MYLISPC_ID_TOKEN) {
    zltString s = zltStrClone(zltStrMakeBE(start0, end0));
    if (s.data == zltInvPtr) {
      goto A;
    }
    *dest = zltTypeAlloc(mylispcIDAtom);
    if (!*dest) {
      goto A;
    }
    zltPointTo(*dest, mylispcIDAtom) = mylispcIDAtomMake(pos, );
    goto A;
  }
  A:
  mylispcReportBad(err, MYLISPC_OOM_FATAL, pos);
  return zltInvPtr;
}
