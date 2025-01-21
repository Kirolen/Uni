//Умова: Перевірити чи є число числом Кармайкла
#include <iostream>
using namespace std;


bool CarmichaelNumber(int n);
bool isPrimeNumber(int a);
int gcd(int a, int b);
int power(int x, int y, int mod);

int main()
{
    int num;
    cin >> num;

    if (CarmichaelNumber(num))
    {
        cout << num << " is Carmichael number." << endl;
    }
    else
    {
        cout << num << " isn`t Carnichael number." << endl;
    }
}


bool CarmichaelNumber(int n)
{
    if (isPrimeNumber(n) || n <= 2)
    {
        return 0;
    }
    for (int i = 3; i < n; i++)
    {
        if (gcd(i, n) == 1)
        {
            if (power(i, n - 1, n) != 1)
            {
                return 0;
            }

        }
    }
    return 1;
}

bool isPrimeNumber(int a) //Функція для перевірки числа на простоту
{
    if (a % 2 == 0)
    {
        return 0;
    }
    for (int div = 3; div <= sqrt(a); div += 2)
    {
        if (a % div == 0)
        {
            return 0;
        }
    }
    return 1;
}
int gcd(int a, int b)     //Функція для знаходження НСД
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
int power(int x, int y, int mod)
{
    int result = 1;
    while (y > 0) //Алгоритм виконується  доки у (степінь) > 0
    {
        if (y % 2 == 1) //Якщо степінь не парний, то виконується алгоритм нижче
        {
            result = (result * x) % mod;
        }
        x = (x * x) % mod;
        /*Функція підносить х до квадрату
        і ділить його за модулем, що допомагає
        зменшити степінь вдвічі. При цьому фунція
        використовує властивість модульного піднесення до степення*/
        y /= 2; //Зменшує у вдвічі
    }
    /*Ця функція використувує алгоритм бінарного піднесення до степення
    та властовисті модульного піднесення до степення */
    return result;
}


