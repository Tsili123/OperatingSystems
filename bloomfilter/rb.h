#ifndef __rb__
#define __rb__
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
//#define NB_ELEMS 250

//A Red-Black tree node structure
struct node
{
    char *id;
    char color;
    struct node *left, *right, *parent;
};


void LeftRotate(struct node **root,struct node *x);
void rightRotate(struct node **root,struct node *y);
void insertFixUp(struct node **root,struct node *z);
void insert(struct node **root, char *data);
void inorder(struct node *root);
int search_rbt(struct node *root,char *id/*,struct node **pointer*/);

#endif


