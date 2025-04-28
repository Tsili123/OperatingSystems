#ifndef __testis__
#define __testis__
/*
rules
1. every node is either red or black
2. root is always black
3. no adjacent red nodes (red node cannot have a red parent or red child)
4. every path from root to a null node has been number of black nodes (one sub stree can be twice as long)


1. (no parent) N is root -> color it black
2. parent is black -> do nothing
3. parent is red
  look at uncle
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BLACK true
#define RED false

typedef struct node {
  bool color;
  char* datum;
  struct node *parent;
  struct node *left;
  struct node *right;
} Node;

//Node tmp = {BLACK, NULL, NULL, NULL, NULL};
//Node *LEAF = &tmp;

//int print(Node *tree);
int clear(Node **tree);
//int insert(Node **tree, char* datum);
int insertBST(Node **tree, Node *nd);
void rebalance(Node *nd);
Node * getUncle(Node  *node);
Node * getParent(Node *node);
Node * getGrandparent(Node *node);
Node * getSibling(Node *node);
void rotateLeft(Node *node);
void rotateRight(Node *node);
void deletes(Node **tree, char* value);
Node * successor(Node *node);
void delete_case1(Node *node);
void delete_case2(Node *node);
void delete_case3(Node *node);
void delete_case4(Node *node);
void delete_case5(Node *node);
void delete_case6(Node *node);
void delete_one_child(Node *node);
void replacement(Node *node, Node *child);
void inorder(struct node *root);

#endif

