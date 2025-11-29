#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <locale> 

using namespace std;

// Извлекаем имя
string getFirstName(const string& fio) {
    size_t firstSpace = fio.find(' ');
    size_t secondSpace = fio.find(' ', firstSpace + 1);
    return fio.substr(firstSpace + 1, secondSpace - firstSpace - 1);
}

// Извлекаем фамилию
string getLastName(const string& fio) {
    size_t spacePos = fio.find(' ');
    return fio.substr(0, spacePos);
}

int main() {
    setlocale(LC_ALL, "");
    vector<string> list;
    bool dataEntered = false;

    while (true) {
        cout << "\nОБРАБОТКА ФИО\n";

        if (!dataEntered) {
            int N;
            cout << "Здравствуйте, Александр Алексеевич!\n";
            cout << "Введите количество записей: ";
            cin >> N;
            cin.ignore();

            list.clear();
            list.reserve(N);

            cout << "Введите ФИО:\n";
            for (int i = 0; i < N; i++) {
                string fio;
                cout << i + 1 << ": ";
                getline(cin, fio);
                list.push_back(fio);
            }
            dataEntered = true;
            cout << "Супер, спасибо!\n";
        }

        cout << "\nМеню операций:\n";
        cout << "1 - Вывести все ФИО, отсортированные по фамилии\n";
        cout << "2 - Найти ФИО по первой букве имени\n";
        cout << "3 - Определить количество уникальных фамилий\n";
        cout << "4 - Найти самую длинную запись\n";
        cout << "5 - Ввести новые данные\n";
        cout << "0 - Выйти из программы\n";
        cout << "Выберите вариант: ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "Выход из программы. До свидания, Александр Алексеевич!\n";
            break;
        }
        else if (choice == 1) {
            vector<string> sortedList = list;
            sort(sortedList.begin(), sortedList.end(), [](const string& a, const string& b) {
                return getLastName(a) < getLastName(b);
                });

            cout << "\nФИО, ОТСОРТИРОВАННЫЕ ПО ФАМИЛИИ\n";
            for (const string& fio : sortedList) {
                cout << fio << endl;
            }
        }
        else if (choice == 2) {
            char letter;
            cout << "Введите первую букву имени: ";
            cin >> letter;
            cin.ignore();

            letter = toupper(letter);

            cout << "\nРЕЗУЛЬТАТЫ ПОИСКА ПО БУКВЕ '" << letter << "'\n";

            bool found = false;
            for (const string& fio : list) {
                string name = getFirstName(fio);
                if (!name.empty() && toupper(name[0]) == letter) {
                    cout << fio << endl;
                    found = true;
                }
            }
            if (!found) {
                cout << "Записей с такой первой буквой имени не найдено.\n";
            }
        }
        else if (choice == 3) {
            vector<string> sortedList = list;
            sort(sortedList.begin(), sortedList.end(), [](const string& a, const string& b) {
                return getLastName(a) < getLastName(b);
                });

            int uniqueCount = 0;
            string lastLastName = "";
            for (const string& fio : sortedList) {
                string currentLastName = getLastName(fio);
                if (currentLastName != lastLastName) {
                    uniqueCount++;
                    lastLastName = currentLastName;
                }
            }

            cout << "\nСТАТИСТИКА ФАМИЛИЙ\n";
            cout << "Количество уникальных фамилий: " << uniqueCount << endl;
        }
        else if (choice == 4) {
            string longest = list[0];
            for (const string& fio : list) {
                if (fio.length() > longest.length()) {
                    longest = fio;
                }
            }

            cout << "\nСАМАЯ ДЛИННАЯ ЗАПИСЬ\n";
            cout << longest << endl;
            cout << "Длина: " << longest.length() << " символов\n";
        }
        else if (choice == 5) {
            dataEntered = false;
            cout << "Подготовка к вводу новых данных.\n";
            continue;
        }
        else {
            cout << "Некорректный вариант выбора. Попробуйте снова.\n";
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "\nНажмите Enter, чтобы продолжить.";
        cin.get();
    }

    return 0;
}