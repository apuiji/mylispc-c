#ifndef MYLISPC_H
#define MYLISPC_H

#include<stdio.h>
#include"zlt/link.h"
#include"zlt/rbtree.h"
#include"zlt/string.h"

typedef struct mylispcPos mylispcPos;

struct mylispcPos {
  const char *file;
  int li;
  const mylispcPos *prev;
};

static inline mylispcPos mylispcPosMake(const char *file, int li, const mylispcPos *prev) {
  return (mylispcPos) { .file = file, .li = li, .prev = prev };
}

typedef struct {
  zltRBTree rbTree;
  mylispcPos pos;
} mylispcPosTree;

int mylispcPosCmpForFind(const mylispcPos *data, const mylispcPosTree *tree);

static inline mylispcPosTree *mylispcPosTreeFind(const mylispcPosTree *tree, const mylispcPos *pos) {
  return (mylispcPosTree) zltBiTreeFind(tree, (zltBiTreeCmpForFind *) mylispcPosCmpForFind, pos);
}

mylispcPosTree **zltBiTreeFindForInsert(mylispcPosTree **parent, mylispcPosTree **tree, const mylispcPos *pos) {
  void **a = zltBiTreeFindForInsert((void **) parent, (void **) tree, (zltBiTreeCmpForFind *) mylispcPosCmpForFind, pos);
  return (mylispcPosTree **) a;
}

typedef struct {
  int clazz;
  const mylispcPos *pos;
} mylispcNode;

static inline mylispcNode mylispcNodeMake(int clazz, const mylispcPos *pos) {
  return (mylispcNode) { .clazz = clazz, .pos = pos };
}

typedef struct {
  zltLink link;
  mylispcNode *node;
} mylispcNodeLink;

enum {
  MYLISPC_RAW_ATOM_CLASS = 0x100,
  MYLISPC_ID_ATOM_CLASS,
  MYLISPC_NUM_ATOM_CLASS,
  MYLISPC_TOKEN_ATOM_CLASS,
  MYLISPC_LIST_CLASS = 0x201,
  MYLISPC_STR_ATOM_CLASS
};

enum {
  MYLISPC_EOF_TOKEN,
  MYLISPC_ID_TOKEN,
  MYLISPC_NUM_TOKEN,
  MYLISPC_STR_TOKEN,
  MYLISPC_KWD_TOKEN = 0x100,
  MYLISPC_callee_TOKEN,
  MYLISPC_def_TOKEN,
  MYLISPC_defer_TOKEN,
  MYLISPC_forward_TOKEN,
  MYLISPC_guard_TOKEN,
  MYLISPC_if_TOKEN,
  MYLISPC_length_TOKEN,
  MYLISPC_return_TOKEN,
  MYLISPC_throw_TOKEN,
  MYLISPC_try_TOKEN,
  MYLISPC_PREPROC_TOKEN = 0x200,
  MYLISPC_POUND_TOKEN,
  MYLISPC_POUND2_TOKEN,
  MYLISPC_POUND_def_TOKEN,
  MYLISPC_POUND_if_TOKEN,
  MYLISPC_POUND_include_TOKEN,
  MYLISPC_POUND_movedef_TOKEN,
  MYLISPC_POUND_undef_TOKEN,
  MYLISPC_SYMBOL_TOKEN = 0x300,
  MYLISPC_EXCLAM_TOKEN,
  MYLISPC_PERCENT_TOKEN,
  MYLISPC_AMP2_TOKEN,
  MYLISPC_AMP_TOKEN,
  MYLISPC_LPAREN_TOKEN,
  MYLISPC_RPAREN_TOKEN,
  MYLISPC_ASTERISK2_TOKEN,
  MYLISPC_ASTERISK_TOKEN,
  MYLISPC_PLUS_TOKEN,
  MYLISPC_COMMA_TOKEN,
  MYLISPC_MINUS_TOKEN,
  MYLISPC_DOT_TOKEN,
  MYLISPC_SLASH_TOKEN,
  MYLISPC_LT2_TOKEN,
  MYLISPC_LT_EQ_GT_TOKEN,
  MYLISPC_LT_EQ_TOKEN,
  MYLISPC_LT_TOKEN,
  MYLISPC_EQ2_TOKEN,
  MYLISPC_EQ_TOKEN,
  MYLISPC_GT_EQ_TOKEN,
  MYLISPC_GT3_TOKEN,
  MYLISPC_GT2_TOKEN,
  MYLISPC_GT_TOKEN,
  MYLISPC_AT_TOKEN,
  MYLISPC_CARET2_TOKEN,
  MYLISPC_CARET_TOKEN,
  MYLISPC_VERTICAL2_TOKEN,
  MYLISPC_VERTICAL_TOKEN,
  MYLISPC_TILDE_TOKEN
};

zltString mylispcTokenRaw(int token);

int mylispcRawToken(double *numval, zltString raw);

const char *mylispcHit(mylispcPos *pos, const char *it, const char *end);

const char *mylispcLexer(
  int *token, double *numval, zltString *strval, FILE *err, const mylispcPos *pos, const char *it, const char *end);

void mylispcParse(mylispcNodeLink **dest, FILE *err, mylispcPos *pos, const char *it, const char *end);

enum {
  MYLISPC_WARN,
  MYLISPC_ERROR = 0x100,
  MYLISPC_FATAL = 0x200,
  MYLISPC_UNTERMINATED_STR_FATAL
};

void mylispcReportBad(FILE *dest, int bad, const mylispcPos *pos);

#endif
