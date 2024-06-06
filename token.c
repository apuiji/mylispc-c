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

int mylispcRawToken(double *numval, zltString raw) {
  ;
}
