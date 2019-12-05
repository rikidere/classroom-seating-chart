/*
======================================================================
Author: rikidere (github)
Email: eppatric@student.ethz.ch
======================================================================
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <map>
#include <fstream>
#include <string>

using namespace std;

/*
======================================================================
NUMBER OF STUDENTS AND PREFERENCES
======================================================================
*/
const int STUDENTS = 20;
const int PREFERENCES = 3;
/*
======================================================================
PLAY AROUND WITH THESE TO GET BETTER RESULTS
======================================================================
*/
const int SHUFFLES = 2000;
const int SWAPS = 2000;
/*
======================================================================
PRINT SEATING OPTIONS 
Note: 
(LOWERBOUND = INT_MAX or AMOUNT = 0 to ignore)
lower SHUFFLES x SWAPS is recommended for lower bounds
amount of solutions to search for, program is closed after (-1 for indefinite)
======================================================================
*/
// RANGE
const int LOWERBOUND = 23;
const int HIGHERBOUND = 25;
const int AMOUNT = 20;

// OUTPUT TO FILE
const bool OUTPUTTOFILE = true;
const string OUTPUTFILENAME = "output.txt";
ofstream outputfile;

/*
======================================================================
FILE FORMAT (students.txt)
======================================================================
first the list of students, then for each student in the same order their preferences:

student1
student2
...
studentN
studentx1 studenty1 studentz1
studentx2 studenty2 studentz2
...
studentxN studentyN studentzN
*/


struct student {
    int name;
    vector<int> preferences;
};

int getSeatingScore(vector<student> seating);
int getPreferenceScore(student left, student curr, student right);
void printVec(vector<student> seating);
void printVecConvert(vector<student> students, map<int,string> mapping);

int main(int argc, char *argv[]) {
    int starttime = (int)time(0);
    srand((int)time(0));
    vector<string> names;
    //open file
    ifstream textFile("students.txt");
    if (textFile.fail()) {
    cerr << "Unable to open file for reading." << endl;
    exit(1);
    }

    //output file
    if(OUTPUTTOFILE) {
        outputfile.open("output.txt");
    }


    string nextName;
    
    for(int i = 0; i < STUDENTS; i++){
        textFile >> nextName;
        names.push_back(nextName);
    }

    //establish mapping between name and number (1-20)
    map<int, string> mapping;
    map<string, int> mappingByName;
    for(int i = 1; i <= STUDENTS; i++) {
        mapping.insert(pair<int, string>(i,names[i-1]));
        mappingByName.insert(pair<string, int>(names[i-1], i));
    }

    /*
    cout << "Student Mapping: " << endl;
    for (auto it = mapping.begin(); it != mapping.end(); ++it) { 
        cout << it->first << ":"
             << '\t' << it->second << '\n'; 
    } */


    //read in prefs
    vector<student> students;
    for(int i = 1; i <= STUDENTS; ++i) {
        vector<int> pref;
        string nextPref;
        for(int j = 0; j < PREFERENCES; ++j){
            textFile >> nextPref;
            auto it = mappingByName.find(nextPref);

            if(it == mappingByName.end()) {
                cerr << "Unable to open file for reading." << endl;
                exit(1);
            }
            
            int num = it->second;
            pref.push_back(num);
        }
        struct student s = {i, pref};
        students.push_back(s);
    }


    cout << "Students: " << endl;
    for(auto it = students.begin(); it != students.end(); it++){
        
        cout << (*it).name << ":" << '\t' << mapping.find((*it).name)->second << '\t' << "{ ";
        for (auto it2 = (*it).preferences.begin(); it2 != (*it).preferences.end(); ++it2) { 
            cout << mapping.find((*it2))->second << " ";
        } 
        cout << "}" << endl;
    }
    

    random_device rd;
    mt19937 rng(rd());

    vector<student> bestSeating;
    int bestSeatingScore = 0;
    int prevProgress = 0;
    int amount = 0;
    for(int i = 0; i < SHUFFLES; i++) {
        vector<student> newSeating = students;
        shuffle(newSeating.begin(), newSeating.end(), rng);
        int score = getSeatingScore(newSeating);
        
        //update progress
        int prog = (i+1)*100/SHUFFLES;
        if(prevProgress!=prog){
            cout << prog << "%" << endl;
            prevProgress = prog;
        }

        //swap x with y
        for(int i = 0; i < SWAPS; i++) { 
            vector<student> swapSeating = newSeating;
            int x = rand() % 20;
            int y = rand() % 20;
            swapSeating[x] = newSeating[y];
            swapSeating[y] = newSeating[x];
            int swapScore = getSeatingScore(swapSeating);

            if(swapScore > score) {
                newSeating = swapSeating;
                score = swapScore;
            }
            
        }
        //check if after swaps found higher score
        if(score > bestSeatingScore) {
            cout << "Better Seating score found: " << score << endl;
            bestSeatingScore = score;
            bestSeating = newSeating;
            
        }
        if(score >= LOWERBOUND && score <= HIGHERBOUND) {
            cout << "Seating found in range: " << score << endl;
            cout << "with: " << endl;
            printVecConvert(newSeating, mapping);
            amount++;
            if(AMOUNT != -1 && amount >= AMOUNT) {
                cout << "Specified amount of results has been reached in: " << (int)time(0) - starttime  << "s" << endl;
                return 0;
            }
        }
    }

    cout << "Best Seating score found: " << bestSeatingScore << endl;
    cout << "with: " << endl;
    printVecConvert(bestSeating, mapping);
    cout << "Finished after: " << (int)time(0) - starttime  << "s" << endl;
    if(OUTPUTTOFILE) {
        outputfile.close();
    }
    return 0;
}
void printVecConvert(vector<student> students, map<int,string> mapping){
    if(OUTPUTTOFILE){
        outputfile << "Seating score: " << getSeatingScore(students) << endl;
        outputfile << "with: " << endl;
        for(auto it = students.begin(); it != students.end(); it++){
            outputfile << (*it).name << ":" << '\t' << mapping.find((*it).name)->second << '\t' << "{ ";
            for (auto it2 = (*it).preferences.begin(); it2 != (*it).preferences.end(); ++it2) { 
                outputfile << mapping.find((*it2))->second << " ";
            } 
            outputfile << "}" << endl;
        }
        outputfile << endl << endl;
    }
    for(auto it = students.begin(); it != students.end(); it++){
        cout << (*it).name << ":" << '\t' << mapping.find((*it).name)->second << '\t' << "{ ";
        for (auto it2 = (*it).preferences.begin(); it2 != (*it).preferences.end(); ++it2) { 
            cout << mapping.find((*it2))->second << " ";
        } 
        cout << "}" << endl;
    }
    cout << endl << endl;
}
    
void printVec(vector<student> seating) {
    for(auto it = seating.begin(); it != seating.end(); ++it) {
        cout << (*it).name << " ";
    }
}


int getSeatingScore(vector<student> seating){
    int score = 0;
    student left;
    student curr = seating[0];
    student right = seating[1];

    //check left edge
    for(auto it = curr.preferences.begin(); it != curr.preferences.end(); ++it){
        if(*it == right.name) score++;
    }
    //check non-edge
    for(int i = 1; i <= STUDENTS-2; i++){
        left = seating[i-1];
        curr = seating[i];
        right = seating[i+1];
        score += getPreferenceScore(left, curr, right);
    }
    //check right edge
    left = seating[STUDENTS-2];
    curr = seating[STUDENTS-1];

    for(auto it = curr.preferences.begin(); it != curr.preferences.end(); ++it){
        if(*it == left.name) score++;
    }
    return score;
}

int getPreferenceScore(student left, student curr, student right){
    int score = 0;
    for(auto it = curr.preferences.begin(); it != curr.preferences.end(); ++it){
        if(*it == right.name) score++;
        if(*it == left.name) score++;
    }
    return score;
}