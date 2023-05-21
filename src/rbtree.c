#include "rbtree.h"
#include <stdlib.h>
/**
 * CLRS Red-Black Tree
 * Red-Black Tree C언어로 구현하기.
*/
rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  p->nil = (node_t *)calloc(1, sizeof(node_t)); // nil 노드에 노드를 구현
  p->nil->color = RBTREE_BLACK; // 조건에 따라 nil 노드의 색상을 블랙으로 표시
  p->root = p->nil; // 초기 상태 root 노드를 nil 노드와 같은 형태로 선언 (색상과 노드의 구성 (색상, 키값, 부모 자식노드))

  return p;
}

void post_delete(node_t *root, node_t *nil){
  if(root == NULL || root == nil) return;
  post_delete(root->left, nil); // 왼쪽
  post_delete(root->right, nil); // 오른쪽
  if(root != nil) {
    free(root); // 부모 노드 삭제
  }
  else {
    free(root); // nil 노드 삭제
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  post_delete(t->root, t->nil); // 후위순회방식으로 트리의 모든 노드 반환
  free(t); // 트리 반환
}

void left_rotate(rbtree *t, node_t *x){ // 왼쪽방향으로 트리 회전
  node_t* y = x->right;
  x->right = y->left;
  if(y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  }
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x){ // 오른쪽 방향으로 트리 회전
  node_t* y = x->left;
  x->left = y->right;
  if(y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil) {
    t->root = y;
  }
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t* y;
  while(z->parent->color == RBTREE_RED) {
    if(z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      if(y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if(z == z->parent->right) {
        z = z->parent;
        left_rotate(t, z);
          }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      right_rotate(t, z->parent->parent);
      }
    }
    else{
      y = z->parent->parent->left;
      if(y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if(z == z->parent->left) {
        z = z->parent;
        right_rotate(t, z);
        }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* y = t->nil;
  node_t* x = t->root;
  node_t* z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  while(x != t->nil) {
    y = x;
    if(z->key < x->key) {
      x = x->left;
    }
    else {
      x = x->right;
    }
  }
  z->parent = y;
  if(y == t->nil) {
    t->root = z;
  }
  else if(z->key < y->key) {
    y->left = z;
  }
  else {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED; // 삽입시 항상 레드 색상
  rbtree_insert_fixup(t, z); // 삽입 후 색상와 위치를 수정한다.
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) { // 이진탐색트리의 find와 동일한 방식
  // TODO: implement find
  node_t* v;
  v = t->root;
  while(v != t->nil) {
    if(v->key > key) {
      v = v->left;
    }
    else if(v->key < key) {
      v = v->right;
    }
    else {
      break;
    }    
  }
  if(v == t->nil) {
    return NULL;
  }
  else {
    return v;
  }
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* y = t->root;
  while(y->left != t->nil) {
    y = y->left;
  }
  return y;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* y = t->root;
  while(y->right != t->nil) {
    y = y->right;
  }
  return y;
}

void rbtree_transplant(rbtree *t, node_t* u, node_t *v) {
  if(u->parent == t->nil){
    t->root = v;
  }
  else if( u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree* t, node_t* x) {
  node_t* w;
  while(x != t->root && x->color == RBTREE_BLACK){
    if(x == x->parent->left) {
      w = x->parent->right;
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        w->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color ==RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if(w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else {
      w = x->parent->left;
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        w->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color ==RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if(w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

node_t* rbtree_minimum(node_t *nil, node_t* x) { // 삭제 연산 시 오른쪽 서브트리의 가장 작은 값을 반환
  while(x->left != nil){
    x = x->left;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *p) { // 삭제연산
  // TODO: implement erase
  node_t* y = p;
  node_t* x;
  color_t y_original_color = y->color;
  if(p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, x); // 서브트리를 옮기는 함수
  }
  else if(p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, x);
  }
  else {
    y = rbtree_minimum(t->nil, p->right); // 대체할 노드를 찾을 때 오른쪽 서브트리의 가장 작은 수를 찾는다. 동일하게 왼쪽 서브트리의 가장 큰 값을 찾는 방법도 있다.
    y_original_color = y->color;
    x = y->right;
    if(y->parent == p) {
      x->parent = y;
    }
    else {
      rbtree_transplant(t, y, x);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  free(p);
  p = NULL;
  if(y_original_color == RBTREE_BLACK) {
    rbtree_delete_fixup(t, x); // 삭제 후 색상와 위치를 수정한다.
  } 
  return 0;
}

void in_order(node_t *root, node_t *nil, key_t *arr, int* index){
  if (root == nil) return;
  in_order(root -> left, nil, arr, index);
  arr[(*index)++] = root->key;
  in_order(root -> right, nil, arr, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int *index = calloc(1, n);
  in_order(t->root, t->nil, arr, index); // 전위순회 방식으로 배열로 저장하면 가장 작은값부터 오름차순으로 저장
  free(index);
  return 0;
}