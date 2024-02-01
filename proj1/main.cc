#include <proj1/calculator.h>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<std::string> args;
    for (char** ptr = argv+1; ptr != argv + argc; ++ptr) {
        args.push_back(std::string(*ptr));
    }

    run(args);

    return 0;
}