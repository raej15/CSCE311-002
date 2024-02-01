// Written By Rae/Sara Jones
// Final Project for CSCE350-002 2023

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono;

using namespace std;

// prints array
void printArr(float arr[], int length) {
    for (int i = 0; i <= length; i++){
        std::cout << arr[i] << ", ";
    }
    std::cout << "\n";
}

// swaps two elements in array
float* swap(float arr[], int i, int j) {
    float temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;

    return arr;
}

// finds median of three elements
float newMedianOfThree (float left, float med, float right) {
    if (left > med){
        if (med > right){
            return med;
        }
        else if (left > right){
            return right;
        }
        else {
            return left;
        }
    }
    else {
        if (right > med){
            return med;
        }
        else if (left > right){
            return left;
        }
        else {
            return right;
        }
    }
}

// partitions array during quicksort
int partition(float arr[],int left,int right)
{
    int mid = (left + right) / 2;

    // uses median of three to find pivot
    float pivot = newMedianOfThree(arr[left], arr[mid], arr[right]);

    int i = left-1;
    int j = right + 1;

    // works based on algorithm in textbook
    do {
        do {
            i++;
        } while  (arr[i] < pivot) ;
        do {
            j--;

        } while (arr[j] > pivot) ;

        arr = swap(arr, i, j);
    } while (i<j);
    arr = swap(arr, i, j);
    arr = swap(arr, left, j);

    return j;
}

// quicksort algorithm
float* quickSort(float arr[], int left, int right) {
   
    if (left < right) {
        // partitions array and returns index of pivot
        int s = partition(arr, left, right);

        // recursively calls quicksort on left and right sides of pivot
        quickSort(arr, left, s);
        quickSort(arr, s+1, right);
    }

    return arr;
}
void outputSortedArray(float arr[], int length, char* b) {
    string fileName = b;
    ofstream file(fileName);
    for (int i = 0; i <= length; i++){
        file << arr[i] << " ";
    }
    file.close();
}

// appends exec time to Jones_Sara_executionTime.txt
void outputDuration(double duration, int size, char* b) {
    //does she need three files for the command line?? -- no apppend exec time to big file
    ofstream file("Jones_Sara_executionTime.txt", ios::app);
        file << std::scientific << duration << "    " << std::defaultfloat << size << "\n";
    file.close();
}

// runs quicksort on input file and outputs sorted array to output file + calls method to append exec time to Jones_Sara_executionTime.txt
void runFiles(char* a, char* b) {
    string fileName = a;
    ifstream file(fileName);
    string word;
    int count=0;

    vector<float> nums;
    while (file >> word) {
        nums.push_back(stof(word));
    }
    file.close();

    int size = nums.size();
    float arr[size];
    for (int i=0; i<size; i++) {
        arr[i] = nums[i];
    }

    microseconds micro(1000); 
     
    micro = micro*60;
     
   auto startTime = std::chrono::steady_clock::now();


    //get duration in microseconds
    float* sortedArr = quickSort(arr, 0, size-1);
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double, std::micro>(endTime - startTime); // or std::micro, std::giga, etc
    
    outputSortedArray(sortedArr, size-1, b);
    outputDuration(duration.count(), size, b);


}

//
void readOutputFile(char* a) {
    string fileName = a;
    ifstream file(fileName);
    string word;
    int count=0;

    vector<float> nums;
    while (file >> word) {
        nums.push_back(stof(word));
        std::cout << word << std::endl;
    }
    int size = nums.size();
    float arr[size];
    for (int i=0; i<size; i++) {
        arr[i] = nums[i];
    }
}

int main(int argc, char **argv) {
    // g++ -o Jones_Sara_QuickSort Jones_Sara_QuickSort.cpp 
    // ./Jones_Sara_QuickSort inputFiles/size10/size10_1.txt output.txt
     
    if (argc == 1) {
        cout << "Needs 2 additional arguments passed"
             << endl;
    }
    if (argc == 3) {
        cout << "----Command Line Arguments Passed----\n"
             << argv[1] << " " << argv[2]
             
             << endl;
        
        runFiles(argv[1], argv[2]);
        
    }
    if (argc > 3) {
        cout << "Exceeded Required Number of Arguments, Try Again" << endl;
    }
 
    return 0;
}
