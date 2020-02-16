/*
Name: Matthew Vu
Crossword search
HW2-CS1337-Matthew-Vu
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
    Change log 2/12/20:
        1. added skipanycommentLines method
        2. Crudely implemented continueCheck method, searches in one direction, need to check for spaces in words
        3. added directions integers
    Change log 2/14/20:
        1. finished continueCheck method
        2. finished findDirection and findLocation method
        3. added functions for restarting the puzzle
        4. Added ability to add another puzzle to solve
        5. finished game.

    Notes:
        I need to figure out the logic of searching the vectors and finish the searchMovies function.
        I'm having trouble keeping it together i.e the logic. I may need to condense everything and encapsulate them.
*/

/**
MAJOR ADDITIONS AND KEY WORDS INCLUDE:
    1. Ability to enter another puzzle at the end
    2. Checked words by continuing in one direction till it hits a boundary or finds entire word
    3. Used a giant switch statement for the continue checking direction function
    4. NAME OF PUZZLE FILES ARE testPartial.txt and testTxt.txt AND family.txt ANOTHER crossword puzzle made on my own.
**/

#include <iostream>
#include <vector>
#include <fstream> // for ifstream
#include <cstdlib> // for c_str() to read filenames

using namespace std;
// height and width that will be read from in the file
int MATRIX_HEIGHT = 0;
int MATRIX_WIDTH = 0;
// this will be incremented whenever the matrix is done searching for a particular word
int CURR_WORD_POS = 0;
// used to help identify what letter we're currently on
const int FIRST_LETTER = 0;
const int SECOND_LETTER = 1;
const int THIRD_LETTER = 2;
// cases for directions
const int NORTH_WEST = 1;
const int NORTH = 2;
const int NORTH_EAST = 3;
const int WEST = 4;
const int EAST = 5;
const int SOUTH_WEST = 6;
const int SOUTH = 7;
const int SOUTH_EAST = 8;
// bool that is set to true when a word is found
bool wordFound = false;
// List of movies intially
vector <string> movieList;
// list of movies not founds to be printed last
vector <string> moviesNotFound;
// Initial locations of the movies found
vector <vector<int> > locations;

void resetPuzzle(); // will be called from start function
// is called when the user wants to enter in another puzzle.
// resets all relevant variables, and clears screen
void setup()
{
    system("cls");
    // to make sure that an item is found straight off the bat
    wordFound = false;
    // clears all vectors of items.
    movieList.clear();
    moviesNotFound.clear();
    CURR_WORD_POS = 0; // sets it back to zero so that we read the matrix from the first letter again.
} // end setup
void skipAnyCommentLines(ifstream &infile)
{
    string textLine;
    char ch;
    ch = infile.peek();
    while (ch == '#' || ch == '\n')
    {
        getline(infile,textLine);
        ch = infile.peek();
    }
    //when we leave, we're at a non-comment, non-blank line
} //skipAnyCommentLines
// insertMovies into a vector string.
void insertMovies(ifstream &file)
{
    string input; // the input buffer?

    if(file)
    {
        while(getline(file, input)) // gets each line from the file
        {
            skipAnyCommentLines(file); // takes out comments and newlines
            movieList.push_back(input); // adds the movie to the list if it's a valid input
            // cout << input << " ";
        }
    }
    /* Debug code, prints out all movies
    for(int i = 0; i < movieList.size(); i++)
    {
        cout << movieList[i] << endl;
    }
    */
} // end insert movies
// Search and print the vector of notFoundmovies. O(n)
void printNotFound()
{
    cout << "\n\nMovies not found: \n";
    for(int k = 0; k < moviesNotFound.size(); k++)
    {
        cout << moviesNotFound[k] << endl;
    }
    cout << endl;
} // end printNotFound
// Creates the matrix and reads in data from input file
vector<vector<char> > createMatrix(ifstream &sinput)
{
    vector<vector<char> > tempMatrix(MATRIX_HEIGHT, vector<char> (MATRIX_WIDTH, 'a')); // initializes vector with 'a' in all locations
    for(int i = 0; i < MATRIX_HEIGHT; i++)
    {
        for(int j = 0; j < MATRIX_WIDTH; j++)
        {
            skipAnyCommentLines(sinput); // skips lines before reading in input.
            sinput >> tempMatrix[i][j];
        }
    }
    cout << endl;
    return tempMatrix;
} // end createMatrix
void drawTopOrBottomLine()
{
    for(int j = 0; j < MATRIX_WIDTH; j++)
    {
        cout << "-" << "  ";
    }
} // end drawTopOrBottomLine
// draws a top border which helps user solve word puzzle on the x axis
void createTopBorder()
{
    for(int rowNum = 1; rowNum <= MATRIX_WIDTH; rowNum++)
    {
        if(rowNum < 10)   // this is to fix formatting issues with the numbers when they become 10 or greater
        {
            cout << rowNum << "  ";
        }
        else   //
        {
            cout << rowNum << " ";
        }
    }
    cout << endl;
    // draws a dotted line.
    drawTopOrBottomLine();
    cout << endl;
} // end createTopBorder
// draws a border that helps user find the y values in the word search puzzle
void createRightBorder(int currHeight)
{
    cout << "|" << currHeight;
} // end createRightBorder
// output the matrix to console for testing and debugging along with border
void renderPuzzle(vector<vector<char> > &currVect)
{
    createTopBorder();
    for(int height = 0; height < MATRIX_HEIGHT; height++)
    {
        for(int width = 0; width < MATRIX_WIDTH; width++)
        {
            cout << currVect[height][width] << "  ";
        }
        createRightBorder(height+1);
        cout << endl;
    }
    cout << endl;
} // end renderMatrix

// opens file, checks if opened, gets the width and height and then create a matrix
vector<vector <char> > setupPuzzle()
{
    vector<vector <char> > newPuzzle; // vector that is going to be returned filled with all the characters in the matrix
    ifstream sinput; // will read the file
    string filename;

    cout << "What is the name of your file?\n";
    cin >> filename; // user input for filename

    while(true) // loop until valid filename is entered
    {
        system("cls"); // clear screen

        sinput.open(filename.c_str()); // open file

        if (sinput.is_open())   // the file opened just fine
        {

            // debug code cout << "Opened, current height is " << MATRIX_HEIGHT << endl;
            skipAnyCommentLines(sinput);
            sinput >> MATRIX_HEIGHT >> MATRIX_WIDTH; // read in height and width
            cout << "Num of Rows: " << MATRIX_HEIGHT << "\nNum of Columns: " << MATRIX_WIDTH << endl << endl;
            newPuzzle = createMatrix(sinput); // passes in the ifstream so that file can be continue to be read
            renderPuzzle(newPuzzle); // outputs the puzzle and borders
            insertMovies(sinput); // passes if stream so that movies can be added to a vector
            sinput.close(); // closes ifstream
            break;
        }
        else
        {
            // error checking
            perror("");
            cout << "File '" << filename << "' could not be found or opened\n\n";
            cout << "Enter another file name to use (Or type 'quit')\n\n";
            cin >> filename;
            if(filename != "quit")
            {
                continue;
            }
            else
            {
                cout << "Quitting at user's request\n";
                exit(0); // exit program
            }
        }
    }
    return newPuzzle;
} // end setupPuzzle
// needs to continue down the direction set by the direction, checking to see if the word corresponds to the entire word.
bool continueCheck(vector<vector<char> > puzzle, int currHeight, int currWidth, int oldHeight, int oldWidth, int dir)
{
    int character = THIRD_LETTER; // the current character that we're looking at 3rd index
    switch(dir)
    {
    case NORTH_WEST: // when checking northWest
        currHeight--;
        currWidth--;
        // continues to check until it hits a boundary or the entire word is found
        while(currHeight < MATRIX_HEIGHT && currWidth < MATRIX_WIDTH && currHeight >= 0 && currWidth >= 0 && character <= movieList[CURR_WORD_POS].size()) // boundary
        {
            if(movieList[CURR_WORD_POS][character] == ' ')   // if there is a space in between letters skip it.
            {
                character++;
                continue;
            }
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // individual character of the word matches the matrix
            {
                character++;
                currHeight--;
                currWidth--;
                if(character == movieList[CURR_WORD_POS].size()) // means we've reached the end of the word and that it is found !!!
                {
                    wordFound = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = NW)" << endl;
                    return true;
                }
            }
            else // means we had a false positive
            {
                return false;
            }
        }
        break;
    case NORTH:
        currHeight--;
        // continues to check until it hits a boundary or the entire word is found
        while(currHeight < MATRIX_HEIGHT && currWidth < MATRIX_WIDTH && currHeight >= 0 && currWidth >= 0 && character <= movieList[CURR_WORD_POS].size()) // boundary
        {
            if(movieList[CURR_WORD_POS][character] == ' ')   // if there is a space in between letters skip it.
            {
                character++;
                continue;
            }
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // individual character of the word matches
            {
                character++;
                currHeight--;
                if(character == movieList[CURR_WORD_POS].size()) // means word is found !!!
                {
                    // outputs word, location (in human speak, i.e no 0 based numbers), and direction it is found
                    wordFound = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = N)" << endl;
                    return true;
                }
            }
            else // means we had a false positive so go back to searching
            {
                return false;
            }
        }
        break;
    case NORTH_EAST:
        currHeight--;
        currWidth++;
        // continues to check until it hits a boundary or the entire word is found
        while(currHeight < MATRIX_HEIGHT && currWidth < MATRIX_WIDTH && currHeight >= 0 && currWidth >= 0 && character <= movieList[CURR_WORD_POS].size()) // boundary
        {
            if(movieList[CURR_WORD_POS][character] == ' ')   // if there is a space in between letters skip it.
            {
                character++;
                continue;
            }
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // individual character of the word matches
            {
                character++;
                currHeight--;
                currWidth++;
                if(character == movieList[CURR_WORD_POS].size()) // means word is found !!!
                {
                    wordFound = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = NE)" << endl;
                    return true;
                }
            }
            else // means we had a false positive
            {
                return false;
            }
        }
        break;
    case WEST:
        currWidth--;
        // continues to check until it hits a boundary or the entire word is found
        while(currHeight < MATRIX_HEIGHT && currWidth < MATRIX_WIDTH && currHeight >= 0 && currWidth >= 0 && character <= movieList[CURR_WORD_POS].size()) // boundary
        {
            if(movieList[CURR_WORD_POS][character] == ' ')   // if there is a space in between letters skip it.
            {
                character++;
                continue;
            }
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // individual character of the word matches
            {
                character++;
                currWidth--;
                if(character == movieList[CURR_WORD_POS].size()) // means word is found !!!
                {
                    wordFound = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = W)" << endl;
                    return true;
                }
            }
            else // means we had a false positive
            {
                return false;
            }
        }
        break;

    case EAST:
        currWidth++;
        // continues to check until it hits a boundary or the entire word is found
        while(currHeight < MATRIX_HEIGHT && currWidth < MATRIX_WIDTH && currHeight >= 0 && currWidth >= 0 && character <= movieList[CURR_WORD_POS].size()) // boundary
        {
            if(movieList[CURR_WORD_POS][character] == ' ')   // if there is a space in between letters skip it.
            {
                character++;
                continue;
            }
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // individual character of the word matches
            {
                character++;
                currWidth++;
                if(character == movieList[CURR_WORD_POS].size()) // means word is found !!!
                {
                    wordFound = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = E)" << endl;
                    return true;
                }
            }
            else // means we had a false positive
            {
                return false;
            }
        }
        break;

    case SOUTH_WEST:
        currHeight++;
        currWidth--;
        // continues to check until it hits a boundary or the entire word is found
        while(currHeight < MATRIX_HEIGHT && currWidth < MATRIX_WIDTH && currHeight >= 0 && currWidth >= 0 && character <= movieList[CURR_WORD_POS].size()) // boundary
        {
            if(movieList[CURR_WORD_POS][character] == ' ')   // if there is a space in between letters skip it.
            {
                character++;
                continue;
            }
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // individual character of the word matches
            {
                character++;
                currHeight++;
                currWidth--;
                if(character == movieList[CURR_WORD_POS].size()) // means word is found !!!
                {
                    wordFound = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = SW)" << endl;
                    return true;
                }
            }
            else // means we had a false positive so leave the function
            {
                return false;
            }
        }
        break;

    case SOUTH:
        currHeight++;
        // continues to check until it hits a boundary or the entire word is found
        while(currHeight < MATRIX_HEIGHT && currWidth < MATRIX_WIDTH && currHeight >= 0 && currWidth >= 0 && character <= movieList[CURR_WORD_POS].size()) // boundary
        {
            if(movieList[CURR_WORD_POS][character] == ' ')   // if there is a space in between letters skip it.
            {
                character++;
                continue;
            }
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // individual character of the word matches
            {
                character++;
                currHeight++;
                if(character == movieList[CURR_WORD_POS].size()) // means word is found !!!
                {
                    wordFound = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = S)" << endl;
                    return true;
                }
            }
            else // means we had a false positive so leave the function
            {
                return false;
            }
        }
        break;
    case SOUTH_EAST:
        currHeight++;
        currWidth++;
        // continues to check until it hits a boundary or the entire word is found
        while(currHeight < MATRIX_HEIGHT && currWidth < MATRIX_WIDTH && currHeight >= 0 && currWidth >= 0 && character <= movieList[CURR_WORD_POS].size()) // boundary
        {
            if(movieList[CURR_WORD_POS][character] == ' ')   // if there is a space in between letters skip it.
            {
                character++;
                continue;
            }
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // when individual character of the word matches the current location
            {
                character++;
                currHeight++;
                currWidth++;
                if(character == movieList[CURR_WORD_POS].size()) // means word is found !!!
                {
                    wordFound = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = SE)" << endl;
                    return true;
                }
            }
            else // means we had a false positive so leave the function
            {
                return false;
            }
        }
        break;
    }

    return false;
} // end continueCheck
// I'm going to subtract the currHeight/Width by the currentHeight/Width to get the direction
/*
        |y-1, x-1|y-1, x|y-1, x + 1|
        |y  , x-1|y  , x|y  , x + 1|
        |y+1, x-1|y+1, x|y+1, x + 1|
        example: we're at 15, 14 and we found a letter at 14, 14.
        subtract the values from each other and we get 1,0 which is the direction we need to go
*/
int findDirection(int neighborHeight, int neightborWidth, int currHeight, int currWidth)
{
    int direction = 0;
    int m = neighborHeight - currHeight;
    // cout << "This is m: " << m << endl;
    int n = neightborWidth - currWidth;
    //cout << "This is n: " << n << endl;
    if(m == -1 && n == -1) // NorthWest
    {
        direction = NORTH_WEST;
    }
    else if(m == -1 && n == 0) // North
    {
        direction = NORTH;
    }
    else if(m == -1 && n == 1) // NORTH EAST
    {
        direction = NORTH_EAST;
    }
    else if(m == 0 && n == -1) // WEST
    {
        direction = WEST;
    }
    else if(m == 0 && n == 1) // EAST
    {
        direction = EAST;
    }
    else if(m == 1 && n == -1) // SOUTHWEST
    {
        direction = SOUTH_WEST;
    }
    else if(m == 1 && n == 0) // SOUTH
    {
        direction = SOUTH;
    }
    else if(m == 1 && n == 1) // SOUTH EAST
    {
        direction = SOUTH_EAST;
    }
    return direction;
} // end findDirection
// checks for the surrounding 8 indices around the cell, also makes sure we don't go out of bounds of the vector
bool checkNeighbors(vector<vector <char> > puzzle, int currHeight, int currWidth)
{
    int testInt = 0;
    for(int neighborHeight = currHeight - 1; neighborHeight < currHeight + 2; neighborHeight++)
    {
        if(neighborHeight < 0 || neighborHeight >= MATRIX_HEIGHT)   // check to see if we go out of bounds
        {
            continue;
        }
        for(int neighborWidth = currWidth - 1; neighborWidth < currWidth + 2; neighborWidth++)
        {
            if(neighborWidth < 0 || neighborWidth >= MATRIX_WIDTH)   // checks for out of bounds
            {
                continue;
            }
            if(neighborWidth == currWidth && neighborHeight == currHeight)   // make sure we're not checking the current positions
            {
                continue;
            }
            if(puzzle[neighborHeight][neighborWidth] == movieList[CURR_WORD_POS][SECOND_LETTER])   // if we find the 2nd letter, go in a straight line and search till found/notFound
            {
                int dir = findDirection(neighborHeight, neighborWidth, currHeight, currWidth); // finds the direction we're going to search in
                if(continueCheck(puzzle, neighborHeight, neighborWidth, currHeight, currWidth, dir) == true)   // checks in that direction, returns true, else false
                {
                    return true; // means we found the word
                }
            }
        }
    }
    return false; // means no letter matching the 2nd letter in the word was found
} // end checkNeighbors

// will check for first letter, then checkneighbors for the second letter, then add them into vector of locations
void findLocation(vector<vector<char> > puzzle, int currHeight, int currWidth)
{
    bool locationFound = false;
    if(puzzle[currHeight][currWidth] == movieList[CURR_WORD_POS][FIRST_LETTER]) // see's if the first letter of the word is at the current location in the matrix
    {
        locationFound = checkNeighbors(puzzle, currHeight, currWidth);
        if(locationFound == true)
        {
            // Debug code for finding locations
            vector<int> location; // single dimensional vector which is then added to the 2d vector for the location
            location.push_back(currHeight+1); // height + 1 is the coordinates in human terms same for width + 1
            location.push_back(currWidth+1);
            locations.push_back(location); // adds y, x coordinates to a vector.
        }
    }
    //return false;
} // end findLocation
// loop through puzzle row by row until it finds a word
void searchMatrix (vector<vector<char> > puzzle)
{
    for(int height = 0; height < MATRIX_HEIGHT; height++)
    {
        for(int width = 0; width < MATRIX_WIDTH; width++)
        {
            findLocation(puzzle, height, width);
        }
    }
    // if it reaches this point, then that means the word was not found. Add it to the list of movies not found
    if(wordFound == false)
    {
        moviesNotFound.push_back(movieList[CURR_WORD_POS]);
    }
} // end searchMatrix
// print word locations
void printLocations()
{
    cout << "Movie Locations: \n\n";
    for(int height = 0; height < locations.size(); height++)
    {
        for(int width = 0; width < locations[height].size(); width++)
        {
            cout << locations[height][width] << ",";
            if(width % 2 == 1) // every odd number of times print a newline.. formatting
            {
                cout << endl;
            }
        }
    }
} // end print locations
// used to start the Puzzle
void startPuzzleSolving()
{
// creates a board, create a loop that can broken out of once game is finished to have choice for multiple
    vector <vector<char> > wordPuzzle = setupPuzzle();
    cout << "Movies found: \n";
    for(int listPos = 0; listPos < movieList.size(); listPos++ )
    {
        searchMatrix(wordPuzzle); // search matrix for current word
        wordFound = false;
        CURR_WORD_POS += 1; // increments the movie position to check the next word
    }
    printNotFound();
    resetPuzzle(); // asks the user if they want to enter another puzzle
} // end startPzzleSolving
// if the user wants to enter another word puzzle
void resetPuzzle()
{
    char resetKey;
    cout << "Do you want to enter another word puzzle? ('y' or 'n')\n";
    cin >> resetKey;
    switch(resetKey) // if yes will call startPuzzleSolving
    {
    case 'y':
        setup();
        startPuzzleSolving();
        break;
    default:
        cout << "Exiting program based on user choice...\n";
        exit(0);
        break;
    }

}// end resetPuzzle
// main function
int main()
{
    startPuzzleSolving();
    return 0;
}
