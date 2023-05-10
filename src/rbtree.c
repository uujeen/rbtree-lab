#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;

  return p;
}

void post_delete(node_t *root, node_t *nil){
  if(root == NULL || root == nil) return;
  post_delete(root->left, nil);
  post_delete(root->right, nil);
  if(root != nil) {
    free(root);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  post_delete(t->root, t->nil);
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *x){
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

void right_rotate(rbtree *t, node_t *x){
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
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
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

node_t* rbtree_minimum(node_t *nil, node_t* x) {  
  while(x->left != nil){
    x = x->left;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t* y = p;
  node_t* x;
  color_t y_original_color = y->color;
  if(p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if(p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else {
    y = rbtree_minimum(t->nil, p->right);
    y_original_color = y->color;
    x = y->right;
    if(y->parent == p) {
      x->parent = y;
    }
    else {
      rbtree_transplant(t, y, y->right);
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
    rbtree_delete_fixup(t, x);
  }  
  t->nil->parent = NULL;
  t->nil->right = NULL;
  t->nil->left = NULL;
  t->nil->color = RBTREE_BLACK;
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
  in_order(t->root, t->nil, arr, index);
  free(index);
  return 0;
}