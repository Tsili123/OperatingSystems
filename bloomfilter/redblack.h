#ifndef __redblack__
#define __redblack__

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
// Basic type defintions:

enum color_t { BLACK, RED };

typedef struct tnode {
  struct tnode *parent;
  struct tnode *left;
  struct tnode *right;
  enum color_t color;
  char *key;
}Node;

//extern struct tnode *LEAF;
extern struct tnode *NILL;
extern struct tnode *ROOT;

Node* GetParent(Node* n);
Node* GetGrandParent(Node* n);
Node* GetSibling(Node* n);
Node* GetUncle(Node* n);
void RotateLeft(Node* n);
void RotateRight(Node* n);
void InsertRepairTree(Node* n);
void InsertCase1(Node* n);
void InsertCase2(Node* n);
void InsertCase3(Node* n);
void InsertCase4(Node* n);
void InsertCase4Step2(Node* n);
void insert_recurse(Node* root, Node* n);
Node* Insert(Node* root, Node* n);
void inorder(Node *root);
void DeleteOneChild(Node* n);
void ReplaceNode(Node* n, Node* child);
void DeleteCase1(Node* n);
void DeleteCase2(Node* n);
void DeleteCase3(Node* n);
void DeleteCase4(Node* n);
void DeleteCase5(Node* n);
void DeleteCase6(Node* n);
Node* find_succesor(Node* root);
int search_rbt(Node *root,char *key);
Node* search_rbts(Node *root,char *key);
int is_leaf(Node *n);
Node *search(Node *temp, char *key);
void red_black_delete_fixup(struct tnode *x);
void red_black_transplant(struct tnode *u, struct tnode *v);
void red_black_insert_fixup(struct tnode *z);
void left_rotate(struct tnode *x);
void right_rotate(struct tnode *x);
struct tnode *tree_search(char* key);
void red_black_insert(char* key);
void tree_print(struct tnode *x);
struct tnode *tree_search(char* key);
void red_black_delete(struct tnode *z);

void left_rotate(struct tnode *x);
void right_rotate(struct tnode *x);
void tree_print(struct tnode *x);
void red_black_insert(char* key);
void red_black_insert_fixup(struct tnode *z);
struct tnode *tree_search(char* key);
struct tnode *tree_minimum(struct tnode *x);
void red_black_transplant(struct tnode *u, struct tnode *v);
void red_black_delete(struct tnode *z);
void red_black_delete_fixup(struct tnode *x);
#endif

