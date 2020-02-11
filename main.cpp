/*
Goal of program to a create a word search program that will find a word within the matrix.
    1. After finding the word return the column and row location and also return the direction that the word ends in.
    2. There will be two functions that will:
        1. return true when the there is the 2nd character of the word in the surrounding 8 location around the currPos
        2. return false and let the program continue searching for the word
        3. Search through the matrix row by row
    3. Read through a txt file, find the vector size,  and put them on a vector. After finding the movie print them out. AT THE END.
        1. If all movies are found say all movies are found.
    4. If there is a movie that is still in the list push them onto a vector and then print the movies out AT THE END.
*/

#include <iostream>
#include <vector>
#include <fstream> // for ifstream
#include <cstdlib> // for c_str() to read filenames

int MATRIX_HEIGHT = 0;
int MATRIX_WIDTH = 0;

using namespace std;

// Creates the matrix
vector<vector<char> > createMatrix(ifstream &sinput)
{
    vector<vector<char> > tempMatrix(MATRIX_HEIGHT, vector<char> (MATRIX_WIDTH, 'a'));
    for(int i = 0; i < MATRIX_HEIGHT; i++)
    {
        for(int j = 0; j < MATRIX_WIDTH; j++)
        {
            sinput >> tempMatrix[i][j];
        }
    }
    return tempMatrix;
} // end createMatrix
// opens file, checks if opened, gets the width and height and then create a matrix
vector<vector <char> > setupPuzzle()
{
    vector<vector <char> > newMatrix;
    ifstream sinput;
    string filename = "testTxt.txt";
    sinput.open(filename.c_str());

    if (sinput.is_open())   // the file opened just fine
    {
        // debug code cout << "Opened, current height is " << HEIGHT << endl;
        sinput >> MATRIX_HEIGHT >> MATRIX_WIDTH;
        // debug code cout << "Curr height/width after reading it in is: " << HEIGHT << " " << WIDTH << endl;
        newMatrix = createMatrix(sinput); // passes in the ifstream so that file can be continue to be read
        sinput.close(); // closes ifstream
    }
    else
    { // error checking
        cout << "File '" << filename << "' did not open." << endl;
        exit(0);
    }
    return newMatrix;
} // end setupPuzzle
// output the matrix for testing and debugging
void renderMatrix(vector<vector<char> > &currVect)
{
    for(int height = 0; height < MATRIX_HEIGHT; height++)
    {
        for(int width = 0; width < MATRIX_WIDTH; width++)
        {
            cout << currVect[height][width] << " ";
        }
        cout << endl;
    }
} // end renderMatrix
vector <vector<string> > searchMatrix (int height, int width)
{

} // end searchMatrix
// checks for the surrounding 8 indices around the cell, also makes sure we don't go out of bounds of the vector
vector <vector<string> > checkNeighbors(vector<vector <char> > puzzle, int currHeight, int currWidth)
{
    for(int height = currHeight - 1; height < currHeight + 2; height++){
        if(height < 0 || height >= MATRIX_HEIGHT) { // check to see if we go out of bounds
            continue;
        }
        for(int width = currWidth - 1; width < currWidth + 2; width++) {
            if(width < 0 || width >= MATRIX_WIDTH) { // checks for out of bounds
                continue;
            }
            if(width == currWidth && height == currHeight) { // make sure we're not including ourselves
                continue;
            }
            // if(puzzle[height][width] == ") think on this later
        }
    }
} // end checkNeighbors

vector <vector<string> > continueCheck(int currHeight, int currWidth)
{

} // end continueCheck
int main()
{
    // creates a board
    vector<vector<char> > newMatrix = setupPuzzle();
    renderMatrix(newMatrix);
    return 0;
}
