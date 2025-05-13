
// this file will need to be commented out for main.cpp to run, and is just used to generate the test script.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
//#include <algorithm>
using namespace std;

// pick and choose some cities from world_cities.cvs into testscript.csv
// include the first three cities per country code
// if less than three cities in one country code, include all
// include the first city in that country twice
// then,  load csv into testscript.txt (into format main.cpp will read as proper input)

// using just the above structure would mean 1 in 4 searches will be a value previously cached (not necessarily a cache hit)
// i was going to shuffle the included cities so that it wouldn't be the same structure over and over,
// but i realized it would be extremely unlikely for any cache hits to occur that way...
// so i implemented something to bring back the previously first-cities, as a way to stress-test LFU
// (this is the 'frequentCities' queue, which adds a previous city for 2 cities found in a country past the third one
// now the ratio of unique to repeated searches isn't so patterned and predictable
// brings the test script to a nice 948 searches with a nice distribution on repeated cities



int main()
{
    // populate testscript.csv with cities to be searched
    ifstream world_cities("world_cities.csv");
    ofstream testscript("testscript.txt");
    if (!world_cities.is_open()) {
        cerr << "Error: Could not open world_cities.csv" << endl;
        return 1;
    }
    if (!testscript.is_open()) {
        cerr << "Error: Could not open testscript.txt" << endl;
        return 1;
    }

    string line;
    getline(world_cities, line); // discarding first line (which is just info)
    int pattern = 0;
    string currentCountry;
    queue<string> frequentCities;

    while (getline(world_cities, line)) {
        stringstream ss(line); // turn string into input stream, so that i can get country code and city name from current line
        string foundCountry;
        string foundCity;
        string waste;
        getline(ss, foundCountry, ',');
        getline(ss, foundCity, ',');

        if (foundCountry != currentCountry) // new country code
        {
            pattern = 0;
            currentCountry = foundCountry;

            // adding twice for the first city in country
            testscript << foundCountry << endl << foundCity << endl << foundCountry << endl << foundCity << endl;

            // setting up to bring back older cities
            frequentCities.push(line);
        }
        else
        {
            pattern++;
            if (pattern < 3) // only want to add the next two cities in the country
            {
                testscript << foundCountry << endl << foundCity << endl;
            }
            else if (pattern < 5) // for the next two cities in a country which wouldn't be added
            {
                // bring back older cities
                if (!frequentCities.empty())
                {
                    stringstream frequentSS(frequentCities.front());
                    string freqFoundCountry;
                    string freqFoundCity;
                    getline(frequentSS, freqFoundCountry, ',');
                    getline(frequentSS, freqFoundCity, ',');

                    testscript << freqFoundCountry << endl << freqFoundCity << endl;

                    // create rotation of cities so that it's not just the same country's first city being repeated 2 more times
                    frequentCities.push(frequentCities.front());
                    frequentCities.pop();
                }
            }

        }
    }
    testscript << "-1" << endl;
    world_cities.close();
    testscript.close();
}