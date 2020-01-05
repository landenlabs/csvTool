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

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
 
// using namespace std;

namespace CsvTool {

// Build options:
//  #define USE_IBUFFER
//  #define KEEP_ROWS
//  #define USE_KEEP_QUTOES
//  #define DUMP_TABLE
//  #define USE_BIG_STACK_BUFFER
//  #define USE_BIG_HEAP_BUFFER
//  #define USE_CR_EOL

#ifdef USE_IBUFFER
// ===============================================================================================
// Custom buffered input stream.
class IBuffer {
  
    ifstream in;
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

typedef std::vector<std::string> CsvFields;
typedef CsvFields CsvRow;


// ===============================================================================================
// Split string into (comma-separated-values) parts
// Support fields spanning rows, quoted fields and escaped quotes.
class CsvParser
{
  public:
    char delim = ',';
    const char* quotes = "'\"";
    char inEOL = '\n';
    bool keepQuotes = false;
    char ignoreEOL = '\r';
    
  public:

    // Parse a CSV row from input stream, store in fields using field as working storage.
    bool getRow(CsvStream& istream, std::string& field, CsvFields& fields)  {
        char ch;
        while (!istream.eof()) {
            field.clear();
            
            // Remove unwanted leading characters.
            while (istream.get(ch) && (isspace(ch) || ch == ignoreEOL));
            if (ch == inEOL || ch == '\0')
                return !istream.eof();
            
            if (strchr(quotes, ch) != nullptr)  {
                // Handle quoted field.
                char quote = ch;
                bool addQuote = keepQuotes;
                do {
                    if (addQuote)
                        field.push_back(quote);
                    while (istream.get(ch) && ch != quote) {
                        field.push_back(ch);
                    }
                    addQuote = true;
                    // Handle escaped field.
                } while (istream.get(ch) && ch == quote);
                if (ch != delim && ch != inEOL) {
                    while (istream.get(ch) && ch != delim && ch != inEOL);
                }
                if (keepQuotes)
                    field.push_back(quote);
            }
            else
            {
                do {
                    if (ch == delim || ch == inEOL) {
                        break;
                    } else if (ch != ignoreEOL) {
                        field.push_back(ch);
                    }
                } while (istream.get(ch));
            }
            fields.push_back(field);
            if (ch == inEOL)
                break;
        }

        return !fields.empty(); //  !istream.eof();
    }
};

} // namespace
