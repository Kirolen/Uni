//Дописати до рядка найменшу кількість символів, щоб він став паліндромом
#include <iostream>
#include <cctype>

using namespace std;

string doPalindrom(string str, int* SIZE);

int main()
{
    string str;
    string str2;
    cout << "Enter a string: ";
    cin >> str;
    int SIZE = str.length();
    int SIZE2 = SIZE;
    for (int i = 0; i < SIZE; i++)
    {
        str2 += str[SIZE - 1 - i];
    }
    str = doPalindrom(str, &SIZE);
    str2 = doPalindrom(str2, &SIZE2);

    cout << "New string: ";
    if (SIZE < SIZE2) cout << str << endl;
    else cout << str2 << endl;

    return 0;
}

string doPalindrom(string str, int* SIZE)
{
    for (int i = 0; i < *SIZE; i++)
    {
        //Перебор кожного елементу
        if (str[i] != str[*SIZE - 1 - i])
        {
            str += str[i];
            *SIZE = str.length();
            //Міняю елементи так, щоб рядок став паліндромом
            for (int j = 0; j < i; j++)
            {
                char temp = str[*SIZE - 1 - j];
                str[*SIZE - 1 - j] = str[*SIZE - 2 - j];
                str[*SIZE - 2 - j] = temp;
            }
            i = 0;
        }
    }
    return str;
}
