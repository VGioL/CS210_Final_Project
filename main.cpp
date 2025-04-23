#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
using namespace std;

// returns population
int searchCSV(string countryCode, string cityName)
{
    ifstream file("world_cities.csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open world_cities.csv" << endl;
        return -1;
    }
    string line;
    string word;
    // linear search
    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, word, ',');
        if(word == countryCode)
        {
            getline(ss, word, ',');
            if(word == cityName)
            {
                getline(ss, word, ',');
                return stoi(word); // string to int
            }
//            else
//            {
//                delete ss;
//            }
        }
//        else
//        {
//            delete ss;
//        }

    }
    file.close();
    return -1;
}


int main() {



    return 0;
}
