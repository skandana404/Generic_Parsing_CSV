#include <iostream>

#include <fstream>

#include <vector>

#include <string>

#include <algorithm>

using namespace std;

using CSVRow = vector<pair<string, string>>;         // One row of data, stored as key-value pairs (header and value )

using CSVData = vector<CSVRow>;             // Entire file, made up of multiple CSVRow entries which are key-value pairs

string RemoveMultLine(const string& input) {                       //  newlines with spaces only if the previous character wasn’t already a space.

    string output;                                              //collect the final result.

    bool prevSpace = false;                                  //flag to avoid inserting multiple spaces in a row.

    for (char c : input) {

        if (c == '\n' || c == '\r') {   // checking for newline and carriage return

            if (!prevSpace) {

                output += ' ';

                prevSpace = true;

            }

        }

        else {

            output += c;

            prevSpace = (c == ' '); //shorthand assignment same like if-esle stament -sets the value of prevSpace to true if c is a space, and false otherwise

        }

    }

    return output;

}

// Read and parse a single logical CSV line (handles multiline fields, delimiter and  escaped quotes)

vector<string> ParsingCSVLine(ifstream& file, char delimiter, char Text_Qualifier) {

    vector<string> result;          //store all parsed values from the line.

    string token;                //accumulates characters for a single value.

    string line;             //store each physical line from the file.

    bool insideQuotes = false;   //tracks whether we're inside a quoted string.

    while (getline(file, line)) {

        size_t i = 0;

        while (i < line.length()) {

            char current = line[i];

            if (current == Text_Qualifier) {

                // Handle escaped quote ""

                if (insideQuotes && i + 1 < line.length() && line[i + 1] == Text_Qualifier) {

                    token += Text_Qualifier;

                    i += 2;      // We skip both quote characters

                }

                else {

                    insideQuotes = !insideQuotes; // Toggle quote mode

                    i++;

                }

            }

            else if (current == delimiter && !insideQuotes) {         // condition checking current element is delimeter and its is not present inside the quotes

                result.push_back(RemoveMultLine(token));          // Delimiter outside quotes -> split

                token.clear();

                i++;

            }

            else {

                token += current; // Add any regular character to the token

                i++;

            }

        }

        if (!insideQuotes) {

            break;       // Logical line complete

        }

        else {

            token += '\n'; // Preserve newline inside quoted field

        }

    }

    result.push_back(RemoveMultLine(token)); // Add the last token ,RemoveMultLine(token) Cleans up internal newlines and excessive spaces in quoted fields.

    return result;

}

// Parse a CSV file into structured data

CSVData parseCSV(const string& filepath, char delimiter, char Text_Qualifier) {

    ifstream file(filepath); // Open file to read it line by line

    CSVData data;

    if (!file.is_open()) {

        cerr << "Error: Cannot open file " << filepath << endl;

        return data;

    }


    // Read the header row

    vector<string> headers;

    while (file && headers.empty()) {

        headers = ParsingCSVLine(file, delimiter, Text_Qualifier);

        // Remove empty or whitespace-only header rows  ,erase(start, end) > start = remove_if(headers.begin(), headers.end(), [](const string& h) { return h.empty() || all_of(h.begin(), h.end(), ::isspace); })

        //end= headers.end()

        headers.erase(remove_if(headers.begin(), headers.end(),

            [](const string& h) { return h.empty() || all_of(h.begin(), h.end(), ::isspace); }),                    // lambda function - anonymous function (i.e., a function without a name)

            headers.end());                                                                                         // lambda function here takes input parameter h , retuns true if h is empty or h is white space (isspace)

    }

    if (headers.empty()) {

        cerr << "Error: No valid headers found in the file or file is empty." << endl;

        return data;

    }

    // Read each record

    while (file.peek() != EOF) {                        //file.peek() reads the next character in the file without removing it from the input stream.

        vector<string> values = ParsingCSVLine(file, delimiter, Text_Qualifier);

        if (values.empty()) continue;       //If the line is empty or doesn't contain any values, just skip it and move on to the next line in the file

        CSVRow row;

        for (size_t i = 0; i < headers.size(); i++) {

            string value = (i < values.size() ? values[i] : "");     //If i is less than values.size(), then use values[i] ,Else, use an empty string ""

            row.emplace_back(headers[i], value); //headers[i] is the 'key' and value is the 'value ' ,Construct the object in place inside the vector, avoiding unnecessary copying. eg 

        }

        data.push_back(row);                  //Adds the complete row (as a list of column-value pairs) to the final data structure.  push_back() -Adds a copy or move of an existing object.

    }

    // Error check: File had headers but no data rows

    if (data.empty()) {

        cerr << "Error: Header found, but no data rows were present in the file." << endl;

    }

    return data;

}

//argc-Argument Count — how many arguments were passed (including the program name), Argument Vector argv— actual arguments as an array of C-style strings

int main(int argc, char* argv[]) {

    if (argc < 3) {            //This checks if the user provided at least one argument after the program name.

        cerr << "Usage: " << argv[0] << " <filepath> [delimiter][Text_Qualifier]" << endl;

        return 1;                                //exits the program early with an error code.

    }

    string filename = argv[1];  //Retrieves the file path (CSV file location) from the command line.

    char delimiter = (argc >= 3) ? argv[2][0] : ','; //argv[0] is always the name of the program itself.eg:argv[0] = "./parsecsv.exe" , //argv[2]  is the delimiter 

    char Text_Qualifier = (argc >= 3) ? argv[3][0] : '"';  //argv[3]  is the text qualifer


    CSVData csv = parseCSV(filename, delimiter, Text_Qualifier); // Calls parseCSV() with the file path.

    // Print parsed CSV content , csv is a vector<CSVRow> — a list of all CSV rows

    for (const auto& row : csv) {                      // auto - findes type automatically , and &- giving a reference to the original data

        for (const auto& pair : row) {                   //Each row is a vector of pairs.

            cout << pair.first << ": " << pair.second << "  ";  //pair.first - Column name ,pair.second - Actual data value for that column

        }

        cout << "\n";

    }

    return 0;

}

