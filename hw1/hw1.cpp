#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

// Извлекаем ФИО из строки
string getName(const string& line) {
    int first = line.find('"');
    int second = line.find('"', first + 1);
    if (first != string::npos && second != string::npos)
        return line.substr(first + 1, second - first - 1);
    else
        return "";
}

// Извлекаем телефон из строки
string getPhone(const string& line) {
    int pos = line.find('+');
    if (pos != string::npos)
        return line.substr(pos);
    else
        return "";
}

// Линейный поиск
bool linearSearch(string fileName, string target, string& foundPhone) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        string name = getName(line);
        if (name == target) {
            foundPhone = getPhone(line);
            return true;
        }
    }

    return false;
}

// Бинарный поиск 
bool binarySearch(string fileName, string target, string& foundPhone) {
    ifstream file(fileName, ios::binary);
    if (!file.is_open()) {
        cout << "Ошибка" << endl;
        return false;
    }

    file.seekg(0, ios::end);
    long long fileSize = file.tellg();
    long long left = 0, right = fileSize - 1;

    while (left <= right) {
        long long mid = (left + right) / 2;
        file.seekg(mid);

        string skip;
        getline(file, skip);
        long long pos = file.tellg();

        if (pos >= fileSize) break;

        string line;
        getline(file, line);
        if (line.empty()) break;

        string name = getName(line);

        if (name == target) {
            foundPhone = getPhone(line);
            return true;
        }
        else if (name < target) {
            left = file.tellg();
        }
        else {
            right = mid - 1;
        }
    }

    return false;
}

// Генерируем случайное имя 
string randomName(string first, string last) {
    string firstNames[] = {
        "Adam", "Brian", "Charles", "David", "Edward", "Frank", "George", "Henry",
        "Ivan", "John", "Kevin", "Louis", "Michael", "Nicholas", "Oliver", "Peter",
        "Robert", "Steven", "Thomas", "Victor", "William", "Zachary",
        "Alice", "Barbara", "Clara", "Diana", "Emma", "Fiona", "Grace", "Helen"
    };

    string lastNames[] = {
        "Adams", "Brown", "Clark", "Davis", "Evans", "Fisher", "Green", "Harris",
        "Irwin", "Johnson", "King", "Lee", "Miller", "Nelson", "Owens", "Parker",
        "Quinn", "Roberts", "Smith", "Taylor", "Urban", "Vance", "White", "Young"
    };

    string middleNames[] = {
        "Alexander", "Benjamin", "Christopher", "Daniel", "Edward", "Francis",
        "Gabriel", "Henry", "Isaac", "James", "Kenneth", "Lawrence", "Matthew",
        "Nathaniel", "Oscar", "Patrick", "Richard", "Samuel", "Theodore", "Vincent"
    };

    char minLetter = toupper(first[0]);
    char maxLetter = toupper(last[0]);

    string lastName;
    do {
        lastName = lastNames[rand() % (sizeof(lastNames) / sizeof(lastNames[0]))];
    } while (toupper(lastName[0]) < minLetter || toupper(lastName[0]) > maxLetter);

    string firstName = firstNames[rand() % (sizeof(firstNames) / sizeof(firstNames[0]))];
    string middleName = middleNames[rand() % (sizeof(middleNames) / sizeof(middleNames[0]))];

    return lastName + " " + firstName + " " + middleName;
}

// Структура для статистики
struct BenchmarkStats {
    long long minTime = LLONG_MAX;
    long long maxTime = 0;
    long long totalTime = 0;
    int count = 0;

    void addTime(long long time) {
        minTime = min(minTime, time);
        maxTime = max(maxTime, time);
        totalTime += time;
        count++;
    }

    double getAverage() const {
        return count > 0 ? (double)totalTime / count : 0;
    }
};

// Бенчмарк
void runBenchmark(string fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    string firstLine, lastLine;
    getline(file, firstLine);
    string firstName = getName(firstLine);

    file.seekg(0, ios::end);
    long long size = file.tellg();
    long long pos = size - 1;
    file.seekg(pos);

    while (pos > 0) {
        file.seekg(--pos);
        char c;
        file.get(c);
        if (c == '\n') break;
    }

    getline(file, lastLine);
    string lastName = getName(lastLine);
    cout << "Первое ФИО: " << firstName << endl;
    cout << "Последнее ФИО: " << lastName << endl;

    const int N = 1000;
    vector<string> names;
    for (int i = 0; i < N; i++)
        names.push_back(randomName(firstName, lastName));

    BenchmarkStats linearStats, binaryStats;

    cout << "\nЗапуск бенчмарка..." << endl;

    for (int i = 0; i < N; i++) {
        string found;

        // Линейный поиск
        auto start = high_resolution_clock::now();
        linearSearch(fileName, names[i], found);
        auto end = high_resolution_clock::now();
        auto linearTime = duration_cast<microseconds>(end - start).count();
        linearStats.addTime(linearTime);

        // Бинарный поиск
        start = high_resolution_clock::now();
        binarySearch(fileName, names[i], found);
        end = high_resolution_clock::now();
        auto binaryTime = duration_cast<microseconds>(end - start).count();
        binaryStats.addTime(binaryTime);
    }

    cout << fixed << setprecision(2);
    cout << "\nРезультаты" << endl;
    cout << "Общее количество запросов: " << N << endl;

    cout << "\nЛИНЕЙНЫЙ ПОИСК:" << endl;
    cout << "Минимальное время: " << linearStats.minTime << " мкс" << endl;
    cout << "Максимальное время: " << linearStats.maxTime << " мкс" << endl;
    cout << "Среднее время: " << linearStats.getAverage() << " мкс" << endl;

    cout << "\nБИНАРНЫЙ ПОИСК:" << endl;
    cout << "Минимальное время: " << binaryStats.minTime << " мкс" << endl;
    cout << "Максимальное время: " << binaryStats.maxTime << " мкс" << endl;
    cout << "Среднее время: " << binaryStats.getAverage() << " мкс" << endl;
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(0));

    string fileName;
    cout << "Введите путь к файлу: ";
    getline(cin, fileName);

    int mode = 1;

    while (true) {
        cout << "\nТекущий режим: " << (mode == 1 ? "Линейный" : "Бинарный") << endl;
        cout << "1 - Сменить режим" << endl;
        cout << "2 - Найти ФИО" << endl;
        cout << "3 - Бенчмарк" << endl;
        cout << "4 - Выход" << endl;
        cout << "Выбор: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            mode = (mode == 1 ? 2 : 1);
            cout << "Режим изменен на: " << (mode == 1 ? "Линейный" : "Бинарный") << endl;
        }
        else if (choice == 2) {
            string target;
            cout << "Введите ФИО (например, Ivanov Ivan Ivanovich): ";
            getline(cin, target);

            string phone;

            // Измеряем время
            auto start = high_resolution_clock::now();
            bool found;
            if (mode == 1)
                found = linearSearch(fileName, target, phone);
            else
                found = binarySearch(fileName, target, phone);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);

            if (found)
                cout << "Найден номер: " << phone << endl;
            else
                cout << "ФИО не найдено." << endl;

            cout << "Время выполнения: " << duration.count() << " мкс" << endl;
        }
        else if (choice == 3) {
            runBenchmark(fileName);
        }
        else if (choice == 4) {
            cout << "Выход из программы." << endl;
            break;
        }
        else {
            cout << "Неверный ввод" << endl;
        }
    }

    return 0;
}