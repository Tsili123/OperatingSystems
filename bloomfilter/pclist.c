#include "pclist.h"


  ///OK
/* function to insert a new_node in a list in sorted way. 
   Note that this function expects a pointer to head node 
   as this can modify the head of the input linked list */
bool sortedInsert(struct Node** head_ref, struct Node* new_node/*,struct RBNODE_T *id*/) 
{ 
  struct Node* current = *head_ref; 

  // Case 1 of the above algo 
  if (current == NULL) 
  { 
     new_node->next = new_node; 
     *head_ref = new_node; 
      new_node->reference=NULL;
     /*
     struct rbpointers *p=malloc(sizeof(struct rbpointers));
     p->identity=id;
     (*head_ref)->refernce=p;*/
  } 
  
  // Case 2 of the above algo 
  else if (current->data >= new_node->data) 
  { 
  	if(current->data==new_node->data)
  		return false;
  
    /* If value is smaller than head's value then 
      we need to change next of last node */
    while(current->next != *head_ref) 
        current = current->next; 
    current->next = new_node; 
    new_node->next = *head_ref; 
    *head_ref = new_node; 
    new_node->reference=NULL;
  } 
  
  // Case 3 of the above algo 
  else
  { 
    /* Locate the node before the point of insertion */
    while (current->next!= *head_ref &&  
           current->next->data < new_node->data) 
      current = current->next; 
  
  	if(current->next->data==new_node->data)
  		return false;
  	
    new_node->next = current->next; 
    current->next = new_node; 
    new_node->reference=NULL;
  } 
  
  return true;
} 
  
void free_pointers_to_rbt(struct rbpointers *start){
	
	struct rbpointers *todel,*todel2;
	//first node
	todel=start;
	//Until we reach the end
	while(todel!=NULL)
	{
		
		todel2=todel;
		//go to the next node
	 	todel=todel->next;
	 	//free the previous
	 	free(todel2);
	}
	
}
  
void deletes(struct Node **head, int data) {
    struct Node *temp;
    struct Node *start = *head;
    temp=start;
    
    if(temp->data==data ){
    	
    	while (temp->next != start &&
            (temp->next)->data != data) {
        temp= temp-> next;
    	}
    	
    	temp->next=temp->next->next;
    	*head=temp->next;
    //	free_pointers_to_rbt(start->reference);
    	free(start);
    	return;
	}
    
    
    /*Go to the Node for which the Node next to it has to be deleted */
    while (temp->next != temp &&
            (temp->next)->data != data) {
        temp = temp -> next;
    }
    if (temp->next == start) {
        printf("Element %d is not present in the list\n", data);
        return;
    }
    
    /* Now temp points to a Node and the Node next to it has to be removed */
    struct Node *pointer = temp -> next;
    /*pointer points to the Node which has to be removed*/
    temp->next = pointer->next;
    
    //free_pointers_to_rbt(pointer->reference);
    
    /*We removed the Node which is next to the pointer (which is also temp) */
    free(pointer);
    /* Beacuse we deleted the Node, we no longer require the memory used for it .  free() will deallocate the memory.*/
    return;
}  
  

//printvoters per pcX,delete,search
//OK ??
//Checks if the given pc exists
bool searchpc(struct Node *head,int pc){
	struct Node* current = head;  // Initialize current 
    while (current != NULL) 
    { 
        if (current->data == pc) 
            return true; 
        current = current->next; 
    } 
    return false; 
}

//OK
//Checks if the given pc exists
struct Node* searchpc_n(struct Node *head,int pc){
	struct Node* current = head;  // Initialize current 
    while (current != NULL) 
    { 
        if (current->data == pc) 
            return current; 
        current = current->next; 
    } 
    return NULL;
}

//Checks if the given pc exists
struct rbpointers * freevoterpc(struct Node *head,char *key){
	struct Node* current = head;  // Initialize current 
	struct rbpointers *temp=head->reference;
    while (temp != NULL) 
    { 
        if (strcmp(temp->identity->key,key)==0) {
        	return temp;
		}
          
        temp=temp->next;
    } 
    return NULL;
}
//OK
void list_destroy_node(struct rbpointers **l)
{
	free(*l);
	l=NULL;
}
//
void list_remove(struct rbpointers **l,char *key)
{
	
	
	struct rbpointers *temp=*l,*temp_prev=*l;	
	
		
		while(temp!=NULL  )
		{
			if(strcmp(temp->identity->key,key)==0)
			{
					//case of first node
					if(temp==*l)
					{
						temp_prev=temp->next;
						*l=(temp)->next;
						list_destroy_node(&temp);
						(temp)=(temp_prev);
						
					}	
					else if(temp!=*l)
					{
						(temp_prev)->next=(temp)->next;
						list_destroy_node(&temp);
						(temp)=(temp_prev)->next;
					}
			}
			else
				(temp)=(temp)->next;
				
			if((temp)==*l)
				continue;
			else 
			{
				if( (temp_prev)!=NULL && (temp_prev)->next!=temp)
					(temp_prev)=(temp_prev)->next;	
			}
		}	
}

/*Prints the number of people in pc*/
int printvotersinpc(struct Node *start,int pc) 
{ 
  struct rbpointers *temprb;
  temprb=start->reference;
  int counter=0;
  
  if(start != NULL && searchpc(start,start->data)) {  
    printf("\n"); 
    while(temprb != NULL ) { 
      printf("%p person's key : %s \n", temprb,temprb->identity->key); 
      temprb = temprb->next;
	  counter++; 
    } 
  } 
  return counter;
} 
//OK
/* Function to print  postscodes and return the total number of it*/
int printpcnum(struct Node *head) 
{ 
  struct Node *temp; 
  temp=head;
  int counter=0;
   
  printf("%d postcode \n", temp->data); 
  temp=temp->next; 
  while(temp != head) { 
    printf("%d postcode \n", temp->data); 
	counter++; 
	temp = temp->next;
   }  
  return counter;
} 

//OK
int printpcvoters(struct Node *node) 
{ 

	struct rbpointers *temprb;
	temprb=node->reference;
	int counter=0;
	  
    if(node != NULL /*&& searchpc(node,node->data)*/) {  
	    //printf("\n"); 
	    while(temprb != NULL) { 
	    
	  	  if(temprb->identity!=NULL)
	  	  {
	  	  		counter++;
	  	  		printf("%s_ \n",temprb->identity->key);
			} 
	      temprb = temprb->next;
	    } 
	} 
  return counter;

}

int printpcv(struct Node *node) 
{ 

	struct rbpointers *temprb;
	temprb=node->reference;
	int counter=0;
	  
    if(node != NULL /*&& searchpc(node,node->data)*/) {  
	    //printf("\n"); 
	    while(temprb != NULL) { 
	    
	  	  if(temprb->identity->hasvoted)
	  	  {
	  	  	printf("MPIKE\n");
	  	  		counter++;
			} 
	      temprb = temprb->next;
	    } 
	} 
  return counter;

}

//OK
//Inserts the tab node at the end of the double linked cyclic list

void InsertVoterInPc(struct Node *start,RBNODE_T *ptr)
{
	
	struct rbpointers *new_ptr,*temprb;//,temprb_prev;
	
	new_ptr=malloc(sizeof(struct rbpointers));
	
	struct Node *temp;

	temp=start;
	
	//If the is no rbt ptr in the list of  pcs
	if(temp->reference==NULL){
		temp->reference=new_ptr;
		//temp->reference->next=NULL;
		new_ptr->identity=ptr;
		new_ptr->next=NULL;
	}//else traverse list of pointers and put it to the end of it
	else{
		temprb=temp->reference;
		//temprb_prev=temprb;
		
		
		
		//go to previous of last node
		while(temprb!=NULL && temprb->next!=NULL)
			temprb=temprb->next;
			
		temprb->next=new_ptr;
		new_ptr->next=NULL;
		new_ptr->identity=ptr;
	}
}



/* Driver program to test above functions 
int main() 
{ 
  int arr[] = {12, 56, 3,8}; 
  int list_size, i; 
  
  //start with empty linked list 
  struct Node *start = NULL; 
  struct Node *temp; 
  RBNODE_T *address=NULL;
  
  // Create linked list from the array arr[]. 
  //  Created linked list will be 1->2->11->12->56->90 
  for (i = 0; i< 6; i++) 
  { 
    temp = (struct Node *)malloc(sizeof(struct Node)); 
    temp->data = arr[i]; 
    sortedInsert(&start, temp); 
  } 
  
  for(i=0;i<4;i++){
  	deletes(&start,start->data);
  }
  
  //if(start==NULL){
  //	printf("OK\n");
 // }
  
 //temp = (struct Node *)malloc(sizeof(struct Node)); 
  //   temp->data = 1; 
 // sortedInsert(&start,temp);
  
  //deleteNode(start, 56); 
  
 // printf("List After Deletion: "); 
  
  printList(start); 
  
  return 0; 
} */
