#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

int ProcNum = 0;   // Number of available processes
int ProcRank = 0;  // Rank of current process
int GridSize;      // Size of virtual processor grid
int GridCoords[2]; // Coordinates of current processor in grid
MPI_Comm GridComm; // Grid communicator
MPI_Comm ColComm;  // Column communicator
MPI_Comm RowComm;  // Row communicator

// Function for random initialization of matrix elements
void RandomDataInitialization(double *pAMatrix, double *pBMatrix, int Size)
{
    int i, j; // Loop variables

    srand(unsigned(clock()));

    for (i = 0; i < Size; i++)
        for (j = 0; j < Size; j++)
        {
            pAMatrix[i * Size + j] = rand() % (100);
            pBMatrix[i * Size + j] = rand() % (100);
        }
}

// Function for matrix multiplication
void SerialResultCalculation(double *pAMatrix, double *pBMatrix,
                             double *pCMatrix, int Size)
{
    int i, j, k; // Loop variables
    // Initialize result matrix to zero
    for (i = 0; i < Size * Size; i++)
    {
        pCMatrix[i] = 0;
    }

    for (i = 0; i < Size; i++)
    {
        for (j = 0; j < Size; j++)
            for (k = 0; k < Size; k++)
                pCMatrix[i * Size + j] += pAMatrix[i * Size + k] * pBMatrix[k * Size + j];
    }
}

// Function for block multiplication
void BlockMultiplication(double *pAblock, double *pBblock,
                         double *pCblock, int Size)
{
    int i, j, k;
    // Accumulate results - don't reinitialize pCblock
    for (i = 0; i < Size; i++)
    {
        for (j = 0; j < Size; j++)
            for (k = 0; k < Size; k++)
                pCblock[i * Size + j] += pAblock[i * Size + k] * pBblock[k * Size + j];
    }
}

// Function for creating the two-dimensional grid communicator
// and communicators for each row and each column of the grid
void CreateGridCommunicators()
{
    int DimSize[2];  // Number of processes in each dimension of the grid
    int Periodic[2]; // =1, if the grid dimension should be periodic
    int Subdims[2];  // =1, if the grid dimension should be fixed

    DimSize[0] = GridSize;
    DimSize[1] = GridSize;
    Periodic[0] = 0;
    Periodic[1] = 0;

    // Creation of the Cartesian communicator
    MPI_Cart_create(MPI_COMM_WORLD, 2, DimSize, Periodic, 1, &GridComm);

    // Determination of the cartesian coordinates for every process
    MPI_Cart_coords(GridComm, ProcRank, 2, GridCoords);

    // Creating communicators for rows
    Subdims[0] = 0; // Dimensionality fixing
    Subdims[1] = 1; // The presence of the given dimension in the subgrid

    MPI_Cart_sub(GridComm, Subdims, &RowComm);

    // Creating communicators for columns
    Subdims[0] = 1;
    Subdims[1] = 0;

    MPI_Cart_sub(GridComm, Subdims, &ColComm);
}

// Function for memory allocation and data initialization
void ProcessInitialization(double *&pAMatrix, double *&pBMatrix,
                           double *&pCMatrix, double *&pAblock, double *&pBblock, double *&pCblock,
                           double *&pMatrixAblock, int &Size, int &BlockSize, bool isTest = false)
{
    if (ProcRank == 0)
    {
        if (!isTest)
        {
            do
            {
                printf("\nEnter the size of matrices: ");
                scanf("%d", &Size);

                if (Size % GridSize != 0)
                {
                    printf("Size of matrices must be divisible by the grid size!\n");
                }
            } while (Size % GridSize != 0);
        }
    }

    MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    BlockSize = Size / GridSize;
    pAblock = new double[BlockSize * BlockSize];
    pBblock = new double[BlockSize * BlockSize];
    pCblock = new double[BlockSize * BlockSize];
    pMatrixAblock = new double[BlockSize * BlockSize];

    // Initialize all block arrays
    for (int i = 0; i < BlockSize * BlockSize; i++)
    {
        pAblock[i] = 0;
        pBblock[i] = 0;
        pCblock[i] = 0;
        pMatrixAblock[i] = 0;
    }

    if (ProcRank == 0)
    {
        pAMatrix = new double[Size * Size];
        pBMatrix = new double[Size * Size];
        pCMatrix = new double[Size * Size];

        // Initialize matrices
        RandomDataInitialization(pAMatrix, pBMatrix, Size);

        // Initialize result matrix
        for (int i = 0; i < Size * Size; i++)
        {
            pCMatrix[i] = 0;
        }
    }
}

// Function for checkerboard matrix decomposition
void CheckerboardMatrixScatter(double *pMatrix, double *pMatrixBlock,
                               int Size, int BlockSize)
{
    double *MatrixRow = new double[BlockSize * Size];

    // First, scatter block-rows to the first column of processes
    if (GridCoords[1] == 0)
    {
        MPI_Scatter(pMatrix, BlockSize * Size, MPI_DOUBLE, MatrixRow,
                    BlockSize * Size, MPI_DOUBLE, 0, ColComm);
    }

    // Broadcast each block-row to all processes in that row
    MPI_Bcast(MatrixRow, BlockSize * Size, MPI_DOUBLE, 0, RowComm);

    // Extract the block for this process
    for (int i = 0; i < BlockSize; i++)
    {
        for (int j = 0; j < BlockSize; j++)
        {
            pMatrixBlock[i * BlockSize + j] =
                MatrixRow[i * Size + GridCoords[1] * BlockSize + j];
        }
    }

    delete[] MatrixRow;
}

// Data distribution among the processes
void DataDistribution(double *pAMatrix, double *pBMatrix, double *pMatrixAblock, double *pBblock, int Size, int BlockSize)
{
    // Scatter the matrix among the processes
    CheckerboardMatrixScatter(pAMatrix, pMatrixAblock, Size, BlockSize);
    CheckerboardMatrixScatter(pBMatrix, pBblock, Size, BlockSize);
}

// Function for gathering the result matrix
void ResultCollection(double *pCMatrix, double *pCblock, int Size, int BlockSize)
{
    double *pResultRow = new double[Size * BlockSize];

    // Gather blocks from each process in the row to process with GridCoords[1]==0
    for (int i = 0; i < BlockSize; i++)
    {
        MPI_Gather(&pCblock[i * BlockSize], BlockSize, MPI_DOUBLE,
                   &pResultRow[i * Size], BlockSize, MPI_DOUBLE, 0, RowComm);
    }

    // Gather all rows to process 0
    if (GridCoords[1] == 0)
    {
        MPI_Gather(pResultRow, BlockSize * Size, MPI_DOUBLE, pCMatrix,
                   BlockSize * Size, MPI_DOUBLE, 0, ColComm);
    }

    delete[] pResultRow;
}

// Broadcasting blocks of the matrix A to process grid rows
void ABlockCommunication(int iter, double *pAblock, double *pMatrixAblock,
                         int BlockSize)
{
    // Defining the leading process of the process grid row
    int Pivot = (GridCoords[0] + iter) % GridSize;

    // Copying the transmitted block in a separate memory buffer
    if (GridCoords[1] == Pivot)
    {
        for (int i = 0; i < BlockSize * BlockSize; i++)
            pAblock[i] = pMatrixAblock[i];
    }

    // Block broadcasting
    MPI_Bcast(pAblock, BlockSize * BlockSize, MPI_DOUBLE, Pivot, RowComm);
}

// Function for cyclic shifting the blocks of the matrix B
void BblockCommunication(double *pBblock, int BlockSize)
{
    MPI_Status Status;
    int NextProc = GridCoords[0] - 1; // FIXED: Send upward
    if (GridCoords[0] == 0)
        NextProc = GridSize - 1;

    int PrevProc = GridCoords[0] + 1; // FIXED: Receive from below
    if (GridCoords[0] == GridSize - 1)
        PrevProc = 0;

    MPI_Sendrecv_replace(pBblock, BlockSize * BlockSize, MPI_DOUBLE,
                         NextProc, 0, PrevProc, 0, ColComm, &Status);
}

// Function for parallel execution of the Fox method
void ParallelResultCalculation(double *pAblock, double *pMatrixAblock,
                               double *pBblock, double *pCblock, int BlockSize)
{
    for (int iter = 0; iter < GridSize; iter++)
    {
        // Sending blocks of matrix A to the process grid rows
        ABlockCommunication(iter, pAblock, pMatrixAblock, BlockSize);

        // Block multiplication
        BlockMultiplication(pAblock, pBblock, pCblock, BlockSize);

        // Cyclic shift of blocks of matrix B in process grid columns
        BblockCommunication(pBblock, BlockSize);
    }
}

// Function for testing the matrix multiplication result
void TestResult(double *pAMatrix, double *pBMatrix, double *pCMatrix,
                int Size)
{
    double *pSerialResult;   // Result matrix of serial multiplication
    double Accuracy = 1.e-6; // Comparison accuracy
    int equal = 0;           // =1, if the matrices are not equal
    int i;                   // Loop variable

    if (ProcRank == 0)
    {
        pSerialResult = new double[Size * Size];

        SerialResultCalculation(pAMatrix, pBMatrix, pSerialResult, Size);

        printf("\n=== Comparison ===\n");
        for (i = 0; i < Size * Size; i++)
        {
            if (fabs(pSerialResult[i] - pCMatrix[i]) >= Accuracy)
            {
                printf("Mismatch at index %d: Serial = %f, Parallel = %f\n",
                       i, pSerialResult[i], pCMatrix[i]);
                equal = 1;
            }
        }

        if (equal == 1)
            printf("\nThe results of serial and parallel algorithms are NOT identical.\n");
        else
            printf("\nThe results of serial and parallel algorithms are identical.\n");

        delete[] pSerialResult;
    }
}

// Function for computational process termination
void ProcessTermination(double *pAMatrix, double *pBMatrix,
                        double *pCMatrix, double *pAblock, double *pBblock, double *pCblock,
                        double *pMatrixAblock)
{
    if (ProcRank == 0)
    {
        delete[] pAMatrix;
        delete[] pBMatrix;
        delete[] pCMatrix;
    }

    delete[] pAblock;
    delete[] pBblock;
    delete[] pCblock;
    delete[] pMatrixAblock;
}

void RunExperimentn(double *pAMatrix, double *pBMatrix,
                    double *pCMatrix, double *pAblock, double *pBblock, double *pCblock,
                    double *pMatrixAblock, int &Size, int &BlockSize, bool isTest)
{
    double Start, Finish, Duration;

    // Creating the cartesian grid, row and column communicators
    CreateGridCommunicators();

    // Memory allocation and initialization of matrix elements
    ProcessInitialization(pAMatrix, pBMatrix, pCMatrix, pAblock, pBblock,
                          pCblock, pMatrixAblock, Size, BlockSize, isTest);

    Start = MPI_Wtime();
    DataDistribution(pAMatrix, pBMatrix, pMatrixAblock, pBblock, Size, BlockSize);

    // Execution of the Fox method
    ParallelResultCalculation(pAblock, pMatrixAblock, pBblock, pCblock, BlockSize);

    // Gathering the result matrix
    ResultCollection(pCMatrix, pCblock, Size, BlockSize);
    Finish = MPI_Wtime();
    Duration = Finish - Start;

    //TestResult(pAMatrix, pBMatrix, pCMatrix, Size);

    if (ProcRank == 0)
    {
        printf("\nTime of execution = %f seconds\n", Duration);
    }

    // Process Termination
    ProcessTermination(pAMatrix, pBMatrix, pCMatrix, pAblock, pBblock,
                       pCblock, pMatrixAblock);
}

int main(int argc, char *argv[])
{
    double *pAMatrix; // First argument of matrix multiplication
    double *pBMatrix; // Second argument of matrix multiplication
    double *pCMatrix; // Result matrix
    int Size;         // Size of matrices
    int BlockSize;    // Sizes of matrix blocks
    double *pAblock;  // Initial block of matrix A
    double *pBblock;  // Initial block of matrix B
    double *pCblock;  // Block of result matrix C
    double *pMatrixAblock;
    double Start, Finish, Duration;
    int isTest = 0;

    setvbuf(stdout, 0, _IONBF, 0);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0)
    {
        printf("Select mode: 1 - Test, 0 - Normal: ");
        scanf("%d", &isTest);

        if (isTest != 1)
        {
            printf("Invalid input. Setting mode to 0 (Normal).\n");
            isTest = 0;
        }
    }

    MPI_Bcast(&isTest, 1, MPI_INT, 0, MPI_COMM_WORLD);

    GridSize = sqrt((double)ProcNum);

    if (ProcNum != GridSize * GridSize)
    {
        if (ProcRank == 0)
        {
            printf("Number of processes must be a perfect square \n");
        }
    }
    else
    {
        if (ProcRank == 0)
            printf("Parallel matrix multiplication program\n");

        if (isTest)
        {
            int testSizes[] = {12, 102, 504, 1002, 1500, 2004, 2502, 3000};
            int numTests = sizeof(testSizes) / sizeof(testSizes[0]);

            for (int t = 0; t < numTests; t++)
            {
                Size = testSizes[t];
                if (ProcRank == 0)
                    printf("\nTest %d: Matrix size = %d\n", t + 1, Size);

                RunExperimentn(pAMatrix, pBMatrix, pCMatrix, pAblock, pBblock, pCblock, pMatrixAblock, Size, BlockSize, isTest);
            }
        }
        else
            RunExperimentn(pAMatrix, pBMatrix, pCMatrix, pAblock, pBblock, pCblock, pMatrixAblock, Size, BlockSize, isTest);
    }

    MPI_Finalize();
}