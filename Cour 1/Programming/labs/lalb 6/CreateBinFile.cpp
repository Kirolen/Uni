#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
    srand(time(0));
    int size, range;
    cout << "Enter the number of digits: ";
    cin >> size;
    cout << "Enter the upper limit: ";
    cin >> range;

    int* numbers = new int[size];
    for (int i = 0; i < size; i++)
    {
        numbers[i] = rand() % range;
    }

    string filePathNum = "C:\\Users\\Admin\\Desktop\\Programming\\Labs\\Lab #6\\V4\\number.txt";
    string filePathBinNum = "C:\\Users\\Admin\\Desktop\\Programming\\Labs\\Lab #6\\V4\\binNumber.bin";
    ofstream fileNum(filePathNum);
    ofstream fileBinNum(filePathBinNum, ios_base::binary);
    if (fileNum.is_open() && fileBinNum.is_open())
    {
        for (int i = 0; i < size; i++)
        {
            fileNum << numbers[i] << endl;
            fileBinNum << numbers[i] << endl;
        }
        cout << "Numbers were recorded";
    }

    fileNum.close();
    fileBinNum.close();
    delete[] numbers;
}
