#ifndef MYLISPC_H
#define MYLISPC_H

#include<stdio.h>
#include"pos.h"
#include"zlt/link.h"

#include"zlt/ifcpp_begin.h"

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

void mylispcParse(mylispcNodeLink **dest, FILE *err, mylispcPos *pos, const char *it, const char *end);

enum {
  MYLISPC_WARN,
  MYLISPC_ERROR = 0x100,
  MYLISPC_FATAL = 0x200,
  MYLISPC_OOM_FATAL,
  MYLISPC_UNTERMINATED_STR_FATAL
};

void mylispcReportBad(FILE *dest, int bad, const mylispcPos *pos);

#include"zlt/ifcpp_end.h"

#endif
