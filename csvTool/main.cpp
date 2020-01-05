//
// Read, Parse and Store CSV data.
//
// Created by Dennis Lang on 3-Jan-2020
//
// ---
// Copyright (C) 2019 Dennis Lang (landenlabs@gmail.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "csvtool.h"

// using namespace std;
using namespace CsvTool;

// Build options:
//  #define USE_IBUFFER
#define KEEP_ROWS
//  #define USE_KEEP_QUTOES
#define DUMP_TABLE
//  #define USE_BIG_STACK_BUFFER
//  #define USE_BIG_HEAP_BUFFER
// #define USE_CR_EOL


// ===============================================================================================
int main(int argc, const char *argv[])
{
    std::string field;    // Reuse for better memory performance.

#ifdef KEEP_ROWS
    // typedef vector<string> CsvRow;
    typedef std::vector<CsvRow> CsvTable;
    CsvTable csvTable;
#endif
    CsvRow csvRow;
    CsvParser csvParser;
    const char outEOL = '\n';
    
    CsvStream inFS(argv[1], std::ifstream::binary);
    
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
    
    // Parse and count rows
#ifdef USE_CR_EOL
    csvParser.inEOL = '\r';
#endif
#ifdef USE_KEEP_QUTOES
    csvParser.keepQuotes = true;
#endif
    unsigned rowCount = 0;
    while (csvParser.getRow(inFS, field, csvRow))  {
#ifdef KEEP_ROWS
        CsvRow nextRow;
        nextRow.reserve(csvRow.capacity());
        nextRow.swap(csvRow);
        csvTable.push_back(std::move(nextRow));
#endif
        csvRow.clear();
        rowCount++;
    }

    inFS.close();
    
#if defined(DUMP_TABLE) && defined(KEEP_ROWS)
    // Dump out rows
    for (auto &row: csvTable)
    {
        bool sep = false;
        for (auto &col: row)
        {
            if (sep) std::cout << ",";
            std::cout << col;
            sep = true;
        }
        std::cout << outEOL;
    }
#else
    // Display parsed row count.
    std::cout << rowCount << outEOL;
#endif
    return 0;
}
