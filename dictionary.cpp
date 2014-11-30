/*********************************************************************************************************
Subject           :  Advanced Programming Lab (CS6140)

Assignment        :  02 - Implementing DICTONARY ADT Operations

File name         :  DICTONARY.CPP

Description       :  Implements main functions required to implement Dictonary ADT Operations

Programmers       :  (1) Pradip Hatwar     (CS13M022)
                     (2) Hem Kapil         (CS13M023)
                     (3) Pradeep Vankadaru (CS13M037)

Compiler name     :  g++-4.6

Date created      :  12/09/2013

List of functions : (1)  int 	main(int argc, char *argv[])
		    (2)  int	ValidateCmdline(int, char *[], char **, char **, char **, char **);
		    (3)  int 	CheckFiles(char *, char *,char *,char *);
		    (4)	 int	ValidateFile(char *);	
		    (5)  int 	ValidateInput(char *);
		    (6)  void	PopulateDictionary(char *, char *);
		    (7)  void	LocateInDictionary(char *,char *);
		    (8)  void	ShowMenu(char *, char *);	
		    (9)  int	WriteTimeinfo(char *, long , long);	
                    
History           :  

*********************************************************************************************************/
#include<iostream>
#include<cstdlib>
#include<ctype.h>
#include<malloc.h>
#include<fstream>
#include<string.h>
#include<math.h>
#include<sstream>
#include<sys/timeb.h>

#include "dictionary.h"

//The below variables are made global to preserve the Prototypes of PopulateDictonary and LocateInDictonary methods

DICTONARY 	*DPtr; //Base Class Pointer
BST		BSTObj;	//BST Derived class Object
RBT		RBTObj; // RBT Derived Class Object
bool		BSTFlag, RBTFlag, TimeFlag; //Flags used for command line options

/*********************************************************************************************************
Function Name            : main 
Inputs                   : (1) Number of command line arguements passed
			   (2) List of Command line arguements
Outputs                  : int
Description              : This is a 'main'function which implements Dictonary ADT Operations.
*********************************************************************************************************/

int main(int argc, char *argv[])
{		
	int 		retval = 0;
	char		*inputfile, *searchfile,*timeinsertfile,*timesearchfile;
	
	

	inputfile = (char *)" " ;	
	searchfile = (char *)" ";
	timeinsertfile = (char *)" ";
	timesearchfile = (char *)" ";

		
	// Check if proper command line arguements are given 
	if(ValidateCmdline(argc,argv,&inputfile,&searchfile,&timeinsertfile,&timesearchfile) == -1)  
	{   		
		cout << "Exiting ....." <<endl;
		exit(-1);		
	}					
	// Check for successful files existence operation on all files
	// successful create operation on outfile file given
	else if(CheckFiles(inputfile,searchfile,timeinsertfile,timesearchfile) == -1) 
	{					    
		cout << "Exiting......" << endl;
		exit(-1);	
	}	
	else if(ValidateFile(inputfile) == -1) // Validating dictonary.txt Input file 
	{
		cout << "Failed to validate input file dictionary.txt, Exiting....." << endl;
		exit(-1);
	}		
	else if(ValidateFile(searchfile) == -1) // Validating locate.txt Input file 
	{
		cout << "Failed to validate input file locate.txt, Exiting...... " << endl;
		exit(-1);
	}							
	else
	{	
		//Assigning ADT Object as per flags given in the command line
		if(BSTFlag)
			DPtr = &BSTObj;		
		else
			DPtr = &RBTObj;	

		PopulateDictionary(inputfile,timeinsertfile);	// Populates Dictonary ADT 
		LocateInDictionary(searchfile,timesearchfile);	// Searches Dictonary ADT 
	} // End 
	//Populates ADT Operations Menu to the user 
	ShowMenu(timeinsertfile, timesearchfile);
	
	getchar();
	return 0;
}
/*********************************************************************************************************
Function Name            : ValidateCmdline
Inputs                   : (1) Number of Command line arguements passed
			   (2) List of Command line arguements
			   (3) Reference to input file
			   (4) Reference to search file
			   (5) Reference to TimeInsert file
			   (6) Reference to TimeSearch file
Outputs                  : int 
			   0 If no error
			  -1 If Error
Description              : This function Validates Command line arguements
*********************************************************************************************************/
int	ValidateCmdline(int argc, char *argv[], char **inputfile, char **searchfile, char **timeinsertfile, char **timesearchfile)
{
	int 	error = 0, iter;
	int 	dictionary_index = 0, locate_index = 0, bst_insert_index = 0;
	int	bst_search_index = 0, rbt_insert_index = 0, rbt_search_index =0 ;
	int	bst_index = 0, rbt_index = 0, time_index = 0;	
		
	
	if(argc < 3)	
	{	
		cout << "Error: valid command for execution is " << endl;
		cout << "<exename> <dictionary_i.txt> <locate_i.txt>"<< endl;		
		error = -1;
	}
	else
	{
		//This loop sets Flags as per command line arguements for internal use
		for(iter = 1; iter < argc ; iter++)
		{			
			if(strstr(argv[iter],"-bst"))
			{
				BSTFlag = true;
				bst_index = iter;					
				
			}
			if(strstr(argv[iter],"-t"))
			{
				TimeFlag = true;
				time_index = iter;				
			}
			if(strstr(argv[iter],"dictionary"))
			{
				dictionary_index = iter;				
				*inputfile = (char *)argv[iter];	
			
			}
			if(strstr(argv[iter],"locate"))
			{
				locate_index = iter;				
				*searchfile = (char *)argv[iter];	
			}
			if(strstr(argv[iter],"TimeFileInsert_Bst"))
			{
				bst_insert_index = iter;				
				*timeinsertfile = (char *)argv[iter];
			}
			if(strstr(argv[iter],"TimeFileSearch_Bst"))
			{
				bst_search_index = iter;
				*timesearchfile = (char *)argv[iter];
			}
			if(strstr(argv[iter],"TimeFileInsert_Rbt"))
			{
				rbt_insert_index = iter;
				*timeinsertfile = (char *)argv[iter];
			}
			if(strstr(argv[iter],"TimeFileSearch_Rbt"))
			{
				rbt_search_index = iter;
				*timesearchfile = (char *)argv[iter];
			}				
			
		}
		//If -bst is not passed 
		if(!BSTFlag)
		{
			RBTFlag = true;
			rbt_index = 0;			
		}
		//If -bst is not passed validating for RBT 
		if(RBTFlag)
		{
			//Either of dictionary or locate txt files are not given -- RBT
			if((dictionary_index == 0) || (locate_index == 0))
			{
				cout << "Error: valid command for execution is " << endl;
				if(TimeFlag)
					cout << "<exename> <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Rbt.txt TimeFileSearch_Rbt.txt"<< endl;
				else
					cout << "<exename> <dictionary_i.txt> <locate_i.txt> "<< endl;	
				error = -1;				
			}
			//dictionary and locate are not in proper order -- RBT
			else if((dictionary_index + 1) != locate_index)
			{
				cout << "Error: valid command for execution is " << endl;
				if(TimeFlag)
					cout << "<exename> <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Rbt.txt TimeFileSearch_Rbt.txt"<< endl;
				else
					cout << "<exename> <dictionary_i.txt> <locate_i.txt> "<< endl;	

				error = -1;
			}
			else 
			{	
				// If -t is passed 
				if(TimeFlag)
				{
					//Either TimeFileInsert or TimeFileSearch are not given -- RBT
					if((rbt_insert_index == 0) || (rbt_search_index == 0))
					{
						cout << "Error: valid command for execution is  "<< endl ; 						
						cout << "<exename> <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Rbt.txt TimeFileSearch_Rbt.txt "<< endl;
						error = -1;				
					}
					// TimeFileInsert and TimeFileSearch are before TimeFlag or not in proper order -- RBT
					else if(((time_index + 1) != rbt_insert_index ) || ((time_index + 2) != rbt_search_index ))
					{
						cout << "Error: valid command for execution is  "<< endl ; 						
						cout << "<exename> <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Rbt.txt TimeFileSearch_Rbt.txt "<< endl;
						error = -1;
					}					
					else
						error = 0;	
				}
			}	

		}	
		//If -bst is passed validating for BST
		else if(BSTFlag)
		{
			//Either dictionary or locate are not given -- BST
			if((dictionary_index == 0) || (locate_index == 0))
			{
				cout << "Error: valid command for execution is " << endl;
				if(TimeFlag)
					cout << "<exename> -bst <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Bst.txt TimeFileSearch_Bst.txt"<< endl;
				else
					cout << "<exename> -bst <dictionary_i.txt> <locate_i.txt> "<< endl;	
				error = -1;				
			}			
			//dictionary and locate are not next to bst flag or not in proper order -- BST
			else if(((bst_index + 1) != dictionary_index) || ((bst_index + 2) != locate_index))			
			{
				cout << "Error: valid command for execution is " << endl;
				if(TimeFlag)
					cout << "<exename> -bst <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Bst.txt TimeFileSearch_Bst.txt"<< endl;
				else
					cout << "<exename> -bst <dictionary_i.txt> <locate_i.txt> "<< endl;	

				error = -1;
			}
			else
			{		
				//If -t is passed
				if(TimeFlag)
				{
					//Either TimeFileInsert or TimeFileSearch are not given -- BST
					if((bst_insert_index == 0) || (bst_search_index == 0))
					{
						cout << "Error: valid command for execution is  "<< endl ; 						cout << "<exename> <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Bst.txt TimeFileSearch_Bst.txt "<< endl;
						error = -1;				
					}
					// TimeFileInsert or TimeFileSearch are before TimeFlag -- BST
					else if(((time_index + 1) != bst_insert_index) || ( (time_index + 2) != bst_search_index ))
					{
						cout << "Error: valid command for execution is  "<< endl ; 						cout << "<exename> <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Bst.txt TimeFileSearch_Bst.txt "<< endl;
						error = -1;
					}
					// TimeFileInsert, TimeFileSearch indexes are not in proper order -- BST
					else if((bst_insert_index + 1 ) != bst_search_index )
					{
						cout << "Error: valid command for execution is  "<< endl ; 						cout << "<exename> <dictionary_i.txt> <locate_i.txt> -t TimeFileInsert_Bst.txt TimeFileSearch_Bst.txt "<< endl;
						error = -1;
					}
					else
						error = 0;	
				}
			}
		}		
	
	}
	return error;
}
/*********************************************************************************************************
Function Name            : check_files 
Inputs                   : (1) Reference to input file
			   (2) Reference to search file
			   (3) Reference to TimeInsert file
			   (4) Reference to TimeSearch file	
Outputs                  : -1 In case of error 
                            0 In case of sucess
Description              : This function verifies the existence of input file, search file, TimeInsert file, TimeSearch file  and 
						   creates TimeInsert file, TimeSearch file if they does not exist
*********************************************************************************************************/
int CheckFiles(char *inputfile, char *searchfile,char *timeinsertfile,char *timesearchfile)
{

	int		retval = 0;
	ifstream	inpstrm ; // Input file reference
	ofstream	optstrm ; // Output file reference
	

	inpstrm.open(inputfile, ios::in); /* Opening inputfile */

	if(!inpstrm)
	{
		cout << "File * "  << inputfile << " does not exist" << endl;		
		retval = -1;
	}
	else
	{	
		inpstrm.close();
		inpstrm.open(searchfile, ios::in); /* Opening searchfile */
		if(!inpstrm)
		{
			cout << "File " << searchfile << " does not exist" << endl;			
			retval = -1;
		}
		else
		{
			inpstrm.close();
			if(TimeFlag)
			{	
				optstrm.open(timeinsertfile, ios::app); /* Opening Insert timing file in append mode */	
				if(!optstrm)
				{
					cout << "Failed to open File " << timeinsertfile <<  endl;					
					retval = -1;
				}
				else
				{	
					optstrm.close();
					optstrm.open(timesearchfile, ios::app);  /* Opening Search timing file in append mode */	
					if(!optstrm)
					{
						cout << "Failed to open File " << timesearchfile << endl;						
						retval = -1;
					}
					else
					{	
						optstrm.close();
					} 
				} 
			}
		}			
	} 
	
		
	return retval;
}

/*********************************************************************************************************
Function Name            : PopulateDictionary
Inputs                   : (1) Reference to input file
			   (2) Reference to TimeInsertFile
Outputs                  :   
Description              : This function reads data from input file and Populates into DICTONARY ADT
*********************************************************************************************************/
void PopulateDictionary(char *inpfile, char *timeinsertfile)
{	
	int		count = 0, retval = 0;
	ifstream	inpstrm ;	
	char		buff1[10241],buff[100],ch;
	stringstream 	sis;	
	string		str;
	bool		value_exists = false ;
	timeb		time1,time2;
	long		total_time = 0, total_count = 0;


	
	inpstrm.open(inpfile, ios::in);	/* Opening Input file */

	if(!inpstrm){
		cout << "Input File does not exist" << endl;				
	}
	else
	{			
		while(inpstrm)
		{			
			inpstrm.getline(buff1,10240,'\n');	// Reading Input file line wise 
			
			if(inpstrm)
			{				
				if(inpstrm.eof()) 
				{
					buff1[strlen(buff1)-1] = '\0';					
				}
				sis << buff1;	
				while(getline(sis,str,'\n'))
				{
					strncpy(buff, str.c_str(), sizeof(buff));

					value_exists = DPtr->SEARCH(atoi(buff));					
					if(value_exists)
					{
						cout << "Element::  "<< atoi(buff) << " is already  present in the Dictonary ADT, Skipping adding to ADT " << endl; 
					}
					else
					{	
						ftime(&time1); //Time Before Insert
						DPtr->INSERT(atoi(buff)); //Insert item into ADT						
						ftime(&time2); //Time after Insert						
						total_count++;
						total_time = total_time + ((time2.time - time1.time)*1000 + (time2.millitm - time1.millitm));//time1.time is gives time from 01 jan 1970 in seconds
					             								//time1.millitime is gives time in milliseconds 						
					}											
				}
				sis.clear();
			}
			else
			{				
				if(inpstrm.eof())
				{
					cout << "Reached end of the file " << inpfile << " inside PopulateDictionary " << endl;					
				}
				else
				{
					cout << "Failed to read data from file " << inpfile << "inside PopulateDictionary  " << endl; 
					retval = -1;				
				}			
			}
			if(retval == -1)
				break;	
		}
		inpstrm.close(); /* Closing the input file */
		cout << "Total time taken for inserting " << total_count <<  " elements is " << total_time << " milliseconds " << endl;	
		if(TimeFlag)
		{
			if(WriteTimeinfo(timeinsertfile,total_count,total_time) == -1)// Check for successful write operation to time file
			{	
				cout << "Failed to write to Time file" << endl;
			}
			
		}
	}
			 	
}
/*********************************************************************************************************
Function Name            : ValidateFile
Inputs                   : (1) Reference to file to be validated					

Outputs                  : -1 In case of any error occurrs
                            0 In case of all items in the file are proper
Description              :  This function validates the file consisting of a set of integers
*********************************************************************************************************/

int ValidateFile(char *inpfile)
{

	int		retval = 0, count = 0;
	ifstream	inpstrm ;	
	char		buff1[2000],buff[100],ch;
	stringstream 	sis;	
	string		str;

	
	inpstrm.open(inpfile, ios::in);	// Opening Input file 

	if(!inpstrm){		
		cout << "Exiting...." << endl;
		retval = -1;
	}
	else
	{			
		while(inpstrm)
		{		
			inpstrm.getline(buff1,20000, '\n');	// Reading Input file line wise			
			if(inpstrm)
			{					
				if(inpstrm.eof()) 
				{
					buff1[strlen(buff1)-1] = '\0';
				}
				sis << buff1;					
				while(getline(sis,str,'\n'))
				{
			strncpy(buff, str.c_str(), sizeof(buff));									 							
					retval = ValidateInput(buff); // Validating items read from Input file 					
					if(retval == -1)
					{ 						
						break;	
					}
				}
				sis.clear();
			}
			else
			{
				if(inpstrm.eof())
				{
					cout << "Reached end of the file " << inpfile << " inside validate_file " << endl;					
				}
				else
				{
					cout << "Failed to read data from file " << inpfile << "inside validate_file " << endl; 
					retval = -1;				
				}
			}	
			if(retval == -1)
				break;
		}
		inpstrm.close(); // Closing the input file 
	}	
	return retval;
}

/*********************************************************************************************************
Function Name            : ValidateInput 
Inputs                   : (1) Reference to input item				
Outputs                  : -1 In case of any error occurrs
                            0 In case of valid item
Description              :  This function validates the token read by ValidateFile to be a valid integer
*********************************************************************************************************/
int ValidateInput(char *input)
{

	int	retval =0, iter =0;

	//Verifying if blank space is received
	if(strlen(input) == 0)
	{
		retval = -1;
	}
	else
	{
		//Verifying if first character is starting with  either minus symbol or digit
		if((input[0] == '-') || (isdigit(input[0]))|| (input[0] == '+'))
		{
			//Verifying if string is starting with -1, checking all digits from 2nd digit onwards
			if((input[0] == '-') || (input[0] == '+'))
			{
				//Verifying If string is starting with -/+ and it's length is '1', it's not a valid number
				if(strlen(input) == 1)
				{
					retval = -1;
					cout << input << " is not a valid number, Exiting" << endl;	
				}
				else
				{
					for(iter = 1; iter <  strlen(input); iter++)
					{
						if(!isdigit(input[iter]))
						{
							retval = -1;
							cout << input << " is not a valid number, Exiting " << endl;
							break;
						}									
					}			
				}
			}
			//Verifying if string is starting with digit, checking all digits from first digit onwards
			else
			{

				for(iter = 0; iter <  strlen(input); iter++)
				{
					if(!isdigit(input[iter]))
					{
						retval = -1;
						cout << input << " is not a valid number, Exiting " << endl;
						break;
					}									
				}
				
			}
		}
		else
		{
			cout << input << " is not a valid number, Exiting" << endl;			
			retval = -1;

		}	
	}
	return retval;
}
/*********************************************************************************************************
Function Name            : LocateInDictionary
Inputs                   : (1) Reference to locate file
			   (2) Reference to TimeSearchFile
Outputs                  :   
Description              : This function reads data from locate file and Searches in DICTONARY ADT
*********************************************************************************************************/
void LocateInDictionary(char *locatefile,char *timesearchfile)
{
	int		retval  = 0, count = 0;
	ifstream	inpstrm ;	
	char		buff1[10241],buff[100],ch;
	stringstream 	sis;	
	string		str;
	bool 		searchretval = false;	
	timeb		time1,time2;
	long		total_time = 0, total_count = 0; 	

	
	inpstrm.open(locatefile, ios::in);	/* Opening Locate file */

	if(!inpstrm){
		cout << "Locate File does not exist" << endl;		
	}
	else
	{	
		
		while(inpstrm)
		{			
			inpstrm.getline(buff1,10240, '\n');	// Reading Input file line wise 
			if(inpstrm)
			{				
				if(inpstrm.eof()) 
				{
					buff1[strlen(buff1)-1] = '\0';
				}
				sis << buff1;	
				while(getline(sis,str,'\n'))
				{
					strncpy(buff, str.c_str(), sizeof(buff));
					ftime(&time1);	//Time Before Search
					searchretval = DPtr->SEARCH(atoi(buff)); //Search for item into ADT					
					ftime(&time2);	//Time After Search				
					total_time = total_time + ((time2.time - time1.time)*1000 + (time2.millitm - time1.millitm)); //time1.time is gives time from 01 jan 1970 in seconds
					             								//time1.millitime is gives time in milliseconds		
					total_count++;
					if(!searchretval)
					{
						cout << "Element::  " << atoi(buff) << " is not found in the Dictionary ADT" << endl;	
					}					
				}
				sis.clear();
			}
			else
			{
				if(inpstrm.eof())
				{
					cout << "Finished reading data from file " << locatefile << " inside LocateInDictionary " << endl;
				}
				else
				{
					cout << "Failed to read data from file " << locatefile << " inside LocateInDictionary " << endl;
					retval = -1;				
				}
			}
			if(retval == -1)
				break;
		}

		inpstrm.close(); /* Closing the locate file */
		cout << "Total time taken for searching " << total_count << " elements is "  <<  total_time  << " milliseconds " << endl;	
		if(TimeFlag)
		{
			if(WriteTimeinfo(timesearchfile,total_count,total_time) == -1)// Check for successful write operation to time file
			{	
				cout << "Failed to write to Time file" << endl;
			}	
		}
	}					
}
/*********************************************************************************************************
Function Name            : ShowMenu
Inputs                   : (1) Reference to TimeInsertFile
			   (2) Reference to TimeSearchFile
Outputs                  :   
Description              : This function Displays Menu for user interaction to do Dictionary ADT Operations and records timing information
			   into TimeInsertFile, TimeSearchFile	
 *********************************************************************************************************/
void ShowMenu(char *timeinsertfile, char *timesearchfile)
{
	int 		user_option = 0, item = 0;
	long int	insert_totaltime = 0, search_totaltime = 0;
	bool		retval = false;	
	long int	insert_count = 0, search_count = 0;	
	timeb		inserttime1,inserttime2;
	timeb		searchtime1,searchtime2;

	while(true)
	{
		cout << "		Menu 		" << endl;

		cout << "1. INSERT " << endl;
		cout << "2. DELETE " << endl;
		cout << "3. SEARCH " << endl;
		cout << "4. DISPLAY " << endl;
		cout << "5. CLEAR " << endl;
		cout << "6. EXIT " << endl;
		
		cout << "Select the option from the Menu " << endl;	
		cin >> user_option;

		switch(user_option)
		{
			case 1:
				cout << "Enter the item to insert into ADT" << endl;
				cin >> item;
				ftime(&inserttime1);
				DPtr->INSERT(item);
				ftime(&inserttime2);
				insert_totaltime = insert_totaltime + ((inserttime2.time - inserttime1.time)*1000 + (inserttime2.millitm - inserttime1.millitm));		
				insert_count++;		
				cout << "Insert count is " << insert_count;
				break;
			case 2:
				cout << "Enter the item to delete from ADT" << endl;
				cin >> item;
				retval = DPtr->DELETE(item);
				if(retval)
				{
					cout << " Item " << item << " is deleted from ADT " << endl;
				}
				
				break;	
			case 3:
				cout << "Enter the item to search in ADT" << endl;
				cin >> item;
				ftime(&inserttime1);
				retval = DPtr->SEARCH(item);
				ftime(&inserttime2);
				search_totaltime = search_totaltime + ((searchtime2.time - searchtime1.time)*1000 + (searchtime2.millitm - searchtime1.millitm));		
				search_count++;					
				if(retval)
				{
					cout << " Item " << item << " is found in ADT " << endl;
				}
				else
				{
					cout << " Item " << item << " is not found in ADT " << endl;
				}
				break;
			case 4:
				DPtr->DisplayADT();
				break;
			case 5:
				DPtr->ClearADT(); 
				cout << "ADT Cleared " << endl;			
				break;
			case 6:
				cout << "Exiting the Program...." << endl;
				break;
			default:
				cout << "Invalid Option, Enter valid option "<< endl;	
		}
		if(user_option == 6)
		break;
	}		
	if(TimeFlag)
	{	
		//Total Elements Count inserted from Menu	
		if(insert_count > 0)
		{
			cout << "Insert count from Menu is " << insert_count << endl;
		
			if(WriteTimeinfo(timeinsertfile,insert_count,insert_totaltime) == -1)// Check for successful write operation to time file
			{	
				cout << "Failed to write to Time file" << endl;
			}
		}		
		//Total Elements Count Searched from Menu
		if(search_count > 0)
		{
			cout << "Search count from Menu is " << search_count << endl;	

			if(WriteTimeinfo(timesearchfile,search_count,search_totaltime) == -1)// Check for successful write operation to time file
			{	
				cout << "Failed to write to Time file" << endl;
			}
		}	
	}
	
}
/*********************************************************************************************************
Function Name            : WriteTimeinfo 
Inputs                   : (1) Reference to Timing info file
			   (2) Total Number of Elements inserted/searched in ADT
			   (3) Total Time taken to do above operations 
Outputs                  : -1 In case of any error occurrs
                            0 In case of sucessful write
Description              :  This function writes timing information of ADT Operations in to Timing file
*********************************************************************************************************/
int WriteTimeinfo(char *timefile, long total_count, long totaltime)
{

	int 		retval = 0,p1 = 0, no_chars = 0;
	ofstream 	optstrm;
	char		buff[100];
	size_t 		pos=-1;
	
	optstrm.open(timefile, ios::app); /* Opening file in Append Mode*/

	if(!optstrm)
	{
		cout << "Failed to open File " << timefile << endl;		
		retval = -1;
	}
	else
	{			
		//writing value to the output file
		no_chars = sprintf(buff,"%ld",total_count);
		buff[no_chars] = '\0';
		if(optstrm.write(buff,no_chars))
		{
			//For writing blank space after every value is written to the file
			buff[0] = ' ';
			no_chars = 1;
			buff[no_chars] = '\0';
			if(optstrm.write(buff,no_chars))
			{
				no_chars = sprintf(buff,"%ld",totaltime);
				buff[no_chars] = '\0';
				if(optstrm.write(buff,no_chars))
				{
					//For writing blank space after every value is written to the file
					buff[0] = '\n';
					no_chars = 1;
					buff[no_chars] = '\0';
					if(!optstrm.write(buff,no_chars))
					{
						retval = -1;					

					}					
				}
				else
				{
					retval = -1;
				}				
			}
			else
			{
				retval = -1;	
				
			}
		}
		else
		{
			retval = -1;
		}
		optstrm.close();	// Closing output file 	
	}	
	
	return retval;	
}
	

