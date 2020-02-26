//
//  main.cpp
//  StoogeSortFn
//
//  Created by Sean Lo on 7/8/18.
//  Copyright © 2018 Sean Lo. All rights reserved.
//
//Source COde Referenced from wikipedia and is also provided by the teacher
//from the homework assignment. Also from
// rosettacode.org/wiki/Sorting_algorithms/Stooge_sort#C.2B.2B as well

#include <iostream>
#include <fstream>

using namespace std;

/**********************************************************************************************************
 * Description: THe purpose of this class is to sort the given data in a "stooge" algorithm. This algorithm
 * is referenced from an online source that is written above
 *********************************************************************************************************/

class stooge
{
public:
    void sort( int* arr, int start, int end )
    {
        if( arr[start] > arr[end - 1] ) swap( arr[start], arr[end - 1] );
        int n = end - start; if( n > 2 )
        {
            n /= 3; sort( arr, start, end - n );
            sort( arr, start + n, end ); sort( arr, start, end - n );
        }
    }
};

/**********************************************************************************************************
 * Description: This is the main menu that receive input, process it through the file and output the result
 *********************************************************************************************************/
 
 int main( int argc, char* argv[] )
{
    stooge s;
    
    int count; //Number of input
    int i = 0; //current counter
    int currentInput;
    
    //read input from file name "data.txt"
    ifstream inFile;
    ofstream outFile;
    
    inFile.open("data.txt");
    inFile >> count;
    
    int arr[count];
    if(inFile)
    {
        while( i < count)
        {
            inFile >> currentInput;
            arr[i] = currentInput;
            i++;
        }
    }
    inFile.close();
    
    s.sort(arr, 0, count);
    
    outFile.open("stooge.out");
    
    for (i = 0; i < count; i++)
    {
        outFile << arr[i] << " ";
    }
    outFile << '\n';
    
    outFile.close();
    
    return 0;
}

