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
  mylispcLexer1(&token1, ctx->err, &ctx->pos, start1, end);
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

static bool idAtom(void **dest, Context *ctx, const Pos *pos, zltString raw);
static bool numAtom(void **dest, Context *ctx, const Pos *pos, zltString raw, double value);
static bool strAtom(void **dest, Context *ctx, const Pos *pos, zltString value);
static It list(void **dest, Context *ctx, const Pos *pos, It end0, It end);
static bool tokenAtom(void **dest, Context *ctx, const Pos *pos, int token);

It node(void **dest, Context *ctx, It start0, It end) {
  mylispcLexerDest ld0;
  It end0 = mylispcLexer(&ld0, ctx->err, &ctx->pos, start0, end);
  if (end0 == zltInvPtr) {
    return zltInvPtr;
  }
  if (ld0.token == MYLISPC_EOF_TOKEN || ld0.token == MYLISPC_RPAREN_TOKEN) {
    return NULL;
  }
  const Pos *pos0 = mylispcAddPos(ctx->posTree, &ctx->pos);
  if (pos0 == zltInvPtr) {
    mylispcReportBad(ctx->err, MYLISPC_OOM_FATAL, &ctx->pos);
    return zltInvPtr;
  }
  if (ld0.token == MYLISPC_ID_TOKEN) {
    if (!idAtom(dest, ctx, pos0, zltStrMakeBE(start0, end0))) {
      return zltInvPtr;
    }
    return end0;
  }
  if (ld0.token == MYLISPC_NUM_TOKEN) {
    if (!numAtom(dest, ctx, pos0, zltStrMakeBE(start0, end0), ld0.numval)) {
      return zltInvPtr;
    }
    return end0;
  }
  if (ld0.token == MYLISPC_STR_TOKEN) {
    if (!strAtom(dest, ctx, pos0, ld0.strval)) {
      return zltInvPtr;
    }
    return end0;
  }
  if (ld0.token == MYLISPC_LPAREN_TOKEN) {
    It end1 = list(dest, ctx, pos0, end0, end);
    if (end1 == zltInvPtr) {
      return zltInvPtr;
    }
    return end1;
  }
  return tokenAtom(dest, ctx, pos0, ld0.token) ? end0 : zltInvPtr;
}

bool idAtom(void **dest, Context *ctx, const Pos *pos, zltString raw) {
  const zltString *raw0 = mylispcAddStr(ctx->strTree, raw);
  if (raw0 == zltInvPtr) {
    goto OOM;
  }
  *dest = zltTypeAlloc(mylispcIDAtom);
  if (!*dest) {
    goto OOM;
  }
  zltPointTo(*dest, mylispcIDAtom) = mylispcIDAtomMake(pos, raw0);
  return true;
  OOM:
  mylispcReportBad(ctx->err, MYLISPC_OOM_FATAL, pos);
  return false;
}

bool numAtom(void **dest, Context *ctx, const Pos *pos, zltString raw, double value) {
  const zltString *raw0 = mylispcAddStr(ctx->strTree, raw);
  if (raw0 == zltInvPtr) {
    goto OOM;
  }
  *dest = zltTypeAlloc(mylispcNumAtom);
  if (!*dest) {
    goto OOM;
  }
  zltPointTo(*dest, mylispcNumAtom) = mylispcNumAtomMake(pos, raw0, value);
  return true;
  OOM:
  mylispcReportBad(ctx->err, MYLISPC_OOM_FATAL, pos);
  return false;
}

bool strAtom(void **dest, Context *ctx, const Pos *pos, zltString value) {
  const zltString *value0 = mylispcAddStr(ctx->strTree, value);
  if (value0 == zltInvPtr) {
    goto OOM;
  }
  *dest = zltTypeAlloc(mylispcStrAtom);
  if (!*dest) {
    goto OOM;
  }
  zltPointTo(*dest, mylispcStrAtom) = mylispcStrAtomMake(pos, value);
  return true;
  OOM:
  mylispcReportBad(ctx->err, MYLISPC_OOM_FATAL, pos);
  return false;
}

It list(void **dest, Context *ctx, const Pos *pos, It end0, It end) {
  void *prod1 = NULL;
  It start2 = nodes(&prod1, ctx, end0, end);
  if (start2 == zltInvPtr) {
    goto A;
  }
  int token2;
  It end2 = mylispcLexer1(&token2, ctx->err, &ctx->pos, start2, end);
  if (end2 == zltInvPtr) {
    mylispcReportBad(ctx->err, MYLISPC_UNTERMINATED_LIST_FATAL, pos);
    goto A;
  }
  *dest = zltTypeAlloc(mylispcList);
  if (!*dest) {
    mylispcReportBad(ctx->err, MYLISPC_OOM_FATAL, pos);
    goto A;
  }
  zltPointTo(*dest, mylispcList) = mylispcListMake(pos, prod1);
  return end2;
  A:
  mylispcNodeClean(prod1);
  return zltInvPtr;
}

bool tokenAtom(void **dest, Context *ctx, const Pos *pos, int token) {
  *dest = zltTypeAlloc(mylispcTokenAtom);
  if (!*dest) {
    mylispcReportBad(ctx->err, MYLISPC_OOM_FATAL, pos);
    return false;
  }
  zltPointTo(*dest, mylispcTokenAtom) = mylispcTokenAtomMake(pos, token);
  return true;
}
