#ifndef PROJ2_MYSERVER_H_
#define PROJ2_MYSERVER_H_
#define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nxy.socket" // get rid of this
#define BUFFER_SIZE 12 // get rid of this

#include <iostream>
#include <string>
#include <vector>

float add(float a, float b) {
    return a + b;
}
float subtract(float a, float b) {
    return a - b;
}
float multiply(float a, float b) {
    return a*b;
}

float divide(float a, float b) {
    return a/b;
}

// finds and performs all the addition and subtraction operations in vector
std::vector<std::string> addSub(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while ( itr != result.end() ) {
        if (*itr == "+" || *itr == "-") {
            // get the two surrounding floats
            float a = std::stof(*(itr-1));
            float b = std::stof(*(itr+1));

            if (*itr == "+") {
                // perform addition, replace "+" with result
                *itr = std::to_string(add(a, b));
            } else {
                // perform subtraction, replace "-" with result
                *itr = std::to_string(subtract(a, b));
            }

            // remove the two surrounding floats
            itr = result.erase(itr-1);
            itr = result.erase(itr+1);
        } else {
            // move to the next element
            ++itr;
        }
    }
    // returns new vector
    return result;
}

// finds and performs all the addition and subtraction operations in vector
std::vector<std::string> multDiv(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while (itr != result.end()) {
        if (*itr == "x" || *itr == "/") {
            // get the two surrounding floats
            float a = std::stof(*(itr-1));
            float b = std::stof(*(itr+1));

            if (*itr == "/") {
                // perform division, replace "/" with result
                *itr = std::to_string(divide(a, b));
            } else {
                // perform multiplication, replace "x" with result
                *itr = std::to_string(multiply(a, b));
            }

            // remove the two surrounding floats
            itr = result.erase(itr-1);
            itr = result.erase(itr+1);

        } else {
            // move to the next element
            ++itr;
        }
    }

    // returns new vector
    return result;
}

// needs to accept negs
void run(std::vector<std::string> eqn) {
    std::vector<std::string> MDVect = multDiv(eqn);  // MD of pemdas done
    std::vector<std::string> newVect = addSub(MDVect);  // AS of pemdas done

    // print the resulting vector
    for (auto itr = newVect.begin(); itr != newVect.end(); ++itr) {
        std::cout << *itr << std::endl;
    }
}

std::vector<std::string> parseArgs(string data, std::vector<std::string> args) {
    
    return args;
}

string loadData(string fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> data;
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            data += line + "\n";
        }
        file.close();
    }
    return data;
}


#endif  // PROJ2_MYSERVER_H_
