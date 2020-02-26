/***************************************************************************
*Author: Hsiang LO
*Date: 10/30/2018
*Description: connect to otp_dec_d and will ask it to decrypt ciphertext
*using a passed-in ciphertext and TBox, and otherwise performs exactly
*like otp_enc, and must be runnable in the same three ways.
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 80000

//* Declarations of function

//* Error 
void error(const char *msg, int num);
//* Listener for connect
int tListen(char* arg1, int nConnect_S);

//* process messages
void c_Process(int con_Port);


void genText(char** p_Text, char**TBox);

//* Use to convert char to num
char conv_To_AZ(int i);
//* Use to convert char to num
int conv_To_Num(char c);


/*********************************************************************************************
Description: Main funciton that will initiate operation/fork
********************************************************************************************/
int main (int argc, char *argv[])
{
    int procSocket,e_FD;
    struct sockaddr_in clientAddress;


    socklen_t c_SizeP;
    pid_t pid;
    

    //* Set up tListen
    procSocket = tListen(argv[1], 5);
    
    
    //* Fork new proc 
    while(1)
    {
        //* connect and block non-avaialble one


        c_SizeP = sizeof(clientAddress);


        e_FD = accept(procSocket/*current socket */, (struct sockaddr *) &clientAddress, &c_SizeP /*size of proc*/);
        
        //* if < 0, indicating error
        if(e_FD < 0)
        {
            //*output error msg
            error("ERROR: connection", 1);
        }
        



        //* fork process
        pid = fork();
        switch(pid)
        {
            //* if error
            case -1:
                error("Error: Fork", 1);
                break;

            //* else pass to child
            case 0:
                c_Process(e_FD);
                exit(0);
                break;
        }
        
        //* close port
        close(e_FD);
        
    }
    
    //* socket
    close(procSocket);
    return 0;
}



/******************************************************************************************************************
Description: set connections/sockets, return file descri
 *******************************************************************************************************************/
int tListen(char* arg1, int nConnect_S)
{
    struct sockaddr_in serverAddress;
    int pNum,
    procSocket,
    isTrue = 1;
    
    
    //* Create socket
    procSocket = socket(AF_INET, SOCK_STREAM, 0);
    

    //*Ref for socket ifno
    //https://stackoverflow.com/questions/14388706/socket-options-so-reuseaddr-and-so-reuseport-how-do-they-differ-do-they-mean-t
    setsockopt(procSocket/*proc socket*/, SOL_SOCKET, SO_REUSEADDR, &isTrue, sizeof(int));
    

    //* Set address
    memset((char *)&serverAddress, '\0', sizeof(serverAddress));

    //* Atoi ref 
    //* http://www.cplusplus.com/reference/cstdlib/atoi/

    pNum = atoi(arg1);

    //* create socket
    serverAddress.sin_family = AF_INET;

    //* Set Num
    serverAddress.sin_port = htons(pNum);


    //* ANY CAN ACCESS
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    
    if(bind(procSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        error("ERROR: Connect", 1);
    }
    
    listen(procSocket, nConnect_S);
    
    return procSocket;
}



/*******************************************************************************************
Description: Encrypt using keyGen
 ********************************************************************************************/
void genText(char** p_Text, char**TBox)
{
    int i, textNum, TBoxNum, eNum, l_of_txt;
    
    //* get msg
    l_of_txt = (strlen(*p_Text));


    for (i = 0; i < l_of_txt; i++)
    {
        //* get decoded Num
        textNum = conv_To_Num/*double ptr for ref/hash*/(*(*p_Text + i));
        TBoxNum = conv_To_Num/*double ptr for ref/hash */(*(*TBox + i));
        
        eNum = ((textNum + TBoxNum) % 27);
        

        //*return hash
        *(*p_Text + i) = conv_To_AZ(eNum);
        
    }
    
    //* Add endl

    *(*p_Text + i) = '\0';
    
    return;
}



/*********************************************
Description: Convert to character
 **********************************************/
char conv_To_AZ(int i)
{
    //* library
    static const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

    if( (i < 0) || (i > 27) )
    {
        fprintf(stderr, "Error: unknown char");
    }
    
    return chars[i];
}


/*********************************************
Description: Conver to num
 **********************************************/
int conv_To_Num(char c)
{
    int i;

    //* library
    static const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    
    for (i = 0; i < 27; i++)
    {
        if(c == chars[i])
        {
            return i;
        }
    }

    //* else return error
    fprintf(stderr, "Error: unknown char");
}



/*****************************************************************************************************************************
Description: process erro rmessages and exit #
 ******************************************************************************************************************************/
void error(const char *msg, int num)
{
    //* error msg
    perror(msg);

    //* exit
    exit(num);
}


/*****************************************************************************************************************************
Description: Accept, Encrypt and send data to opt_enc
 ******************************************************************************************************************************/
void c_Process(int con_Port)
{
    //* Predefined variables needed for the function
    int i, status, cInput, mSize, nBytes;

    char* TBox;
    char* incomeM;
    char* iPort;
    
    char currentCheck[BUFSIZE];
    
    //* Reference for bzero 
    //* https://stackoverflow.com/questions/3492647/what-are-the-alternative-for-win32-to-alarm-bzero-bcopy-when-porting-c-code
    bzero(currentCheck, sizeof(currentCheck));
    
    
    recv(con_Port/*port to connect*/, currentCheck, sizeof(currentCheck) -1, 0);


    //* set auth to _
    if(strcmp(currentCheck, "_") == 0)
    {
        char auth[] = "_";

        send(con_Port/*port*/, auth, sizeof(auth), 0);
    }
    //* Set auth to +
    else if(strcmp(currentCheck, "!") == 0)
    {
        
        char auth[] = "+";

        send(con_Port/*port*/, auth, sizeof(auth), 0);
    }
    //* Set auth to ?
    else
    {
        char auth[] = "?";

        send(con_Port/*port*/, auth, sizeof(auth), 0);
    }
    
    //* Now we need to reset

    //* Ref - ntohl
    //* https://stackoverflow.com/questions/36924598/understanding-htonl-and-ntohl


    //* Empty current checker
    bzero(currentCheck, sizeof(currentCheck));
    
    
    //* get # bytes
    status = recv(con_Port, &nBytes/*Number of bytes*/, sizeof(int), 0);
    nBytes = ntohl(nBytes);
    

    //* receive msgs
    cInput = recv(con_Port, currentCheck, nBytes, 0);
    if (cInput < 0)
    {
        fprintf(stderr, "Error: Socket Read");
    }
    
    //* read data
    while (cInput != nBytes)
    {
        cInput += recv(con_Port,/*add these two form input + char conv*/ currentCheck + cInput, nBytes, 0);
        if (cInput < 0)
        {
            fprintf(stderr, "Error:  Socket Read");
        }
    }
    
    
    //tokenize the recv data into incomeM and TBox by using *
    incomeM = strtok_r(currentCheck, "*", &iPort);
    TBox = strtok_r(NULL, "*", &iPort);
    
    
    //encrypt the incomeM using the TBox
    genText(&incomeM, &TBox);
    
    
    //* return data
    //* Ref - ntohl
    //* https://stackoverflow.com/questions/36924598/understanding-htonl-and-ntohl

    mSize = strlen(incomeM);
    mSize = htonl(mSize);

    send(con_Port, &mSize/*return size of msg for accurary*/, sizeof(mSize), 0);
    

    //* then send msg
    int cWrote = 0;
    while(cWrote != strlen(incomeM))
    {
        cWrote += send(con_Port, incomeM/*this is actual msg now*/, strlen(incomeM), 0);
    } 
    
    //* if less than 0
    if(cWrote < 0)
    {
        fprintf(stderr,  "Error: Socket Read");
    }
    //* if less than received 
    if(cWrote < strlen(incomeM))
    {
        fprintf(stderr, "Error: Data not 100% transmitted");
    }
    
    return;
}
