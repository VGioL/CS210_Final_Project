#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
using namespace std;

// returns population
int searchCSV(string countryCode, string cityName, deque<string> cache)
{
    string line; // ???
    string word; // ???

    // check cache
    for (deque<string>::iterator cached = cache.begin(); cached != cache.end();)
    {
        stringstream ss(*cached);
        getline(ss, word, ',');
        if(word == countryCode) {
            getline(ss, word, ',');
            if (word == cityName) {
                cache.erase(cached);
                cache.push_back(*cached); // add city info to cache
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
                if (cache.size() == 10)
                {
                    cache.pop_front();
                }
                cache.push_back(line); // add city info to cache
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
    deque<string> cache;
    cout << "Welcome to the world city search script. Type -1 at any prompt to end program." << endl;
    cout << "--------------------" << endl;

    while(true)
    {
        cout << "Please enter a country code:";
        string inputCountryCode; // ???
        cin >> inputCountryCode;
        if (inputCountryCode == "-1")
        {
            return 0;
        }

        cout << "Please enter a city name:";
        string inputCityName; // ???
        cin >> ws;
        getline(cin, inputCityName);


        if (inputCityName == "-1")
        {
            return 0;
        }

        int pop = searchCSV(inputCountryCode, inputCityName, cache);
        if (pop < 0)
        {
            cout << "City not found. Try again." << endl << endl;
            cout << "your country code: '" << inputCountryCode << "'" << endl;
            cout << "your city name: '" << inputCityName << "'"  << endl;
        }
        else
        {
            cout << "Population: " << pop << endl << endl;
        }
    }
}
