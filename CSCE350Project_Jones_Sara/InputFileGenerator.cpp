// Written By Rae/Sara Jones
// Final Project for CSCE350-002 2023

#include <fstream>
#include <iostream>
#include <random>

using namespace std;

//generates random number -100 to 100 inclusive
string genRand(int num) {
    string list = "";
    std::random_device generator;
    std::uniform_real_distribution<float> distribution(-100,100);
    for (int i = 0; i < num; i++) {
        //appends each rand num to the end of a list, separated by a space
        list = list.append(to_string(distribution(generator))+ " ");
    }

    return list;
}

void createFiles() {
    //creates 3 different sizes of files (10,100,1000)
    for (int i=10; i<=1000; i=i*10){
        //creates 25 files for each size
        for (int j=1;j<=25;j++) {
            ofstream file("inputFiles/size" +to_string(i)+"/size" + to_string(i)+ "_" +to_string(j)+".txt");
            //writes the random list to the file
            file << genRand(i);
            file.close();
        }
    }
}

int main() {
    createFiles();
}