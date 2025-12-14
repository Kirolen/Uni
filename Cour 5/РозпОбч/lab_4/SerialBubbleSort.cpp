#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <algorithm> // для std::sort
using namespace std;

const double RandomDataMultiplier = 1000.0;

// Прототипи функцій
void ProcessInitialization(double *&pData, int& DataSize);
void ProcessTermination(double *pData);
void DummyDataInitialization(double*& pData, int& DataSize);
void RandomDataInitialization(double *&pData, int& DataSize);
void SerialBubble(double *pData, int DataSize);
void PrintData(double *pData, int DataSize);
void SerialStdSort(double *pData, int DataSize);

int main(int argc, char *argv[]) {
    double *pData = 0;
    int DataSize = 0;
    time_t start, finish;
    double duration = 0.0;
    int isTest = 0;

    printf("Serial bubble sort program\n");
    printf("Enter 1 for automatic testing, 0 for manual input: ");
    scanf("%d", &isTest);

    if(isTest == 1) {
        int testSizes[] = {10, 100, 10000, 20000, 30000, 40000, 50000};
        int numTests = sizeof(testSizes) / sizeof(testSizes[0]);

        for(int t = 0; t < numTests; t++) {
            DataSize = testSizes[t];
            printf("\n--- Testing with %d elements ---\n", DataSize);

            // Ініціалізація даних
            ProcessInitialization(pData, DataSize);
            
            // Сортування SerialBubble
            double *pBubble = new double[DataSize];
            memcpy(pBubble, pData, DataSize * sizeof(double));
            start = clock();
            SerialBubble(pBubble, DataSize);
            finish = clock();
            duration = (finish - start) / double(CLOCKS_PER_SEC);
            printf("SerialBubble sort time: %f seconds\n", duration);
            delete[] pBubble;

            // Сортування std::sort
            double *pStd = new double[DataSize];
            memcpy(pStd, pData, DataSize * sizeof(double));
            start = clock();
            SerialStdSort(pStd, DataSize);
            finish = clock();
            duration = (finish - start) / double(CLOCKS_PER_SEC);
            printf("std::sort time: %f seconds\n", duration);
            delete[] pStd;

            ProcessTermination(pData);
        }
    } else {
        // Режим ручного вводу
        ProcessInitialization(pData, DataSize);
        printf("Data before sorting\n");
        PrintData(pData, DataSize);

        start = clock();
        SerialBubble(pData, DataSize);
        finish = clock();

        printf("Data after sorting\n");
        PrintData(pData, DataSize);

        duration = (finish - start) / double(CLOCKS_PER_SEC);
        printf("Time of execution: %f\n", duration);

        ProcessTermination(pData);
    }

    return 0;
}

// Function for allocating the memory and setting the initial values
void ProcessInitialization(double *&pData, int& DataSize) {
    if (DataSize <= 0) {
        do {
            printf("Enter the size of data to be sorted: ");
            scanf("%d", &DataSize);
            if(DataSize <= 0)
                printf("Data size should be greater than zero\n");
        } while(DataSize <= 0);
    }
    printf("Sorting %d data items\n", DataSize);
    pData = new double[DataSize];
    // Simple setting the data
    //DummyDataInitialization(pData, DataSize);
    // Setting the data by the random generator
    RandomDataInitialization(pData, DataSize);
}

// Function for computational process termination
void ProcessTermination(double *pData) {
    delete []pData;
}

// Function for simple setting the initial data
void DummyDataInitialization(double*& pData, int& DataSize) {
    for(int i = 0; i < DataSize; i++)
        pData[i] = DataSize - i;
}

// Function for initializing the data by the random generator
void RandomDataInitialization(double *&pData, int& DataSize) {
    srand( (unsigned)time(0) );
    for(int i = 0; i < DataSize; i++)
        pData[i] = double(rand()) / RAND_MAX * RandomDataMultiplier;
}

// Function for the serial bubble sort algorithm
void SerialBubble(double *pData, int DataSize) {
    double Tmp;
    for(int i = 1; i < DataSize; i++)
        for(int j = 0; j < DataSize - i; j++)
            if(pData[j] > pData[j + 1]) {
                Tmp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = Tmp;
            }
}

// Function for formatted data output
void PrintData(double *pData, int DataSize) {
    for(int i = 0; i < DataSize; i++)
        printf("%7.4f ", pData[i]);
    printf("\n");
}

// Sorting by the standard library algorithm
void SerialStdSort(double *pData, int DataSize) {
    sort(pData, pData + DataSize);
}
