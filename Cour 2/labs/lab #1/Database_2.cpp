#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

#define random true
#define fullClear true
#define searchByName true
#define searchByQuantity true
#define searchByDate true

using namespace std;

template <typename T>
T getValidInput(const string& prompt);
string generateName(int len)
{
    string name;
    for (int i = 0; i < len; i++)
        name += char(97 + rand() % 26);
    return name;
}

template<typename DatabaseType>
void interactiveMode(DatabaseType& database);
template<typename DatabaseType>
void interactiveModifyProduct(DatabaseType& database);
template<typename DatabaseType>
void interactiveSearchProduct(DatabaseType& database);
template<typename DatabaseType>
void interactiveRemove(DatabaseType& database);

void demonstrationMode();

void benchmarkMode();
template<typename DatabaseType>
void benchmark(DatabaseType& database, int num);

struct Date
{
    int year, month, day, hour, minute, second;

    Date(int year = int(), int month = int(), int day = int(), int hour = int(), int minute = int(), int second = int()) :
        year(year), month(month), day(day), hour(hour), minute(minute), second(second) {}

    Date(bool randomDate)
    {
        if (randomDate) generateDate();
    }


    Date addDaysToDate(int days)
    {
        Date resultDate = *this;
        while (days > 0) {
            int daysInCurrentMonth = resultDate.dayInMonth(resultDate.month);
            if (days <= daysInCurrentMonth - resultDate.day) {
                resultDate.day += days;
                days = 0;
            }
            else {
                days -= (daysInCurrentMonth - resultDate.day + 1);
                resultDate.day = 1;
                resultDate.month += 1;
                if (resultDate.month > 12) {
                    resultDate.month = 1;
                    resultDate.year += 1;
                }
            }
        }
        return resultDate;
    }
    int dayInMonth(int month)
    {
        switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
        default:
            return -1;
        }
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
    void printDate() { cout << this->year << "." << this->month << "." << this->day << endl; }
    void printTime() { cout << this->hour << ":" << this->minute << ":" << this->second << endl; }
    void generateDate()
    {
        this->year = rand() % 10 + 2015;
        this->month = rand() % 12 + 1;
        this->day = rand() % dayInMonth(this->month) + 1;
        this->hour = rand() % 60;
        this->minute = rand() % 60;
        this->second = rand() % 60;
    }

    bool operator>(const Date& other) const
    {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        if (day != other.day) return day > other.day;
        if (hour != other.hour) return hour > other.hour;
        if (minute != other.minute) return minute > other.minute;
        return second > other.second;
    }
};
struct Product
{
    int id, shelf_life;
    string name, unit;
    double quantity;
    Date production_date, shelf_time_date;

    void getInfo() //Функція для отримання данних про товар від користувача
    {
        this->name = getValidInput<string>("Enter name: ");
        do { this->unit = getValidInput<string>("Enter unit: "); } while (this->unit != "kg" && this->unit != "liter" && this->unit != "package" && this->unit != "piece");
        do { this->quantity = getValidInput<double>("Enter quantity: "); } while (this->quantity < 0);
        production_date.getDate();
        do { this->shelf_life = getValidInput<int>("Enter shelf life(in days. Less than 10 years): "); } while (this->shelf_life < 0 || this->shelf_life > 3660);
    }
    void printInfo() //Функція для виведення данних про товар для користувача
    {
        cout << "=====================\n";
        cout << "Id: " << this->id << endl;
        cout << "Product: " << this->name << endl;
        cout << "Unit: " << this->unit << endl;
        cout << "Quantity: " << this->quantity << endl;
        cout << "Production date: "; production_date.printDate();
        cout << "Production time: "; production_date.printTime();
        cout << "Shelf time: "; production_date.addDaysToDate(shelf_life).printDate();
        cout << "=====================\n";
    }
    void generateProduct()
    {
        string units[] = { "kg", "piece", "package", "liter" };
        generateName(rand() % 15 + 1);
        this->unit = units[rand() % 4];
        this->quantity = (this->unit == "liter" || this->unit == "kg") ? static_cast<double>(rand() % 1000) / 10.0 : rand() % 100;
        this->production_date.generateDate();
        this->shelf_life = rand() % 3360;
    }
};

class Database {
protected:
    vector<Product> buffer;
public:
    void add(bool genRanElem)
    {
        Product newProduct;
        genRanElem ? newProduct.generateProduct() : newProduct.getInfo();
        buffer.empty() ? newProduct.id = 1 : newProduct.id = buffer.back().id + 1;
        buffer.push_back(newProduct);
    }

    void print()
    {
        for (Product& p : buffer) {
            p.printInfo();
            cout << endl;
        }
    }
    bool isEmpty() {
        return buffer.empty();
    }

    vector<Product> search(bool exactName = false, const string& fragment = "",
        bool exactQuantity = false, double diaposoneStart = 0.0, double diaposoneEnd = 0.0,
        bool checkDate = false, const Date& startDate = Date()) {
        vector<Product> allCoincidence;
        for (int i = 0; i < buffer.size(); i++)
        {
            if ((exactName && buffer[i].name.find(fragment) != 0) ||
                (exactQuantity && (buffer[i].quantity > diaposoneEnd ||
                    buffer[i].quantity < diaposoneStart)) || (checkDate && startDate > buffer[i].production_date))
                continue;
            allCoincidence.push_back(buffer[i]);
        }
        return allCoincidence;
    }

    void modify(Product* product, bool changeName = false, const string& newName = "",
        bool changeUnit = false, const string& newUnit = "kg",
        bool changeQuantity = false, double newQuantity = 0.0,
        bool changeProductionDate = false, const Date& newProductionDate = Date(),
        bool changeShelfLife = false, int newShelfLife = 0)
    {
        if (changeName) product->name = newName;
        if (changeUnit) product->unit = newUnit;
        if (changeQuantity) product->quantity = newQuantity;
        if (changeProductionDate) product->production_date = newProductionDate;
        if (changeShelfLife) product->shelf_life = newShelfLife;
    }

    Product* find(int id)
    {
        int size = buffer.size();
        for (int i = 0; i < size; i++)
            if (buffer[i].id == id) return &buffer[i];
        return nullptr;
    }

    bool remove(bool delAll = true, int delId = int())
    {
        if (delAll)
        {
            buffer.clear();
            return true;
        }
        else
        {
            for (int i = 0; i < buffer.size(); i++)
            {
                if (buffer[i].id == delId)
                {
                    buffer.erase(buffer.begin() + i);
                    return true;
                }
            }
        }
        return false;
    }
};
class txtDatabase : public Database {
private:
    string filename;
    ofstream databaseFile;
    int id;
public:
    txtDatabase(string filename = "database.txt") : filename(filename), id(0) {
        databaseFile.open(filename, ios::app);
        if (!databaseFile.is_open()) {
            cerr << "Error opening the file " << filename << "." << endl;
            exit(EXIT_FAILURE);
        }
        restore();
    }

    void save()
    {
        ofstream clearFile(filename, ios::trunc);
        if (!clearFile.is_open()) {
            cerr << "Error opening the file " << filename << "." << endl;
            exit(EXIT_FAILURE);
        }
        clearFile.close();

        for (const Product& p : buffer) {
            databaseFile << p.id << " " << p.name << " " << p.unit << " " << p.quantity << " "
                << p.production_date.year << " " << p.production_date.month << " " << p.production_date.day << " "
                << p.production_date.hour << " " << p.production_date.minute << " " << p.production_date.second << " "
                << p.shelf_life << endl;
        }
    }
    void restore() {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Error opening the file " << filename << "." << endl;
            exit(EXIT_FAILURE);
        }

        buffer.clear();
        Product product;
        while (inputFile >> product.id >> product.name >> product.unit >> product.quantity
            >> product.production_date.year >> product.production_date.month >> product.production_date.day
            >> product.production_date.hour >> product.production_date.minute >> product.production_date.second
            >> product.shelf_life) {
            buffer.push_back(product);
            id = max(id, product.id);
        }
        inputFile.close();
    }

    ~txtDatabase() {
        if (databaseFile.is_open())
            databaseFile.close();
    }
};
class binDatabase : public Database {
private:
    string filename;
    ofstream databaseFile;
    int id;

public:
    binDatabase(string filename = "database.bin") : filename(filename), id(0) {
        databaseFile.open(filename, ios::app | ios::binary);
        if (!databaseFile.is_open()) {
            cerr << "Error opening the file " << filename << "." << endl;
            exit(EXIT_FAILURE);
        }
        restore();
    }

    void save()
    {
        ofstream clearFile(filename, ios::trunc);
        if (!clearFile.is_open()) {
            cerr << "Error opening the file " << filename << "." << endl;
            exit(EXIT_FAILURE);
        }
        clearFile.close();

        for (const Product& p : buffer) {
            databaseFile << p.id << " " << p.name << " " << p.unit << " " << p.quantity << " "
                << p.production_date.year << " " << p.production_date.month << " " << p.production_date.day << " "
                << p.production_date.hour << " " << p.production_date.minute << " " << p.production_date.second << " "
                << p.shelf_life << endl;
        }
    }
    void restore() {
        ifstream inputFile(filename, ios::binary);
        if (!inputFile.is_open()) {
            cerr << "Error opening the file " << filename << "." << endl;
            exit(EXIT_FAILURE);
        }

        buffer.clear();
        Product product;
        while (inputFile >> product.id >> product.name >> product.unit >> product.quantity
            >> product.production_date.year >> product.production_date.month >> product.production_date.day
            >> product.production_date.hour >> product.production_date.minute >> product.production_date.second
            >> product.shelf_life) {
            buffer.push_back(product);
            id = max(id, product.id);
        }
        inputFile.close();
    }

    ~binDatabase() {
        if (databaseFile.is_open())
            databaseFile.close();
    }
};
class vectorDatabase : public Database
{
private:
    vector<Product> database;
    int id;

public:
    vectorDatabase() : id(0) {
        restore();
    }
    void save()
    {
        database = buffer;
    }
    void restore() {
        buffer = database;
    }

};

int main()
{
    binDatabase myBinDatabase("interactiveBin.bin");
    txtDatabase myTxtDatabase("interactiveTxt.txt");
    vectorDatabase myVectorDatabase;

    int mode;
    do {
        mode = getValidInput<int>("Choose mode : \n"
                                  "1. Interactive Mode\n"
                                  "2. Demonstration Mode\n"
                                  "3. Benchmark Mode\n"
                                  "0. Exit\n"
                                  "Enter mode: ");
        switch (mode)
        {
            case 0:
                break;
            case 1:
            {
                int dbType;
                do {
                    dbType = getValidInput<int>("Choose database type: \n"
                                                "1. Text Database\n"
                                                "2. Binary Database\n"
                                                "3. Vector Database\n"
                                                "Enter database type: ");
                } while (dbType < 1 || dbType > 3);

                if (dbType == 1) 
                    interactiveMode(myTxtDatabase);
                else if (dbType == 2)
                    interactiveMode(myBinDatabase);
                else
                    interactiveMode(myVectorDatabase);
            }
            break;

            case 2:
                demonstrationMode();
                break;
            case 3:
                benchmarkMode();
                break;
            default:
                cout << "\n### Invalid mode. Please try again. ###\n";
                break;
        }
    } while (mode != 0);

    return 0;
}

template<typename DatabaseType>
void interactiveMode(DatabaseType& database)
{
    int operation;
    do
    {
        cout << "Please select a operation: \n"
                "1. Add an element to the database buffer\n"
                "2. Save database (Record buffer to database)\n"
                "3. Restore database to the database buffer\n"
                "4. Print the database\n"
                "5. Search an element in database by criterias\n"
                "6. Modify an element in the database\n"
                "7. Delete all database buffer or delete an element in database buffer\n"
                "0. Exit\n";
        operation = getValidInput<int>("Please enter a number of operation: ");
        system("cls");
        switch (operation)
        {
        case 0:
            break;
        case 1:
            cout << "=== Add menu ===\n";
            database.add(!random);
            cout << "=== Element added succesfully!\n\n";
            break;
        case 2:
            database.save();
            cout << "=== Database saved succesfully!\n\n";
            break;
        case 3:
            database.restore();
            cout << "=== Database restored to buffer succesfully!\n\n";
            break;
        case 4:
            if (database.isEmpty()) {
                cout << "=== Database is empty! ===\n\n";
            }
            else {
                cout << "All product in database:\n";
                database.print();
            }
            break;
        case 5:
            interactiveSearchProduct(database);
            break;
        case 6:
            interactiveModifyProduct(database);
            break;
        case 7:
            interactiveRemove(database);
            break;
        default:
            cerr << "\n### Error! You enter incorect mode! ###\n\n";
            break;
        }
    } while (operation != 0);
    
}
void demonstrationMode()
{
    system("cls");
    bool manage = true;
    Date zeroValueDate;
    binDatabase myBinDatabase("demonstration.bin");
    cout << "=== START DEMONSTRATION MODE ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- ADD 12 ELEMENTS TO DATABASE ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    for (int i = 0; i < 12; i++)
    {
        myBinDatabase.add(random);
    }
    cout << "\n=== Elements added succesfully! ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- SAVE THE DATABASE ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    myBinDatabase.save();
    cout << "\n=== Database saved successfully! ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- RECORD THE DATABASE ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    myBinDatabase.restore();
    cout << "\n=== Database was recorded successfully! ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- PRINT THE DATABASE ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    myBinDatabase.print();
    cout << "\n=== Database was read successfully! ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- SEARCH THE DATABASE FOR ALL ELEMENTS STARTING WITH THE LETTER 'A' AND HAVING A NUMBER FROM 0 TO 100 -- - \n\n";
    this_thread::sleep_for(chrono::seconds(2));
    vector<Product> searched = myBinDatabase.search(true, "a", true, 0, 100);
    if (searched.empty() != 0)
        cout << "\n### Elements not founded! ###\n";
    else
    {
        cout << "\nAll founded elements: \n";
        this_thread::sleep_for(chrono::seconds(2));
        for (Product& p : searched)
            p.printInfo();
    }
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- CHANGE ELEMENT WITH 1 ID IN DATABASE. CHANGE THE ELEMENT NAME TO 'MILK' --- \n\n";
    this_thread::sleep_for(chrono::seconds(2));
    myBinDatabase.modify(myBinDatabase.find(1), true, "Milk");
    cout << "\n=== Name changed succesfully! === \n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- DELETE ELEMENT WITH ID 5 IN THE DATABASE ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    myBinDatabase.remove(false, 5);
    cout << "\n=== Element with ID 5 was deleted! ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- PRINT THE DATABASE ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    myBinDatabase.print();
    cout << "\n=== Database was read successfully! ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- FULL CLEAR DATABASE ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    myBinDatabase.remove(fullClear);
    cout << "\n=== Database was cleared successfully! ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- SAVE THE DATABASE ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    myBinDatabase.save();
    cout << "\n=== Database saved successfully! ===\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n\n--- EXIT ---\n\n";
    this_thread::sleep_for(chrono::seconds(2));
    cout << "\n=== DATABASE CLOSE ===";
}
void benchmarkMode()
{
    cout << "\n=== Benchmark mode ===\n";
    txtDatabase myTxtDatabase("benchamrkTxt.txt");
    binDatabase myBinDatabase("benchmarkBin.bin");
    vectorDatabase myVectorDatabase;

    int num;
    do { num = getValidInput<int>("Enter a number of element for database: "); } while (num < 1);

    for (int mode = 2; mode >= 0; mode--) 
    {
        if (mode == 2)
        {
            cout << "\n\n=== START BENCHMARK TXT DATABASE ===\n";
            benchmark(myTxtDatabase, num);
        }

        else if (mode == 1)
        {
            cout << "\n\n=== START BENCHMARK BIN DATABASE ===\n";
            benchmark(myBinDatabase, num);
        }
        else
        {
            cout << "\n\n=== START BENCHMARK VECTORE DATABASE ===\n";
            benchmark(myVectorDatabase, num);
        }
    
    }
    cout << "\n\n=== BENCHMARK OVER ===\n";

}
template<typename DatabaseType>
void benchmark(DatabaseType& database, int num)
{
    database.remove(fullClear);
    auto startTime = chrono::high_resolution_clock::now();
    auto startTimeAdd = chrono::high_resolution_clock::now();
    for (int i = 0; i < num; i++)
        database.add(random);
    auto endTimeAdd = chrono::high_resolution_clock::now();

    auto startTimeRecord = chrono::high_resolution_clock::now();
    database.save();
    auto endTimeRecord = chrono::high_resolution_clock::now();

    auto startTimeRestore = chrono::high_resolution_clock::now();
    database.restore();
    auto endTimeRestore = chrono::high_resolution_clock::now();

    auto startTimeSearchAll = chrono::high_resolution_clock::now();

    auto startTimeSearchName = chrono::high_resolution_clock::now();
    vector<Product> searched = database.search(searchByName, generateName(rand() % 3 + 1));
    auto endTimeSearchName = chrono::high_resolution_clock::now();

    double startDiap = static_cast<double>(rand() % 1000) / 10.0;
    double endDiap = static_cast<double>(rand() % 1000) / 10.0;
    if (startDiap > endDiap) swap(startDiap, endDiap);

    auto startTimeSearchQuantity = chrono::high_resolution_clock::now();
    searched = database.search(!searchByName, "", searchByQuantity, startDiap, endDiap);
    auto endTimeSearchQuantity = chrono::high_resolution_clock::now();

    auto startTimeSearchDate = chrono::high_resolution_clock::now();
    Date searchData(true);
    searched = database.search(!searchByName, "", !searchByQuantity, 0, 0, searchByDate, searchData);
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
    cout << "\nTime add: " << durationAdd.count() << "milliseconds";
    cout << "\nRecording time: " << durationRecord.count() << "milliseconds";
    cout << "\nRestore time: " << durationRestore.count() << "milliseconds";
    cout << "\nSearch time by name: " << durationSearchName.count() << "milliseconds";
    cout << "\nSearch time by quantity: " << durationSearchDate.count() << "milliseconds";
    cout << "\nSearch time by date: " << durationSearchDate.count() << "milliseconds";
    cout << "\nTotal search time: " << durationSearchAll.count() << "milliseconds";
    cout << "\nTotal working time: " << duration.count() / 1000.0 << "seconds";
    cout << "\n=============================\n";

    
}

template<typename DatabaseType>
void interactiveModifyProduct(DatabaseType& database)
{
    cout << "=== Modification menu ===\n";
    int idModify;
    idModify = getValidInput<int>("Enter the ID of the item you want to change: ");
    Product* product = database.find(idModify);
    if (product == nullptr)
    {
        cout << "\n### Element didn't found! ###\n";
        return;
    }

    cout << "\n=== Product found! ===\n";
    bool changeName = false, changeUnit = false, changeQuantity = false, changeProductionDate = false, changeShelfLife = false;
    string newName, newUnit;
    double newQuantity = 0.0;
    Date newProductionDate;
    int newShelfLife = 0;

    int operation;
    bool changed = false;
    do
    {
        system("cls");
        changed ? cout << "\nDo you want to change anything else?\n" : cout << "\nWhat do you want to change?\n";
        operation = getValidInput<int>("1. Product name\n"
                                       "2. Product unit\n"
                                       "3. Quantity of products\n"
                                       "4. Product production date\n"
                                       "5. Product shelf life\n"
                                       "0. Nothing\n"
                                       "Enter operation: ");
        changed = true;

        switch (operation)
        {
        case 0:
            break;
        case 1:
            newName = getValidInput<string>("Enter a new name: ");
            changeName = true; 
            break;
        case 2:
            do { newUnit = getValidInput<string>("Enter a new unit: "); } while (newUnit != "kg" && newUnit != "liter" && newUnit != "package" && newUnit != "piece");
            changeUnit = true; 
            break;
        case 3:
            newQuantity = getValidInput<double>("Enter a new quantity: ");
            changeQuantity = true; 
            break;
        case 4:
            cout << "Enter a new production date: ";
            newProductionDate.getDate();
            changeProductionDate = true; 
            break;
        case 5:
            newShelfLife = getValidInput<int>("Enter a new shelf life(in days. Days must be less than 3660): ");
            changeShelfLife = true;
            break;
        default:
            cout << "### You entered the wrong number! ###\n";
            break;
        }
    } while (operation != 0);

    database.modify(product, changeName, newName, changeUnit, newUnit, changeQuantity, newQuantity,
        changeProductionDate, newProductionDate, changeShelfLife, newShelfLife);
    cout << "\n=== Element changed succesfully!\n";
}
template<typename DatabaseType>
void interactiveSearchProduct(DatabaseType& database)
{
    cout << "=== Search menu ===\n";
    string fragment;
    double diaposoneStart = 0, diaposoneEnd = 0;
    Date startDate;
    bool exactName = false, exactQuantity = false, checkDate = false;
    int criterion;
    do {
        system("cls");
        criterion = getValidInput<int>("Select criterion:\n"
                                       "1. Product name\n"
                                       "2. Quantity of products\n"
                                       "3. Date of manufacture of the product\n"
                                       "0. Exit\n"
                                       "Please, enter a number of criterion: ");
        switch (criterion) {
        case 0:
            break;
        case 1:
            fragment = getValidInput<string>("Enter a product name for saerch: ");
            exactName = true;
            break;
        case 2:
            cout << "Enter the range of the number of products: \n";
            do { diaposoneStart = getValidInput<double>("Enter the smallest value: "); } while (diaposoneStart < 0);
            do { diaposoneEnd = getValidInput<double>("Enter the greatest value: "); } while (diaposoneStart < 0);
            exactQuantity = true;
            break;
        case 3:
            cout << "Enter start date (YYYY MM DD HH MM SS): ";
            startDate.getDate();
            checkDate = true;
            break;
        default:
            cerr << "\n### You entered a wrong number! ###\n";
            break;
        }
    } while (criterion != 0);

    system("cls");
    if (!exactName && !exactQuantity && !checkDate) {
        cout << "### You didn't select any criterion! ###\n";
        return;
    }

    vector<Product> allCoincidence = database.search(exactName, fragment,
                                                     exactQuantity, diaposoneStart, diaposoneEnd,
                                                     checkDate, startDate);

    if (allCoincidence.empty()) cout << "### Products didn`t founded! ###\n";
    else
    {
        cout << "Founded products: \n";
        for (Product& p : allCoincidence) { p.printInfo(); }
    }
}
template<typename DatabaseType>
void interactiveRemove(DatabaseType& database)
{

    int operation;
    do {
        operation = getValidInput<int>("Select operation for delete:\n"
            "1. Delete element in database buffer\n"
            "2. Delete all database buffer\n"
            "0. Exit\n"
            "Enter operation: ");
    } while (operation < 0 || operation > 2);

    system("cls");
    if (operation == 1)
    {
        int delId;
        delId = getValidInput<int>("Enter the ID of the product you want to share: ");
        if (database.remove(!fullClear, delId)) cout << "\n=== Product with id " << delId << " deleted! ===\n";
        else  cout << "\n### Element with id " << delId << " didn`t deleted! ###\n";

        return;
    }
    else
    {
        database.remove(fullClear);
        cout << "\n=== Database buffer deleted succesfully! ===\n";
    }
        
}
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
