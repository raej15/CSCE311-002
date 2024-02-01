#include <proj1/calculator.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
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

void run(char* x, char* y, char op) {
    float a = atof(x);
    float b = atof(y);

    float result;

    switch(op) {
        case '+':
            result = add(a, b);
            break;
        case '-':
            result = subtract(a, b);
            break;
        case 'x':
            result = multiply(a, b);
            break;
        case '/':
            result = divide(a, b);
            break;
        default:
            std::cout << "Invalid operator" << std::endl;
    }

    std::cout << result << std::endl;
}
std::vector<std::string> addSub(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while(itr != result.end()) {
        if (*itr == "+" || *itr == "-") {
            float a = std::stof(*(itr-1));
            //std::cout << "a: "<< a << std::endl;
            float b = std::stof(*(itr+1));
            //std::cout << "b: " << b << std::endl;

            if (*itr == "+") {
                // perform division, replace "/" with result
                *itr = std::to_string(add(a, b));
            }
            else {
                // perform division, replace "/" with result
                *itr = std::to_string(subtract(a, b));            
            }
            
            // remove the two surrounding floats
            itr = result.erase(itr-1);
            itr = result.erase(itr+1);
   
            std::cout << "new" << std::endl;
        }
        else {
            ++itr;
        }

        //  for (auto itr = result.begin(); itr != result.end(); ++itr){
        // std::cout << *itr << std::endl;
    }
    
    std::cout << "over" << std::endl;
    return result;
}

std::vector<std::string> multDiv(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while(itr != result.end()) {
        if (*itr == "x" || *itr == "/") {
            float a = std::stof(*(itr-1));
            //std::cout << "a: "<< a << std::endl;
            float b = std::stof(*(itr+1));
            //std::cout << "b: " << b << std::endl;

            if (*itr == "/") {
                // perform division, replace "/" with result
                *itr = std::to_string(divide(a, b));
            }
            else {
                // perform division, replace "/" with result
                *itr = std::to_string(multiply(a, b));            
            }

            // remove the two surrounding floats
            itr = result.erase(itr-1);
            itr = result.erase(itr+1);
   
            std::cout << "new" << std::endl;
        }
        else {
            ++itr;
        }

        //  for (auto itr = result.begin(); itr != result.end(); ++itr){
        // std::cout << *itr << std::endl;
    }
    
    std::cout << "over" << std::endl;

    for (auto itr = result.begin(); itr != result.end(); ++itr){
        std::cout << *itr << std::endl;
    }
    return result;
    // do the same for addition and subtraction
    //addSub(result);
}


void run2(std::vector<std::string> eqn) {
    //PE(MD)(AS)
    //  for (auto itr = eqn.begin(); itr != eqn.end(); ++itr){
    //     std::cout << *itr << std::endl;
    //  }
    std::vector<std::string> MDVect = multDiv(eqn);
    std::vector<std::string> newVect = addSub(MDVect);

    for (auto itr = newVect.begin(); itr != newVect.end(); ++itr){
        std::cout << *itr << std::endl;
     }



}
