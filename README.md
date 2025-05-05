Attached testcase Doc : [ParsingCSV_TSD.xlsx](https://github.com/user-attachments/files/20033540/ParsingCSV_TSD.xlsx)

# Generic CSV File Parsing Logic in C++

This project provides a robust CSV parser written in C++ that handles:

- Custom delimiters
- Quoted multiline fields
- Escaped quotes inside quoted fields
- Whitespace and empty line handling
- Error messages for missing headers or data

## Features

- **Handles edge cases** such as multiline fields (`"New\nYork"`), escaped quotes (`""`), and missing data.
- **Flexible input**: You can specify both the delimiter and the text qualifier.
- **Cleans data** by removing redundant newlines and managing whitespace.

## Usage

### Compilation

```bash
g++ -o ParsingFile Parsing.cpp
```

### Running the Program

```bash
./ParsingFile <filepath> [delimiter] [text_qualifier]
```

- `filepath`: Path to the CSV file.
- `delimiter` (optional): Default is comma `,`.
- `text_qualifier` (optional): Default is double quote `"`.

### Example

```bash
./ParsingFile input.csv , "
```

## Sample CSV

```
Name,Address,Notes
"John Doe","123 Main St","Loves
pizza"
"Jane, Smith","456 Side St","Allergic to ""peanuts"""
```

### Output

```
Name: John Doe  Address: 123 Main St  Notes: Loves pizza
Name: Jane, Smith  Address: 456 Side St  Notes: Allergic to "peanuts"
```

## Error Handling

- Prints an error if the file is missing or can't be opened.
- Alerts if the header row is missing or empty.
- Warns if headers are present but no data rows are found.

## Code Structure

- `RemoveMultLine()`: Flattens newlines and trims spaces.
- `ParsingCSVLine()`: Parses logical CSV rows respecting quoted(text qualifers) content and delimiters.
- `parseCSV()`: Reads and structures CSV data into a `vector` of rows.
- `main()`: Takes command-line inputs and prints the parsed output.

