// An example of:
//   - Reading command line arguments,
//   - Using the string class constructor to build string class objects from
//     cstrings (null-terminated character arrays),
//   - Filling a vector from a source using a for-loop,
//   - Iterating over a vector using a ``for each'' loop, and
//   - Iterating over a vector using an interator.
//

//g++ --std=c++2a -o poop proj1/poop.cc


#include <iostream>
#include <string>
#include <vector>

// The main entry point of program.
//
// The two parameters argc and argv store the number of arguments supplied at
// the command line (including the command itself, "./main") and the
// white-space delimited strings, respectively.
//
int main(int argc, char* argv[]) {
  std::vector<std::string> arguments;


  // for-loop to fill vector of strings, using pointer as a ``dumb iterator''
  for (char** ptr = argv; ptr != argv + argc; ++ptr)
    // Use std::string(const char[]) constructor to fill vector of std::string
    // Could make implicit call as: arguments.push_back(*ptr)
    arguments.push_back(std::string(*ptr));
  //   EQUIVALENT TO
  // for (int i = 0; i < argc; ++i)
  //   arguments.push_back(std::string(argv[i]));
  //     OR
  //   arguments.push_back(argv[i]);  // implicitly calls std::string(argv[i])


  // for-each loop to print vector of strings
  for (const std::string& argument : arguments)
    std::cout << argument << ' ';  // argument in this context is a read-only
                                   // reference to each std::string stored in
  std::cout << std::endl;          // arguments
  


  // use iterator to add pre and post characters to arguments
  for (auto itr = arguments.begin(); itr != arguments.end(); ++itr)
    *itr = '>' + *itr + '<';

  // use const iterator to print results of previous update
  for (auto c_itr = arguments.cbegin(); c_itr != arguments.cend(); ++c_itr)
    std::cout << *c_itr << std::endl;


  // for-each loop to remove previous character additions
  for (std::string& argument : arguments)
    // use substring method to remove first and last character
    argument = argument.substr(1, argument.length() - 2);  // argument is
                                                           // a write-able
                                                           // reference in this
                                                           // context

  // for-each loop to print vector of strings
  for (const auto& argument : arguments)
    std::cout << argument << ' ';  // argument in this context is a read-only
                                   // reference to each std::string stored in
  std::cout << std::endl;          // arguments


  return 0;  // normal exit code
}