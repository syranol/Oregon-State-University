/***************************************************************************
*Author: Hsiang LO
*Date: 10/30/2018
*Description: Generate a key, as to protect the true data
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{
  srand(time(NULL));


  int pick;
  int key_Size;
  int i;

  //* Ref for atoi
  //* http://www.cplusplus.com/reference/cstdlib/atoi/


  key_Size = atoi(argv[1]);


  //* ref for gen
  //* https://stackoverflow.com/questions/11845157/how-to-create-serials-key-to-protect-an-application
  for(i = 0; i < key_Size; i++)
  {
    pick = 65 + (rand() % 27);

    if(pick == 91)
    {
      pick = 32;
    }

    //*output result
    fprintf(stdout, "%c", pick);
  }



  //* add new line
  fprintf(stdout, "\n");

  return 0;
}