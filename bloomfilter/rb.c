// C program for Red-Black Tree insertion
#include "rb.h"

// Left Rotation
void LeftRotate(struct node **root,struct node *x)
{
    if (!x || !x->right)
        return ;
    //y stored pointer of right child of x
    struct node *y = x->right;

    //store y's left subtree's pointer as x's right child
    x->right = y->left;

    //update parent pointer of x's right
    if (x->right != NULL)
        x->right->parent = x;

    //update y's parent pointer
    y->parent = x->parent;

    // if x's parent is null make y as root of tree
    if (x->parent == NULL)
        (*root) = y;

    // store y at the place of x
    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;

    // make x as left child of y
    y->left = x;

    //update parent pointer of x
    x->parent = y;
}


// Right Rotation (Similar to LeftRotate)
void rightRotate(struct node **root,struct node *y)
{
    if (!y || !y->left)
        return ;
    struct node *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent =y->parent;
    if (x->parent == NULL)
        (*root) = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// Utility function to fixup the Red-Black tree after standard BST insertion
void insertFixUp(struct node **root,struct node *z)
{
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z != (*root)->left && z != (*root)->right && z->parent->color == 'R')
    {
        struct node *y;

        // Find uncle and store uncle in y
        if (z->parent && z->parent->parent && z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;

        // If uncle is RED, do following
        // (i)  Change color of parent and uncle as BLACK
        // (ii) Change color of grandparent as RED
        // (iii) Move z to grandparent
        if (!y)
            z = z->parent->parent;
        else if (y->color == 'R')
        {
            y->color = 'B';
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
        }

        // Uncle is BLACK, there are four cases (LL, LR, RL and RR)
        else
        {
            // Left-Left (LL) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Right Rotate Grandparent
            if (z->parent == z->parent->parent->left &&
                z == z->parent->left)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent->parent);
            }

            // Left-Right (LR) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Left Rotate Parent
            // (iii) Right Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->left &&
                z == z->parent->right)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root,z->parent);
                rightRotate(root,z->parent->parent);
            }

            // Right-Right (RR) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Left Rotate Grandparent
            if (z->parent && z->parent->parent &&
                z->parent == z->parent->parent->right &&
                z == z->parent->right)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root,z->parent->parent);
            }

            // Right-Left (RL) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Right Rotate Parent
            // (iii) Left Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->right &&
                z == z->parent->left)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent);
                LeftRotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; //keep root always black
}

// Utility function to insert newly node in RedBlack tree
void insert(struct node **root, char *data)
{
    // Allocate memory for new node
    struct node *z = malloc(sizeof(struct node));
    
    //copy id to struct member
    int strl=strlen(data);
    z->id=malloc(strl+1);
    strcpy(z->id,data);
    //printf("%s\n",z->id);
    z->left = z->right = z->parent = NULL;
	
	
     //if root is null make z as root
    if (*root == NULL)
    {
        z->color = 'B';
        (*root) = z;
           // printf("OKI\n",z->id);

    }
    else
    {
        struct node *y = NULL;
        struct node *x = (*root);
        
        //printf("OKI\n");
		
        // Follow standard BST insert steps to first insert the node
        while (x != NULL)
        {
        	
            y = x;
           if(strcmp(z->id ,x->id) < 0 ) 
			{
				//printf("OKI\n");
				x = x->left;
			}                
            else
            {
            	//printf("OKI\n");
            	x = x->right;   
			}
                 
        }
        
        z->parent = y;
        
        
        if(strcmp(z->id ,y->id) > 0 )
        {
        	y->right = z;
        //	printf("OKI\n");
		}
        else
        {
        	y->left = z;
        //	printf("OKI\n");
		}
        z->color = 'R';
		
        // call insertFixUp to fix reb-black tree's property if it
        // is voilated due to insertion.
        insertFixUp(root,z);
    }
    
}

int search_rbt(struct node *root,char *id)
{
		
	if(root==NULL)
	{
		//*pointer=NULL;
		return 0;
	}
	
	if(strcmp(id,root->id)==0)
	{
		return 1;
	}
	else if(strcmp(id,root->id)<0)//(TSDDA_mikrotero(stoixeio, tree->dedomena))
	{
		search_rbt(root->left, id);
	}
	else if(strcmp(id,root->id)>0)
	{
		search_rbt(root->right, id);
	}

}



// A utility function to traverse Red-Black tree in inorder fashion
void inorder(struct node *root)
{
	/*
    char *last;
    int strl=strlen(root->id);
    last=malloc(strl+1);
    strcpy(last,root->id);
	*/

	//base case
    if (root == NULL)
        return;
    
    inorder(root->left);
    printf("%s ", root->id);
    /*
    if (strcmp(root->id,last) < 0)
        printf("\nPUTE\n");*/
    inorder(root->right);
}

/*
void replace_node(rbtree t, node oldn, node newn) {
    if (oldn->parent == NULL) {
        t->root = newn;
    } else {
        if (oldn == oldn->parent->left)
            oldn->parent->left = newn;
        else
            oldn->parent->right = newn;
    }
    if (newn != NULL) {
        newn->parent = oldn->parent;
    }
}


void rbtree_delete(rbtree t, void* key, compare_func compare) {
    node child;
    node n = lookup_node(t, key, compare);
    if (n == NULL) return; 
    if (n->left != NULL && n->right != NULL) {
        node pred = maximum_node(n->left);
        n->key   = pred->key;
        n = pred;
    }

    assert(n->left == NULL || n->right == NULL);
    child = n->right == NULL ? n->left  : n->right;
    if (node_color(n) == BLACK) {
        n->color = node_color(child);
        delete_case1(t, n);
    }
    replace_node(t, n, child);
    if (n->parent == NULL && child != NULL)
        child->color = BLACK;
    free(n);

    verify_properties(t);
}

static node maximum_node(node n) {
    assert (n != NULL);
    while (n->right != NULL) {
        n = n->right;
    }
    return n;
}

void delete_case1(rbtree t, node n) {
    if (n->parent == NULL)
        return;
    else
        delete_case2(t, n);
}

void delete_case2(rbtree t, node n) {
    if (node_color(sibling(n)) == RED) {
        n->parent->color = RED;
        sibling(n)->color = BLACK;
        if (n == n->parent->left)
            rotate_left(t, n->parent);
        else
            rotate_right(t, n->parent);
    }
    delete_case3(t, n);
}

void delete_case3(rbtree t, node n) {
    if (node_color(n->parent) == BLACK &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        delete_case1(t, n->parent);
    }
    else
        delete_case4(t, n);
}

void delete_case4(rbtree t, node n) {
    if (node_color(n->parent) == RED &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        n->parent->color = BLACK;
    }
    else
        delete_case5(t, n);
}

void delete_case5(rbtree t, node n) {
    if (n == n->parent->left &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == RED &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->left->color = BLACK;
        rotate_right(t, sibling(n));
    }
    else if (n == n->parent->right &&
             node_color(sibling(n)) == BLACK &&
             node_color(sibling(n)->right) == RED &&
             node_color(sibling(n)->left) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->right->color = BLACK;
        rotate_left(t, sibling(n));
    }
    delete_case6(t, n);
}

void delete_case6(rbtree t, node n) {
    sibling(n)->color = node_color(n->parent);
    n->parent->color = BLACK;
    if (n == n->parent->left) {
        assert (node_color(sibling(n)->right) == RED);
        sibling(n)->right->color = BLACK;
        rotate_left(t, n->parent);
    }
    else
    {
        assert (node_color(sibling(n)->left) == RED);
        sibling(n)->left->color = BLACK;
        rotate_right(t, n->parent);
    }
}
*/

/* Driver program to test above function
int main()
{
    srand(time(NULL));
    struct node *root = NULL;
    int i;

    clock_t t0 = clock();
    for ( i = 0; i < NB_ELEMS; ++i)
		insert(&root, rand());	
    clock_t t1 = clock();
    
    printf("inorder Traversal Is :\n");
    inorder(root);
    printf("\n");
    
    float time_taken = (float)(t1 - t0) / CLOCKS_PER_SEC * 1000;
	printf("insertion took %fms -> %fus/elem\n",time_taken,time_taken / NB_ELEMS * 1000);
	
    return 0;
}*/
