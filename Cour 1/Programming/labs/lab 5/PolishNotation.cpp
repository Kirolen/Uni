/*Обернена польська нотація та обчислення математичних виразів
Варіант 1 - Операції: +,-,/,*,^, унарний -, унарний +*/

#include <iostream>
#include <stack>
#include <map>

using namespace std;

map <char, int> priority;
int power(int n, int pow)
{
    int res = 1;
    for (int i = 0; i < pow; i++)
        res*=n;
    return res;
}

double operation (double a, double b, char sign)
{
    if(sign == '+') return a+b;
    if(sign == '-') return a-b;
    if(sign == '*') return a*b;
    if(sign == '/') return a/b;
    if(sign == '^') return power(a, b);
}

double polishNote (string input)
{
    stack <double> polishCode;
    double num = 0, num1, num2;
    bool digits = false;
    for(auto ch: input)
    {
        if(isdigit(ch))
        {
            num = num * 10 + ch - '0';
            digits = true;
        }
        else
        {
            if(ch != ' ')
            {
                num2 = polishCode.top();
                polishCode.pop();
                num1 = polishCode.top();
                polishCode.pop();
                polishCode.push(operation(num1, num2, ch));
                digits = false;
            }
            else
            {
                if(ch == ' ' && digits)
                {
                    polishCode.push(num);
                    num = 0;

                }
            }
        }
    }

    return polishCode.top();
}

string convertToPolish (string str)
{
    // (17-4)*3-(18+2*(4-10))
    string input;
    stack <char> signs;
    input.clear();
    char ch;

    for(int i = 0; i < str.size(); i++)
    {
        ch = str[i];
        if(isdigit(ch))
        {
            input += ch;
        }
        else if(ch == '(')
        {
            signs.push(ch);
            //input += ' ';

        }
        else if(ch == ')')
        {
            while(signs.top() != '(' )
            {
                input += ' ';
                input += signs.top();
                signs.pop();
                if(signs.empty()) return "error";
            }
            signs.pop();

        }
        else if(ch == '+' || ch == '-' || ch == '/'  || ch == '*' || ch == '^')
        {
            if((i != 0 && str[i - 1] == '(' && (ch == '-' || ch == '+')) || (i == 0 && (ch == '-' || ch == '+'))) input+= '0';
            input += ' ';
            while(!signs.empty()&& signs.top() != '(' && priority[ch] >= priority[signs.top()])
            {
                input += signs.top();
                input += ' ';
                signs.pop();
            }
            signs.push(ch);
        }
        else if(ch != ' ') return "error";
    }

    while(!signs.empty())
    {
        input += ' ';
        input += signs.top();
        signs.pop();
    }
    return input;
}

int main()
{
    priority['+'] = 5;
    priority['-'] = 5;
    priority['/'] = 4;
    priority['*'] = 4;
    priority['^'] = 3;
    string input;
    getline(cin, input);

    input = convertToPolish(input);

    if(input != "error") cout << polishNote(input);
    else cout << "ERROR ----- Input string is not correct!!!\n";
}

/*
3/3/3

3 3 / 3 /
1 3 /
0.3333

*/
