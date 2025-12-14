#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function for random initialization of matrix elements
void RandomDataInitialization(double *pAMatrix, double *pBMatrix, int Size)
{
    int i, j; // Loop variables
    srand(unsigned(clock()));
    for (i = 0; i < Size; i++)
        for (j = 0; j < Size; j++)
        {
            pAMatrix[i * Size + j] = rand() / double(1000);
            pBMatrix[i * Size + j] = rand() / double(1000);
        }
}

// Function for memory allocation and initialization of matrix elements
void ProcessInitialization(double *&pAMatrix, double *&pBMatrix, double *&pCMatrix, int &Size, bool isTest = false)
{
    if (!isTest)
    {
        do
        {
            printf("\nEnter size of the initial matrices: ");
            scanf("%d", &Size);
            printf("\nChosen matrices size = %d\n", Size);
            if (Size <= 0)
                printf("\nSize of objects must be greater than 0!\n");
        } while (Size <= 0);
    }

    // Memory allocation
    pAMatrix = new double[Size * Size];
    pBMatrix = new double[Size * Size];
    pCMatrix = new double[Size * Size];

    // Initialization of matrix elements
    RandomDataInitialization(pAMatrix, pBMatrix, Size);

    for (int i = 0; i < Size * Size; i++)
    {
        pCMatrix[i] = 0;
    }
}

void SerialResultCalculation(double *pAMatrix, double *pBMatrix,
                             double *pCMatrix, int Size)
{
    int i, j, k; // Loop variables
    for (i = 0; i < Size; i++)
    {
        for (j = 0; j < Size; j++)
        {
            for (k = 0; k < Size; k++)
            {
                pCMatrix[i * Size + j] += pAMatrix[i * Size + k] * pBMatrix[k * Size + j];
            }
        }
    }
}

// Function for computational process termination
void ProcessTermination(double *pAMatrix, double *pBMatrix,
                        double *pCMatrix)
{
    delete[] pAMatrix;
    delete[] pBMatrix;
    delete[] pCMatrix;
}

void RunExperiment(double *&pAMatrix, double *&pBMatrix, double *&pCMatrix, int Size, bool isTest)
{
    clock_t start, finish;
    double duration;

    // Memory allocation and data initialization
    ProcessInitialization(pAMatrix, pBMatrix, pCMatrix, Size, isTest);

    // Matrix-vector multiplication
    start = clock();
    SerialResultCalculation(pAMatrix, pBMatrix, pCMatrix, Size);
    finish = clock();
    duration = (finish - start) / double(CLOCKS_PER_SEC);

    // Printing the time spent
    printf("Time of execution: %f seconds\n", duration);

    // Computational process termination
    ProcessTermination(pAMatrix, pBMatrix, pCMatrix);
}

int main()
{
    double *pAMatrix; // First argument of matrix multiplication
    double *pBMatrix; // Second argument of matrix multiplication
    double *pCMatrix; // Result matrix
    int Size;         // Size of matrices
    int testSizes[] = {12, 102, 504, 1002, 1500, 2004, 2502, 3000};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    int isTest = 0;

    printf("Serial matrix multiplication program\n");

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
            RunExperiment(pAMatrix, pBMatrix, pCMatrix, Size, isTest);
        }
    }
    else 
        RunExperiment(pAMatrix, pBMatrix, pCMatrix, Size, isTest);

    printf("\nPress Enter to exit...");
    getchar();
    return 0;
}