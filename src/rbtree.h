#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t; // 0 : Red, 1 : BLACK

typedef int key_t;

typedef struct node_t { // 노드의 구성 : 색상, 키, 부모 자식 노드
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct { // 트리의 초기구조 root, nil노드
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
