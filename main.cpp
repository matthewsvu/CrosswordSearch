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

    Change log 1 2/11/20:
        1. Initial commit, added methods create matrix, setupPuzzle, renderPuzzle, checkNeighbors, continue check;
        2. Added functionality for a outputting the matrix
        3. finished createMatrix method
        4. finished setupPuzzle method, added functionality for filename errors.
        5. finished insertMovies

    Notes:
        I need to figure out the logic of searching the vectors and finish the searchMovies function.
        I'm having trouble keeping it together i.e the logic. I may need to condense everything and encapsulate them.
*/

#include <iostream>
#include <vector>
#include <fstream> // for ifstream
#include <cstdlib> // for c_str() to read filenames

int MATRIX_HEIGHT = 0;
int MATRIX_WIDTH = 0;
const int FIRST_LETTER = 0;
const int SECOND_LETTER = 1;

using namespace std;

enum direction {N, NE, E, SE, S, SW, W, NW};
vector <string> movieList;

// insertMovies into a vector string.
void insertMovies(ifstream &file)
{
    string input;
    if(file)
    {
        while(getline(file, input))
        {
            movieList.push_back(input);
           // cout << input << " ";
        }
    }
    for(int i = 0; i < movieList.size(); i++) {
        cout << movieList[i][0] << endl;
    }
}
void searchMovies() {

}
// Creates the matrix
vector<vector<char> > createMatrix(ifstream &sinput)
{
    vector<vector<char> > tempMatrix(MATRIX_HEIGHT, vector<char> (MATRIX_WIDTH, 'a')); // initializes vector with 'a' in all locations
    for(int i = 0; i < MATRIX_HEIGHT; i++)
    {
        for(int j = 0; j < MATRIX_WIDTH; j++)
        {
            sinput >> tempMatrix[i][j];
            cout << tempMatrix[i][j] << " "; // outputs the matrix for debugging
        }
        cout << endl;
    }
    cout << endl;
    return tempMatrix;
} // end createMatrix
// opens file, checks if opened, gets the width and height and then create a matrix
vector<vector <char> > setupPuzzle()
{
    vector<vector <char> > newPuzzle;
    ifstream sinput;
    string filename;

    while(true) // loop until valid filename is entered
    {
        cout << "What is the name of your file?\n";
        cin >> filename; // user input for filename

        sinput.open(filename.c_str());

        if (sinput.is_open())   // the file opened just fine
        {
            // debug code cout << "Opened, current height is " << MATRIX_HEIGHT << endl;
            sinput >> MATRIX_HEIGHT >> MATRIX_WIDTH; // read in height and width
            cout << "Current height/width after reading it in is: " << MATRIX_HEIGHT << " " << MATRIX_WIDTH << endl << endl;
            newPuzzle = createMatrix(sinput); // passes in the ifstream so that file can be continue to be read
            insertMovies(sinput); // passes if stream so that movies can be added to a vector
            sinput.close(); // closes ifstream
            break;
        }
        else
        {
            // error checking
            cout << "File '" << filename << "' could not be found or opened" << endl;
            continue; //
        }
    }
    return newPuzzle;
} // end setupPuzzle
// output the matrix for testing and debugging
void renderPuzzle(vector<vector<char> > &currVect)
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
bool continueCheck(int currHeight, int currWidth)
{
    return false;
} // end continueCheck
// checks for the surrounding 8 indices around the cell, also makes sure we don't go out of bounds of the vector
bool checkNeighbors(vector<vector <char> > puzzle, int currHeight, int currWidth, int currWordPos)
{
    for(int height = currHeight - 1; height < currHeight + 2; height++)
    {
        if(height < 0 || height >= MATRIX_HEIGHT)   // check to see if we go out of bounds
        {
            continue;
        }
        for(int width = currWidth - 1; width < currWidth + 2; width++)
        {
            if(width < 0 || width >= MATRIX_WIDTH)   // checks for out of bounds
            {
                continue;
            }
            if(width == currWidth && height == currHeight)   // make sure we're not including ourselves
            {
                continue;
            }
            if(puzzle[height][width] == movieList[currWordPos][SECOND_LETTER]) { // when we find the 2nd letter, go in a straight line and search
                continueCheck(currHeight, currWidth);
            }
        }
    }
    return false;
} // end checkNeighbors
// loop through puzzle row by row until it finds a word
void searchMatrix (vector<vector<char> > puzzle, int currWordPos)
{
    for(int height = 0; height < MATRIX_HEIGHT; height++)
    {
        for(int width = 0; width < MATRIX_WIDTH; width++) {
            if(puzzle[height][width] == movieList[currWordPos][FIRST_LETTER]) {
                checkNeighbors(puzzle, height, width, currWordPos);
            }
        }
    }
} // end searchMatrix
int main()
{
    // creates a board
    vector <vector<char> > wordPuzzle = setupPuzzle();

    return 0;
}
