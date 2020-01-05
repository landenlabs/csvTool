
# LanDen Labs - UI Test  
CsvTool [Home website](http://landenlabs.com)  
4-Jan-2020  
  
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
  
---  
<a name="csv"></a>  
### CSV Parser 
  
  Example usage to read, parse and store CSV table.
  
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
| [Dennis csvTool](https://github.com/landenlabs/csvTool) | 58 |
|[AriaFallah csvParser](https://github.com/AriaFallah/csv-parser)|331|

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