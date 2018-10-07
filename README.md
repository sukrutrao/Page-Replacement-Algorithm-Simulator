# Page Replacement Algorithm Simulator

[![Build Status](https://travis-ci.com/GoodDeeds/Page-Replacement-Algorithm-Simulator.svg?token=mYsgPy4zsL5qQDoHBaME&branch=master)](https://travis-ci.com/GoodDeeds/Page-Replacement-Algorithm-Simulation)
	
Program to accept number of physical frames, list of page accesses, and the page replacement algorithm and output the number of faults and whether each access was a fault or not. Supports FIFO, LRU, and OPTIMAL algorithms.


## Setup
### Prerequisites
A C++ compiler, such as g++, is required to build the program.

### Building the program
Use
```bash
$ git clone https://github.com/GoodDeeds/Page-Replacement-Algorithm-Simulator.git
$ cd Page-Replacement-Algorithm-Simulator
$ g++ simulator.cpp -o simulator
```

## Input file specifications

There must be an input file named `PageAccessSequence.txt` in the directory. The first line in the file should contain the number of physical frames. Each subsequent line represents one page access, and contains exactly one integer, which represents the page number being accessed.

All values must be non negative and fit in the `int` data type of the system.

## Running the program

Three modes are supported:
	1. FIFO - First In First Out Algorithm 
	2. LRU - Least Recently Used Algorithm 
	3. OPTIMAL - Optimal Algorithm 
	
To execute in a given mode, say `MODE`, use 

```bash
$ ./simulator MODE
```

## Output

Based on the algorithm, the output will be stored in an output file, in the same directory. If the mode was `MODE`, the output file will be `CS15BTECH11036_MODE.out`.

The first line of the output file contains the number of page faults. Each subsequent line corresponds to the page access from the input file. The output is `FAULT` if there was a page fault, and `NOFAULT` otherwise.

## License
This program is provided under the [MIT License](LICENSE).
