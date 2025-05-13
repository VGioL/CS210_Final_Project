#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <cstdlib>
#include <unordered_map>
#include <string>
#include "timer.h"
using namespace std;

string cacheDecision;
deque<pair<string, int>> cache;

pair<int, int> cacheHitRate = {0,0}; // {searches, cache hits}

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


struct TrieNode {
    bool isEndOfWord;
    unordered_map<char, TrieNode*> children;
    int population;

    TrieNode() : isEndOfWord(false), population(-1) {}
};

class Trie {
private:
    TrieNode *root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& cc, const string& city, const int& pop)
    {
        string name = cc + city;
        TrieNode* node = root;
        for (char c : name) {
            c = tolower(c); // Case-insensitive
            if (node->children.count(c) == 0)
                node->children[c] = new TrieNode();
            node = node->children[c];
        }
        node->population = pop;
        node->isEndOfWord = true;
    }

    int search(const string& cc, const string& city)
    {
        cacheHitRate.first ++;
        // check cache
        for (deque<pair<string, int>>::iterator cached = cache.begin(); cached < cache.end(); cached++)
        {
            string word;
            stringstream ss(cached->first);
            getline(ss, word, ',');
            if(word == cc)
            {
                getline(ss, word, ',');
                if (word == city)
                {
                    // handle cache hit
                    cacheHitRate.second++;
                    pair<string, int> temp = *cached;
                    cache.erase(cached);
                    temp.second++; // increase frequency count
                    cache.emplace_back(temp); // move city info to back of cache

                    getline(ss, word, ',');
                    return stoi(word); // string to int
                }
            }
        }

        string name = cc + city;
        TrieNode* node = root;
        for (char c : name) {
            c = tolower(c);
            if (node->children.count(c) == 0)
                return -1; // not found
            node = node->children[c];
        }
        if (node->isEndOfWord) // found (handles case where search starts going down a branch but stops at non-terminal node)
        {
            // handle cache at capacity
            if(cache.size() == 10)
            {
                handleCacheFull();
            }
            cache.emplace_back(cc + "," + city + "," + to_string(node->population) + ".0", 1); // add city info to cache
        }
        return node->population; // will be -1 if not found
    }
};


int main() {
    // read csv into trie
    ifstream file("world_cities.csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open world_cities.csv" << endl;
        return -1;
    }
    Trie trie;
    string line;
    getline(file, line); // discarding first line (which is just info)
    while (getline(file, line))
    {
        string cc;
        string city;
        string pop;
        stringstream ss(line); // turn string into input stream
        getline(ss, cc, ',');
        getline(ss, city, ',');
        getline(ss, pop, ',');

        trie.insert(cc, city, stoi(pop));
    }
    file.close();


    // user interface
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

    ifstream testscript("testscript.txt");
    if (!testscript.is_open()) {
        cerr << "Error: Could not open testscript.txt" << endl;
        return -1;
    }

    Timer timer;
    double aveDuration = 0;
    double totalDuration = timer.get_time();
    vector<double> durations;

    while(true)
    {
//        cout << "Please enter a country code:";
        string inputCountryCode;
        testscript >> inputCountryCode;
        if (inputCountryCode == "-1")
        {
            testscript.close();

            // get average of all recorded durations
            for (double duration : durations)
            {
                aveDuration += duration;
            }
            aveDuration /= durations.size();

            // get total duration
            totalDuration = timer.get_time() - totalDuration;

            // get cache hit rate ratio
            double cacheHitRatio = double(cacheHitRate.second) / double(cacheHitRate.first);

            cout << "total duration (ms): " << totalDuration << endl;
            cout << "average lookup time (ms): " << aveDuration << endl;
            cout << "cache hit rate: " << cacheHitRate.second << "hits to " << cacheHitRate.first << " searches. (" << cacheHitRatio << ")" << endl;

            return 0;
        }

//        cout << "Please enter a city name:";
        string inputCityName;
        testscript >> ws;
        getline(testscript, inputCityName);

        if (inputCityName == "-1")
        {
            testscript.close();
            return 0;
        }

        double startDuration = timer.get_time();
        int pop = trie.search(inputCountryCode, inputCityName);
        durations.push_back(timer.get_time() - startDuration);

        if (pop < 0)
        {
            cout << "City not found. Try again." << endl;
            cout << "      Your country code: '" << inputCountryCode << "'" << endl;
            cout << "      Your city name: '" << inputCityName << "'"  << endl;
            cout << endl;
        }
//        else
//        {
//            cout << "Population: " << pop << endl;
//            cout << "Current Cache: " << endl;
//            for (pair<string, int> cached : cache)
//            {
//                cout << cached.first << " | frequency of " << cached.second << endl;
//            }
//            cout << endl << endl;
//        }
    }
}
