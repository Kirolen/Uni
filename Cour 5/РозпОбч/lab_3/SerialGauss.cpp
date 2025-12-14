#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int *pSerialPivotPos;  // Number of pivot rows selected at the iterations
int *pSerialPivotIter; // Iterations, at which the rows were pivots

void RandomDataInitialization(double *pMatrix, double *pVector, int Size)
{
    srand((unsigned)time(0)); // ініціалізація генератора випадкових чисел

    for (int i = 0; i < Size; i++)
    {
        // Значення вектора — випадкові числа від 1.00 до 100.00
        pVector[i] = ((double)(rand() % 10000) / 100.0) + 1.0;

        for (int j = 0; j < Size; j++)
        {
            if (j == i)
                // Сильна діагональ (від 5000.00 до 5050.00)
                pMatrix[i * Size + j] = ((double)(rand() % 5000) / 100.0) + 5000.0;
            else
                // Інші елементи — від 1.00 до 100.00
                pMatrix[i * Size + j] = ((double)(rand() % 10000) / 100.0) + 1.0;
        }
    }
}

void ProcessInitialization(double *&pMatrix, double *&pVector,
                           double *&pResult, int &Size, bool isTest)
{
    if (!isTest)
    {
        do
        {
            printf("\nEnter the size of the matrix and the vector: ");
            scanf("%d", &Size);
            printf("\nChosen size = %d \n", Size);
            if (Size <= 0)
                printf("\nSize of objects must be greater than 0!\n");
        } while (Size <= 0);
    }

    pMatrix = new double[Size * Size];
    pVector = new double[Size];
    pResult = new double[Size];

    RandomDataInitialization(pMatrix, pVector, Size);
}

void PrintMatrix(double *pMatrix, int RowCount, int ColCount)
{
    for (int i = 0; i < RowCount; i++)
    {
        for (int j = 0; j < ColCount; j++)
            printf("%7.4f ", pMatrix[i * ColCount + j]); // corrected indexing
        printf("\n");
    }
}

void PrintVector(double *pVector, int Size)
{
    for (int i = 0; i < Size; i++)
        printf("%7.4f ", pVector[i]);
    printf("\n");
}

// Find pivot row for column Iter among unused rows
int FindPivotRow(double *pMatrix, int Size, int Iter)
{
    int PivotRow = -1;
    double MaxValue = -1.0; // must be double
    for (int i = 0; i < Size; i++)
    {
        if ((pSerialPivotIter[i] == -1) &&
            (fabs(pMatrix[i * Size + Iter]) > MaxValue))
        {
            PivotRow = i;
            MaxValue = fabs(pMatrix[i * Size + Iter]);
        }
    }
    return PivotRow;
}

void SerialColumnElimination(double *pMatrix, double *pVector, int Pivot,
                             int Iter, int Size)
{
    double PivotValue = pMatrix[Pivot * Size + Iter];
    if (fabs(PivotValue) < 1e-12) {
        // pivot too small -> numeric trouble
        fprintf(stderr, "Warning: small pivot (col %d, row %d) = %e\n", Iter, Pivot, PivotValue);
        return;
    }
    for (int i = 0; i < Size; i++)
    {
        if (pSerialPivotIter[i] == -1)
        {
            double PivotFactor = pMatrix[i * Size + Iter] / PivotValue;
            for (int j = Iter; j < Size; j++)
            {
                pMatrix[i * Size + j] -= PivotFactor * pMatrix[Pivot * Size + j];
            }
            pVector[i] -= PivotFactor * pVector[Pivot];
        }
    }
}

void SerialGaussianElimination(double *pMatrix, double *pVector, int Size)
{
    for (int i = 0; i < Size; i++) {
        pSerialPivotIter[i] = -1; // ensure init (defensive)
    }
    for (int Iter = 0; Iter < Size; Iter++)
    {
        int PivotRow = FindPivotRow(pMatrix, Size, Iter);
        if (PivotRow == -1) {
            fprintf(stderr, "Error: no pivot found for column %d (matrix may be singular)\n", Iter);
            exit(EXIT_FAILURE);
        }
        pSerialPivotPos[Iter] = PivotRow;
        pSerialPivotIter[PivotRow] = Iter;
        SerialColumnElimination(pMatrix, pVector, PivotRow, Iter, Size);
    }
}

void SerialBackSubstitution(double *pMatrix, double *pVector,
                            double *pResult, int Size)
{
    for (int i = Size - 1; i >= 0; i--)
    {
        int RowIndex = pSerialPivotPos[i];
        double diag = pMatrix[RowIndex * Size + i];
        if (fabs(diag) < 1e-12) {
            fprintf(stderr, "Error: zero (or tiny) diagonal element at (row %d, col %d)\n", RowIndex, i);
            exit(EXIT_FAILURE);
        }
        pResult[i] = pVector[RowIndex] / diag;
        for (int j = 0; j < i; j++)
        {
            int Row = pSerialPivotPos[j];
            pVector[Row] -= pMatrix[Row * Size + i] * pResult[i];
            pMatrix[Row * Size + i] = 0.0;
        }
    }
}

void SerialResultCalculation(double *pMatrix, double *pVector,
                             double *pResult, int Size)
{
    pSerialPivotPos = new int[Size];
    pSerialPivotIter = new int[Size];
    for (int i = 0; i < Size; i++)
        pSerialPivotIter[i] = -1;

    SerialGaussianElimination(pMatrix, pVector, Size);
    SerialBackSubstitution(pMatrix, pVector, pResult, Size);

    delete[] pSerialPivotPos;
    delete[] pSerialPivotIter;
}

void ProcessTermination(double *pMatrix, double *pVector, double *pResult)
{
    delete[] pMatrix;
    delete[] pVector;
    delete[] pResult;
}

void RunExperiment(double *&pMatrix, double *&pVector, double *&pResult, int Size, bool isTest)
{
    clock_t start, finish;
    double duration;

    ProcessInitialization(pMatrix, pVector, pResult, Size, isTest);
    // PrintMatrix(pMatrix, Size, Size);
    // printf("\n Vextor:\n");
    // PrintVector(pVector, Size);

    start = clock();
    SerialResultCalculation(pMatrix, pVector, pResult, Size);
    finish = clock();
    duration = (finish - start) / double(CLOCKS_PER_SEC);
    //  printf("\n Result:\n");
    // PrintVector(pResult, Size);

    printf("\n Time of execution: %f\n", duration);
    // Computational process termination
    ProcessTermination(pMatrix, pVector, pResult);
}

int main()
{

    double *pMatrix; // Matrix of the linear system
    double *pVector; // Right parts of the linear system
    double *pResult; // Result vector
    int Size;        // Sizes of the initial matrix and the vector
    int isTest;
    int testSizes[] = {10, 100, 500, 1000, 1500, 2000, 2500, 3000};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    printf("Serial Gauss algorithm for solving linear systems\n");
    // Memory allocation and definition of objects' elements

    printf("Select mode: 1 - Test, 0 - Normal: ");
    scanf("%d", &isTest);

    if (isTest != 1)
    {
        printf("Invalid input. Setting mode to 0 (Normal).\n");
        isTest = 0;
    }

    if (isTest)
    {
        for (int t = 0; t < numTests; t++)
        {
            Size = testSizes[t];
            printf("\nCalculating for matrix size = %d\n", Size);
            RunExperiment(pMatrix, pVector, pResult, Size, isTest);
        }
    }
    else
        RunExperiment(pMatrix, pVector, pResult, Size, isTest);

    printf("\nPress Enter to exit...");
    getchar();
    return 0;
}
