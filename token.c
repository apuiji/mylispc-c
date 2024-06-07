#include<math.h>
#include<string.h>
#include"mylispc.h"

zltString mylispcTokenRaw(int token) {
  #define ifToken(t, raw) \
  if (token == MYLISPC_##t##_TOKEN) { \
    return zltStrMake(raw, sizeof(raw) - 1); \
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

static zltString rawNum(double *dest, zltString raw);
static int rawKwd(zltString raw);
static int rawPound(zltString raw);
static int rawToken1(int c);
static int rawToken2(const char *raw);
static int rawToken3(const char *raw);

int mylispcRawToken(double *numval, zltString raw) {
  if (!zltStrToDouble(numval, raw, rawNum).size) {
    return MYLISPC_NUM_TOKEN;
  }
  int i = rawKwd(raw);
  if (i > 0) {
    return i;
  }
  i = rawPound(raw);
  if (i > 0) {
    return i;
  }
  if (raw.size == 1) {
    i = rawToken1(*raw.data);
  } else if (raw.size == 2) {
    i = rawToken2(raw.data);
  } else if (raw.size == 3) {
    i = rawToken3(raw.data);
  }
  if (i > 0) {
    return i;
  }
  return MYLISPC_ID_TOKEN;
}

static bool rawBasedInt(unsigned long *dest, zltString raw);

zltString rawNum(double *dest, zltString raw) {
  unsigned long ul;
  if (rawBasedInt(&ul, raw)) {
    *dest = ul;
    goto A;
  }
  if (raw.size == 3 && !strncmp(raw.data, "NaN", 3)) {
    *dest = NAN;
    goto A;
  }
  if (raw.size == 8 && !strncmp(raw.data, "infinity", 8)) {
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

int rawKwd(zltString raw) {
  if (raw.size == 6 && !strncmp(raw.data, "callee", 6)) {
    return MYLISPC_callee_TOKEN;
  }
  if (raw.size == 3 && !strncmp(raw.data, "def", 3)) {
    return MYLISPC_def_TOKEN;
  }
  if (raw.size == 5 && !strncmp(raw.data, "defer", 5)) {
    return MYLISPC_defer_TOKEN;
  }
  if (raw.size == 7 && !strncmp(raw.data, "forward", 7)) {
    return MYLISPC_forward_TOKEN;
  }
  if (raw.size == 5 && !strncmp(raw.data, "guard", 5)) {
    return MYLISPC_guard_TOKEN;
  }
  if (raw.size == 2 && !strncmp(raw.data, "if", 2)) {
    return MYLISPC_if_TOKEN;
  }
  if (raw.size == 6 && !strncmp(raw.data, "length", 6)) {
    return MYLISPC_length_TOKEN;
  }
  if (raw.size == 6 && !strncmp(raw.data, "return", 6)) {
    return MYLISPC_return_TOKEN;
  }
  if (raw.size == 5 && !strncmp(raw.data, "throw", 5)) {
    return MYLISPC_throw_TOKEN;
  }
  if (raw.size == 3 && !strncmp(raw.data, "try", 3)) {
    return MYLISPC_try_TOKEN;
  }
  return 0;
}

int rawPound(zltString raw) {
  if (raw.size == 1 && *raw.data == '#') {
    return MYLISPC_POUND_TOKEN;
  }
  if (raw.size == 2 && !strncmp(raw.data, "##", 2)) {
    return MYLISPC_POUND2_TOKEN;
  }
  if (raw.size == 4 && !strncmp(raw.data, "#def", 4)) {
    return MYLISPC_POUND_def_TOKEN;
  }
  if (raw.size == 3 && !strncmp(raw.data, "#if", 3)) {
    return MYLISPC_POUND_if_TOKEN;
  }
  if (raw.size == 8 && !strncmp(raw.data, "#include", 8)) {
    return MYLISPC_POUND_include_TOKEN;
  }
  if (raw.size == 8 && !strncmp(raw.data, "#movedef", 8)) {
    return MYLISPC_POUND_movedef_TOKEN;
  }
  if (raw.size == 6 && !strncmp(raw.data, "#undef", 6)) {
    return MYLISPC_POUND_undef_TOKEN;
  }
  return 0;
}

int rawToken1(int c) {
  if (c == '!') {
    return MYLISPC_EXCLAM_TOKEN;
  }
  if (c == '%') {
    return MYLISPC_PERCENT_TOKEN;
  }
  if (c == '&') {
    return MYLISPC_AMP_TOKEN;
  }
  if (c == '(') {
    return MYLISPC_LPAREN_TOKEN;
  }
  if (c == ')') {
    return MYLISPC_RPAREN_TOKEN;
  }
  if (c == '*') {
    return MYLISPC_ASTERISK_TOKEN;
  }
  if (c == '+') {
    return MYLISPC_PLUS_TOKEN;
  }
  if (c == ',') {
    return MYLISPC_COMMA_TOKEN;
  }
  if (c == '-') {
    return MYLISPC_MINUS_TOKEN;
  }
  if (c == '.') {
    return MYLISPC_DOT_TOKEN;
  }
  if (c == '/') {
    return MYLISPC_SLASH_TOKEN;
  }
  if (c == '<') {
    return MYLISPC_LT_TOKEN;
  }
  if (c == '=') {
    return MYLISPC_EQ_TOKEN;
  }
  if (c == '>') {
    return MYLISPC_GT_TOKEN;
  }
  if (c == '@') {
    return MYLISPC_AT_TOKEN;
  }
  if (c == '^') {
    return MYLISPC_CARET_TOKEN;
  }
  if (c == '|') {
    return MYLISPC_VERTICAL_TOKEN;
  }
  if (c == '~') {
    return MYLISPC_TILDE_TOKEN;
  }
  return 0;
}

int rawToken2(const char *raw) {
  if (!strncmp(raw, "&&", 2)) {
    return MYLISPC_AMP2_TOKEN;
  }
  if (!strncmp(raw, "**", 2)) {
    return MYLISPC_ASTERISK2_TOKEN;
  }
  if (!strncmp(raw, "<<", 2)) {
    return MYLISPC_LT2_TOKEN;
  }
  if (!strncmp(raw, "<=", 2)) {
    return MYLISPC_LT_EQ_TOKEN;
  }
  if (!strncmp(raw, "==", 2)) {
    return MYLISPC_EQ2_TOKEN;
  }
  if (!strncmp(raw, ">=", 2)) {
    return MYLISPC_GT_EQ_TOKEN;
  }
  if (!strncmp(raw, ">>", 2)) {
    return MYLISPC_GT2_TOKEN;
  }
  if (!strncmp(raw, "^^", 2)) {
    return MYLISPC_CARET2_TOKEN;
  }
  if (!strncmp(raw, "||", 2)) {
    return MYLISPC_VERTICAL2_TOKEN;
  }
  return 0;
}

int rawToken3(const char *raw) {
  if (!strncmp(raw, "#if", 3)) {
    return MYLISPC_POUND_if_TOKEN;
  }
  if (!strncmp(raw, "<=>", 3)) {
    return MYLISPC_LT_EQ_GT_TOKEN;
  }
  if (!strncmp(raw, ">>>", 3)) {
    return MYLISPC_GT3_TOKEN;
  }
  return 0;
}
