// COPYRIGHT Rae Jones 2024
#include <proj2/myServer.h>
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
                std::cout << a << "+" << b << subtract(a, b);
            } else {
                // perform subtraction, replace "-" with result
                *itr = std::to_string(subtract(a, b));
               std::cout << a << "-" << b << subtract(a, b);
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
        std::cout << "poop" << std::endl;
    // print the resulting vector
    std::stringstream ss;
    for (auto itr = newVect.begin(); itr != newVect.end(); ++itr) {
        std::cout << *itr << std::endl;
        ss << *itr << " ";
    }
    //std::cout << newVect.begin() << std::endl;
    //ss << newVect.begin();
    //return ss.str();
}

std::vector<std::string> loadData(std::string fileName) {
        std::ifstream currFile (fileName);
    std::vector<std::string> data;
    std::string line;
    if (currFile.is_open()) {
        while (getline(currFile, line)) {
        	//std::cout << line << std::endl;
		data.push_back(line);
		//std::cout << "kijdb" << data.back() << std::endl;
		
        }
        currFile.close();
    }
    if (!currFile) {
    	std::cout << "file does not exist" << std::endl;
    	data.push_back("INVALID FILE");
    	
    	return data;
    }

    return data;
}

void parseArgs(std::vector<std::string> data, std::vector<std::string> argLines) {
    for (int i=0; i<argLines.size(); i++) {
        std::vector<std::string> parsedEqn;
        int curr = stoi(argLines.at(i))-1;
        std::string eqn = data.at(curr);
        std::cout << "EQUATION: " << eqn << std::endl;

        std::istringstream ss(eqn);
        std::string element;
        while (ss >> element) 
        {
            std::cout << element << std::endl;
            parsedEqn.push_back(element);
        }
        //return parsedEqn; //fix hoe
    }
    //return ;
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
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    char buffer[BUFFER_SIZE];
    std::string path;

           

	
           /* Create local socket. */

           connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
           if (connection_socket == -1) {
               perror("socket");
               exit(EXIT_FAILURE);
               close(connection_socket);

           /* Unlink the socket. */


           unlink(SOCKET_NAME);
           }
           

	
           /*
            * For portability clear the whole structure, since some
            * implementations have additional (nonstandard) fields in
            * the structure.
            */

           memset(&name, 0, sizeof(name));

           /* Bind socket to socket ame. */

           name.sun_family = AF_UNIX;
           strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

           ret = bind(connection_socket, (const struct sockaddr *) &name, sizeof(name));
           if (ret == -1) {
               perror("bind");

               close(connection_socket);

           /* Unlink the socket. */


           unlink(SOCKET_NAME);
           exit(EXIT_FAILURE);
           }
           
           int maxClients = get_nprocs_conf() - 1;

	std::cout<< "\nSERVER STARTED\n" << "MAX CLIENTS: " << maxClients << std::endl;

           /*
            * Prepare for accepting connections. The backlog size is set
            * to 20. So while one request is being processed other requests
            * can be waiting.
            */

           ret = listen(connection_socket, 64);
           if (ret == -1) {
               perror("listen");
               exit(EXIT_FAILURE);
           }

           /* This is the main loop for handling connections. */

           for (;;) {

               /* Wait for incoming connection. */

               data_socket = accept(connection_socket, NULL, NULL);
               if (data_socket == -1) {
                   perror("accept");
                   exit(EXIT_FAILURE);
               }
               
               	//std::cout<< "CLIENT CONNECTED" << std::endl;
                   /* Wait for next data packet. */
                    std::vector<std::string> data;
    		std::vector<std::string> argLines;
    		
    		//sprintf(buffer, "%s", "SERVER CONNECTION ACCEPTED");
                //ret = write(data_socket, buffer, sizeof(buffer));
               
               
               result = 0;
               int i = 0;
               for (;;) {

                   /* Wait for next data packet. */

                   ret = read(data_socket, buffer, sizeof(buffer));
                   if (ret == -1) {
                       //perror("read");
                       //exit(EXIT_FAILURE);
                   }

                   /* Ensure buffer is 0-terminated. */

                   buffer[sizeof(buffer) - 1] = 0;

                   /* Handle commands. */

                   if (!strncmp(buffer, "DOWN", sizeof(buffer))) {
                       down_flag = 1;
                       break;
                   }

                   if (!strncmp(buffer, "END", sizeof(buffer))) {
                       break;
                   }

                   /* Add received summand. */
	
                    if (i==0) {
                        path = std::string(buffer);
                        data = loadData(buffer); // "/acct/sej15/Desktop/CSCE311-002/proj2/dat/equations_1.txt"
                        //std::cout << data.back();
                        if (data.at(0) == "INVALID FILE") {
                             sprintf(buffer, "%s", "INVALID FILE");
               			ret = write(data_socket, buffer, sizeof(buffer));
                    
               			break;
                        
                        }

                        std::cout << "PATH: " << buffer << std::endl;
                    }
                    else if (i==1) {
                        std::cout << "Lines: " << buffer;
                        argLines.push_back(std::string(buffer));
                    }
                    else {
                        //result += atoi(buffer);
                        std::cout << ", " << buffer;

                        argLines.push_back(std::string(buffer));

                    }
                    i++;
                }
		std::cout << "\n";
                //std::string answer = run(parseArgs(data,argLines));
                
               buffer[sizeof(buffer) - 1] = 0;
               std::string eqnstr = clientEqns(data, argLines);
               std::cout << eqnstr << std::endl;
                	
               	
               	sprintf(buffer, "SERVER CONNECTION ACCEPTED\n%s", eqnstr.c_str());
               	ret = write(data_socket, buffer, sizeof(buffer));
                

                   
                   

               /* Send result. */


               sprintf(buffer, "%d", result);
               ret = write(data_socket, buffer, sizeof(buffer));
               if (ret == -1) {
                   //perror("write");
                   //exit(EXIT_FAILURE);
               }

               /* Close socket. */

               close(data_socket);

               /* Quit on DOWN command. */

               if (down_flag) {
                   break;
               }
           }

           close(connection_socket);

           /* Unlink the socket. */


           unlink(SOCKET_NAME);

           exit(EXIT_SUCCESS);
       }
