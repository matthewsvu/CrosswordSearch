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
    Change log 2/12/20:
        1. added skipanycommentLines method
        2. Crudely implemented continueCheck method, searches in one direction, need to check for spaces in words
        3. added directions integers
        4.

    Notes:
        I need to figure out the logic of searching the vectors and finish the searchMovies function.
        I'm having trouble keeping it together i.e the logic. I may need to condense everything and encapsulate them.
*/

#include <iostream>
#include <vector>
#include <fstream> // for ifstream
#include <cstdlib> // for c_str() to read filenames

using namespace std;

int MATRIX_HEIGHT = 0;
int MATRIX_WIDTH = 0;
int CURR_WORD_POS = 0;

const int FIRST_LETTER = 0;
const int SECOND_LETTER = 1;
const int THIRD_LETTER = 2;
const int NORTH_WEST = 1;
const int NORTH = 2;
const int NORTH_EAST = 3;
const int WEST = 4;
const int EAST = 5;
const int SOUTH_WEST = 6;
const int SOUTH = 7;
const int SOUTH_EAST = 8;

bool found = false;
// List of movies intially
vector <string> movieList;
// list of movies not founds to be printed last
vector <string> moviesNotFound;
// Initial locations of the movies found
vector <vector<int> > locations;

void resetPuzzle(); // will be called from start function
// is called when the user wants to enter in another puzzle.
void setup() {
    system("cls");
    found = false;
    // clears vectors of items.
    movieList.clear();
    moviesNotFound.clear();
    CURR_WORD_POS = 0; // sets it back to zero so that we read the matrix from the first letter again.
}

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
            if(input.empty())   // checks if the line has any character or not, and restart the loop if it doesn't.
            {
                continue;
            }
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
}
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
// Creates the matrix
vector<vector<char> > createMatrix(ifstream &sinput)
{
    vector<vector<char> > tempMatrix(MATRIX_HEIGHT, vector<char> (MATRIX_WIDTH, 'a')); // initializes vector with 'a' in all locations
    for(int i = 0; i < MATRIX_HEIGHT; i++)
    {
        for(int j = 0; j < MATRIX_WIDTH; j++)
        {
            skipAnyCommentLines(sinput);
            sinput >> tempMatrix[i][j];
        }
    }
    cout << endl;
    return tempMatrix;
} // end createMatrix
// output the matrix to console for testing and debugging
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
    system("cls"); // clear screen
    while(true) // loop until valid filename is entered
    {
        sinput.open(filename.c_str());

        if (sinput.is_open())   // the file opened just fine
        {

            // debug code cout << "Opened, current height is " << MATRIX_HEIGHT << endl;
            skipAnyCommentLines(sinput);
            sinput >> MATRIX_HEIGHT >> MATRIX_WIDTH; // read in height and width
            cout << "Current height/width after reading it in is: " << MATRIX_HEIGHT << " " << MATRIX_WIDTH << endl << endl;
            newPuzzle = createMatrix(sinput); // passes in the ifstream so that file can be continue to be read
            renderPuzzle(newPuzzle); // outputs the puzzle
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
            getline(cin, filename);
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
                    found = true;
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
                    found = true;
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
                    found = true;
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
                    found = true;
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
                    found = true;
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
                    found = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = SW)" << endl;
                    return true;
                }
            }
            else // means we had a false positive
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
                    found = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = S)" << endl;
                    return true;
                }
            }
            else // means we had a false positive
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
            if(movieList[CURR_WORD_POS][character] == puzzle[currHeight][currWidth]) // individual character of the word matches
            {
                character++;
                currHeight++;
                currWidth++;
                if(character == movieList[CURR_WORD_POS].size()) // means word is found !!!
                {
                    found = true;
                    cout << movieList[CURR_WORD_POS] << " found at " << oldHeight+1 << ", " << oldWidth+1 << ": (direction = SE)" << endl;
                    return true;
                }
            }
            else // means we had a false positive
            {
                return false;
            }
        }
        break;
    }

    return false;
} // end continueCheck
// I'm going to subtract the currHeight/Width by the currentHeight/Width to get the direction
int findDirection(int currHeight, int currWidth, int oldMatrixHeight, int oldMatrixWidth)
{
    int direction = 0;
    int m = currHeight - oldMatrixHeight;
    // cout << "This is m: " << m << endl;
    int n = currWidth - oldMatrixWidth;
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
// how am I going to set the direction of where the search is going.
// checks for the surrounding 8 indices around the cell, also makes sure we don't go out of bounds of the vector
bool checkNeighbors(vector<vector <char> > puzzle, int currHeight, int currWidth)
{
    int testInt = 0;
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
            if(puzzle[height][width] == movieList[CURR_WORD_POS][SECOND_LETTER])   // if we find the 2nd letter, go in a straight line and search
            {
                int dir = findDirection(height, width, currHeight, currWidth); // finds the direction we're pointing to
                if(continueCheck(puzzle, height, width, currHeight, currWidth, dir) == true)   // checks in that direction, returns true, else false
                {
                    return true; // means we found the word
                }
            }
        }
    }
    return false; // means no letter matching the 2nd letter in the word was found
} // end checkNeighbors

// will check for first letter, then checkneighbors for the second letter, then add them into
bool findLocation(vector<vector<char> > puzzle, int currHeight, int currWidth)
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
            return true;
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
    if(found == false)
    {
        moviesNotFound.push_back(movieList[CURR_WORD_POS]);
    }
} // end searchMatrix
// print word locations
void printLocations()
{
    cout << "Movie Locations: \n\n";
    for(int i = 0; i < locations.size(); i++)
    {
        for(int j = 0; j < locations[i].size(); j++)
        {
            cout << locations[i][j] << ",";
            if(j % 2 == 1)
            {
                cout << endl;
            }
        }
    }
}
// used to start the game
void startGame()
{
// creates a board, create a loop that can broken out of once game is finished to have choice for multiple
    vector <vector<char> > wordPuzzle = setupPuzzle();
    for(int listPos = 0; listPos < movieList.size(); listPos++ )
    {
        searchMatrix(wordPuzzle); // search matrix for current word
        found = false;
        CURR_WORD_POS += 1; // increments the movie position to check the next word
    }
    printNotFound();
    resetPuzzle(); // asks the user if they want to enter another puzzle
}
// if the user wants to enter another word puzzle
void resetPuzzle()
{
    char resetKey;
    cout << "Do you want to enter another word puzzle? ('y' or 'n')\n";
    cin >> resetKey;
    switch(resetKey)
    {
    case 'y':
        setup();
        startGame();
        break;
    default:
        cout << "Exiting program based on user choice...\n";
        exit(0);
    break;
    }

}

int main()
{
    startGame();
    return 0;
}
