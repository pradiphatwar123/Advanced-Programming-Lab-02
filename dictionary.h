
#ifndef _DICTONARY_H_
#define _DICTONARY_H_


#define POSITIVE_SENTINEL ((pow(2,((sizeof(int)*8)-1))) - 1)

#define NEGATIVE_SENTINEL (-(pow(2,((sizeof(int)*8)-1)))) 

 
using namespace std;

typedef enum {RED, BLACK} NodeColor;

//Binary Search Tree Node 
class BSTNode
{
	public:
		int		key;		//Node Value
		BSTNode 	*left_child;	//Left Child
		BSTNode		*right_child;	//Right Child
		BSTNode		*parent;	//Parent Node
};
//Red Black Tree Node
class RBTNode
{
	public: 
		int		key;		//Node Value
		RBTNode 	*left_child;	//Left Child
		RBTNode		*right_child;	//Right Child
		RBTNode		*parent;
		NodeColor 	color;	//Color of the Node	
};

//Dictonary ADT class
class DICTONARY
{

	public:
		DICTONARY(){};
		~DICTONARY(){};
		//The below functions are virtual functions
		virtual void 	INSERT(int itemToInsert){};//Discuss Prototype return value with TA		
		virtual bool 	DELETE(int itemToDelete){};
		virtual bool 	SEARCH(int itemToSearch){};
		virtual void 	ClearADT(){};//Discuss Prototype return value with TA		
    		virtual void 	DisplayADT(){};//Discuss Prototype return value with TA		
		
};
//BST class inherting DICTONARY ADT class
class BST : public DICTONARY{
	public:
		BST();
		~BST();

		//Verify the below redeclarations inside BST class
		void 		INSERT(int itemToInsert);
		bool 		DELETE(int itemToDelete);
		bool 		SEARCH(int itemToSearch);
		void 		ClearADT();
    		void 		DisplayADT();

		BSTNode* 	Successor(BSTNode *);		
				//Subtree rooted at first parameter is getting replaced by subtree rooted at second parameter			
		BSTNode* 	Tree_Minimum(BSTNode *);
		BSTNode* 	Tree_Maximum(BSTNode *);
		void 		inOrderTraverse(BSTNode *);

};
//RBT class inherting DICTONARY ADT class
class RBT : public DICTONARY{
	public:
		RBT();
		~RBT();

		//Verify the below redeclarations inside RBT class
		void 		INSERT(int itemToInsert);
		bool 		DELETE(int itemToDelete);
		bool 		SEARCH(int itemToSearch);
		void 		ClearADT();
    		void 		DisplayADT();

		
		RBTNode* 	Tree_Minimum(RBTNode *);		
				//Subtree rooted at first parameter is getting replaced by subtree rooted at second parameter	
		void 		Transplant(RBTNode *, RBTNode *);
		void		Left_Rotate(RBTNode *);
		void		Right_Rotate(RBTNode *);
		void 		Insert_Fixup(RBTNode *);
		void		Delete_Fixup(RBTNode *);
		void        	inOrderTraverse(RBTNode *);		
};


int	ValidateCmdline(int, char *[], char **, char **, char **, char **);
int 	CheckFiles(char *, char *,char *,char *);
int	ValidateFile(char *);	
int 	ValidateInput(char *);
void	PopulateDictionary(char *, char *);
void	LocateInDictionary(char *,char *);
void	ShowMenu(char *, char *);	
int	WriteTimeinfo(char *, long , long);	

#endif 
