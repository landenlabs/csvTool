
# LanDen Labs - CsvTool (csv parser)
CsvTool [Home website](http://landenlabs.com)  
8-Jan-2020  
  
### Compact Fast CSV Parser   
Simple compact fast CSV parser in single class header file.

## Key features   
1. Both CR and CRLF line termination
2. Custom line termination
3. Fields spanning multiple lines
4. Quoted fields
5. Escaped - double quoted fields
 
***  
  
[Home webpage ](http://landenlabs.com/index.html) 

Related implementations:

* [stackoverflow discussion on CSV parser](https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c)
* [AriaFallah/csv-parser](https://github.com/AriaFallah/csv-parser)
* [ben-strasser/fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)
* [Chris Welshman/cwcsv](https://github.com/cwzx/cwcsv)
* [What is the Fastest CSV Parser](http://datahunters.us/fastest-csv-parser)
* [CSV command line tool](https://github.com/BurntSushi/xsv)
  
---  
<a name="csv"></a>  
### CSV Parser 
  
  Example usage to read, parse and store CSV table.
  
  [![Build status](https://travis-ci.org/landenlabs/csvTool.svg?branch=master)](https://travis-ci.org/landenlabs/csvTool)
  
```cpp
#include "csvtool.h"
using namespace CsvTool;

int main(int argc, const char *argv[])
{
    std::string field;    // Reuse for better memory performance.
    typedef std::vector<CsvRow> CsvTable;
    CsvTable csvTable;
    CsvRow csvRow;
    CsvParser csvParser;
    
    // Read and Parse CSV
    CsvStream inFS(argv[1], std::ifstream::binary);
    while (csvParser.getRow(inFS, field, csvRow))  {
        CsvRow nextRow;
        nextRow.reserve(csvRow.capacity());
        nextRow.swap(csvRow);
        csvTable.push_back(std::move(nextRow));
        csvRow.clear();
    }
    inFS.close();
    
    // Dump out CSV table
    for (auto &row: csvTable) {
        bool sep = false;
        for (auto &col: row) {
            if (sep) std::cout << ",";
            std::cout << col;
            sep = true;
        }
        std::cout << std::endl;
    }
    return 0;
}
```

Altnerate version to read header and then rows.
```cpp
int readCsvHeaderThenRows(int argc, const char *argv[])
{
    std::string field;    // Reuse for better memory performance.
    typedef std::vector<CsvRow> CsvTable;
    CsvTable csvTable;
    CsvRow csvHeader;
    CsvRow csvRow;
    CsvParser csvParser;

    CsvStream inFS(argv[1], std::ifstream::binary);
    if (csvParser.getRow(inFS, field, csvHeader))  {
        while (csvParser.getRow(inFS, field, csvRow))  {
            CsvRow nextRow;
            nextRow.reserve(csvRow.capacity());
            nextRow.swap(csvRow);
            csvTable.push_back(std::move(nextRow));
            csvRow.clear();
        }
    }

    inFS.close();
    return 0;
}

```


The CSV parser  adjustments:
```cppp
    char delim = ',';             // field delimeters
    const char* quotes = "'\"";   // quoting characters
    char inEOL = '\n';            // input End-Of-Line character
    bool keepQuotes = false;	  // keep quotes in parsed fields
    char ignoreEOL = '\r';        // Ignore End-Of-Line character
```

Example adjusting the parser:
```cpp
    CsvParser csvParser;
    csvParser.keepQuotes = true;
    csvParser.delim = '\t';
    
    while (csvParser.getRow(inFS, field, csvRow))  {
       . . . 
    } 
```

[To Top](#csv)  


### CSV Compare to other parsers

***Lines of code***
 
|Author| Lines of code |
|--|--|
| [Dennis csvTool](https://github.com/landenlabs/csvTool) | 60  |
| [Dennis csvTool](https://github.com/landenlabs/csvTool) | 100 with custom IBuffer  |
|[AriaFallah csvParser](https://github.com/AriaFallah/csv-parser)|331|
|[Chris Welshman/cwcsv](https://github.com/cwzx/cwcsv) | ~340 (6 files)|

[To Top](#csv)  

### CSV Performance

Comparing performance of CsvTool and AriaFallah's parser:

**Compare compiled code size**

clang++ -std=c++11 csvtool.cpp -o csvtool
clang++ -std=c++11 -O2 csvtool.cpp -o csvtoolO2

     72,688  csvTool     ; Default compilation
     29,916  csvToolO2   ; Optimized

[AriaFallah csv parser]([https://github.com/AriaFallah/csv-parser](https://github.com/AriaFallah/csv-parser))

     79,076  csvparser     ; AriaFallah's csv parser
     30,304  csvparserO2   ; AriaFallah's csv parser optimized

---
 **Compare memory usage and speed**
 
 The cpu performance is similar between the two implementations,
 The memory usage is lower with this parser. 
 
 
 ***Dennis CsvTool***
 ```
usr/bin/time -l dennis/csvTool  data/benchtest-nl.csv
        0.13 real         0.13 user         0.00 sys
    782,336  maximum resident set size
```
         
  Enable custom input buffer and (similar to AriaFallah's):
  clang++ -std=c++11 -DUSE_IBUFFER  main.cpp -o dennis/csvToolIB
  ```
/usr/bin/time -l dennis/csvToolIB data/benchtest-nl.csv
        0.10 real         0.10 user         0.00 sys
    905,216  maximum resident set size
```
  
 ***AriaFallah CsvParser***
```
/usr/bin/time -l AriaFallah/main.o data/benchtest-nl.csv
        0.10 real         0.09 user         0.00 sys
    913,408  maximum resident set size
```

---
***Optimized code***
Now compare both programs compiled with optimization
   clang++ -std=c++11 -O2 -DUSE_IBUFFER  main.cpp -o dennis/csvToolIBO2

Performance is similar for both cpu and memory, so call it a tie.

```
/usr/bin/time -l dennis/csvToolIBO2 data/benchtest-nl.csv
        0.03 real         0.03 user         0.00 sys
    884,736  maximum resident set size
```


```
/usr/bin/time -l AriaFallah/mainO2 data/benchtest-nl.csv
        0.04 real         0.03 user         0.00 sys
    888,832  maximum resident set size
```
[To Top](#csv)  

### Test / Validate

I used AriaFallah test suite and validate the parser.
Compile the tester.cpp program and run it above the data directory.

```
clang++ -std=c++11 tester.cpp -o tester

csvTool/tester
===============================================================================
All tests passed (14 assertions in 14 test cases)
```

[To Top](#csv) 

### Code details

The main.cpp source file and csvtool.h  have a collection of pre-compiler switches to adjust the csv tool behavior.

Example compiler commands:
```
clang++ -std=c++11 -O2 -DUSE_IBUFFER  main.cpp -o bin/csvTool
clang++ -std=c++11 -O2 -DKEEP_ROWS -DDUMP_TABLE  main.cpp -o bin/csvTool 
```


Code comment block listing all the pre-compiler switches:
```cpp
// Build options (defines set in code or compile command):
//
//  #define KEEP_ROWS               // Keep parsed rows, default just counts rows
//  #define USE_KEEP_QUTOES         // Keep original quotes, default removes quotes
//  #define DUMP_TABLE              // Dump CSV table to terminal, default count rows.
//  #define USE_CR_EOL              // Force CR line terminator, defaults to LF and CRLF
//
// Default no special input buffer, use either USE_IBUFFER or one of these but not both:
//  #define USE_BIG_STACK_BUFFER    // 128K stack input buffer
//  #define USE_BIG_HEAP_BUFFER     // 128k heap input buffer
//
// Build options (csvtool.h):
//  #define USE_IBUFFER             // Custon input buffer
//  #define BUFFER_ENTIRE_FILE      // Use with USE_IBUFFER to buffer entire input csv file

```

If you have very laerge files, or poor file system performnace and have adequte memory you can adjust the input buffer.
You can provide a private buffer either on the Stack or Heap.
```
    // Optional input buffering hacks
#ifdef USE_BIG_STACK_BUFFER
    static constexpr int BUFFER_SIZE = 1024 * 128;
    char bigBuffer[BUFFER_SIZE];
    inFS.rdbuf()->pubsetbuf(bigBuffer, BUFFER_SIZE);
#endif
#ifdef USE_BIG_HEAP_BUFFER
    static constexpr int BUFFER_SIZE = 1024 * 128;
    vector<char> bigBuffer(BUFFER_SIZE);
    inFS.rdbuf()->pubsetbuf(&bigBuffer[0], BUFFER_SIZE);
#endif
```

I also provide a custom input stream which includes buffering. 
```cpp
#ifdef USE_IBUFFER
// ===============================================================================================
// Custom buffered input stream.
class IBuffer {
  
    std::ifstream in;
    static constexpr int BUFFER_SIZE = 1024 * 128;
    char mBuffer[BUFFER_SIZE]{};
    size_t inPos = BUFFER_SIZE;
    size_t inLen = BUFFER_SIZE;
    bool mEof = false;
    
public:
    IBuffer(const char* file, int flags) {
        in.open(file, flags);
    }
    void close() {
        in.close();
    }
    bool eof() {
        return mEof;
    }
    bool get(char& ch) {
        if (inPos < inLen) {
            ch = mBuffer[inPos++];
            return true;
        }
        if (!in.eof()) {
            inPos = 0;
            inLen = in.read(mBuffer, BUFFER_SIZE).gcount();
            return get(ch);
        }
        mEof = true;
        return !mEof;
    }
};
typedef IBuffer CsvStream;
#else
typedef std::ifstream CsvStream;
#endif
```
[To Top](#csv) 


# Web Page  
  
[Home web page](http://landenlabs.com/android/index.html) for more information.  
  
[To Top](#csv)  
# License  
  
```  
Copyright 2019 Dennis Lang  
  
Licensed under the Apache License, Version 2.0 (the "License");  
you may not use this file except in compliance with the License.  
You may obtain a copy of the License at  
  
 http://www.apache.org/licenses/LICENSE-2.0  
Unless required by applicable law or agreed to in writing, software  
distributed under the License is distributed on an "AS IS" BASIS,  
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
See the License for the specific language governing permissions and  
limitations under the License.  
```   
  
[To Top](#csv)  
<br>[Home website](http://landenlabs.com)