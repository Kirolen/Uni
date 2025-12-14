#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function for random setting of the matrix and vector elements
void RandomDataInitialization(double *pMatrix, double *pVector, int Size)
{
    int i, j; // Loop variables
    srand(unsigned(clock()));
    for (i = 0; i < Size; i++)
    {
        pVector[i] = rand() / double(1000000);
        for (j = 0; j < Size; j++)
            pMatrix[i * Size + j] = rand() / double(1000000);
    }
}

// Function for memory allocation and data initialization
void ProcessInitialization(double *&pMatrix, double *&pVector,
                           double *&pResult, int &Size, bool isTest = false)
{
    // Setting the size of the initial matrix and vector
    if (!isTest)
    {
        do
        {
            printf("\nEnter size of the initial objects: ");
            scanf("%d", &Size);
            printf("\nChosen objects size = %d\n", Size);
            if (Size <= 0)
                printf("\nSize of objects must be greater than 0!\n");
        } while (Size <= 0);
    }

    // Memory allocation
    pMatrix = new double[Size * Size];
    pVector = new double[Size];
    pResult = new double[Size];

    // Setting the values of the matrix and vector elements
    RandomDataInitialization(pMatrix, pVector, Size);
}

void PrintMatrix(double *pMatrix, int RowCount, int ColCount)
{
    int i, j; // Loop variables
    for (i = 0; i < RowCount; i++)
    {
        for (j = 0; j < ColCount; j++)
            printf("%7.4f ", pMatrix[i * ColCount + j]);
        printf("\n");
    }
}

// Function for formatted vector output
void PrintVector(double *pVector, int Size)
{
    int i;
    for (i = 0; i < Size; i++)
        printf("%7.4f ", pVector[i]);
    printf("\n");
}

// Function for computational process termination
void ProcessTermination(double *pMatrix, double *pVector, double *pResult)
{
    delete[] pMatrix;
    delete[] pVector;
    delete[] pResult;
}

// Function for matrix-vector multiplication
void ResultCalculation(double *pMatrix, double *pVector, double *pResult,
                       int Size)
{
    int i, j; // Loop variables
    for (i = 0; i < Size; i++)
    {
        pResult[i] = 0;
        for (j = 0; j < Size; j++)
            pResult[i] += pMatrix[i * Size + j] * pVector[j];
    }
}

void RunExperiment(double *&pMatrix, double *&pVector, double *&pResult, int Size, bool isTest)
{
    clock_t start, finish;
    double duration;

    // Memory allocation and data initialization
    ProcessInitialization(pMatrix, pVector, pResult, Size, isTest);

    // Matrix-vector multiplication
    start = clock();
    ResultCalculation(pMatrix, pVector, pResult, Size);
    finish = clock();
    duration = (finish - start) / double(CLOCKS_PER_SEC);

    // Printing the time spent
    printf("Time of execution: %f seconds\n", duration);

    // Computational process termination
    ProcessTermination(pMatrix, pVector, pResult);
}

int main()
{
    double *pMatrix; // First argument - initial matrix
    double *pVector; // Second argument - initial vector
    double *pResult; // Result vector for matrix-vector multiplication
    int Size;        // Sizes of initial matrix and vector
    int isTest = 0;

    printf("Serial matrix-vector multiplication program\n");

    printf("Select mode: 1 - Test, 0 - Normal: ");
    scanf("%d", &isTest);

    if (isTest != 1)
    {
        printf("Invalid input. Setting mode to 0 (Normal).\n");
        isTest = 0;
    }

    if (isTest)
    {
        int testSizes[] = {10, 100, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
        int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
        // Run tests for predefined sizes
        for (int t = 0; t < numTests; t++)
        {
            Size = testSizes[t];
            printf("\nTest %d: Matrix size = %d\n", t + 1, Size);
            RunExperiment(pMatrix, pVector, pResult, Size, isTest);
        }
    }
    else
        RunExperiment(pMatrix, pVector, pResult, Size, isTest);

    printf("\nPress Enter to exit...");
    getchar();
    return 0;
}
