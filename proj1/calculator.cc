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
std::vector<std::string> multDiv(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while(itr != result.end()){
        if (*itr == "x" || *itr == "/") {
            float a = std::stof(*(itr-1));
            std::cout << "a: "<< a << std::endl;
            float b = std::stof(*(itr+1));
            std::cout << "b: " << b << std::endl;

            if (*itr == "/") {
                //std::cout << "result: " << *itr << std::endl;
                *itr = std::to_string(divide(a, b));

                itr = result.erase(itr-1);
                itr = result.erase(itr+1);
            
                //itr = result.erase(itr+1);
                //itr = std::next(itr);
                //itr = itr + result.begin() + 1;
                
                //result.erase(itr+1);
                std::cout << "new" << std::endl;
                //multDiv(result);
                //++itr;
                //std::cout << "result: " << divide(a, b) << std::endl;

                //std::cout << "result: " << *itr << std::endl;
            }
            else {
                multiply(a, b);
            }
        }
        else {
            ++itr;
        }
         for (auto itr = result.begin(); itr != result.end(); ++itr){
        std::cout << *itr << std::endl;
     }

        // if(*itr == "x"){
        //     float a = std::stof(*(itr-1));
        //     float b = std::stof(*(itr+1));
        //     float c = multiply(a, b);
        //     result.push_back(std::to_string(c));
        //     itr += 2;
        // }
        // else if(*itr == "/"){
        //     float a = std::stof(*(itr-1));
        //     float b = std::stof(*(itr+1));
        //     float c = divide(a, b);
        //     result.push_back(std::to_string(c));
        //     itr += 2;
        // }
        // else{
        //     result.push_back(*itr);
        // }
        
    }

    std::cout << "over" << std::endl;
    return result;
}
void run2(std::vector<std::string> eqn) {
    //PE(MD)(AS)
     for (auto itr = eqn.begin(); itr != eqn.end(); ++itr){
        std::cout << *itr << std::endl;
     }
    multDiv(eqn);

}
