/*********************************************************************************************************
Subject           :  Advanced Programming Lab (CS6140)

Assignment        :  02 - Dictonary ADT Implementation using Red-Black Trees

File name         :  RBT.CPP

Description       :  Implements Interfaces for DICTONARY Abstract Data Type (ADT) using Red-Black Trees

Programmers       :  (1) Pradip Hatwar     (CS13M022)
                     (2) Hem Kapil         (CS13M023)
                     (3) Pradeep Vankadaru (CS13M037)

Compiler name     :  g++-4.6

Date created      :  12/09/2013

List of functions : (1)  		RBT(void)
                    (2)  		~RBT(void)
                    (3)  int  		INSERT(int)
                    (4)  int 		DELETE(int )
                    (5)  bool 		SEARCH(int );
		    (6)	 void 		ClearADT();
    		    (7)	 void 		DisplayADT();
		    (8)  RBTNode* 	Tree_Minimum(RBTNode *);
				//First Parameter is root node, subtree rooted at second parameter is getting replaced by subtree rooted at third parameter	
		    (9)	 void 		Transplant(RBTNode *, RBTNode *, RBTNode *);
		    (10) void		Left_Rotate(RBTNode *);
		    (11) void		Right_Rotate(RBTNode *);
		    (12) void 		Insert_Fixup(RBTNode *);
		    (13) void		Delete_Fixup(RBTNode *);		
		    (14) void        	inOrderTraverse(RBTNode *);	
History           :  

*********************************************************************************************************/

#include<iostream>
#include"dictionary.h"
#include<math.h>

extern RBT		RBTObj;

int	nodes_count = 0;

RBTNode	*rbtroot, *NilNode, *NodeToDelete;// Pointer to the root node of RBT, NilNode, Node to Delete (in case of delete operation)

RBT::RBT()
{
	//Creating NilNode 	
	try{
	    	NilNode = new RBTNode;
    	}catch (bad_alloc xa) {        	
        	cout << "Failed to Intialize NilNode of Red-Black Tree" << endl;           
    	}
	NilNode->key = NEGATIVE_SENTINEL;
	NilNode->left_child = NilNode;
	NilNode->right_child = NilNode;
	NilNode->parent = NilNode;	
	NilNode->color = BLACK;
	
	rbtroot = NilNode;	
}
RBT::~RBT()
{	
	delete(rbtroot);				
}
/*********************************************************************************************************
Function Name            : INSERT 
Inputs                   : (1) int itemToInsert                          
Returns                  : void
Description              : This function inserts element 'itemtoInsert' in the Red-Black Tree
*********************************************************************************************************/
void RBT::INSERT(int itemToInsert)
{
	int 	q=0, error = 0;	
	RBTNode *x, *y, *z;
    
    y = NilNode;
    x = rbtroot;

    //Looking for the place to Insert
   while ( x != NilNode )
    {
        y = x;        
        if( itemToInsert < x->key )
            x = x->left_child;
        else if (itemToInsert > x->key )
          x = x->right_child;
           
 
    }
    
       //Creating a new Node
   	try{
	    	 z = new RBTNode;
    	}catch (bad_alloc xa) {        	
        	cout << "Failed to Intialize new node of Red-Black Tree" << endl;           
    	}
	z->key = itemToInsert;
	z->left_child = NilNode;
	z->right_child = NilNode;
	z->parent = y;
	z->color = RED;
   
    //If it is the first node make it root node	
    if( y == NilNode)
        rbtroot = z;
    //Otherwise add it proper place as per binary search tree property 	
    else
    {        
        if ( itemToInsert < y->key )
            y->left_child = z;
        else
            y->right_child = z;
    }
    //When a new node is added it may violate any of Red Black tree properties, so rebalancing the tree	
    Insert_Fixup(z);

	
}
/*********************************************************************************************************
Function Name            : DELETE 
Inputs                   : (1) int position                          
Returns                  : false In case of any error occurred during DELETE operation on Red-Black Tree
                            true In case of successfull DELETE operation on Red-Black Tree
Description              : This function deletes an element in the Red-Black Tree
*********************************************************************************************************/
bool RBT::DELETE(int ItemToDelete)
{
	bool		error=false, retval = false;
    	RBTNode 	*temp; 	
    	RBTNode 	*x, *y, *z;
	int		y_original_color;

	

	retval = SEARCH(ItemToDelete); //Searching for existence of the item to delete
	if(retval)
    	{	
		cout << "Deleting Node " << ItemToDelete << endl;
		z = NodeToDelete; //Reference to the node to be deleted
	    	y = z;
		y_original_color = y->color;
		// If z has no left child
		if(z->left_child == NilNode)
		{
			x = z->right_child;
			Transplant(z,z->right_child);
		
		}
		// If z has no right child
		else if(z->right_child == NilNode)
		{
			x = z->left_child;
			Transplant(z,z->left_child);
		
		}
		//If z has 2 children look for inorder successor 
		else
		{
			y = Tree_Minimum(z->right_child);
		
			y_original_color = y->color;
			x = y->right_child;
			//Inorder successor is right child of z
			if(y->parent == z)
			{				
				x->parent = y;
			}
			//Inorder successor is in right child's subtree 
			else
			{
				//Replacing Inorder successor with it's right child				
				Transplant(y,y->right_child);
				y->right_child = z->right_child;
				y->right_child->parent = y;
			}
			Transplant(z,y);
			y->left_child = z->left_child;
			y->left_child->parent = y;
			y->color = z->color;	
		}
		//If the color of the node which is getting deleteed or moved within the tree is BLACK
		//It may voilate the Red Black tree properties so fixing it
		if(y_original_color == BLACK)
		{
				
			Delete_Fixup(x);	
		}	
		//Freeing the Memory occupied by z
		z->left_child = NULL;
		z->right_child = NULL;
		z->parent = NULL;
		delete(z);
    	}
	else
	{
		cout << "Element is not found inside ADT" << endl;	
	}
	
	return error;
}
/*********************************************************************************************************

Function Name            : SEARCH 
Inputs                   : (1) int position                          
Returns                  : false In case of any error occurred during SEARCH operation on Red-Black Tree
                            true In case of successfull SEARCH operation on Red-Black Tree
Description              : This function searches an element in the Red-Black Tree
*********************************************************************************************************/
bool RBT::SEARCH(int ItemToSearch)
{	
	bool 	keyfound=false;
	RBTNode *temp;
	
	temp = rbtroot;
	while( temp != NilNode)
    	{
               if(temp->key == ItemToSearch) //identifire 'T' is undefined
               {
                    keyfound = true;    
                    NodeToDelete = temp;    
                    break;
               }
               else if (ItemToSearch < temp->key)
                  temp = temp->left_child;
               else
                  temp = temp->right_child;
    	}
	return keyfound;
}
/*********************************************************************************************************
Function Name            : DisplayADT 
Inputs                   : void                         
Returns                  : void
Description              : This function prints all the items present in the Red-Black Tree
*********************************************************************************************************/
void RBT::DisplayADT(void)
{
	RBTNode *p = rbtroot, *tmp;

	nodes_count = 0;
	inOrderTraverse(p);
	cout << endl;	
}


/*********************************************************************************************************
Function Name            : inOrderTraverse 
Inputs                   : RBT  Node                        
Returns                  : void
Description              : This function prints all the items present in the Red Black Tree in order traversal
*********************************************************************************************************/

void RBT::inOrderTraverse(RBTNode *q)
{	
	if ( q != NilNode )
    	{    
        	inOrderTraverse( q->left_child );	
	       	cout << "Item count  " << nodes_count++ << "  " << q->key << "(" << q->color << ")" << endl;		
        	inOrderTraverse( q->right_child );
    	}    
}

/*********************************************************************************************************
Function Name            : ClearADT 
Inputs                   : void                         
Returns                  : void
Description              : This function clears all the items present in the Red-Black Tree
*********************************************************************************************************/
void RBT::ClearADT(void)
{
	RBTNode	*temp;

	temp = rbtroot;
	while(temp != NilNode)
	{		
		DELETE(temp->key);
		temp = 	rbtroot;			
	}
					
}

/*********************************************************************************************************
Function Name            : Tree_Minimum 
Inputs                   : Root Node of subtree                        
Returns                  : void
Description              : This function gives minimum key node in the Red BlackTree
*********************************************************************************************************/
RBTNode* RBT::Tree_Minimum(RBTNode *Node)
{
	while(Node->left_child != NilNode)
		Node = Node->left_child;
	return Node;
}
/*********************************************************************************************************
Function Name            : Transplant 
Inputs                   : Root Node of 1st sub tree, Root Node of 2nd 		 Subtree                        
Returns                  : void
Description              : This function replaces subtree rooted at u with subtree rooted at v in the Red-Black Tree
*********************************************************************************************************/
void RBT::Transplant(RBTNode *u, RBTNode *v)
{
	if(u->parent == NilNode)
	{
		rbtroot = v;
	}
	else if(u == u->parent->left_child)
	{
		u->parent->left_child = v;
	}
	else
	{
		u->parent->right_child = v;
	}
	v->parent = u->parent;	
}	
/*********************************************************************************************************
Function Name            : Left_Rotate 
Inputs                   : Reference to the Node around which Right Rotation has to be done
Returns                  : void
Description              : This function right rotates the sub tree rooted at Node in the Red-Black Tree
*********************************************************************************************************/				
void RBT::Left_Rotate(RBTNode *x)
{
	    RBTNode* y;   

    y=x->right_child; //set y

    x->right_child=y->left_child;  //Turn y's left sub tree into x's right subtree

    if (y->left_child != NilNode)

        y->left_child->parent=x;

    y->parent=x->parent;	//Link x's parent to y	
    
    if(x->parent == NilNode)
         rbtroot = y;
    else if(x==x->parent->left_child)
        x->parent->left_child=y;
    else
        x->parent->right_child=y;

    y->left_child=x;	//Put x on y's left

    x->parent=y;
    
}
/*********************************************************************************************************
Function Name            : Right_Rotate
Inputs                   : Reference to the Node around which Right Rotation has to be done                       
Returns                  : void
Description              : This function right rotates the sub tree rooted at Node in the Red-Black Tree
*********************************************************************************************************/
void RBT::Right_Rotate(RBTNode *y)
{
	    RBTNode* x;

    x=y->left_child;	 //set x

    y->left_child=x->right_child;	//Turn x's right sub tree into y's left subtree

    if (x->right_child != NilNode)

        x->right_child->parent=y;

    x->parent=y->parent;	//Link y's parent to x
    
    
    if(y->parent == NilNode)
        rbtroot = x;

    else if( y == y->parent->right_child)

       y->parent->right_child=x;

    else

        y->parent->left_child=x;

    x->right_child=y;	//Put y on x's right

    y->parent=x;


}
/*********************************************************************************************************
Function Name            : Insert_Fixup 
Inputs                   : Reference to the Node containg item to be inserted
Returns                  : void
Description              : This function Fixes violation occured by insertion of Node in the Red-BlackTree
*********************************************************************************************************/
void RBT::Insert_Fixup(RBTNode *z)
{
   
     RBTNode *y;
    //Loop Continues until z's parent color is RED
    while (z->parent->color == RED)

    {
	//If parent of z is left child of z's grandparent
        if(z->parent==z->parent->parent->left_child)

        {
            y=z->parent->parent->right_child;
	
	    //z's uncle y  is RED -- Case1
            if(y->color==RED)

            {
                z->parent->color=BLACK;

                y->color=BLACK;

                z->parent->parent->color=RED;

                z=z->parent->parent;
             }
	    //z's uncle y is BLACK	 
            else
            {
		  //z's uncle y is BLACK and z is right child -- Case2
                  if(z==z->parent->right_child)
                  {
                  
                     z=z->parent;                     
                     Left_Rotate(z);
                  }
		//z's uncle y is BLACK and z is left child -- -- Case3
                z->parent->color=BLACK;
    
                z->parent->parent->color=RED;
              
                Right_Rotate(z->parent->parent);
            }                                     
         
        }
	//If parent of z is right child of z's grandparent
        else
        {

             y=z->parent->parent->left_child;
	    //z's uncle y  is RED -- Case1		
            if(y->color==RED)

            {

                z->parent->color=BLACK;

                y->color=BLACK;

                z->parent->parent->color=RED;

                z=z->parent->parent;

            }
	    //z's uncle y is BLACK
            else
            {
	 	 //z's uncle y is BLACK and z is left child -- Case2
                 if(z==z->parent->left_child)
                 {
                         z=z->parent;
                         Right_Rotate(z);
                 }
		//z's uncle y is BLACK and z is left child -- -- Case3
                z->parent->color=BLACK;

                z->parent->parent->color=RED;

                Left_Rotate(z->parent->parent);
            }    
        }

    }
    rbtroot->color = BLACK;    
   
}
/*********************************************************************************************************
Function Name            : Delete_Fixup 
Inputs                   : Reference to the Node containg item to be deleted
Returns                  : void
Description              : This function Fixes violation occured by deletion of Node in the Red-Black Tree
*********************************************************************************************************/
void RBT::Delete_Fixup(RBTNode *x)
{
	RBTNode *w;
	
	
	while(x != rbtroot && x->color == BLACK)
	{
		// If x is left chld	
		if(x == x->parent->left_child)
        	{
	        	w=x->parent->right_child;
			//x's sibling w is RED - case1
	             	if(w->color == RED)
             		{
			
                 		w->color=BLACK;
		                x->parent->color=RED;
                 		Left_Rotate(x->parent);
                 		w=x->parent->right_child;
             		}
			//x's sibling w is BLACK and both of w's children are BLACK - case2
             		if((w->left_child->color == BLACK)&&(w->right_child->color == BLACK))
             		{
			
  		        	w->color=RED;
                 		x=x->parent;
             		}
		  	else 
			{
				//x's sibling w is BLACK and w's left child is RED and w's right child is BLACK - case3
				if(w->right_child->color == BLACK)
             			{
					
		                	w->left_child->color=BLACK;
					w->color=RED;
	 	                	Right_Rotate(w);
		                	w=x->parent->right_child;
				}
				//x's sibling w is BLACK and w's right child is RED - case4
             			w->color=x->parent->color;
             			x->parent->color=BLACK;
				w->right_child->color = BLACK;
             			Left_Rotate(x->parent);
		                x = rbtroot;
			}
        	}
		// If x is right chld
        	else
        	{
	        	w=x->parent->left_child;
			//x's sibling w is RED - case1
             		if(w->color == RED)
             		{
			
		        	w->color=BLACK;
				x->parent->color=RED;
				Right_Rotate(x->parent);
				w=x->parent->left_child;
			}
			//x's sibling w is BLACK and both of w's children are BLACK - case2
             		if((w->right_child->color == BLACK)&&(w->left_child->color == BLACK))
			{
			
				w->color=RED;
				x=x->parent;
			}
			else 
			{
				//x's sibling w is BLACK and w's  w's right child is RED and left child is BLACK - case3
				if(w->left_child->color == BLACK)
				{
				
					w->right_child->color=BLACK;
					w->color=RED;
					Left_Rotate(w);
					w=x->parent->left_child;
				}
				//x's sibling w is BLACK and w's left child is RED - case4
				w->color=x->parent->color;
             			x->parent->color=BLACK;
				w->left_child->color = BLACK;
             			Right_Rotate(x->parent);
			
	     			x=rbtroot;
			}
             	}
	}   
	x->color = BLACK;
}

