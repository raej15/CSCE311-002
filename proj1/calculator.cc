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