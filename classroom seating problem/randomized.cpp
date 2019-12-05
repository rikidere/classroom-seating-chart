#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <map>
#include <fstream>
#include <string>

using namespace std;

const int SHUFFLES = 5000;
const int SWAPS = 10000;



struct student {
    int name;
    vector<int> preferences;
};

int getSeatingScore(vector<student> seating);
int getPreferenceScore(student left, student curr, student right);
void printVec(vector<student> seating);
void printVecConvert(vector<student> students, map<int,string> mapping);

int main(int argc, char *argv[]) {
    srand((int)time(0));
    vector<string> names;
    //open file
    ifstream textFile("students.txt");
    if (textFile.fail()) {
    cerr << "Unable to open file for reading." << endl;
    exit(1);
    }

    string nextName;
    
    for(int i = 0; i < 20; i++){
        textFile >> nextName;
        names.push_back(nextName);
    }

    //establish mapping between name and number (1-20)
    map<int, string> mapping;
    map<string, int> mappingByName;
    for(int i = 1; i <= 20; i++) {
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
    for(int i = 1; i <= 20; ++i) {
        vector<int> pref;
        string nextPref;
        for(int j = 0; j < 3; ++j){
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


    //print students
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
    }

    cout << "Best Seating score found: " << bestSeatingScore << endl;
    cout << "with: " << endl;
    printVecConvert(bestSeating, mapping);

    return 0;
}
void printVecConvert(vector<student> students, map<int,string> mapping){
    for(auto it = students.begin(); it != students.end(); it++){
        
        cout << (*it).name << ":" << '\t' << mapping.find((*it).name)->second << '\t' << "{ ";
        for (auto it2 = (*it).preferences.begin(); it2 != (*it).preferences.end(); ++it2) { 
            cout << mapping.find((*it2))->second << " ";
        } 
        cout << "}" << endl;
    }
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
    for(int i = 1; i <= 18; i++){
        left = seating[i-1];
        curr = seating[i];
        right = seating[i+1];
        score += getPreferenceScore(left, curr, right);
    }
    //check right edge
    left = seating[18];
    curr = seating[19];

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