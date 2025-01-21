//Зчитати дані з бінарного файлу та знайти середнє арифмитичне усіх чисел
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    string Path = "C:\\Users\\Admin\\Desktop\\Programming\\Labs\\Lab #6\\V4\\binNumber.bin";
    ifstream file(Path, ios_base::binary);

    double sum = 0;
    double count = -1;

    if (file.is_open())
    {
        int num, temp = 0;
        while (file >> num)
        {
                sum += num * temp;
                count++;
                temp = num;
        }
    }
    file.close();


    cout << sum << " " << count << endl;
    double avarage = sum / count;
    cout << "Avarage: " << avarage;
}
