# Operating-Systems-P1-Badger-Fortune

## Overview
Badger Fortune is a command-line utility developed in C for UNIX-based systems that provides fortune-telling functionality by selecting quotes from a provided database file.

## Features
- Fetch and display a specific fortune from the database file.
- Process and output multiple fortunes specified in a batch file.
- Output fortunes to the command line or a specified file.

## Prerequisites
- UNIX-based system (Linux preferred)
- C compiler with support for `-Wall` and `-Werror` flags

## Installation
1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Compile the source code using the command: gcc -Wall -Werror -o badger-fortune badger_fortune.c

## Usage
Run Badger Fortune using the following syntax: /badger-fortune -f <file> -n <number> [-o <output file>]
Or for batch mode: ./badger-fortune -f <file> -b <batch file> [-o <output file>]

- `<file>`: Path to the fortune database file.
- `<number>`: Specific fortune number to fetch.
- `<batch file>`: Path to a file containing a list of fortune numbers.
- `<output file>` (optional): Path to file where the fortunes will be written. If omitted, output will be directed to STDOUT.

## File Formats
- **Fortune file format**: Starts with the total number of fortunes and the maximum length of any fortune, followed by the fortunes themselves, each separated by a `%` character.
- **Batch file format**: Contains a list of integers, each representing the number of a fortune in the database.

## Error Handling
The program handles various errors, such as insufficient arguments, invalid flag types, file existence issues, and more. Appropriate error messages are displayed for each scenario.

## Acknowledgments
This project is inspired by the [fortune utility by Brian M. Clapper](http://software.clapper.org/fortune/) and utilizes some of his fortune database in testing.
