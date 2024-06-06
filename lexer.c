#include<ctype.h>
#include<stdlib.h>
#include"mylispc.h"
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

static It lexerStr(zltString *dest, FILE *err, const Pos *pos, int quot, It it, It end);
static It consumeRaw(It it, It end);

It mylispcLexer(int *token, double *numval, zltString *strval, FILE *err, const Pos *pos, It it, It end) {
  if (it == end) {
    *token = MYLISPC_EOF_TOKEN;
    return end;
  }
  if (*it == '(') {
    *token = MYLISPC_LPAREN_TOKEN;
    return it + 1;
  }
  if (*it == ')') {
    *token = MYLISPC_RPAREN_TOKEN;
    return it + 1;
  }
  if (*it == '\'' || *it == '"') {
    It it1 = lexerStr(strval, err, pos, *it, it + 1, end);
    if (!it1) {
      return NULL;
    }
    *token = MYLISPC_STR_TOKEN;
    return it1;
  }
  It it1 = consumeRaw(it, end);
  zltString raw = zltStrMakeBE(it, it1);
  *token = mylispcRawToken(numval, raw);
  return it1;
}

static It lexerStr1(zltStack *dest, FILE *err, const Pos *pos, int quot, It it, It end);

It lexerStr(zltString *dest, FILE *err, const Pos *pos, int quot, It it, It end) {
  zltStack k = zltStackMake(malloc(64), 64);
  It it1 = lexerStr1(&k, err, pos, *it, it + 1, end);
  if (!it1) {
    goto A;
  }
  size_t size = k.top - k.data;
  *dest = zltStrMake((const char *) realloc(k.data, size), size);
  return it1;
  A:
  free(k.data);
  return NULL;
}

static It escapedChar(char *dest, It it, It end);

It lexerStr1(zltStack *dest, FILE *err, const Pos *pos, int quot, It it, It end) {
  if (it == end) {
    mylispcReportBad(err, MYLISPC_UNTERMINATED_STR_FATAL, pos);
    return NULL;
  }
  if (*it == quot) {
    return it + 1;
  }
  if (*it == '\\') {
    char c;
    It it1 = escapedChar(c, it + 1, end);
    zltStackPush(dest, &c, 1);
    return lexerStr1(dest, err, pos, quot, it1, end);
  }
  zltStackPush(dest, it, 1);
  return lexerStr1(dest, err, pos, quot, it + 1, end);
}

static It escapedChar8(char *dest, It it, It end, size_t limit);

It escapedChar(char *dest, It it, It end) {
  if (it == end) {
    goto A;
  }
  if (*it == '"' || *it == '\'' || *it == '\\') {
    *dest = *it;
    return it + 1;
  }
  if (*it == 'n') {
    *dest = '\n';
    return it + 1;
  }
  if (*it == 'r') {
    *dest = '\r';
    return it + 1;
  }
  if (*it == 't') {
    *dest = '\t';
    return it + 1;
  }
  if (*it >= '0' && *it <= '3') {
    return escapedChar8(dest, it, end, 3);
  }
  if (*it >= '4' && *it <= '7') {
    return escapedChar8(dest, it, end, 2);
  }
  if (*it == 'x' && end - it > 3) {
    int x = zltIsBasedDigitChar(it[1], 16);
    if (x < 0) {
      goto A;
    }
    int y = zltIsBasedDigitChar(it[2], 16);
    if (y < 0) {
      goto A;
    }
    *dest = (x << 4) | y;
    return it + 3;
  }
  A:
  *dest = '\\';
  return end;
}

It escapedChar8(char *dest, It it, It end, size_t limit) {
  *dest = 0;
  int i = 0;
  for (; it != end && i < limit; ++it, ++i) {
    *dest = (*dest << 3) | (*it - '0');
  }
  return it + i;
}
