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
#include <fstream>
#include <cstdlib>

int HEIGHT = 0;
int WIDTH = 0;

using namespace std;

vector <vector<char> > createMatrix() {
    vector <vector<char> > tempMatrix;
    ifstream sinput;
    string filename = "testTxt.txt";
    sinput.open(filename.c_str());

    if (sinput.good()){ //then the file opened just fine
        sinput >> HEIGHT >> WIDTH;
        for(int i = 0; i < HEIGHT; i++) {
            for(int j = 0; j < WIDTH; j++) {
                sinput >> tempMatrix[i][j];
            }
        }
        sinput.close();
    }
    else {
        cout << "File did not open" << endl;
        exit(0);
    }
    return tempMatrix;
}
void renderMatrix(vector<vector<char> > currVect) {
    for(int height = 0; height < HEIGHT; height++) {
            for(int width = 0; width < WIDTH; width++) {
                cout << currVect[height][width] << " ";
            }
            cout << endl;
        }
}
vector <vector<char> > searchMatrix (int height, int width) {

}

vector <vector<char> > checkNeighbors(int currHeight, int currWidth) {

}

vector <vector<char> > continueCheck(int currHeight, int currWidth) {

}
int main()
{
   vector<vector<char> > newMatrix = createMatrix();
    renderMatrix(newMatrix);
    cout << "Hello world!" << endl;
    return 0;
}
