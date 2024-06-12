#ifndef MYLISPC_H
#define MYLISPC_H

#include<stdio.h>
#include"pos.h"
#include"zlt/link.h"
#include"zlt/stack.h"
#include"zlt/strtree.h"

#include"zlt/ifcpp_begin.h"

const zltString *mylispcAddStr(zltStrTree **tree, zltString str);

typedef struct {
  zltLink link;
  int clazz;
  const mylispcPos *pos;
} mylispcNode;

#define mylispcNodeMemb(p, m) zltMemb(p, mylispcNode, m)

static inline mylispcNode mylispcNodeMake(int clazz, const mylispcPos *pos) {
  return (mylispcNode) { .clazz = clazz, .pos = pos };
}

void mylispcNodeDtor(void *node);

static inline void mylispcNodeClean(void *node) {
  zltLinkClean(node, NULL, mylispcNodeDtor);
}

enum {
  MYLISPC_RAW_ATOM_CLASS = 0x100,
  MYLISPC_ID_ATOM_CLASS,
  MYLISPC_NUM_ATOM_CLASS,
  MYLISPC_TOKEN_ATOM_CLASS,
  MYLISPC_LIST_CLASS = 1,
  MYLISPC_STR_ATOM_CLASS
};

const char *mylispcHit(mylispcPos *pos, const char *it, const char *end);

typedef struct {
  int token;
  union {
    double numval;
    zltString strval;
  };
} mylispcLexerDest;

const char *mylispcLexer(mylispcLexerDest *dest, FILE *err, const mylispcPos *pos, const char *it, const char *end);
const char *mylispcLexer1(int *token, FILE *err, const mylispcPos *pos, const char *it, const char *end);

typedef struct {
  FILE *err;
  mylispcPos pos;
  zltStrTree **strTree;
  mylispcPosTree **posTree;
} mylispcParseContext;

int mylispcParse(void **dest, mylispcParseContext *ctx, const char *it, const char *end);

typedef struct {
  zltStack params;
  mylispcNode *body;
} mylispcMacro;

typedef struct {
  zltStrTree strTree;
  mylispcMacro macro;
} mylispcMacroTree;

const mylispcMacro *mylispcAddMacro(mylispcMacroTree **tree, const mylispcMacro *macro);

typedef struct {
  FILE *err;
  mylispcPos *prevPos;
  zltStrTree **strTree;
  mylispcPosTree **posTree;
  mylispcMacroTree **macroTree;
} mylispcPreprocContext;

int mylispcPreproc(void **dest, mylispcPreprocContext *ctx, void **src);

enum {
  MYLISPC_WARN,
  MYLISPC_ERROR = 0x100,
  MYLISPC_FATAL = 0x200,
  MYLISPC_OOM_FATAL,
  MYLISPC_UNRECOGNIZED_SYMBOL_FATAL,
  MYLISPC_UNTERMINATED_LIST_FATAL,
  MYLISPC_UNTERMINATED_STR_FATAL
};

void mylispcReportBad(FILE *dest, int bad, const mylispcPos *pos);

#include"zlt/ifcpp_end.h"

#endif
