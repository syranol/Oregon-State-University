/***************************************************************************
*Author: Hsiang LO
*Date: 10/30/2018
*Description: connect to otp_dec_d and will ask it to decrypt ciphertext
*using a passed-in ciphertext and key, and otherwise performs exactly
*like otp_enc, and must be runnable in the same three ways.
*************************************************************************/
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stddef.h>

#define BUFSIZE 80000


//* Function Declarations

//* Set up connectio, return portID
int prepNect(char* portID);


//* Output error message
void error(const char *msg);

//* Use for sending data
void dataToSend(char* f_name, char* f_name2, int socketFD);



int main(int argc, char *argv[])
{


    //Variable declaration
    int socketFD, portNumber, charsWritten, c_Read;
    int status = 1;
    int returnSize;
    int totalSize;
   
    char currentCheck[BUFSIZE];
    
    //* Reference for bzero 
    //* https://stackoverflow.com/questions/3492647/what-are-the-alternative-for-win32-to-alarm-bzero-bcopy-when-porting-c-code
    bzero(currentCheck, sizeof(currentCheck));

    //*  Connection
    socketFD = prepNect(argv[3]);
    

    //* input, inputsize
    int cTxt = open(argv[1], O_RDONLY);
    int cTxtSize = lseek(cTxt, 0 , SEEK_END);
    //* keygen, keygenSize

    int solution = open(argv[2], O_RDONLY);
    int solutionSize = lseek(solution, 0, SEEK_END);


    //* Now intitates different checks

    //* check for input size
    if(solutionSize < cTxtSize)
    {
        fprintf(stderr, "Error: Too Short");
        exit(1);
    }
    
    //* check for invalid characters
    int ifTrue = open(argv[1], 'r');


    while(read(ifTrue, currentCheck, 1) != 0)
    {
        if( isalpha(currentCheck[0]) /* if alpha */ || (currentCheck[0] == '\n') /*if is endl */ || isspace(currentCheck[0]) /*if space */)
        {
            //Do nothing
        }
        else
        {
            fprintf(stderr, "Error: Bad characters \n");
            exit(1);
        }
    }


    //* empty 
    bzero(currentCheck, sizeof(currentCheck));
    
    //* send data 
    dataToSend(argv[1], argv[2], socketFD);
    
    //* return info
    returnSize = recv(socketFD, &totalSize, sizeof(int), 0);
    totalSize = ntohl(totalSize);
    
    //* return msg from server
    c_Read = recv(socketFD, currentCheck, totalSize, 0);
    if (c_Read < 0)
    {
        fprintf(stderr, "Error: reading socket message");
    }
    
    //* read data
    do
    {
        c_Read += recv(socketFD, currentCheck/*from input*/+ c_Read, totalSize, 0);
        if(c_Read < 0)
        {
            fprintf(stderr, "Error: reading socket message");
        }
    }
    while (c_Read != totalSize);



    //* print result
    fprintf(stdout, "%s", currentCheck);
    fprintf(stdout, "\n");
    
    //*Close socket when done
    close(socketFD);
    return 0;
}


/****************************************************************************************
Description: Set connection, return socket file descri
****************************************************************************************/

int prepNect(char* arg3)
{


    //* Variables
    char currentCheck[BUFSIZE];

    /*use ! as check*/ 
    char auth_Connect[] = "!";

    //*struct for server/host
    struct hostent* serverHostInfo;
    //*struct for address
    struct sockaddr_in serverAddress;

    //* variables as defined in teacher's files
    int socketFD,portID,isTrue = 1;
    
    //* conver to int
    //* http://www.cplusplus.com/reference/cstdlib/atoi/
    portID = atoi(arg3);
    
 
    //*setup server
    memset((char*)&serverAddress, '\0', sizeof(serverAddress));


    //* create socket
    serverAddress.sin_family = AF_INET;

    //* store port #
    serverAddress.sin_port = htons(portID);

    //* use localhost 
    serverHostInfo = gethostbyname(/*testing on server*/"localhost");
    
    if(serverHostInfo == NULL)
    {
        fprintf(stderr, "ERROR: no such host\n");
        exit(0);
    }
    
    memcpy((char*) &serverAddress.sin_addr.s_addr, (char*) serverHostInfo->h_addr, serverHostInfo->h_length);
    
    //*set socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);


    //* Ref for reuseaddr 
    //* https://stackoverflow.com/questions/14388706/socket-options-so-reuseaddr-and-so-reuseport-how-do-they-differ-do-they-mean-t

    //*set opt
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR/*refer to above*/, &isTrue, sizeof(int));
    
    //* initiate transmission
    if( connect(socketFD, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0)
    {
        error("ERROR: connect");
    }
    //* send token
    send(socketFD, auth_Connect/*reminder, this is *!* */, sizeof(auth_Connect), 0);

    //* read token
    recv(socketFD, currentCheck, sizeof(currentCheck), 0);


    //* check for validation

    if(strcmp(currentCheck, "!"/*checks for ! */) != 0)
    {
        fprintf(stderr, "Error: connection rejected");
        exit(2);
    }
    else if(strcmp(currentCheck, "+"/*checks for + */) == 0)
    {
        fprintf(stderr, "Error: connection rejected");
        exit(2);
    }

    return socketFD;
}
/****************************************************************************************
Description: Output error message from client.c and exit
****************************************************************************************/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}


/****************************************************************************************
Description: Take data file and socket, cat data and send data
****************************************************************************************/

void dataToSend(char* f_name, char* f_name2, int socketFD)
{

    //* Data variables
    char currentCheck[BUFSIZE];
    char currentCheck2[BUFSIZE];

    //* Reference for bzero 
    //* https://stackoverflow.com/questions/3492647/what-are-the-alternative-for-win32-to-alarm-bzero-bcopy-when-porting-c-code
    bzero(currentCheck, sizeof(currentCheck));
    
    bzero(currentCheck2, sizeof(currentCheck2));
    
    //* Variables for msg and len
    int  lenMsg;
    size_t lenSize;
    


    //* Read Data For file 1
    FILE* fileName= fopen(f_name, "r");
    if(fileName != NULL)
    {
        lenSize = fread(currentCheck/*read data of size char*/, sizeof(char), BUFSIZE, fileName);
        
        if(ferror(fileName) != 0)
        {
            fprintf(stderr, "Error: reading file1");
        }

        currentCheck[--lenSize/*lens size*/] = '*';
        lenSize++;
    }
    fclose(fileName);
    
    //* Read Data For file 2
    fileName = fopen(f_name2, "r");
    if(fileName != NULL)
    {
        lenSize += fread(currentCheck2/*read data of size char*/, sizeof(char), BUFSIZE-1, fileName);
        
        if(ferror(fileName) != 0)
        {
            fprintf(stderr, "Error: reading file2");
        }
        
        currentCheck[--lenSize/*lens size*/] = '\0';
    }
    fclose(fileName);


    //* now that all the data have been loaded, concenate the set of data
    
    //* cat data
    strncat(currentCheck, currentCheck2, strlen(currentCheck2));

    
    //* send size
    lenMsg = strlen(currentCheck);

    //* htonl reference
    //https://stackoverflow.com/questions/36924598/understanding-htonl-and-ntohl
    lenMsg = htonl(lenMsg);
    send(socketFD, &lenMsg, sizeof(lenMsg), 0);
    
    int charsWritten = 0;

    while(strlen(currentCheck) != charsWritten )
    {
        charsWritten += send(socketFD, currentCheck,/*length of currentCheck */ strlen(currentCheck), 0);
    }
    
    //* Output error messages if neccessary
    if(charsWritten < 0)
    {
        fprintf(stderr, "Error: Socket Write");
    }
    if(strlen(currentCheck) > charsWritten)
    {
        fprintf(stderr, "Error:  Missing data from write");
    }
    
    return;
}

