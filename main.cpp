#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <cstdlib>
using namespace std;

string cacheDecision;
deque<pair<string, int>> cache;


void handleCacheFull()
{
    // Least Frequently Used
    if (cacheDecision == "1")
    {
        deque<pair<string, int>>::iterator lowestFreqIterator;
        int lowestFreq = INT_MAX;
        for (deque<pair<string, int>>::iterator cached = cache.begin(); cached < cache.end(); cached++) // begin() = front of deque, so oldest
        {
            if (cached->second < lowestFreq)
            {
                lowestFreq = cached->second;
                lowestFreqIterator = cached;
            }
        }
        cache.erase(lowestFreqIterator);
    }

    // First-in First-out
    if (cacheDecision == "2")
    {
        cache.pop_front();
    }

    // Random Replacement
    if (cacheDecision == "3")
    {
        // adapting some code for using rng from cppreference.com https://en.cppreference.com/w/cpp/numeric/random/rand
        srand(time({})); // uses current time for the rng seed
        unsigned random = rand();
        unsigned offset = (random % 10);

        cache.erase(cache.begin() + offset); // random int from 0-9
    }
}


// returns population
int searchCSV(const string& countryCode, const string& cityName)
{
    string line;
    string word;

    // check cache
    for (deque<pair<string, int>>::iterator cached = cache.begin(); cached < cache.end(); cached++)
    {
        stringstream ss(cached->first);
        getline(ss, word, ',');
        if(word == countryCode)
        {
            getline(ss, word, ',');
            if (word == cityName)
            {
                // handle cache hit
                pair<string, int> temp = *cached;
                cache.erase(cached);
                temp.second++; // increase frequency count
                cache.emplace_back(temp); // move city info to back of cache

                getline(ss, word, ',');
                return stoi(word); // string to int
            }
        }

    }

    ifstream file("world_cities.csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open world_cities.csv" << endl;
        return -1;
    }

    // linear search
    while (getline(file, line))
    {
        stringstream ss(line); // turn string into input stream
        getline(ss, word, ',');
        if(word == countryCode)
        {
            getline(ss, word, ',');
            if(word == cityName)
            {
                // handle cache at capacity
                if(cache.size() == 10)
                {
                    handleCacheFull();
                }
                cache.emplace_back(line, 1); // add city info to cache
                getline(ss, word, ',');
                file.close();
                return stoi(word); // string to int
            }
        }
    }
    file.close();
    return -1;
}




int main() {
     // file is re-opened and closed for every search. is this the better option?
    cout << "Welcome to the world city search script. Type -1 at any prompt to end program." << endl;
    cout << "--------------------" << endl;
    bool success = false;
    while (!success)
    {
        cout << "Choose one of the following cache options by typing the corresponding number:" << endl
        << "1: LFU (Least Frequently Used)" << endl
        << "2: FIFO (First-In, First-out)" << endl
        << "3: Random Replacement" << endl;
        cin >> cacheDecision;
        if (cacheDecision == "-1")
        {
            return 0;
        }
        if (cacheDecision != "1" && cacheDecision != "2" && cacheDecision != "3")
        {
            cout << "Invalid input; please type -1, 1, 2, or 3." << endl;
        }
        else
        {
            success = true;
            cout << "--------------------" << endl;
        }


    }

    while(true)
    {
        cout << "Please enter a country code:";
        string inputCountryCode;
        cin >> inputCountryCode;
        if (inputCountryCode == "-1")
        {
            return 0;
        }

        cout << "Please enter a city name:";
        string inputCityName;
        cin >> ws;
        getline(cin, inputCityName);


        if (inputCityName == "-1")
        {
            return 0;
        }

        int pop = searchCSV(inputCountryCode, inputCityName);
        if (pop < 0)
        {
            cout << "City not found. Try again." << endl;
            cout << "      Your country code: '" << inputCountryCode << "'" << endl;
            cout << "      Your city name: '" << inputCityName << "'"  << endl;
            cout << endl;
        }
        else
        {
            cout << "Population: " << pop << endl;
//            cout << "Current Cache: " << endl;
//            for (pair<string, int> cached : cache)
//            {
//                cout << cached.first << " | frequency of " << cached.second << endl;
//            }
            cout << endl << endl;
        }
    }
}
