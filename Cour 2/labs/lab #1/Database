#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

//Функція для перевірки правильності введення даних користувачем
template <typename T>
T getValidInput(const string& prompt) {
    T value;
    bool cinFail;
    do
    {
        cinFail = false;
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cerr << "\n### Invalid input. Please enter a valid number. ###\n\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cinFail = true;
        };
    } while (cinFail);
    return value;
}

//Структури
struct date
{
    int year, month, day, hour, minute, second;

    int dayInMonth(int month); //Функція для визначення кількості днів в місяці
    date addDaysToDate(int days)
    {
        date resultDate = *this;
        while (days > 0)
        {
            int daysInCurrentMonth = resultDate.dayInMonth(resultDate.month);
            if (days <= daysInCurrentMonth - resultDate.day)
            {
                resultDate.day += days;
                days = 0;
            }
            else
            {
                days -= (daysInCurrentMonth - resultDate.day + 1);
                resultDate.day = 1;
                resultDate.month += 1;
                if (resultDate.month > 12)
                {
                    resultDate.month = 1;
                    resultDate.year += 1;
                }
            }
        }
        return resultDate;
    }
    void getDate()     //Функція запиту дати від користувача та перевірка кожного значення на правилність введення
    {
        do { this->year = getValidInput<int>("Enter year: "); } while (this->year < 2015);
        do { this->month = getValidInput<int>("Enter month: "); } while (this->month > 12 || this->month < 1);
        do { this->day = getValidInput<int>("Enter day: "); } while (this->day > dayInMonth(this->month) || this->day < 1);
        do { this->hour = getValidInput<int>("Enter hour: "); } while (this->hour > 24 || this->hour < 0);
        do { this->minute = getValidInput<int>("Enter minute: "); } while (this->minute > 59 || this->minute < 0);
        do { this->second = getValidInput<int>("Enter second: "); } while (this->second > 59 || this->second < 0);
    }
    void printDate() { cout << this->year << "." << this->month << "." << this->day << "\n"; }
    void printTime() { cout << this->hour << ":" << this->minute << ":" << this->second << "\n"; }

    bool operator>(const date& other)
    {
        if (this->year > other.year) return true;
        else if (this->year < other.year) return false;

        if (this->month > other.month) return true;
        else if (this->month < other.month) return false;

        if (this->day > other.day) return true;
        else if (this->day < other.day) return false;

        if (this->hour > other.hour) return true;
        else if (this->hour < other.hour) return false;

        if (this->minute > other.minute) return true;
        else if (this->minute < other.minute) return false;

        return this->second > other.second;
    }
};
struct Product
{
    int id, shelf_life;
    string name, unit;
    double quantity;
    date production_date, shelf_time_date;

    void getInfo() //Функція для отримання данних про товар від користувача
    {
        this->name = getValidInput<string>("Enter name: ");
        do { this->unit = getValidInput<string>("Enter unit: "); } while (this->unit != "kg" && this->unit != "liter" && this->unit != "package" && this->unit != "piece");
        do { this->quantity = getValidInput<double>("Enter quantity: "); } while (this->quantity < 0);
        production_date.getDate();
        do { this->shelf_life = getValidInput<int>("Enter shelf life(in days. Less than 10 years): "); } 
        while (this->shelf_life < 0 || this->shelf_life > 3660);
    }

    void printInfo() //Функція для виведення данних про товар для користувача
    {
        cout << "=====================\n";
        cout << "Id: " << this->id << "\n";
        cout << "Product: " << this->name << "\n";
        cout << "Unit: " << this->unit << "\n";
        cout << "Quantity: " << this->quantity << "\n";
        cout << "Production date: "; production_date.printDate();
        cout << "Production time: "; production_date.printTime();
        cout << "Shelf time: "; production_date.addDaysToDate(shelf_life).printDate();
        cout << "=====================\n";
    }
};

//Функції винисені за структуру
int date::dayInMonth(int month)
{
    switch (month)
    {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
    default:
        return -1;  // Повертаємо -1 у випадку некоректного місяця
    }
}

//Функції
//Функції для текстової і бінарної бази данних
void DatabaseManagement(const string& filename, bool isTxtDatabase);
void addElement(vector<Product>& bufferDatabase,bool randomElement,  bool isBenchmark);
void recordDatabase(const string& filename, const vector<Product>& bufferDatabase, bool isTxtDatabse, bool isBenchmark);
void restore(const string& filename, vector<Product>& database, bool isTxtDatabse, bool isBenchmark);
void readAllDatabase(vector<Product>& bufferDatabase);
void searchElementsManage(vector<Product>& bufferDatabase);
vector<Product> search(vector<Product> bufferDatabase, bool name, string fragment, bool numOfUnit, double diaposoneStart, double diaposoneEnd, bool checkDate, date startDate);
void modifyElement(vector<Product>& bufferDatabase);
void delElement(vector<Product>& bufferDatabase);

//Векторна база даних
void VectorDatabase();

void benchmarkMode();
void demonstrationMode();

Product generateRandomElement();
string generateString(int len);
date getRandomDate();

int main()
{
    srand(time(nullptr));
    cout << "\n--- Database loaded successfully! ---\n";
    int mode;
    string filename;
    mode = getValidInput<int>("Please, select a number of mode:\n1. Database management\n2. Demontstration mode \n3. Benchmark\nMode: ");
    
    switch(mode)
    {
        case 1:
            int modeManagement;
            modeManagement = getValidInput<int>("\nPlease, select a number of database:\n1. Vector Database \n2. TXT File Database\n3. Bin File Database\nDatabase: ");;
            switch(modeManagement)
            {
                case 1:
                    VectorDatabase();
                    break;
                case 2:
                    filename = "database.txt";
                    DatabaseManagement(filename, true);
                    break;
                case 3:
                    filename = "database.bin";
                    DatabaseManagement(filename, false);
                    break;
                default:
                    cerr << "Error! You enter incoretct database\n";
                    break;
            }
            break;
        case 2:
            demonstrationMode();
            break;
        case 3:
            benchmarkMode();
            break;
        default:
            cerr << "Error! You enter incoretct mode\n";
            break;
    }
}

void DatabaseManagement(const string& filename, bool isTxtDatabse)
{
    ios_base::openmode fileMode = isTxtDatabse ? ios::app : ios::binary | ios::app;
    fstream outputFile(filename, fileMode);

    if (!outputFile.is_open())
    {
        cerr << "\n### Error opening the file " << filename << ". ###\n";
        return;
    }

    bool manage = true;
    vector<Product> bufferDatabase; //Масив для збереження тимчасових елементів
    restore(filename, bufferDatabase, isTxtDatabse, false);

    while (manage)
    {
        cout << "\nHere you can manage the text database. Please select a operation: \n";
        cout << "0. Exit\n1. Add element to database\n2. Record date to file\n3. Restore database\n4. Output database\n5. Searh for an product by criteria\n6. Modify an element\n7. Delete database or element\n";
        int operation;
        operation = getValidInput<int>("Please enter a number of operation: ");

        switch(operation)
        {
            case 0:
                manage = false;
                break;
            case 1:
                addElement(bufferDatabase, false, false);
                break;
            case 2: 
                recordDatabase(filename, bufferDatabase, isTxtDatabse, false);
                break;
            case 3:
                restore(filename, bufferDatabase, isTxtDatabse, false);
                break;
            case 4:
                readAllDatabase(bufferDatabase);
                break;
            case 5:
                searchElementsManage(bufferDatabase);
                break;
            case 6:
                modifyElement(bufferDatabase);
                break;
            case 7:
                delElement(bufferDatabase);
                break;
            default:
                cerr << "Error! You enter incoretct mode\n";
                break;
        }
    }
    
    cout << "\n=== Text database file '" << filename << "' has been created/updated. ===\n";
    outputFile.close();
}
void VectorDatabase()
{
    bool manage = true;
    vector<Product> bufferDatabase, Database, copyDatabase; //Масив для збереження тимчасових елементів

    while (manage)
    {
        cout << "\nHere you can manage the text database. Please select a operation: \n";
        cout << "0. Exit\n1. Add element to database\n2. Record date to file\n3. Restore database\n4. Output database\n5. Searh for an product by criteria\n6. Modify an element\n7. Delete database or element\n";
        int operation;
        operation = getValidInput<int>("Please enter a number of operation: ");

        switch (operation)
        {
        case 0:
            manage = false;
            break;
        case 1:
            addElement(bufferDatabase, false, false);
            break;
        case 2:
            Database = bufferDatabase;
            break;
        case 3:
            int record;
            record = getValidInput<int>("\n1. Create copy\n2. restore copy");
            if (record == 1) copyDatabase = Database;
            else if (record == 2) Database = copyDatabase;
            else cout << "\n### You enter incorect number! ###\n";
            break;
        case 4:
            readAllDatabase(bufferDatabase);
            break;
        case 5:
            searchElementsManage(bufferDatabase);
            break;
        case 6:
            modifyElement(bufferDatabase);
            break;
        case 7:
            delElement(bufferDatabase);
            break;
        default:
            cerr << "Error! You enter incoretct mode\n";
            break;
        }
    }

    cout << "\n=== Vector Database has been deleted. ===\n";
}

void addElement(vector<Product>& bufferDatabase,bool randomElement,  bool isBenchmark)
{
    Product newProduct;
    if (!randomElement) newProduct.getInfo();
    else newProduct = generateRandomElement();
    bufferDatabase.empty() ? newProduct.id = 1 : newProduct.id = bufferDatabase[bufferDatabase.size() - 1].id + 1;
    bufferDatabase.push_back(newProduct);
    if (!isBenchmark) cout << "\n=== Product added to buffer successfully. ===\n";
}
void recordDatabase(const string& filename, const vector<Product>& bufferDatabase, bool isTxtDatabse, bool isBenchmark)
{
    ofstream clearFile(filename, ios::trunc);
    clearFile.close();
    ios_base::openmode fileMode = isTxtDatabse ? ios::in : ios::binary | ios::in;
    ofstream outputFile(filename, fileMode);

    if (!outputFile.is_open())
    {
        cerr << "\n### Error opening the file " << filename << "###\n";
        return;
    }

    for (const Product& product : bufferDatabase)
    {
        outputFile << product.id << ' ' << product.name << ' ' << product.unit << ' ' << product.quantity << ' ';
        outputFile << product.production_date.year << ' ' << product.production_date.month << ' ' << product.production_date.day << ' ';
        outputFile << product.production_date.hour << ' ' << product.production_date.minute << ' ' << product.production_date.second << ' ';
        outputFile << product.shelf_life << '\n';
    }

    if (!isBenchmark) cout << "\n=== Database has been recorded to the file '" << filename << "'. ===\n";
    outputFile.close();
}
void readAllDatabase(vector<Product>& readVector)
{
    readVector.empty() ==0 ? cout << "\nAll element in databse: \n" : cout << "\n### Darabase is clear! ###\n";
    for (auto& p : readVector)
    {
        p.printInfo();
    }
}
void restore(const string& filename, vector<Product>& bufferDatabase, bool isTxtDatabse, bool isBenchmark)
{
    ios_base::openmode fileMode = isTxtDatabse ? ios::out : ios::binary | ios::out;
    ifstream readFile(filename, fileMode);
    bufferDatabase.clear();

    if (!readFile.is_open()) {
        cerr << "\n### Error opening the file " << filename << ". ###\n";
        return;
    }

    Product product;
    while (readFile >> product.id >> product.name >> product.unit >> product.quantity
           >> product.production_date.year >> product.production_date.month >> product.production_date.day
           >> product.production_date.hour >> product.production_date.minute >> product.production_date.second
           >> product.shelf_life) 
    {

        bufferDatabase.push_back(product);
    }

    if (!isBenchmark) cout << "\n=== Database has been restored/copy from the file " << filename << " to buffer. ===\n";
    readFile.close();
}
void searchElementsManage(vector<Product>& bufferDatabase) {
    vector<Product> allCoincidence;
    string fragment;
    double diaposoneStart = 0, diaposoneEnd = 0;
    date startDate = { 0 };
    bool name = false, numOfUnit = false, checkDate = false;
    int criterion;
    do {
        criterion = getValidInput<int>("Select criterion:\n""1. Name\n2. Numbers of units\n3. Date\nPlease, enter a number criterion or `0` for exit: ");
        switch (criterion) {
        case 0:
            break;
        case 1:
            fragment = getValidInput<string>("Enter a name of product: ");
            name = true;
            break;
        case 2:
            cout << "\n=== Enter a range units ===\n";
            do { diaposoneStart = getValidInput<double>("Enter the smallest value: "); } while (diaposoneStart < 0);
            do { diaposoneEnd = getValidInput<double>("Enter the greatest value: "); } while (diaposoneStart < 0);
            numOfUnit = true;
            break;
        case 3:
            cout << "Enter start date (YYYY MM DD HH MM SS): ";
            startDate.getDate();
            checkDate = true;
            break;
        default:
            cerr << "\n### You entered a wrong number ###\n";
            break;
        }
    } while (criterion != 0);

    if (!name && !numOfUnit && !checkDate) {
        cout << "\n### You didn't select any criterion ###\n";
        return;
    }

    allCoincidence = search(bufferDatabase, name, fragment, numOfUnit, diaposoneStart, diaposoneEnd, checkDate, startDate);

    if (allCoincidence.empty()) cout << "\n### Elemetnts didn`t founded! ###\n";
    else  for (auto& p :allCoincidence) { p.printInfo(); }
}
vector<Product> search(vector<Product> bufferDatabase, bool name, string fragment, bool numOfUnit, double diaposoneStart, double diaposoneEnd, bool checkDate, date startDate )
{
    vector<Product> allCoincidence;
    for (int i = 0; i < bufferDatabase.size(); i++)
    {
        if ((name && bufferDatabase[i].name.find(fragment) != 0) ||
            (numOfUnit && (bufferDatabase[i].quantity > diaposoneEnd ||
                bufferDatabase[i].quantity < diaposoneStart)) || (checkDate && startDate > bufferDatabase[i].production_date))
            continue;
        allCoincidence.push_back(bufferDatabase[i]);
    }
    return allCoincidence;
}
void modifyElement(vector<Product>& bufferDatabase)
{
    int idModify;
    idModify = getValidInput<int>("\nEnter id of the element which you want to modify: ");
    for (int i = 0; i < bufferDatabase.size(); i++)
    {
        if (bufferDatabase[i].id == idModify)
        {
            cout << "\n=== Element founded! ===\n";
            int operation;
            bool changed = false;
            do
            {
                changed ? cout << "\nDo you want to change anything else?n\n" : cout << "\nWhat do you want to change?\n";
                operation = getValidInput<int>("0. Nothing\n1. Name\n2. Unit\n3. Quantity\n4. Production date\n5. shelf life\nEnter opetarion: ");
                changed = true;

                switch (operation)
                {
                case 0:
                    break;
                case 1:
                    bufferDatabase[i].name = getValidInput<string>("Enter a new name: ");
                    cout << "\n=== Name changed succesfully! ===\n";
                    break;
                case 2:
                    bufferDatabase[i].unit = getValidInput<string>("Enter a new unit: ");
                    cout << "\n=== Unit changed succesfully! ===\n";
                    break;
                case 3:
                    do { bufferDatabase[i].quantity = getValidInput<double>("Enter a new quantity: "); } while (bufferDatabase[i].quantity < 0);
                    cout << "\n=== Quantity changed succesfully! ===\n";
                    break;
                case 4:
                    cout << "Enter a new production date: ";
                    bufferDatabase[i].production_date.getDate();
                    cout << "\n=== Production date changed succesfully! ===\n";
                    break;
                case 5:
                    do { bufferDatabase[i].shelf_life = getValidInput<int>("Enter a new shelf life(in days. Days must be less than 3660): "); } while (bufferDatabase[i].shelf_life < 0 || bufferDatabase[i].shelf_life > 3660);
                    cout << "\n=== Shelf life changed succesfully! ===\n";
                    break;
                default:
                    cout << "\n### You enter wrong number! ###\n";
                    break;
                }
            } while (operation != 0);
            break;
        }
    }
}
void delElement(vector<Product>& bufferDatabase)
{
    int operation;
    operation = getValidInput<int>("\nSelect operation for delete:\n0. Exit\n1. Delete element by id\n2. Delete all data in database\nEnter operation: ");
    if (operation <= 0 || operation > 2) return;

    if (operation == 1)
    {
        int delId;
        delId = getValidInput<int>("Enter a id file which you want to delete: ");
        bool deleted;
        Product product;
        for (int i = 0; i < bufferDatabase.size(); i++)
        {
            if (bufferDatabase[i].id == delId)
            {
                deleted = true;
                bufferDatabase.erase(bufferDatabase.begin() + i);
            }
        }

        if (deleted) cout << "\n=== Element with id " << delId << " was deleted! ===\n";
        else  cout << "\n### Element with id " << delId << " wasn`t deleted! ###\n";

        return;
    }
    else
    {
        bufferDatabase.clear();
        cout << "\n=== Data was deleted succesfully! ===\n";
    }
}

void benchmarkMode()
{
    cout << "\n=== Benchmark mode ===\n";

    string txt = "benchmarkTxt.txt", bin = "benchmarkBin.bin";
    fstream benchmarkTxt(txt, ios::app), benchmarkBin(bin, ios::binary | ios::app);
    vector<Product> benchmarkVector, buffer, searching;

    int num;
    do { num = getValidInput<int>("Enter a number of element for database: "); } while (num < 1);

    for (int benchmark = 2; benchmark >= 0; benchmark--)
    {
        string file = txt;
        bool isTxtDatabse = true;
        if (benchmark == 2)
        {
            cout << "\n=== START BENCHMARK TXT DATABASE ===\n";
        }
        else if (benchmark == 1)
        {
            cout << "\n\n=== START BENCHMARK BIN DATABASE ===\n";
            file = bin;
            isTxtDatabse = false;
        }
        else 
        {
            cout << "\n\n=== START BENCHMARK VECTORE DATABASE ===\n";
            buffer.clear();
        }
        auto startTime = chrono::high_resolution_clock::now();
        auto startTimeAdd = chrono::high_resolution_clock::now();
        for (int i = 0; i < num; i++)
        {
            addElement(buffer, true, true);
        }
        auto endTimeAdd = chrono::high_resolution_clock::now();

        auto startTimeRecord = chrono::high_resolution_clock::now();
        if (benchmark != 0) recordDatabase(file, buffer, true, true);
        else benchmarkVector = buffer;
        auto endTimeRecord = chrono::high_resolution_clock::now();

        auto startTimeRestore = chrono::high_resolution_clock::now();
        if (benchmark != 0) restore(file, buffer, true, true);
        else buffer = benchmarkVector;
        auto endTimeRestore = chrono::high_resolution_clock::now();


        auto startTimeSearchAll = chrono::high_resolution_clock::now();

        auto startTimeSearchName = chrono::high_resolution_clock::now();
        vector<Product> searching = search(buffer, true, generateString(rand() % 3 + 1), false, 0, 0, false, { 0 });
        auto endTimeSearchName = chrono::high_resolution_clock::now();
        
        double startDiap = static_cast<double>(rand() % 1000) / 10.0;
        double endDiap = static_cast<double>(rand() % 1000) / 10.0;
        if (startDiap > endDiap) swap(startDiap, endDiap);

        auto startTimeSearchQuantity = chrono::high_resolution_clock::now();
        searching = search(buffer, false, "", true, startDiap, endDiap , false, {0});
        auto endTimeSearchQuantity = chrono::high_resolution_clock::now();

        auto startTimeSearchDate = chrono::high_resolution_clock::now();
        searching = search(buffer, false, "", false, 0, 0, true, getRandomDate());
        auto endTimeSearchDate = chrono::high_resolution_clock::now();
        auto endTimeSearchAll = chrono::high_resolution_clock::now();
        auto endTime = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        auto durationAdd = chrono::duration_cast<chrono::milliseconds>(endTimeAdd - startTimeAdd);
        auto durationRecord = chrono::duration_cast<chrono::milliseconds>(endTimeRecord - startTimeRecord);
        auto durationSearchName = chrono::duration_cast<chrono::milliseconds>(endTimeSearchName - startTimeSearchName);
        auto durationSearchQuantity = chrono::duration_cast<chrono::milliseconds>(endTimeSearchQuantity - startTimeSearchQuantity);
        auto durationSearchDate = chrono::duration_cast<chrono::milliseconds>(endTimeSearchDate - startTimeSearchDate);
        auto durationSearchAll = chrono::duration_cast<chrono::milliseconds>(endTimeSearchAll - startTimeSearchAll);

        auto durationRestore = chrono::duration_cast<chrono::milliseconds>(endTimeRestore - startTimeRestore);
        cout << "\n=============================\n";
        cout << "\nTime add: " << durationAdd.count();
        cout << "\nTime record: " << durationRecord.count();
        cout << "\nTime restore: " << durationRestore.count();
        cout << "\nSearch by name: " << durationSearchName.count();
        cout << "\nSearch by quantity: " << durationSearchDate.count();
        cout << "\nSearch by date: " << durationSearchDate.count();
        cout << "\nSearch by all: " << durationSearchAll.count();
        cout << "\nFull work time: " << duration.count();
        cout << "\n=============================\n";
        buffer.clear();
    }
    benchmarkBin.close();
    benchmarkTxt.close();
    

    cout << "\n\n=== BENCHMARK OVER ===\n";

}
void demonstrationMode()
{
    bool manage = true;
    vector<Product> bufferDatabase, demonstationBase; // Масив для збереження тимчасових елементів
    cout << "\n=== START DEMONSTRATION ===\n";
    cout << "\n\n--- Let's add 5 elements ---\n\n";

    string mode = "\nOperation:\n"
        "0. Exit\n"
        "1. Add element to database\n"
        "2. Record date to file\n"
        "3. Restore database\n"
        "4. Output database\n"
        "5. Search for a product by criteria\n"
        "6. Modify an element\n"
        "7. Delete database or element\n"
        "Enter operation: ";

    string modeSearch = "\nSelect criterion:\n"
                        "1.Name\n"
                        "2.Numbers of units\n"
                        "3.Date\n"
                        "Please, enter a number criterion or `0` for exit:";

    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "1\n";

    for (int i = 0; i < 5; i++)
    {
        cout << "\n=== " << i + 1 << " element generated! ===\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        addElement(bufferDatabase, true, false);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    cout << "\n\n--- Let's save the database ---\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "2\n";
    demonstationBase = bufferDatabase;
    cout << "\n=== Database was saved successfully ===\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cout << "\n\n--- Let's record the database ---\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "3\n";
    bufferDatabase = demonstationBase;
    cout << "\n=== Database was recorded successfully ===\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cout << "\n\n--- Let's output the database ---\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "4\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    readAllDatabase(bufferDatabase);
    cout << "\n=== Database was read successfully ===\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cout << "\n\n--- Let's search all elements starting with the letter 'a' and with quantity between 0 - 100 in the database ---\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "5\n";;
    std::this_thread::sleep_for(std::chrono::seconds(1));;
    cout << modeSearch << " 1";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Enter a name of product: a\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << modeSearch << " 2\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Enter the smallest value: 0\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Enter the greatest value : 100\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout <<  modeSearch <<" 0\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    vector<Product> name = search(bufferDatabase, true, "a", true, 0, 100, false, { 0 });
    if (name.empty() != 0)
    {
        cout << "\n### Elements not founded! ###\n";
    }
    else
    {
        cout << "\nAll founded elements: \n";
        for (auto& p : name)
        {
            p.printInfo();
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cout << "\n\n--- Let`s modify element by 1 id in database. Change name 'Milk' --- \n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "6\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "\nWhat do you want to change ? \n0.Nothing\n1.Name\n2.Unit\n3.Quantity\n4.Production date\n5.shelf life\nEnter opetarion:";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "1\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Enter a new name: Milk\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "\n=== Name changed succesfully!=== \n";
    bufferDatabase[0].name = "Milk";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cout << "\n\n--- Let's delete the element with ID 5 in the database ---\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << mode << "Enter operation: 7\nSelect operation for deletion:\n0. Exit\n1. Delete element by ID\n2. Delete all data in the database\nEnter operation:";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "1\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Enter an ID for the element to be deleted: 5\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "\n=== Element with ID 5 was deleted ===\n";
    bufferDatabase.erase(bufferDatabase.begin() + 4);

    cout << "\n\n--- Let's save the database again ---\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "2\n";;
    demonstationBase = bufferDatabase;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "\n=== Database was saved successfully ===\n";

    cout << "\n\n--- Let's output the database again ---\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "4\n";;
    readAllDatabase(bufferDatabase);
    cout << "\n=== Database was read successfully ===\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cout << "\n\n--- Let's exit ---\n\n";
    cout << mode;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "5\n";
    cout << "\n=== DATABASE CLOSE ===";
}

Product generateRandomElement()
{
        Product randomElement;
        string units[] = { "kg", "piece", "package", "liter" };
        randomElement.name = generateString(rand() % 15 + 1);
        randomElement.unit = units[rand() % 4];
        randomElement.quantity = (randomElement.unit == "liter" || randomElement.unit == "kg") ? static_cast<double>(rand() % 1000) / 10.0 : rand() % 100;
        randomElement.production_date = getRandomDate();
        randomElement.shelf_life = rand() % 3360;
       
    return randomElement;
}
string generateString(int len)
{
    string randomString;
    for(int i = 0; i < len; i++)
    {
        randomString += char(97 + rand() % 26);
    }
    return randomString;
}
date getRandomDate()
{
    date randomDate;
    randomDate.year = rand() % 10 + 2015;
    randomDate.month = rand() % 12 + 1;
    randomDate.day = rand() % randomDate.dayInMonth(randomDate.month) + 1;
    randomDate.hour = rand() % 60;
    randomDate.minute = rand() % 60;
    randomDate.second = rand() % 60;
    return randomDate;
}
