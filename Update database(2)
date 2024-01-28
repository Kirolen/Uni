#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    void getDate()     //Функція запиту дати від користувача та перевірка кожного значення на правилність введення
    {
        do { this->year = getValidInput<int>("Enter year: "); } while (this->year < 1);
        do { this->month = getValidInput<int>("Enter month: "); } while (this->month > 12 || this->month < 1);
        do { this->day = getValidInput<int>("Enter day: "); } while (this->day > dayInMonth(this->month) || this->day < 1);
        do { this->hour = getValidInput<int>("Enter hour: "); } while (this->hour > 24 || this->hour < 0);
        do { this->minute = getValidInput<int>("Enter minute: "); } while (this->minute > 59 || this->minute < 0);
        do { this->second = getValidInput<int>("Enter second: "); } while (this->second > 59 || this->second < 0);
    }
    void printDate()    //Функція зручного виводу дати та часу
    {
        cout << "Date: " << this->year << "." << this->month << "." << this->day << "\n";
        cout << "Time: " << this->hour << ":" << this->minute << ":" << this->second << "\n";
    }

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
    date production_date;

    void getInfo() //Функція для отримання данних про товар від користувача
    {
        this->name = getValidInput<string>("Enter name: ");
        do { this->unit = getValidInput<string>("Enter unit: "); } while (this->unit != "kg" && this->unit != "liter" && this->unit != "package" && this->unit != "piece");
        do { this->quantity = getValidInput<double>("Enter quantity: "); } while (this->quantity < 0);
        production_date.getDate();
        do { this->shelf_life = getValidInput<int>("Enter shelf life(in days. Days must be less than 3660): "); } while (this->shelf_life < 0 || this->shelf_life > 3660);
    }

    void printInfo() //Функція для виведення данних про товар для користувача
    {
        cout << "=====================\n";
        cout << "Id: " << this->id << "\n";
        cout << "Product: " << this->name << "\n";
        cout << "Unit: " << this->unit << "\n";
        cout << "Quantity: " << this->quantity << "\n";
        production_date.printDate();
        cout << "Shelf time: " << this->shelf_life << "\n";
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
void DatabaseManagement(const string& filename);
void addElement(const string& filename, vector<Product>& bufferDatabase);
void recordDatabase(const string& filename, const vector<Product>& bufferDatabase);
void restore(const string& filename, vector<Product>& database);
void readAllDatabase(const string& filename);
void searchElements(const string& filename);
void modifyElement(const string& filename, vector<Product>& bufferDatabase);
void delElement(const string& filename, vector<Product>& bufferDatabase);

int main()
{
    cout << "--- Database loaded successfully! ---\n";
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

                    break;
                case 2:
                    filename = "database.txt";
                    DatabaseManagement(filename);
                    break;
                case 3:
                    filename = "database.bin";
                    DatabaseManagement(filename);
                    break;
                default:
                    cerr << "Error! You enter incoretct database\n";
                    break;
            }
            break;
        default:
            cerr << "Error! You enter incoretct mode\n";
            break;
    }
}

void DatabaseManagement(const string& filename)
{
    fstream outputFile(filename, ios::app);

    if (!outputFile.is_open())
    {
        cerr << "\n### Error opening the file " << filename << ". ###\n";
        return;
    }

    bool manage = true;
    vector<Product> bufferDatabase; //Масив для збереження тимчасових елементів

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
                addElement(filename, bufferDatabase);
                break;
            case 2: 
                recordDatabase(filename, bufferDatabase);
                break;
            case 3:
                restore(filename, bufferDatabase);
                break;
            case 4:
                readAllDatabase(filename);
                break;
            case 5:
                searchElements(filename);
                break;
            case 6:
                modifyElement(filename, bufferDatabase);
                break;
            case 7:
                delElement(filename, bufferDatabase);
                break;
            default:
                cerr << "Error! You enter incoretct mode\n";
                break;
        }
    }
    
    cout << "\n=== Text database file '" << filename << "' has been created/updated. ===\n";
    outputFile.close();
}

void addElement(const string& filename, vector<Product>& bufferDatabase)
{
    restore(filename, bufferDatabase);
    Product newProduct;
    bufferDatabase.empty() ? newProduct.id = 1 : newProduct.id = bufferDatabase[bufferDatabase.size()-1].id + 1;
    newProduct.getInfo();
    bufferDatabase.push_back(newProduct);
    cout << "\n=== Product added to buffer successfully. ===\n";
}

void recordDatabase(const string& filename, const vector<Product>& bufferDatabase)
{
    ofstream clearFile(filename, ios::trunc);
    clearFile.close();
    ofstream outputFile(filename, ios::in);

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

    cout << "\n=== Database has been recorded to the file '" << filename << "'. ===\n";
    outputFile.close();
}

void readAllDatabase(const string& filename)
{
    ifstream readFile(filename);

    if (!readFile.is_open())
    {
        cerr << "\n### Error opening the file " << filename << "###\n";
        return;
    }
    cout << "\nAll product in database: \n";
    Product product;
    while (readFile >> product.id >> product.name >> product.unit >> product.quantity
        >> product.production_date.year >> product.production_date.month >> product.production_date.day
        >> product.production_date.hour >> product.production_date.minute >> product.production_date.second
        >> product.shelf_life)
    {
        product.printInfo();
    }

    cout << "\n=== Database has been read from the file " << filename << ". ===\n";
    readFile.close();
}

void restore(const string& filename, vector<Product>& bufferDatabase)
{
    ifstream readFile(filename);

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

    cout << "\n=== Database has been restored from the file " << filename << ". ===\n";
    readFile.close();
}

void searchElements(const string& filename) {
    vector<Product> allCoincidence;
    string fragment;
    double diaposoneStart, diaposoneEnd;
    int criterion;
    date startDate;;
    bool name = false, numOfUnit = false, date = false;

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
            cout << "Enter a range units:\n";
            do { diaposoneStart = getValidInput<double>("Enter the smallest value: "); } while (diaposoneStart < 0);
            do { diaposoneEnd = getValidInput<double>("Enter the greatest value: "); } while (diaposoneStart < 0);
            numOfUnit = true;
            break;
        case 3:
            cout << "Enter start date (YYYY MM DD HH MM SS): ";
            startDate.getDate();
            date = true;
            break;
        default:
            cerr << "\n### You entered a wrong number ###\n";
            break;
        }
    } while (criterion != 0);

    if (!name && !numOfUnit && !date) {
        cout << "\n### You didn't select any criterion ###\n";
        return;
    }

    ifstream readFile(filename);

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
        if ((name && product.name.find(fragment) != 0) || 
            (numOfUnit && (product.quantity > diaposoneEnd || 
             product.quantity < diaposoneStart)) || (date && startDate > product.production_date))
            continue;  
        allCoincidence.push_back(product);
    }
    
    if (allCoincidence.empty()) cout << "\n### Elemetnts didn`t founded! ###\n";

    for (auto& p : allCoincidence)
    {
        p.printInfo();
    }
}

void modifyElement(const string& filename, vector<Product>& bufferDatabase)
{
    int idModify;
    idModify = getValidInput<int>("Enter id of the element which you want to modify: ");
    ifstream readFile(filename, ios::out);
    if (!readFile.is_open())
    {
        cerr << "\n### Error opening the file " << filename << "###\n";
        return;
    }
    bufferDatabase.clear();
    Product product;
    while (readFile >> product.id >> product.name >> product.unit >> product.quantity
        >> product.production_date.year >> product.production_date.month >> product.production_date.day
        >> product.production_date.hour >> product.production_date.minute >> product.production_date.second
        >> product.shelf_life)
    {
        if (product.id == idModify)
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
                        product.name = getValidInput<string>("Enter a new name: ");
                        cout << "\n=== Name changed succesfully! ===\n";
                        break;
                    case 2:
                        product.unit = getValidInput<string>("Enter a new unit: ");
                        cout << "\n=== Unit changed succesfully! ===\n";
                        break;
                    case 3:
                        do { product.quantity = getValidInput<double>("Enter a new quantity: "); } while (product.quantity < 0);
                        cout << "\n=== Quantity changed succesfully! ===\n";
                        break;
                    case 4:
                        cout << "Enter a new production date: ";
                        product.production_date.getDate();
                        cout << "\n=== Production date changed succesfully! ===\n";
                        break;
                    case 5:
                        do { product.shelf_life = getValidInput<int>("Enter a new shelf life(in days. Days must be less than 3660): "); } while (product.shelf_life < 0 || product.shelf_life > 3660);
                        cout << "\n=== Shelf life changed succesfully! ===\n";
                        break;
                    default:
                        cout << "\n### You enter wrong number! ###\n";
                        break;
                }
            } while (operation != 0);

        }
        bufferDatabase.push_back(product);
    }
    readFile.close();
}

void delElement(const string& filename, vector<Product>& bufferDatabase)
{
    int operation;
    operation = getValidInput<int>("\nSelect operation for delete:\n0. Exit\n1. Delete element by id\n2. Delete all data in database\nEnter operation: ");
    if (operation <= 0 || operation > 2) return;

    if (operation == 1)
    {
        int delId;
        delId = getValidInput<int>("Enter a id file which you want to delete: ");
        bool deleted;
        ifstream readFile(filename, ios::out);
        if (!readFile.is_open())
        {
            cerr << "\n### Error opening the file " << filename << "###\n";
            return;
        }
        bufferDatabase.clear();
        Product product;
        while (readFile >> product.id >> product.name >> product.unit >> product.quantity
            >> product.production_date.year >> product.production_date.month >> product.production_date.day
            >> product.production_date.hour >> product.production_date.minute >> product.production_date.second
            >> product.shelf_life)
        {
            if (product.id == delId)
            {
                deleted = true;
                continue;
            }
            bufferDatabase.push_back(product);
        }
        readFile.close();

        if (deleted) cout << "\n=== Element with id " << delId << " was deleted! ===\n";
        else  cout << "\n### Element with id " << delId << " wasn`t deleted! ###\n";

        return;
    }
    else 
    {
        bufferDatabase.clear();
        cout << "\n=== Data was deleted succesfully! ===\n";
    }
};
