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
  if (ld0.token == MYLISPC_EOF_TOKEN || ld0.token == MYLISPC_RPAREN_TOKEN) {
    return NULL;
  }
  const Pos *pos0 = mylispcAddPos(ctx.posTree, &ctx.pos);
  if (pos0 == zltInvPtr) {
    if (ld0.token == MYLISPC_STR_TOKEN) {
      goto B;
    } else {
      goto A;
    }
  }
  if (ld0.token == MYLISPC_ID_TOKEN) {
    zltString raw = mylispcAddStr(ctx->strTree, zltStrMakeBE(start0, end0));
    if (raw.data == zltInvPtr) {
      goto A;
    }
    *dest = zltTypeAlloc(mylispcIDAtom);
    if (!*dest) {
      goto A;
    }
    zltPointTo(*dest, mylispcIDAtom) = mylispcIDAtomMake(pos0, raw);
    return end0;
  }
  if (ld0.token == MYLISPC_NUM_TOKEN) {
    zltString raw = mylispcAddStr(ctx->strTree, zltStrMakeBE(start0, end0));
    if (raw.data == zltInvPtr) {
      goto A;
    }
    *dest = zltTypeAlloc(mylispcNumAtom);
    if (!*dest) {
      goto A;
    }
    zltPointTo(*dest, mylispcNumAtom) = mylispcNumAtomMake(pos0, raw, ld0.numval);
    return end0;
  }
  if (ld0.token == MYLISPC_STR_TOKEN) {
    zltString value = mylispcAddStr(ctx->strTree, ld0.strval);
    if (value.data == zltInvPtr) {
      goto B;
    }
    *dest = zltTypeAlloc(mylispcStrAtom);
    if (!*dest) {
      goto B;
    }
    zltPointTo(*dest, mylispcStrAtom) = mylispcStrAtomMake(pos0, value);
    return end0;
  }
  if (ld0.token == MYLISPC_LPAREN_TOKEN) {
    void *nodes1 = NULL;
    It start2 = nodes(&nodes1, ctx, end0, end);
    if (start2 == zltInvPtr) {
      goto AA;
    }
    int token2;
    It end2 = mylispcLexer1(&token2, ctx.err, &ctx.pos, start2, end);
    if (end2 == zltIntPtr) {
      goto AA;
    }
    if (token2 != MYLISPC_RPAREN_TOKEN) {
      mylispcReportBad(ctx.err, MYLISPC_UNTERMINATED_LIST_FATAL, pos0);
      goto AA;
    }
    AA:
    mylispcNodeClean(nodes1);
    goto A;
  }
  B:
  free(ld0.strval.data);
  A:
  mylispcReportBad(err, MYLISPC_OOM_FATAL, pos0);
  return zltInvPtr;
}
