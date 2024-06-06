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

static bool rawNum(double *dest, zltString raw);
static int rawToken1(int c);
static int rawToken2(const char *raw);
static int rawToken3(const char *raw);
static int rawToken4(const char *raw);

int mylispcRawToken(double *numval, zltString raw) {
  if (rawNum(numval, raw)) {
    return MYLISPC_NUM_TOKEN;
  }
  if (raw.size == 1) {
    return rawToken1(*raw.data);
  }
  if (raw.size == 2) {
    return rawToken2(raw.data);
  }
  if (raw.size == 3) {
    return rawToken3(raw.data);
  }
  if (raw.size == 4) {
    return rawToken4(raw.data);
  }
  return MYLISPC_ID_TOKEN;
}

static bool rawInt(int *dest, zltString raw);

bool rawNum(double *dest, zltString raw) {
  int i;
  if (rawInt(&i, raw)) {
    *dest = i;
    return true;
  }
  *dest = 0;
  zltString s = zltStrToDouble(dest, raw);
  if (!s.size) {
    return true;
  }
  if (s.size == 3 && strncmp(raw.data, "nan", 3)) {
    *dest = nan;
    return true;
  }
  return false;
}

    char c = *raw.data;
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
  }
}
