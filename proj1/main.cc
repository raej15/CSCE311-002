#include "main.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

int main(){
    char* a = "1";
    char* b = "2";
    run(a, b, '-');
    return 0;
}
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