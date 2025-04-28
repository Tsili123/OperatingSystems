#ifndef PCLIST_H
#define PCLIST_H
#include<stdio.h> 
#include<stdlib.h> 
#include <string.h>
#include "red-black.h"

/* structure for a node */
struct Node 
{ 
  int data; 
  struct Node *next; 
  struct rbpointers *reference;
};

struct rbpointers
{
	RBNODE_T *identity;
	struct rbpointers *next;
};



void InsertVoterInPc(struct Node *start,RBNODE_T *ptr);
int printpcvoters(struct Node *node) ;
int printpcnum(struct Node *head) ;
int printvotersinpc(struct Node *start,int pc) ;
bool searchpc(struct Node *head,int pc);
void printList(struct Node *start) ;
void deletes(struct Node **head, int data) ;
//void free_pointers_to_rbt(struct Node *start);
bool sortedInsert(struct Node** head_ref, struct Node* new_node/*,struct RBNODE_T *id*/) ;
struct Node* searchpc_n(struct Node *head,int pc);
struct rbpointers * freevoterpc(struct Node *head,char *key);
void list_destroy_node(struct rbpointers **l);
void list_remove(struct rbpointers **l,char *key);
int printpcv(struct Node *node) ;
  
void free_pointers_to_rbt(struct rbpointers *start);
#endif
