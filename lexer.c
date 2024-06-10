#include<ctype.h>
#include<stdlib.h>
#include"mylispc.h"
#include"token.h"
#include"zlt/stack.h"

typedef const char *It;
typedef mylispcPos Pos;

static It lineComment(Pos *pos, It it, It end);

It mylispcHit(Pos *pos, It it, It end) {
  if (it == end) {
    return end;
  }
  if (*it == '\n') {
    ++pos->li;
    return mylispcHit(pos, it + 1, end);
  }
  if (*it == ';') {
    return lineComment(pos, it + 1, end);
  }
  if (isspace(*it)) {
    return mylispcHit(pos, it + 1, end);
  }
  return it;
}

It lineComment(Pos *pos, It it, It end) {
  if (it == end) {
    return end;
  }
  if (it == '\n') {
    return mylispcHit(pos, it, end);
  }
  return lineComment(pos, it + 1, end);
}

typedef mylispcLexerDest Dest;

static It lexerStr(zltString *dest, FILE *err, const Pos *pos, int quot, It it, It end);
static It consumeRaw(It it, It end);

It mylispcLexer(Dest *dest, FILE *err, const Pos *pos, It it, It end) {
  if (it == end) {
    dest->token = MYLISPC_EOF_TOKEN;
    return end;
  }
  if (*it == '(') {
    dest->token = MYLISPC_LPAREN_TOKEN;
    return it + 1;
  }
  if (*it == ')') {
    dest->token = MYLISPC_RPAREN_TOKEN;
    return it + 1;
  }
  if (*it == '\'' || *it == '"') {
    It it1 = lexerStr(&dest->strval, err, pos, *it, it + 1, end);
    if (!it1) {
      return zltInvPtr;
    }
    dest->token = MYLISPC_STR_TOKEN;
    return it1;
  }
  It it1 = consumeRaw(it, end);
  zltString raw = zltStrMakeBE(it, it1);
  dest->token = mylispcRawToken(&dest->numval, raw);
  if (dest->token != -1) {
    return it1;
  }
  mylispcReportBad(err, MYLISPC_UNRECOGNIZED_SYMBOL_FATAL, pos);
  return zltInvPtr;
}

static It lexerStr1(zltStack *dest, FILE *err, const Pos *pos, int quot, It it, It end);

It lexerStr(zltString *dest, FILE *err, const Pos *pos, int quot, It it, It end) {
  zltStack k;
  k.data = malloc(64);
  if (!k.data) {
    mylispcReportBad(err, MYLISPC_OOM_FATAL, pos);
    return zltInvPtr;
  }
  k.size = 64;
  It it1 = lexerStr1(&k, err, pos, *it, it + 1, end);
  if (it1 == zltInvPtr) {
    goto A;
  }
  size_t size = k.top - k.data;
  *dest = zltStrMake((const char *) realloc(k.data, size), size);
  return it1;
  A:
  free(k.data);
  return zltInvPtr;
}

static size_t escapedChar(char *dest, It it, It end);

It lexerStr1(zltStack *dest, FILE *err, const Pos *pos, int quot, It it, It end) {
  if (it == end) {
    mylispcReportBad(err, MYLISPC_UNTERMINATED_STR_FATAL, pos);
    return zltInvPtr;
  }
  if (*it == quot) {
    return it + 1;
  }
  if (!dest->left && !zltStackRealloc(dest, (dest->top - dest->data) << 1)) {
    mylispcReportBad(err, MYLISPC_OOM_FATAL, pos);
    return zltInvPtr;
  }
  if (*it == '\\') {
    char c;
    size_t n = escapedChar(c, it + 1, end);
    zltStackPush(dest, &c, 1);
    return lexerStr1(dest, err, pos, quot, it + 1 + n, end);
  }
  zltStackPush(dest, it, 1);
  return lexerStr1(dest, err, pos, quot, it + 1, end);
}

static size_t escapedChar8(char *dest, It it, It end, size_t limit);

size_t escapedChar(char *dest, It it, It end) {
  if (it == end) {
    goto A;
  }
  if (*it == '"' || *it == '\'' || *it == '\\') {
    *dest = *it;
    return 1;
  }
  if (*it == 'n') {
    *dest = '\n';
    return 1;
  }
  if (*it == 'r') {
    *dest = '\r';
    return 1;
  }
  if (*it == 't') {
    *dest = '\t';
    return 1;
  }
  if (*it >= '0' && *it <= '3') {
    return escapedChar8(dest, it, end, 3);
  }
  if (*it >= '4' && *it <= '7') {
    return escapedChar8(dest, it, end, 2);
  }
  if (*it == 'x' && end - it > 3) {
    int x = zltIsDigitChar(it[1], 16);
    if (x < 0) {
      goto A;
    }
    int y = zltIsDigitChar(it[2], 16);
    if (y < 0) {
      goto A;
    }
    *dest = (x << 4) | y;
    return 3;
  }
  A:
  *dest = '\\';
  return 0;
}

size_t escapedChar8(char *dest, It it, It end, size_t limit) {
  *dest = 0;
  size_t n = 0;
  for (; it != end && n < limit; ++it, ++n) {
    *dest = (*dest << 3) | (*it - '0');
  }
  return n;
}

It mylispcLexer1(int *token, FILE *err, const Pos *pos, It it, It end) {
  Dest dest;
  It it1 = mylispcLexer(&dest, err, pos, it, end);
  if (it1 == zltInvPtr) {
    return zltInvPtr;
  }
  *token = dest.token;
  if (token == MYLISPC_STR_TOKEN) {
    free(dest.strval.data);
  }
  return token;
}
