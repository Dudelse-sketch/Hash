#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

class HashTable {
private:
    int count[26] = {0};   // Массив для подсчёта букв a-z

    // Вспомогательная функция: возвращает индекс буквы
    int getIndex(char ch) const {
        if (isalpha(ch)) {
            return tolower(ch) - 'a';
        }
        return -1;
    }

public:
    // 1. Добавление буквы
    void add(char ch) {
        int idx = getIndex(ch);
        if (idx != -1) {
            count[idx]++;
        }
    }

    // 2. Удаление буквы (уменьшение количества)
    void remove(char ch) {
        int idx = getIndex(ch);
        if (idx != -1 && count[idx] > 0) {
            count[idx]--;
        }
    }

    // 3. Поиск количества вхождений буквы
    int search(char ch) const {
        int idx = getIndex(ch);
        if (idx != -1) {
            return count[idx];
        }
        return 0;
    }

    // Вывод хеш-таблицы в файл
    void print(ostream& out) const {
        for (int i = 0; i < 26; i++) {
            if (count[i] > 0) {
                out « (char)('a' + i) « " " « count[i] « endl;
            }
        }
    }
};

int main() {
    HashTable ht;

    // Читаем данные из входного файла input.txt
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        cout « "Ошибка: не удалось открыть input.txt" « endl;
        return 1;
    }

    char ch;
    while (fin.get(ch)) {      // Читаем файл посимвольно
        ht.add(ch);
    }
    fin.close();

    // Записываем результат в выходной файл output.txt
    ofstream fout("output.txt");
    if (!fout.is_open()) {
        cout « "Ошибка: не удалось открыть output.txt" « endl;
        return 1;
    }
    ht.print(fout);
    fout.close();

    // Поиск буквы, введённой с клавиатуры
    cout « "Введите букву для поиска: ";
    char letter;
    cin » letter;

    int result = ht.search(letter);
    cout « result « endl;

    return 0;
}
