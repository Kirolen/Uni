//Умова: Порахувати біноміальний коефіцієнт та вищначити його прості дільники
#include <iostream>

using namespace std;
int binomial(int n, int k);
int primeFactors(int num);

int main() {
    int k;
    cin >> k;
    int n;
    cin >> n;

    if (k < 0 || n < k) {
        cout << "Обрахувати можлиао. k >= 0, n > k" << endl;
    }
    else {
        int binCof = binomial(n, k);
        cout << "Binomial Coeficient is " << binCof << endl;
        primeFactors(binCof);
    }
    return 0;
}

int binomial(int n, int k) {
    long long int res = 1;

    if (k > (n - k)) {
        k = n - k; //Обраховуємо кількість цифр які залишаться після n!\k!
    }

    for (int i = 0; i < k; i++)
    {
        res *= (n - i); //Множимо на останню цифру факторіалу і решту, які не поділяться
        res /= (i + 1); //Ділимо на розкдадений факторіл (n-k)! почергово
    }
    return res;
}
int primeFactors(int num) {

    int divCount = 0;
    int devider = 2;

    while (num % 2 == 0) {
        num /= 2;
        divCount++;
    }
    if (divCount != 0) {
        cout << "{" << devider << ", " << divCount << "}" << endl;
    }

    for (devider = 3; num != 1; devider +=2) {
        divCount = 0;
        while (num % devider == 0) {
            num /= devider;
            divCount++;
        }
        if (divCount != 0) {
            cout << "{" << devider << ", " << divCount << "}" << endl;
        }
    }
    return 0;
}
