/*******************************************************************************************
* Author: Hsiang Lo
* Date 11/14/2018
* Description: This is the shell i built for program3. it supports all the functionalities
* as specified in the description including but not limited to cd, exit and ls. It also test
* for  < > $$ and comments too. The way the comments are formatted are as follows,
* refrences will be stated, comments above a line is for reading and comments on the 
* line of code is for interpretation. to run the program type gcc -o smallsh smallsh.
********************************************************************************/
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>

//* global variable for checking foreground
int fore_only;
int exitStat;
pid_t newPid, origPid,pid;
//fore_only changed, nothing else



/* REFERENCE - https://stackoverflow.com/questions/26371610/sigtstp-signal-handler-for-child-process
http://condor.depaul.edu/glancast/374class/docs/shlab_step3.html
*******************************************************************************************
* Description: Handles all SINGALS
********************************************************************************/
void SIGTSTP_handler(int signal)
{
	//* if foreground 
	if (fore_only == 0)
	{
		//* Set to 1
		fore_only = 1;
		//* Display message
		char* outMesg = "\nEntering foreground-only mode (& is now ignored) \n";
		write(STDOUT_FILENO, outMesg, strlen(outMesg)); //write it out to the screen
		fflush(stdout);
	}
	//* otherwise do
	else
	{
		fore_only = 0;
		char* outMesg = "\nExiting foreground-only mode \n";
		write(STDOUT_FILENO, outMesg, strlen(outMesg ));
		fflush(stdout);

	}
}

//(*reference - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux-c
/*******************************************************************************************
* Description: This changes directory
********************************************************************************/
void changeDirectory(char* args[])
{
	//* Set to hoem direct ory if no options provided
	if(args[1] == NULL)
	{
		chdir(getenv("HOME"));
	}
	else
	{
		chdir(args[1]);
	}
}
/*******************************************************************************************
* Description: This checks and end proccesses as well as output messages
********************************************************************************/
void endBackground()
{
	//* kill process
	pid = wait(&exitStat);
		
	//* kill signal
	if(exitStat > 0)
	{
		printf("background pid %d is done: terminated by signal %d\n", origPid, exitStat);
		fflush(stdout);
				
		exitStat = -5;
	}	
	//* exit as normal
	else if(exitStat == 0)
	{
		printf("background pid %d is done: exit value %d\n", origPid, exitStat);
		fflush(stdout);
				
		exitStat = -5;
	}
	
}
int main()
{
	//*Variables to check if exit
	int doneExit = 0;
	//* variable to check if child end exit
	int c_doneExit = -5;
	//*variable to check the source
	int source = -5;
	//* variable to check if a new proc
	int newPlace = -5;
	//* variable to check redirect
	int isRedirect = 0;

	exitStat = -5;
	int numCharsEntered, status = -5;

	//*set value for global variable
	newPid = -5;
	pid=-5;
	origPid = -5;
	fore_only = 0;
	
	//*Coutner for proccess
	int processes[20];
	int processCounter = 0;
	//* variables for token/proccee/storage
	char *input = NULL, *token;
	char* args[513], lineInput[2048];




	//* Signal Struct Declaration - sigint & SIGTSTP
	struct sigaction sigint_action = {0}, SIGTSTP_action = {0};

	//* Ignore SIG_IGN from parent

	sigint_action.sa_handler = SIG_IGN;

	//* handler set ->
	SIGTSTP_action.sa_handler = &SIGTSTP_handler;
	//* if finish, restart
	SIGTSTP_action.sa_flags = SA_RESTART; 

	//* Initialize
	sigaction(SIGINT, &sigint_action, NULL);

	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
	
	int i, j;
	int placeHold = 0;
	int background = 0;

	while(numCharsEntered == 1 || !doneExit)
	{
		//* background process check
		placeHold = waitpid(origPid, &exitStat, WNOHANG);
		//* if status changed
		if(placeHold != 0)
		{
			//* initial the end messages
			endBackground();
		}

		//* IF flags are changed, reset them

		int resetValueOne = 0;
		int resetValueTwo = -5;

		// -5 os set to source/place

		isRedirect = resetValueOne;
		source = resetValueTwo;

		// 0 is set for redirect/bg
		newPlace = resetValueTwo;
		background = resetValueOne;

		// free previous input/intake
		if(input)
		{
			free(input);
			input = NULL;
		}
		
		//* PROMPT for th ebash 
		printf(": ");

		//* Flush after print 
		fflush(stdout);
	

		//* get the user input
		size_t inputSize = sizeof(input);
		//* Get use input
		numCharsEntered = getline(&input, &inputSize, stdin);
		

		//* if -1 clear stdin err
		if(numCharsEntered == -1)
		{
			clearerr(stdin);
		}

		//* All initializtation knowledge has been established from this point forward

		//* If user wangs to exit
		if(input && strcmp(input, "exit\n") == 0)
		{
			for(i = 0; i < processCounter; i++)
			{
				kill(processes[i], 15);
			}
			//set exit flag
			doneExit = 1;
		}
		
		//* Initiate bash run
		if(numCharsEntered > 1 && !doneExit)
		{
			//remove endl
			input[numCharsEntered - 1] = '\0';

			//split into tokens
			strcpy(lineInput, input); //move input into toToken to preserve input
			
			//* Source: http://www.cplusplus.com/reference/cstring/strtok/

			//* this will only receive the first token
			token = strtok(lineInput, " ");
			//* set i to 0
			i = 0;
			
			//input/initialize args through token
			do
			{
				args[i] = token;
				//Strtok usage is referenced from above
				token = strtok(NULL, " ");
				i++;
			}
			while(token != NULL);
			
			//* Redirection checks 
			for(j = 0; j < i; j++)
			{
				//* Indicates handles of input operation
				if(strcmp(args[j], "<") == 0)
				{
					isRedirect = 1;
					source = j + 1;
				}
				
				//* Indicate handles of output operation
				else if(strcmp(args[j], ">") == 0)
				{
					isRedirect = 1;
					newPlace = j + 1;
				}
				
				//* Checks for any other commands or arguments
				else if(strstr(args[j], "$$") != NULL)
				{
					//* strlen arg
					char tempS[strlen(args[j])];


					//* Create a new string 
					char *cTemp;

				
					//* point to the location of the command/arg
					cTemp = strstr(args[j], "$$");

					//* copy information 
					strncpy(tempS, args[j]/*Make sure its args at j*/, cTemp - args[j]); 
					tempS[cTemp -/*Again, make sure its args at j*/ args[j]] = '\0';
					//* Resource - https://blog.udemy.com/sprintf-c/
					sprintf(tempS+(cTemp - /*Again, make sure its args at j*/ args[j]), "%d%s", getpid(), cTemp + 2); //rebuild string with pid in place of '$$'
					
					//* Restructure string so it fits the proper format
					strcpy(args[j], tempS); //move back into args[j]
				}
			}
						
			//* If command by user is status
			if(strcmp(args[0], "status") == 0)
			{
				if(status == 0 || status == 1)
				{
					printf("exit value %d\n", status);
					fflush(stdout);
				}
				//* otherwise print the signal 
				else
				{
					printf("terminated by signal %d\n", status);
					fflush(stdout);
				}
			}
			//* checks for comments
			else if(input[0] == '#')
			{
				//* DO NOTHING IF THE CASE
			}
			//* If command by user is cd -> swithc into changeDirectory
			else if(strcmp(args[0], "cd") == 0)
			{
				changeDirectory(args);
			}
			//* Indicate shandles of running proccesses in the background
			else if(strcmp(args[i - 1], "&") == 0)
			{
				background = 1;
				args[i - 1] = NULL;
			}

			else
			{
				newPid = fork(); //create new process
				
				//* Reference for switch/proccess
				//* https://stackoverflow.com/questions/23036475/program-of-forking-processes-using-switch-statement-in-c
				switch(newPid)
				{
					case -1:
					{
						//* if error
						perror("Error spawning\n");
						//* return -1
						exit(1);
						//* break;
						break;
					}
					//* child code
					case 0: 
					{	
						//check for background process req
						if(background)
						{	
							//check if specified
							if(!fore_only)
							{
								//*ignore from parent
								struct sigaction sigint_new_action = {0};

								//* Set to null
								sigint_new_action/*make sure its the new action*/.sa_handler = SIG_IGN;

								//* Set to null 
								sigaction(SIGINT, &sigint_new_action, NULL);	
								//* direct to null R
								if(source < 0)
								{
									int preQUOTE = open("/dev/null", O_RDONLY);
									int souRef = dup2(preQUOTE, 0);
								}
								//* direct to null W
								if(newPlace < 0)
								{
									int preQUOTE = open("/dev/null", O_WRONLY);
									int desRef = dup2(preQUOTE, 1);
								}
							}
						}
						//* Else if not background
						else if(!background)
						{
							//* terminates
							struct sigaction sigint_new_action = {0};

							//* set to null
							sigint_new_action./*make sure its the new action*/sa_handler = SIG_DFL;

							//* Then set to null 
							sigaction(SIGINT, &sigint_new_action, NULL);
						}
						//* if it is a redirection 
						if(isRedirect)
						{
							//* IF SOURCE AINT NULL
							if(source >= 0)
							{

								int souDirect = open(args[source],/*If source, then read only */ O_RDONLY);

								// RESOURCE https://stackoverflow.com/questions/3642732/using-dup2-for-piping
								int sourceResult = dup2(souDirect, 0); //redirect stdin
								
								if(sourceResult == -1) //if bad file
								{

									//* Print error message
									printf("cannot open %s for input\n", args[source]);
									//* SOURCE - http://www.cplusplus.com/reference/cstdio/fflush/
									fflush(stdout);
									exit(1);
								}
								
								//* REPLACE W/ NULL
								args[source - 1] = NULL;

								//* CLOSE FILE
								close(souDirect);
							}
							//* IF OUTPUT AINTNULL
							if(newPlace >= 0)
							{
								//* Print error message
								int desDirect = open(args[newPlace],/*If output, then write too */ O_WRONLY | O_CREAT| O_TRUNC,0644);
					
								//* RESOURCE https://stackoverflow.com/questions/3642732/using-dup2-for-piping
								int destResult = dup2(desDirect, 1);
								
								if(destResult == -1)//if bad file
								{
									//* Output error message
									perror("no such file or directory");

									//SOURCE - http://www.cplusplus.com/reference/cstdio/fflush/
									fflush(stdout);

									exit(1);
								}
								
								//* REPLACE W/ NULL
								args[newPlace - 1] = NULL;

								//* CLOSE FILE
								close(desDirect);
							}
						}
						

						//Resource - https://stackoverflow.com/questions/27541910/how-to-use-execvp
						args[i] = NULL;
						
						//*EXAMPLE 
						/* 
								char *cmd = "ls";
								char *argv[3];
								argv[0] = "ls";
								argv[1] = "-la";
								argv[2] = NULL;

								execvp(cmd, argv); //This will run "ls -la" as i
						*/
						execvp(*args, args);
						
						//* if error
						perror(input);
						
						fflush(stdout);

						//* exit
						exit(1);
						
						break;
					}


					//* parent  proccess
					default:
					{

						//* check if background
						if(background)
						{
							//* make sure not prev process 
							if(!fore_only)
							{
								//* reset the background flag
								background = 0;
								processCounter++;


								//* store orig pid
								origPid = newPid;

								//* process at counter = original id
								processes[processCounter] = origPid;

								//* print the background pid
								printf("background pid is %d\n", newPid);
								fflush(stdout);
							}		
						}
						else
						{
							//* PID FOR NEW CHILD PROCEES
							newPid = waitpid(newPid, /* make sure this is child one*/&c_doneExit, 0);
							

							//* if child is killed
							if(WIFSIGNALED(c_doneExit) != 0)
							{
								//* status
								status = WTERMSIG/*status for child*/(c_doneExit);


								//* print terminating message
								printf("terminated by signal %d\n", status);
								fflush(stdout);
							}

							//* if normal output
							else if(WIFEXITED/*status for child*/(c_doneExit))
							{

								status = WEXITSTATUS/*status for child*/(c_doneExit);	
							}
						}
						break;
					}
				}
			}

			//* RESET 
			for(j = 0; j < i; j++)
			{
				//* THIS IS VALID SINCE 
				//* REFERENCE https://stackoverflow.com/questions/271588/passing-null-arguments-to-c-sharp-methods
				args[j] = NULL;
			}
		}
	}

	return 0;
}
