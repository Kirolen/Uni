//Наближено порахувати значення функції методом Хорд
#include <iostream>
#include <math.h>

using namespace std;

double function(double x)
{
    return x*x - 4;
}
double hordMetod(double(*function)(double), double a, double b, double epsilon);

int main() {
    double a, b, epsilon;
    cout << "Enter start diapasone [a, b]: ";
    cin >> a >> b;

    cout << "Enter epsilon: ";
    cin >> epsilon;

    double result = hordMetod(function, a, b, epsilon);
    if (!isnan(result)) {
        cout << "The root is equal: " << result << endl;
    }

    return 0;
}

double hordMetod(double(*function)(double), double a, double b, double epsilon)
{
    double x;

    if (function(a) * function(b) > 0) {
        cout << "The sigm between is similar" << endl;
        return NAN;
    }

    do
    {
        x = a;
        a = x - (function(x) * (x - b)) / (function(x) - function(b));
    }
    while (abs(a - x) > epsilon);

    return a;
}
