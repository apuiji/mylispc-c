#include<math.h>
#include<string.h>
#include"token.h"

zltString mylispcTokenRaw(int token) {
  #define ifToken(t, raw) \
  if (token == MYLISPC_##t##_TOKEN) { \
    return zltStrMakeStatic(raw); \
  }
  #define ifKwd(kwd) ifToken(kwd, #kwd)
  ifKwd(callee);
  ifKwd(def);
  ifKwd(defer);
  ifKwd(forward);
  ifKwd(guard);
  ifKwd(if);
  ifKwd(length);
  ifKwd(return);
  ifKwd(throw);
  ifKwd(try);
  #undef ifKwd
  // preproc operations begin
  ifToken(POUND, "#");
  ifToken(POUND2, "##");
  ifToken(POUND_def, "#def");
  ifToken(POUND_if, "#if");
  ifToken(POUND_include, "#include");
  ifToken(POUND_movedef, "#movedef");
  ifToken(POUND_undef, "#undef");
  // preproc operations end
  ifToken(EXCLAM, "!");
  ifToken(PERCENT, "%");
  ifToken(AMP2, "&&");
  ifToken(AMP, "&");
  ifToken(LPAREN, "(");
  ifToken(RPAREN, ")");
  ifToken(ASTERISK2, "**");
  ifToken(ASTERISK, "*");
  ifToken(PLUS, "+");
  ifToken(COMMA, ",");
  ifToken(MINUS, "-");
  ifToken(DOT, ".");
  ifToken(SLASH, "/");
  ifToken(LT2, "<<");
  ifToken(LT_EQ_GT, "<=>");
  ifToken(LT_EQ, "<=");
  ifToken(LT, "<");
  ifToken(EQ2, "==");
  ifToken(EQ, "=");
  ifToken(GT_EQ, ">=");
  ifToken(GT3, ">>>");
  ifToken(GT2, ">>");
  ifToken(GT, ">");
  ifToken(AT, "@");
  ifToken(CARET2, "^^");
  ifToken(CARET, "^");
  ifToken(VERTICAL2, "||");
  ifToken(VERTICAL, "|");
  ifToken(TILDE, "~");
  #undef ifToken
  return zltStrMake(NULL, 0);
}

static bool isRawNum(zltString raw);
static zltString rawNum(double *dest, zltString raw);

int mylispcRawToken(double *numval, zltString raw) {
  if (isRawNum(raw)) {
    if (zltStrToDouble(numval, raw, rawNum).size) {
      return -1;
    }
    return MYLISPC_NUM_TOKEN;
  }
  #define ifRaw(r, t) \
  if (zltStrEqStatic(raw, r)) { \
    return MYLISPC_##t##_TOKEN; \
  }
  #define ifKwd(kwd) ifRaw(#kwd, kwd)
  ifKwd(callee);
  ifKwd(def);
  ifKwd(defer);
  ifKwd(forward);
  ifKwd(guard);
  ifKwd(if);
  ifKwd(length);
  ifKwd(return);
  ifKwd(throw);
  ifKwd(try);
  #undef ifKwd
  // preproc operations begin
  ifRaw("#", POUND);
  ifRaw("##", POUND2);
  ifRaw("#def", POUND_def);
  ifRaw("#if", POUND_if);
  ifRaw("#include", POUND_include);
  ifRaw("#movedef", POUND_movedef);
  ifRaw("#undef", POUND_undef);
  // preproc operations end
  ifRaw("!", EXCLAM);
  ifRaw("%", PERCENT);
  ifRaw("&&", AMP2);
  ifRaw("&", AMP);
  ifRaw("(", LPAREN);
  ifRaw(")", RPAREN);
  ifRaw("**", ASTERISK2);
  ifRaw("*", ASTERISK);
  ifRaw("+", PLUS);
  ifRaw(",", COMMA);
  ifRaw("-", MINUS);
  ifRaw(".", DOT);
  ifRaw("/", SLASH);
  ifRaw("<<", LT2);
  ifRaw("<=>", LT_EQ_GT);
  ifRaw("<=", LT_EQ);
  ifRaw("<", LT);
  ifRaw("==", EQ2);
  ifRaw("=", EQ);
  ifRaw(">=", GT_EQ);
  ifRaw(">>>", GT3);
  ifRaw(">>", GT2);
  ifRaw(">", GT);
  ifRaw("^^", CARET2);
  ifRaw("^", CARET);
  ifRaw("||", VERTICAL2);
  ifRaw("|", VERTICAL);
  ifRaw("~", TILDE);
  #undef ifRaw
  return MYLISPC_ID_TOKEN;
}

static bool isRawNum1(zltString raw);

bool isRawNum(zltString raw) {
  if (*raw.data == '+' || *raw.data == '-') {
    return isRawNum1(zltStrForward(raw, 1));
  }
  return isRawNum1(raw);
}

bool isRawNum1(zltString raw) {
  return raw.size && isdigit(*raw.data);
}

static bool rawBasedInt(unsigned long *dest, zltString raw);

zltString rawNum(double *dest, zltString raw) {
  unsigned long ul;
  if (rawBasedInt(&ul, raw)) {
    *dest = ul;
    goto A;
  }
  if (zltStrEqStatic(raw, "NaN")) {
    *dest = NAN;
    goto A;
  }
  if (zltStrEqStatic(raw, "infinity")) {
    *dest = INFINITY;
    goto A;
  }
  return zltStrToUDouble(dest, raw);
  A:
  return zltStrForward(raw, raw.size);
}

bool rawBasedInt(unsigned long *dest, zltString raw) {
  if (!(raw.size > 2 && *raw.data == '0')) {
    return false;
  }
  int c = raw.data[1];
  size_t base;
  if (c == 'B' || c == 'b') {
    base = 2;
  } else if (c == 'Q' || c == 'q') {
    base = 4;
  } else if (c == 'O' || c == 'o') {
    base = 8;
  } else if (c == 'X' || c == 'x') {
    base = 16;
  } else {
    return false;
  }
  return !zltStrToULong(dest, raw, base).size;
}
