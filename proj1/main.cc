#include <proj1/calculator.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

// int main(){
//     char* a = "1";
//     char* b = "2";
//     run(a, b, '-');
//     return 0;
// }
// int main(int argc, char **argv) {
//     // g++ -o Jones_Sara_QuickSort Jones_Sara_QuickSort.cpp 
//     // ./Jones_Sara_QuickSort inputFiles/size10/size10_1.txt output.txt
     
//     if (argc == 1) {
//         cout << "Needs 2 additional arguments passed"
//              << endl;
//     }
//     if (argc == 3) {
//         cout << "----Command Line Arguments Passed----\n"
//              << argv[1] << " " << argv[2]
             
//              << endl;
        
//         run(argv[1], argv[2]);
        
//     }
//     if (argc > 3) {
//         cout << "Exceeded Required Number of Arguments, Try Again" << endl;
//     }
 
//     return 0;
// }
int main(int argc, char* argv[]) {
    std::vector<std::string> arguments;
    for (char** ptr = argv+1; ptr != argv + argc; ++ptr) {
        arguments.push_back(std::string(*ptr));
    }

//   // for-each loop to print vector of strings
//   for (const std::string& argument : arguments)
//     std::cout << argument << ' ';  // argument in this context is a read-only
//                                    // reference to each std::string stored in
//   std::cout << std::endl;          // arguments
  


//   // use iterator to add pre and post characters to arguments
//   for (auto itr = arguments.begin(); itr != arguments.end(); ++itr)
//     *itr = '>' + *itr + '<';

//   // use const iterator to print results of previous update
//   for (auto c_itr = arguments.cbegin(); c_itr != arguments.cend(); ++c_itr)
//     std::cout << *c_itr << std::endl;


//   // for-each loop to remove previous character additions
//   for (std::string& argument : arguments)
//     // use substring method to remove first and last character
//     argument = argument.substr(1, argument.length() - 2);  // argument is
//                                                            // a write-able
//                                                            // reference in this
//                                                            // context

//   // for-each loop to print vector of strings
//   for (const auto& argument : arguments)
//     std::cout << argument << ' ';  // argument in this context is a read-only
//                                    // reference to each std::string stored in
//   std::cout << std::endl;          // arguments

    run2(arguments);
    return 0;
}