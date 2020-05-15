###### **MTL342 Programming Assignment**
# **Baseball Elimination**
***

##### **CONTENTS OF THIS FILE**
- Introduction
- Prequisites
- Compilation
- Execution
- Top-Down Analysis
***

##### **INTRODUCTION**
The program **baseballEliminations.cpp** is used to determine which teams have been mathematically eliminated from the division in a sports league when standings are given at some point during the season.
When provided with the input, this program, for each team creates a flow network and applies the Edmonds–Karp version of Ford-Fulkerson algorithm to find the max flow and hence determine if the team is eliminated or not.
*  For a full description of the problem, visit [here](https://www.cs.princeton.edu/courses/archive/spring04/cos226/assignments/baseball.html).
*  For a full description of the Edmonds-Karp algorithm, visit [here](https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm).
***
##### **PREREQUISITES**
- First a c++ compiler like, g++ (MinGW.org GCC Build-20200227-1) 9.2.0, should be installed.
- Then iostream, fstream, sstream, climits, string, vector, stack, queue and algorithm header files should be available.
- The input format is the number of teams in the division n followed by one line for each team. Each line contains the team name (with no internal whitespace characters), the number of wins, the number of losses, the number of remaining games, and the number of remaining games against each team in the division.
***
##### **COMPILATION**
- In windows, `g++ -o baseballElimination.exe baseballElimination.cpp` command in command prompt will compile an create an executable.
- In linux, `g++ baseballElimination.cpp` in terminal will create the object.
***
##### **Execution**
- In windows, `baseballElimination.exe` command in command prompt will execute the program. Just enter the input file path.
- In linux, `./a.out` in terminal will execute the program. Again enter the input file path.
***
##### **Top-Down Analysis**
- First the input file is used to create a division. Here division is a class which stores all the data in their right data type.
- Then after creating an object of divison we use it to create a flow network which is a class for graph. There we use the Ford-Fulkerson algorithm to determine if the team is eliminated or not.
***

## Authors

* **Avanish Kumar Singh** - *2017MT10728*