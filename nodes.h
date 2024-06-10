#ifndef MYLISPC_NODES_H
#define MYLISPC_NODES_H

#include"mylispc.h"

#include"zlt/ifcpp_begin.h"

typedef struct {
  mylispcNode node;
  const zltString *raw;
} mylispcIDAtom;

#define mylispcIDAtomMemb(p, m) zltMemb(p, mylispcIDAtom, m)

static inline mylispcIDAtom mylispcIDAtomMake(const mylispcPos *pos, const zltString *raw) {
  return (mylispcIDAtom) { .node = mylispcNodeMake(MYLISPC_ID_ATOM_CLASS, pos), .raw = raw };
}

typedef struct {
  mylispcNode node;
  void *first;
} mylispcList;

#define mylispcListMemb(p, m) zltMemb(p, mylispcList, m)

static inline mylispcList mylispcListMake(const mylispcPos *pos, void *first) {
  return (mylispcList) { .node = mylispcNodeMake(MYLISPC_LIST_CLASS, pos), .first = first };
}

typedef struct {
  mylispcNode node;
  const zltString *raw;
  double value;
} mylispcNumAtom;

#define mylispcNumAtomMemb(p, m) zltMemb(p, mylispcNumAtom, m)

static inline mylispcNumAtom mylispcNumAtomMake(const mylispcPos *pos, const zltString *raw, double value) {
  return (mylispcNumAtom) { .node = mylispcNodeMake(MYLISPC_NUM_ATOM_CLASS, pos), .raw = raw, .value = value };
}

typedef struct {
  mylispcNode node;
  const zltString *value;
} mylispcStrAtom;

#define mylicpcStrAtomMemb(p, m) zltMemb(p, mylispcStrAtom, m)

static inline mylispcStrAtom mylispcStrAtomMake(const mylispcPos *pos, const zltString *value) {
  return (mylispcStrAtom) { .node = mylispcNodeMake(MYLISPC_STR_ATOM_CLASS, pos), .value = value };
}

typedef struct {
  mylispcNode node;
  int token;
} mylispcTokenAtom;

#define mylispcTokenAtomMemb(p, m) zltMemb(p, mylispcTokenAtom, m)

static inline mylispcTokenAtom mylispcTokenAtomMake(const mylispcPos *pos, int token) {
  return (mylispcTokenAtom) { .node = mylispcNodeMake(MYLISPC_TOKEN_ATOM_CLASS, pos), .token = token };
}

#include"zlt/ifcpp_end.h"

#endif
