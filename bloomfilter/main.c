#include "element.h"
#include <math.h>
#include "sizecomp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "pclist.h"
#include "hash.h"
#include "bloom.h"
#include "red-black.h"

#define PCLIST 1
unsigned int seed;
int people=0,total=0;



int main(int argc,char *argv[]) {
	
	srand(time(0)); 
	seed=rand();
	

	int k=0;
	unsigned long long bloom_bit_size;
	unsigned int voter,i,j,numofupdates=3,str_len,counter=0,num;//size;
	char buffer[100],delim[]=" \n";
	char *record,*command,*key,*input,*output;
	float prop=0.001;
	Elem_t *element;
	FILE * fp, *fp2;
	struct Node *pclist=NULL,*temp;
	
	RBTREE_T *root;
	root=rbtree_create(rbtree_strcmp,rbtree_strcpy,free,rbtree_intcpy,free);
	
	//argv argc initial command control
	for(j=1;j<argc;j++)
	{
		if(strcmp(argv[j], "-i")==0)
		{
			str_len=strlen(argv[j+1]);
			input=malloc(str_len+1);
			strcpy(input,argv[j+1]);
			printf("%s \n",input);
		}
		else if(strcmp(argv[j], "-o")==0)
		{
			str_len=strlen(argv[j+1]);
			output=malloc(str_len+1);
			strcpy(output,argv[j+1]);
			printf("%s \n",output);
		}
		else if(strcmp(argv[j], "-n")==0){
			numofupdates=atoi(argv[j+1]);
			printf("%d \n",numofupdates);
		}
	}
	
	
	fp = fopen (input, "r");
	if (fp == NULL) {
    	perror(argv[1]);
  	}
  
  	fp2=fopen ("file.txt", "r");
	int aux = countlines(fp2);
	rewind(fp2);
	
	/* INSTRUCTION BLOCK FOR PARSING LINE*/
	//count lines from file
	voter=countlines(fp);
	rewind(fp);
	//allocate space for the array of copies
	element=malloc(sizeof(Elem_t)*voter);
	//	printf("%d\n",voter);
	//fill the array from file
  	lineByline(fp,element);

	fclose(fp);//fix unexpected bug
	
	/* INSTRUCTION BLOCK FOR COMPUTING BLOOM FILTER SIZE*/
	//compute bloom filter size
	bloom_bit_size = compute_size(voter,&prop);
	//	printf("%llu\n",bloom_bit_size);
	bloom_bit_size=compute_prime(bloom_bit_size);
	printf("%llu\n",bloom_bit_size);
	unsigned long long	bloom_size=fix_bit_size(bloom_bit_size);
	//printf("%llu\n",bloom_size);
	
	
	/* INSTRUCTION BLOCK FOR CREATING BLOOM FILTER AND ADDING HASH FUCTIONS AND DATA TO IT*/
	bloom_t bloom = new_bloom(bloom_size);
	addf_bloom(bloom, PJWHash);
	addf_bloom(bloom,sdbm);
	addf_bloom(bloom, murmur3_32);
	//add elements to bloom filter
	for(i=0;i<voter;i++){
		addk_bloom(bloom,element[i].id,bloom_bit_size);
	}
	


	/** INSTRUCTION BLOCK FOR CREATING RED-BLACK TREE AND ADDING DATA TO IT*/
	for ( i = 0; i < voter; i++)
	{
		
		//create postcode list
		temp = malloc(sizeof(struct Node)); 
    	temp->data = element[i].pc; 
		sortedInsert(&pclist, temp); 
		
		RBNODE_T *ptr=rbtree_put(root,element[i].id,&i,element[i].name,element[i].surname,&element[i].age,&element[i].gender,&element[i].pc);
		
		if(pclist!=NULL && ptr !=NULL){
			struct Node *pctemp=searchpc_n(pclist,element[i].pc);
			#if PCLIST//BUGS WITH RBTREE FUNCTION OCCURS
			InsertVoterInPc(pctemp,ptr);
			#endif
			
			rbtree_make(root,element[i].id,&i,element[i].name,element[i].surname,&element[i].age,&element[i].gender,&element[i].pc);
	}
			
}
		
	inorder(root->root);
	
	//printf("%s \n",root->root->key);
	printf("\n \n");
	
	
	#if PCLIST
	printpcnum(pclist); 
	//CHECK if everything goes ok with insertion in the list and its pointers
	int variable=0;
	struct Node *head=pclist;
	// Function to print Nodes in a given linked list 
	while(pclist!=NULL){
		variable+=printpcvoters(pclist); 
		pclist=pclist->next;
		if(pclist==head)
			break;
	}
	printf("%d variable\n",variable);
	#endif 

	
	
	//	TEST CASES 
	//	printf("Should (probably) be 1: %d\n",	check_bloom(bloom,"BT112865",bloom_bit_size));

	//loop for instructions
	while(1){
		
		command=fgets(buffer, sizeof(buffer),stdin);
	
		if (strncmp(command, "lbf", 3) == 0)
		{
			key=malloc(9);
			strncpy(key,command+4,8);
			int len=strlen(key);
			
			if(len>8){
				key[8]='\0';
			}
			     //I hardcoded it to interrupt copy of unwanted elements
			printf("%s\n",key);
			printf("Should (probably) be 1: %d\n",	check_bloom(bloom,key,bloom_bit_size));	
			free(key);
		}
		else if (strncmp(command, "lrb", 3) == 0)
		{
			key=malloc(9);
			strncpy(key,command+4,8);
			
			printf("%d\n",strlen(key));

			int len=strlen(key);
			printf("%d\n",len);	
			if(len>8){
				key[8]='\0';
			}
			
			printf("%s\n",key);
		
		    RBNODE_T *p= rbtree_find(root, key);
		    if(p==NULL)
		        printf("Should  be 0.Not in the tree\n");
		    else 
		        printf("Should  be 1.Its in the tree\n");
			//printf("Should  be 1: %d\n",search(root,root->root,key) );
			
			free(key);
		}
		else if(strncmp(command, "ins", 3) == 0){
			str_len=strlen(command)+1;
			record=malloc(str_len);
			strncpy(record,command+4,str_len-4);
			printf("%s \n",record);
				    
		    element = realloc(element, (i+1) * sizeof(Elem_t));
			
			element[i].id = malloc(str_len);
			element[i].name = malloc(str_len);
			element[i].surname= malloc(str_len);

			//read the specific given format 
			int index = sscanf( record,"%s %s %s %d %c %d", element[i].id,  element[i].name , element[i].surname, &element[i].age , &element[i].gender, &element[i].pc);
			printf("%d\n",index);
			
			counter++;
		
			printf("\n%s\n",element[i].id);
		    printf("%s\n",element[i].name);
		    printf("%s\n",element[i].surname);
		    printf("%d\n",element[i].age);
		    printf("%c\n",element[i].gender);
		    printf("%d\n",element[i].pc);
		    printf("%d\n",i);
		
			
			//INSERT THE RECORD TO RED BLACK TREE REGISTERY AND TO THE PC LIST
			//rbtree_make(root,element[i].id,&i,element[i].name,element[i].surname,&element[i].age,&element[i].gender,&element[i].pc);
			#if PCLIST
			temp = malloc(sizeof(struct Node)); 
    		temp->data = element[i].pc; 
			sortedInsert(&pclist, temp); 
			#endif
			RBNODE_T *ptr=rbtree_put(root,element[i].id,&i,element[i].name,element[i].surname,&element[i].age,&element[i].gender,&element[i].pc);	
			#if PCLIST
			InsertVoterInPc(pclist,ptr);
			#endif
			///////////////////////////////////////////////////////
			inorder(root->root);
			printf("\n");
			
			if(counter==numofupdates)
			{
				printf("CHECKMATE\n");

				//delete previous bloom filter
				free_bloom(bloom);
				
				//recreate bloom filter
				
				/* INSTRUCTION BLOCK FOR COMPUTING BLOOM FILTER SIZE */
				
				//compute bloom filter size
				bloom_bit_size = compute_size(voter+counter,&prop);
				printf("%llu\n",bloom_bit_size);
				bloom_bit_size=compute_prime(bloom_bit_size);
				printf("%llu\n",bloom_bit_size);
				bloom_size=fix_bit_size(bloom_bit_size);
				printf("%llu\n",bloom_size);
				
				bloom = new_bloom(bloom_size);
				addf_bloom(bloom, PJWHash);
				addf_bloom(bloom,sdbm);
				addf_bloom(bloom, murmur3_32);
				
				
				voter+=counter;
				//add elements to bloom filter
				for(i=0;i<voter;i++){
					addk_bloom(bloom,element[i].id,bloom_bit_size);
				}
				counter=0;
				i--;
			}
			
			i++;
			free(record);
		}
		else if(strncmp(command, "find", 4) == 0)
		{
			key=malloc(9);
			strncpy(key,command+5,8);
			inorder(root->root);
			printf("\n");
			
			
			if(strlen(key)>8)
				key[8]='\0';
			printf("%s\n",key);
			
			
			bool find_flag=check_bloom(bloom,key,bloom_bit_size);
			
			
			if(find_flag==false){
				printf("The key %s does not exist in the catalogue\n",key);
			}
			else{
				
				RBNODE_T *node= rbtree_find(root, key);
		    	if(node==NULL)
		        	printf("False positive \n" );
		    	else {
		    		printf("Should  be 1.Its in the tree\n" );
		    		printf("The key %s exists in the catalogue.It belongs to : %s %s %d %c %d\n",key,node->name,node->surname,*(node->age),*(node->gender),*(node->pc));
				}
		        	/*
				find_flag=search(root,root->root,key);
				if(find_flag==true){
					RBNODE_T *node=search_n(root,root->root,key,false);
					if(node!=NULL){
							printf("The key %s exists in the catalogue.It belongs to : %s %s %d %c %d\n",key,node->name,node->surname,*(node->age),*(node->gender),*(node->pc));
					}
				}
				else{
					printf("False positive: %s \n",key);
				}*/
			}
		}
		else if(strncmp(command, "delete", 6) == 0)
		{
			key=malloc(9);
			strncpy(key,command+7,8);
			
			
			if(strlen(key)>8)
				key[8]='\0';
			printf("%s\n",key);
			/*
			RBNODE_T *rbtmp=search_n(root,root->root,key,false);
			if(rbtmp!=NULL){
				int pckey=*(rbtmp->pc);
				struct Node *pcn = searchpc_n(pclist,pckey);
				struct rbpointers *rbp = freevoterpc(pcn,key);
				list_remove(&rbp,key);
			}
			*/
			
			rbtree_remove(root,key);
			inorder(root->root);
			printf("\n");
			free(key);
		}
		else if(strncmp(command, "vote", 4) == 0){
			key=malloc(9);
			strncpy(key,command+5,8);
			
			if(strlen(key)>8)
				key[8]='\0';
			printf("%s\n",key);
			
			
			
			RBNODE_T *node= rbtree_find(root, key);
		    if(node==NULL){
		    	printf("The key : %s does not exist in the registery \n",key);
				fflush(stdin);
				continue;
			}
		    else {
		    	if(node!=NULL){
						if(check_node_vote(node))//if it's from a voter
							printf("The key : %s belongs to a vote \n",key);
						else {
							printf("The key : %s hasn't voted,so it votes\n",key);
							node_vote(node);
						}
					}
			}
			/*
			if(search(root,root->root,key)==false){
				printf("The key : %s does not exist in the registery \n",key);
				fflush(stdin);
				continue;
			}
			else//The key exists,check if it's from a voter
			{
				RBNODE_T *node=search_n(root,root->root,key,true);
					if(node!=NULL){
						if(check_node_vote(node))//if it's from a voter
							printf("The key : %s belongs to a vote \n",key);
						else {
							printf("The key : %s hasn't voted,so it votes\n",key);
							node_vote(node);
						}
					}
			}*/
			
			free(key);
		}
		else if(strncmp(command, "load", 4) == 0){
			key=malloc(9);
			strncpy(key,command+5,8);
			
			if(strlen(key)>8)
				key[8]='\0';
			printf("%s\n",key);
			
			char *s;
            Elem_t *item;
            
			for(j=voter;j<(voter+aux+1);j++){
				if ((s = readline(fp2)) != NULL){
					
					//printf("s: %s \n",s);
					//SAME BLOCK AS VOTE KEY INSTRUCTION
					
						RBNODE_T *node= rbtree_find(root, s);
					    if(node==NULL){
					    	printf("The key : %s does not exist in the registery \n",s);
							fflush(stdin);
							continue;
						}
					    else {
					    	if(node!=NULL){
									if(check_node_vote(node))//if it's from a voter
										printf("The key : %s belongs to a vote \n",s);
									else {
										printf("The key : %s hasn't voted,so it votes\n",s);
										node_vote(node);
									}
								}
						}
					/*if(search(root,root->root,s)==false){
						printf("The key : %s does not exist in the registery \n",s);
						fflush(stdin);
						continue;
					}
					else//The key exists,check if it's from a voter
					{
						RBNODE_T *node=search_n(root,root->root,s,true);
							if(node!=NULL){
								if(check_node_vote(node))//if it's from a voter
								{
									//	printf("The key : %s belongs to a vote \n",s);
								}
								else {
								//	printf("The key : %s hasn't voted,so it votes\n",s);
									node_vote(node);
								}
							}*/
					}//////////////////////////////////////////	
					else{
						printf("error in reading record file \n");
						break;
					}			
				    free(s);
				}	fclose(fp2);
			}	
		else if(strncmp(command, "see",3) == 0){
			bool value=true;
			inorder(root->root);
			total=sup(root->root,value);
			printf("\n%d people voted from a total of %d \n",people,total);
		}
		else if(strncmp(command, "check postcode",14) == 0){
			key=malloc(6);
			strncpy(key,command+15,5);
			
			if(strlen(key)>5)
				key[5]='\0';
			//printf("%s\n",key);
			int postcode=atoi(key);
			printf("postcodee : %d \n",postcode);
			#if PCLIST
			struct Node *pctemps=searchpc_n(pclist,postcode);
			int votesperpc=printpcv(pctemps);
			printf("\nvotesperpc : %d\n",votesperpc);
			#endif
			free(key);
		}
		else if(strncmp(command, "check votersperpc",17) == 0){
			key=malloc(6);
			strncpy(key,command+15,5);
			
			if(strlen(key)>5)
				key[5]='\0';
			//printf("%s\n",key);
			int postcode=atoi(key);
			printf("postcodee : %d \n",postcode);
			#if PCLIST
			struct Node *pctemps=searchpc_n(pclist,postcode);
			int votesperpc=printpcv(pctemps);
			printf("\nvotesperpc : %d\n",votesperpc);
			#endif
			free(key);
		}
		else if((strncmp(command, "exit",4) == 0))
		{
			//printf("ALL OK!\n");

			//delete bloom filter
			free_bloom(bloom);
			bloom=NULL;
			//delete red black tree
			rbtree_destroy(root);
			root=NULL;
			//delete postcode's list
			
			/*
			int counters=0;
			while(counters<voter-100){				
				deletes(&pclist,pclist->data);
				counter++;
			}
			pclist=NULL;*/
			
			//free array of copies
			destruct_copies(voter,element);//destruct arrays
			free(element);
			element=NULL;
				
			printf("ALL OK!\n");
			break;
		}	
		fflush(stdin);
	}
	return 0;
}
//coutput file
///pc list
//pc per cent
//text arxeio
