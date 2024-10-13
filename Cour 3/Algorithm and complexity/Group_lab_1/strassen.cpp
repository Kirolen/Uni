#include <iostream>
#include <cmath>

using namespace std;

//Функція ініціалізує квадратну матрицю розміру size на динамічній пам'яті.
double** matrix_initialize(int size) {
    double** matrix = new double* [size];
    for (int i = 0; i < size; ++i)
        matrix[i] = new double[size];
    return matrix;
}

//Функція видаляє матрицю з пам'яті.
void del_matrix(double** matrix, int size) {
    for (int i = 0; i < size; ++i)
        delete[] matrix[i];
    delete[] matrix;
}

//Функція для обчислення суми двох матриць.
double** sum(double** matrix_A, double** matrix_B, int size) {
    double** sum_result = matrix_initialize(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            sum_result[i][j] = matrix_A[i][j] + matrix_B[i][j];
    }
    return sum_result;
}

//Функція для обчислення різниці двох матриць.
double** subtraction(double** matrix_A, double** matrix_B, int size) {
    double** sub_result = matrix_initialize(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            sub_result[i][j] = matrix_A[i][j] - matrix_B[i][j];
    }
    return sub_result;
}

//Функція для виведення матриці на екран.
void print(double** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) 
            cout << matrix[i][j] << ' ';
        cout << endl;
    }
}

//Функція розширює матрицю до найближчого розміру, що є степенем двійки.
double** expand_matrix(double** matrix, int size) {
    int k = pow(2, ceil(log2(size)));
    double** expended_matrix = matrix_initialize(k);
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) 
            (i < size && j < size) ? expended_matrix[i][j] = matrix[i][j] : expended_matrix[i][j] = 0;
    }
    del_matrix(matrix, size);
    return expended_matrix;
}

//Функція зменшує матрицю до її початкового розміру після обчислень.
double** narrow(double** matrix, int size) {
    double** B = matrix_initialize(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            B[i][j] = matrix[i][j];
    }
    del_matrix(matrix, pow(2, ceil(log2(size))));
    return B;
}

//Функція реалізує алгоритм Штрассена для множення двох квадратних матриць.
double** Strassen(double** matrix_A, double** matrix_B, int size) {
    if (size == 1) {
        double** strassen_result = matrix_initialize(1);
        strassen_result[0][0] = matrix_A[0][0] * matrix_B[0][0];  
        return strassen_result;
    }

    int k = size / 2;
    //Створення підматриць
    double** A11 = matrix_initialize(k);
    double** A12 = matrix_initialize(k);
    double** A21 = matrix_initialize(k);
    double** A22 = matrix_initialize(k);
    double** B11 = matrix_initialize(k);
    double** B12 = matrix_initialize(k);
    double** B21 = matrix_initialize(k);
    double** B22 = matrix_initialize(k);

    //Заповнення підматриць значеннями з початкових матриць
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            A11[i][j] = matrix_A[i][j];
            B11[i][j] = matrix_B[i][j];
            A12[i][j] = matrix_A[i][k + j];
            B12[i][j] = matrix_B[i][k + j];
            A21[i][j] = matrix_A[k + i][j];
            B21[i][j] = matrix_B[k + i][j];
            A22[i][j] = matrix_A[k + i][k + j];
            B22[i][j] = matrix_B[k + i][k + j];
        }
    }

    //Рекурсивні виклики для обчислення частин матриць
    double** M1 = Strassen(sum(A11, A22, k), sum(B11, B22, k), k);
    double** M2 = Strassen(sum(A21, A22, k), B11, k);
    double** M3 = Strassen(A11, subtraction(B12, B22, k), k);
    double** M4 = Strassen(A22, subtraction(B21, B11, k), k);
    double** M5 = Strassen(sum(A11, A12, k), B22, k);
    double** M6 = Strassen(subtraction(A21, A11, k), sum(B11, B12, k), k);
    double** M7 = Strassen(subtraction(A12, A22, k), sum(B21, B22, k), k);

    // Обчислення елементів підматриць результату
    double** C11 = sum(subtraction(sum(M1, M4, k), M5, k), M7, k);
    double** C12 = sum(M3, M5, k);
    double** C21 = sum(M2, M4, k);
    double** C22 = sum(subtraction(sum(M1, M3, k), M2, k), M6, k);

    // Об'єднання підматриць у одну матрицю
    double** C = matrix_initialize(size);
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            C[i][j] = C11[i][j];
            C[i][k + j] = C12[i][j];
            C[k + i][j] = C21[i][j];
            C[k + i][k + j] = C22[i][j];
        }
    }

    //Видалення проміжних матриць з пам'яті
    del_matrix(A11, k); del_matrix(A12, k); del_matrix(A21, k); del_matrix(A22, k);
    del_matrix(B11, k); del_matrix(B12, k); del_matrix(B21, k); del_matrix(B22, k);
    del_matrix(M1, k); del_matrix(M2, k); del_matrix(M3, k); del_matrix(M4, k);
    del_matrix(M5, k); del_matrix(M6, k); del_matrix(M7, k);
    del_matrix(C11, k); del_matrix(C12, k); del_matrix(C21, k); del_matrix(C22, k);

    return C;
}

//Основна функція програми для тестування алгоритму.
int main() {
    int n = 4; 
    double** A = matrix_initialize(n);
    double** B = matrix_initialize(n);

    A[0][0] = 1.1; A[0][1] = 2.2; A[0][2] = 3.3; A[0][3] = 4.4;
    A[1][0] = 5.5; A[1][1] = 6.6; A[1][2] = 7.7; A[1][3] = 8.8;
    A[2][0] = 9.9; A[2][1] = 10.10; A[2][2] = 11.11; A[2][3] = 12.12;
    A[3][0] = 13.13; A[3][1] = 14.14; A[3][2] = 15.15; A[3][3] = 16.16;

    B[0][0] = 17.17; B[0][1] = 18.18; B[0][2] = 19.19; B[0][3] = 20.20;
    B[1][0] = 21.21; B[1][1] = 22.22; B[1][2] = 23.23; B[1][3] = 24.24;
    B[2][0] = 25.25; B[2][1] = 26.26; B[2][2] = 27.27; B[2][3] = 28.28;
    B[3][0] = 29.29; B[3][1] = 30.30; B[3][2] = 31.31; B[3][3] = 32.32;

    double** C = narrow(Strassen(expand_matrix(A, n), expand_matrix(B, n), pow(2, ceil(log2(n)))), n);

    print(C, n);

    del_matrix(C, n);
    return 0;
}
