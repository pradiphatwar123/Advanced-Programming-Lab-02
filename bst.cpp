/*********************************************************************************************************
Subject           :  Advanced Programming Lab (CS6140)

Assignment        :  02 - Dictonary ADT Implementation using Binary Search Tree

File name         :  BST.CPP

Description       :  Implements Interfaces for DICTONARY Abstract Data Type (ADT) using Binary Search Trees

Programmers       :  (1) Pradip Hatwar     (CS13M022)
                     (2) Hem Kapil         (CS13M023)
                     (3) Pradeep Vankadaru (CS13M037)

Compiler name     :  g++ 4.6

Date created      :  12/09/2013

List of functions : (1)  		BST(void)
                    (2)  		~BST(void)
                    (3)  int  		INSERT(int)
                    (4)  int 		DELETE(int )
                    (5)  bool 		Search(int );
		    (6)	 void 		ClearADT();
    		    (7)	 void 		DisplayADT();
		    (8)  BSTNode* 	Successor(BSTNode *);		
		    (9)  BSTNode* 	Tree_Minimum(BSTNode *);
		    (10) BSTNode* 	Tree_Maximum(BSTNode *);
		    (11) void 		inOrderTraverse(BSTNode *);	
History           :  

*********************************************************************************************************/

#include<iostream>
#include "dictionary.h"
#include<math.h>



BSTNode	*bstroot, *NodeToDelete1;// Pointer to the root node of BST, Node to be delted(In case of delete operation)
int	nodes_total = 0;

BST::BST()
{

}
BST::~BST()
{	
	
}
/*********************************************************************************************************
Function Name            : INSERT 
Inputs                   : (1) int itemToInsert                          
Returns                  : void
Description              : This function inserts element 'itemtoInsert' in the Binary Search Tree
*********************************************************************************************************/
void BST::INSERT(int itemToInsert)
{
	int 	error = 0;
	BSTNode *x,*y,*z;
    
    y=NULL;
    x=bstroot;
    
    //Looking for the place to Insert	
    while(x!=NULL)
    {
             y=x;
             if(itemToInsert<x->key)
                  x=x->left_child;
              else
                  x=x->right_child;                  
    }
    //Creating a new Node
    try{
        z=new BSTNode;
    }
    catch(bad_alloc xa){
                    cout<<"Failed to Initialize new node of BST"<<endl;                          
    }          
        
    z->key=itemToInsert;
    z->left_child=NULL;
    z->right_child=NULL;
    z->parent=y;
    
     //If it is the first node make it root node	
    if(y==NULL)
          bstroot=z;
     //Otherwise add it proper place as per binary search tree property
     else if(itemToInsert<y->key)
           y->left_child=z; 
    else
           y->right_child=z;            

			
}

/*********************************************************************************************************
Function Name            : DELETE 
Inputs                   : (1) int ItemToDelete                          
Returns                  : false In case of any error occurred during DELETE operation on Binary Search Tree
                            true In case of successfull DELETE operation on Binary Search Tree
Description              : This function deletes an element in the Binary Search Tree
*********************************************************************************************************/
bool BST::DELETE(int ItemToDelete)
{
	bool	error=false,retval=false;
	BSTNode *x,*y,*z;     
	
    retval=SEARCH(ItemToDelete); //Searching for existence of the item to delete
	
    if(retval)
    {
     	cout << "Deleting Node " << ItemToDelete << endl;
   	 z = NodeToDelete1;	//Reference to the node to be deleted 
	//If z has no child or single child then y is z
         if(z->left_child == NULL || z->right_child == NULL)
            y=z;    
	//Otherwise y is inorder insuccessor of z                        
         else 
            y=Successor(z);

         //Node x replaces z's inorder successor
         if(y->left_child != NULL)
             x=y->left_child;
         else
             x=y->right_child;
             
         //If inorder successor has child
         if(x != NULL)
             x->parent=y->parent;
         
	//If inorder successor is root node then making child of y root node
         if(y->parent == NULL)
              bstroot=x;
	//Otherwise	
         else 
	 {
		if(y == y->parent->left_child)
                    y->parent->left_child=x;
                else
                    y->parent->right_child=x;
         }     
                   
          if(y != z)
                  z->key=y->key;
                           
	  //Freeing the Memory occupied by y
	  y->left_child = NULL;
	  y->right_child = NULL;
	  y->parent = NULL;
	  delete(y); 		
	  		 
       }   
	else
	    {
	     	
              cout<<"Element is not found inside ADT"<<endl;    	
      }
      return error;
}

/*********************************************************************************************************

Function Name            : SEARCH 
Inputs                   : (1) int position                          
Returns                  : false In case of any error occurred during SEARCH operation on Binary Search Tree
                            true In case of successfull SEARCH operation on Binary Search Tree
Description              : This function searches an element in the Binary Search Tree
*********************************************************************************************************/
bool BST::SEARCH(int ItemToSearch)
{	
	bool 	error=false;
	BSTNode *temp=bstroot;
	while(temp!=NULL)
	{
                     if(ItemToSearch==temp->key)
                     {
                     	error=true;
                     	NodeToDelete1 = temp;
                        break;
                     }
                     else if(ItemToSearch<temp->key) temp=temp->left_child;
                     else temp=temp->right_child;
                     
     	}
	return error;
}
/*********************************************************************************************************
Function Name            : DisplayADT 
Inputs                   : void                         
Returns                  : void
Description              : This function prints all the items present in the Binary Search Tree
*********************************************************************************************************/
void BST::DisplayADT(void)
{	
    	BSTNode *p = bstroot, *tmp;

	nodes_total = 0;
      	inOrderTraverse(p);
	    cout << endl;		
}

/*********************************************************************************************************
Function Name            : inOrderTraverse 
Inputs                   : BST Node                        
Returns                  : void
Description              : This function prints all the items present in the Binary Search Tree in order traversal
*********************************************************************************************************/
void BST::inOrderTraverse(BSTNode *q)
{
   if ( q != NULL )
    {    
        inOrderTraverse( q->left_child );	
        cout << "Item count  " << nodes_total++ << "  " << q->key << endl;
        inOrderTraverse( q->right_child );
    }    
    
}
/*********************************************************************************************************
Function Name            : ClearADT 
Inputs                   : void                       
Returns                  : void
Description              : This function clears all the items present in the Binary Search Tree
*********************************************************************************************************/
void BST::ClearADT(void)
{
	BSTNode	*temp;

	temp = bstroot;
	while(temp != NULL)
	{		
		DELETE(temp->key);
		temp = 	bstroot;			
		
	}
	
}
/*********************************************************************************************************
Function Name            : Successor 
Inputs                   : Node for which successor has to be found                         
Returns                  : If returns successor of a given Node
Description              : This function returns successor of the given node in the Binary Search Tree
*********************************************************************************************************/
BSTNode* BST::Successor(BSTNode *Node)
{
	//BSTNode	 *temp;
	BSTNode	 *y;
    if(Node->right_child != NULL)
        return Tree_Minimum(Node->right_child);
        
    y=Node->parent;
    
    while(y != NULL && Node == y->right_child)            
    {
               Node=y;
               y=y->parent;             
    }
    return y;
}
/*********************************************************************************************************
Function Name            : Tree_Minimum 
Inputs                   : Root Node of SubTree                        
Returns                  : Reference to a node containing minimum key
Description              : This function gives minimum key node  in the Binary Search Tree
*********************************************************************************************************/
BSTNode* BST::Tree_Minimum(BSTNode *Node)
{
         while(Node->left_child != NULL)
              Node=Node->left_child;         
         
         return Node;
}
/*********************************************************************************************************
Function Name            : Tree_Maximum 
Inputs                   : Root Node of SubTree                        
Returns                  : Reference to a node containing maximum key
Description              : This function gives maximum key node  in the Binary Search Tree
*********************************************************************************************************/

BSTNode* BST::Tree_Maximum(BSTNode *Node)
{
         while(Node->right_child != NULL)
              Node = Node->right_child;        
         
         return Node;
}




