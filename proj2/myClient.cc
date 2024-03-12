// COPYRIGHT 2024 Rae Jones
#include <proj2/myClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>     
#include <iostream>
#include <vector>
#include <sys/sysinfo.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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
                //std::cout << a << "+" << b << subtract(a, b);
            } else {
                // perform subtraction, replace "-" with result
                *itr = std::to_string(subtract(a, b));
               //std::cout << a << "-" << b << subtract(a, b);
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
std::string run(std::vector<std::string> eqn) {
    std::vector<std::string> MDVect = multDiv(eqn);  // MD of pemdas done
    std::vector<std::string> newVect = addSub(MDVect);  // AS of pemdas done
        //std::cout << "poop" << std::endl;
    // print the resulting vector
    std::stringstream ss;
    for (auto itr = newVect.begin(); itr != newVect.end(); ++itr) {
        //std::cout << *itr << std::endl;
        ss << *itr << " ";
    }
    //std::cout << newVect.at(0) << std::endl;

    std::string answer = newVect.at(0);
    return answer;
}

std::vector<std::string> loadData(std::string str) {
    std::vector<std::string> data;
    	std::istringstream s(str);
    	std::string line;
    int count = 0;
    while (std::getline(s,line)) {
    	if (count!= 0) {
    		std::string crop = line.substr(line.find(": ")+2, line.length());
    		//std::cout << crop << std::endl;
    		data.push_back(crop);
    	}
    	count++;
    }
    return data;
}

std::vector<std::string> parseArgs(std::string eqn) {
	std::vector<std::string> parsedEqn;
        std::istringstream ss(eqn);
        std::string element;
        while (ss >> element) 
        {
            //std::cout << element << std::endl;
            parsedEqn.push_back(element);
        }
    return parsedEqn;   
}

std::string clientEqns(std::vector<std::string> data, std::vector<std::string> argLines) {
    std::string finalStrng = "";
    for (int i=0; i<argLines.size(); i++) {
        int curr = stoi(argLines.at(i))-1;
        std::string eqn = data.at(curr);
        //std::cout << "EQUATION: " << eqn << std::endl;

        //std::istringstream ss(eqn);
        //std::string element;
        //while (ss >> element) 
        //{
        ///    std::cout << element << std::endl;
        //    parsedEqn.push_back(element);
        //}
        //return parsedEqn; //fix hoe
        finalStrng.append("line ");
        finalStrng.append(std::to_string(curr+1));
        finalStrng.append(": ");
        finalStrng.append(eqn);
        finalStrng.append("\n");
    }
    return finalStrng;
}

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  int ret;
  int data_socket;
  char buffer[BUFFER_SIZE];

  /* Create local socket. */

  data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (data_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /*
   * For portability clear the whole structure, since some
   * implementations have additional (nonstandard) fields in
   * the structure.
   */

  memset(&addr, 0, sizeof(addr));

  /* Connect socket to socket address. */

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

  ret = connect(data_socket, (const struct sockaddr *)&addr, sizeof(addr));
  if (ret == -1) {
    fprintf(stderr, "The server is down.\n");
    exit(EXIT_FAILURE);
  }

  /* Send arguments. */

  for (size_t i = 1; i < argc; ++i) {
    ret = write(data_socket, argv[i], strlen(argv[i]) + 1);
    if (ret == -1) {
      perror("write");
      break;
    }
  }

  /* Request result. */

  strcpy(buffer, "END");
  ret = write(data_socket, buffer, strlen(buffer) + 1);
  if (ret == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  /* Receive result. */

  ret = read(data_socket, buffer, sizeof(buffer));
  if (ret == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  /* Ensure buffer is 0-terminated. */

  buffer[sizeof(buffer) - 1] = 0;

  printf("%.26s\n", buffer); // get rid of this

std::vector<std::string> data = loadData(buffer);
  std::string allEqn =  buffer;
  allEqn = allEqn.erase(0,allEqn.find("\n")+1);
  
  
      for (int i=0; i<data.size(); i++) {
      
      	       std::string curr = data.at(i);
      	       //std::cout << allEqn.find_last_of("line") << std::endl;
      	       std::string newStr = allEqn.substr(0,allEqn.find("\n"));
      	       std::vector<std::string> finalEqn = parseArgs(curr);
 
      	       // run(finalEqn);
      	       std::cout << newStr << " = " << run(finalEqn)<< std::endl;
      	       allEqn = allEqn.erase(0,allEqn.find("\n")+1);
      	       
      }
      
      std::cout << "BYTES RECIEVED: " << sizeof(buffer) << std::endl;


  /* Close socket. */

  close(data_socket);

  exit(EXIT_SUCCESS);
}
