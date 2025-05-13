# Final Project (Milestone 4)
CS210 02  
Vivian G. Lee  
May 13 2025  

## Testing Methodology
### testscriptmaker.cpp
The file `testscript.txt` was created with the code in `testscriptmaker.cpp` in 
order to ultimately be read by `main.cpp`. `testscriptmaker.cpp` loads the
`world_cities.csv` file and selects a specific assortment of cities from 
the csv, and saves them in a format that can be plugged into `main.cpp` as input.  
`testscriptmaker.cpp` adds the first city in a country to the testscript twice, 
and adds that first city to a rotating queue of frequent cities. Then the next 
two cities in the country (if they exist) are added to the testscript. for the
next two cities in the country (if they exist), rather than adding the new cities,
the next two cities in the rotating queue of frequent cities are added again. this
process repeats for all country codes in the `world_cities.csv` csv, and produces
a workable .txt file with all countries included and decent distribution of repeated
cities.  
I was initially planning to first add all the cities-to-be-included to a .csv file
before shuffling the order, but I realized with a volume of searches this high,
the cache would likely never be hit (unless I really skewed the .txt file to
include a much, much larger number of duplicates). So, I decided to implement
this rotating queue to distribute frequent cities in order to give the LFU cache
approach some more legitimacy in this test.
### Changes to main.cpp
Some things were changed in `main.cpp` in order to read the testscript file, and
in order to keep track of info like lookup duration and cache hit rate. The file
`timer.h` was used for timing, which was given in class.  
When running the file, the initial selection of a cache method remains up to user
choice, and remains constant for the entire runtime. This means to test the
different cache methods, `main.cpp` was run multiple unique times.
Most of the print statements were removed from `main.cpp` for clarity in the console,
since they are simple operations that aren't affected by the cache method.
### Testing
I ran `main.cpp` 7 times per caching method, for a total of 21 runs. I recorded
the total duration of each run, the average lookup time for each run, and the cache
hit rate for each run. I recorded this information in a spreadsheet, where I could
then find the average total duration and the average of the average lookup times
(and the average cache hit rate for the random replacement).

## Results
The three caching methods, *Least Frequently Used (LFU);* *First-in, First-out 
(FIFO);* and *Random Replacement (RR);* were tested.  
These caching methods were tested for *total runtime duration,* *average lookup
time,* and *cache hit rate.*  
Charts may be seen on the included spreadsheet: `CS210 Final Milestone 4.xlsx`
### Total Duration
There was some variation causing *LFU* to occasionally take slightly longer
than *FIFO,* but generally the two methods were near-overlapping. Imporant to note
that *FIFO* never look longer than *LFU*. *RR* was near-always taking significantly
longer than the other two methods, only being faster than *LFU* once.  
On average, *FIFO* was the fastest method, about 50ms faster than *LFU* and 350ms
faster than *RR.*
### Average Lookup Time
The average lookup time was calculated in `main.cpp` for each run of the program,
averaging all the search durations that happened during runtime.  
In the same manner as total duration, *LFU* and *FIFO* were near-overlapping,
with *LFU* occasionally taking longer and *RR* taking much longer (except in one 
instance). *LFU* and *FIFO* are actually even closer in terms of average lookup time.
In my analysis after recording the values, I averaged the recorded averages. *FIFO*
on average was 0.034ms faster than *LFU,* and 0.289ms faster than *RR.* 
### Cache Hit Rate
The cache hit rate was calculated by counting the number of searches and the
number of cache hits during runtime, and then after reaching the end of the 
testscript, dividing that the number of cache hits by the number of searches.  
The cache hit rate for *LFU* and *FIFO* were constant, and did not change during
testing. *LFU* had a cache hit rate of 0.232383, and *FIFO* had a cache hit rate
of 0.187919. The cache hit rate for *RR* did change slightly during testing, 
but generally hovered around 0.185 with one exception (spiking to 0.193792). *FIFO*
and *RR* remained very close in their hit rate, while *LFU* had a significantly
larger ratio (0.4 more on average).


## Analysis
- I was surprised that *LFU* had a significantly higher hit rate than the other methods,
yet was only just-as fast as *FIFO* if not slower.  
  - The implication here is that the frequent cache hits in *LFU* sped up the
  process, but just enough to cancel-out the increased time taken to maintain a cache
  based on frequency. I am curious if I had designed my test script differently if
  I would be seeing *LFU* significantly faster or slower.
- *RR's* high durations and low cache hits were unsurprising to me, and I do 
not recommend that method for any scenario.  
  
I think that both *Last Frequently Used* and *First-in/First-out* have merit,
and could both be the fastest option depending on the scenario.
- *LFU* should be used in instances where the expected user behavior is to search
duplicate queries in quick succession. A user might do this if there was no way
for them to record the information, so they needed to search every time they forgot.
Thinking in terms of multiple users/clients, *LFU* would be handy in scenarios where
the demand for items is fast-changing--fast to adopt new items to the cache and
fast to drop items when they stop being accessed.
- *FIFO* should be used in instances where the expected user behavior is to search
for duplicate queries over longer spans of time. Queries for a specific item may 
come in bunches, with long breaks between. This could be applied to serving clients
popular items based on aggregated data--a sudden surge one day in a few new items,
or a wide variety of low popularity items should not dethrone the established 
popular item with a high frequency count.