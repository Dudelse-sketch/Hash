#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

// ====================== KMP ======================
vector<int> computeLPS(const string& pat) {
    int m = pat.length();
    vector<int> lps(m, 0);
    int len = 0, i = 1;
    while (i < m) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            len = (len != 0) ? lps[len - 1] : 0;
            if (len == 0) i++;
        }
    }
    return lps;
}

vector<int> KMPSearch(const string& text, const string& pat) {
    vector<int> positions;
    if (pat.empty()) return positions;

    string txt = text;
    string pattern = pat;

    for (char& c : txt) c = tolower(static_cast<unsigned char>(c));
    for (char& c : pattern) c = tolower(static_cast<unsigned char>(c));

    int n = txt.length();
    int m = pattern.length();
    if (m == 0) return positions;

    vector<int> lps = computeLPS(pattern);
    int i = 0, j = 0;

    while (i < n) {
        if (pattern[j] == txt[i]) {
            i++;
            j++;
        }
        if (j == m) {
            positions.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pattern[j] != txt[i]) {
            j = (j != 0) ? lps[j - 1] : 0;
            if (j == 0) i++;
        }
    }
    return positions;
}

// ====================== Hash Table ======================
struct Node {
    string word;
    vector<int> positions;
    Node* next;
    Node(string w, vector<int> pos) : word(std::move(w)), positions(std::move(pos)), next(nullptr) {}
};

class HashTable {
private:
    vector<Node*> table;
    int capacity;

    int hashFunction(const string& str) {
        unsigned long hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + tolower(static_cast<unsigned char>(c));
        }
        return hash % capacity;
    }

public:
    HashTable(int cap = 256) : capacity(cap), table(cap, nullptr) {}

    void insert(const string& word, vector<int> positions) {
        if (word.empty() || positions.empty()) return;
        
        int index = hashFunction(word);
        Node* newNode = new Node(word, std::move(positions));

        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            Node* curr = table[index];
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
    }

    vector<int> search(const string& word) {
        if (word.empty()) return {};
        int index = hashFunction(word);
        Node* curr = table[index];

        while (curr) {
            if (curr->word == word) {
                return curr->positions;
            }
            curr = curr->next;
        }
        return {};
    }

    \~HashTable() {
        for (auto& head : table) {
            Node* curr = head;
            while (curr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }
};

// ====================== cleanWord ======================
string cleanWord(string s) {
    string result;
    for (char c : s) {
        if (isalnum(static_cast<unsigned char>(c)) || c == '\'' || c == '-') {
            result += tolower(static_cast<unsigned char>(c));
        }
    }
    return result;
}

int main() {
    HashTable ht(256);

    ifstream textFile("text.txt");
    if (!textFile.is_open()) {
        cout << "Ошибка открытия text.txt\n";
        return 1;
    }
    string text, line;
    while (getline(textFile, line)) {
        text += line + " ";
    }
    textFile.close();

    ifstream wordsFile("words.txt");
    if (!wordsFile.is_open()) {
        cout << "Ошибка открытия words.txt\n";
        return 1;
    }
    string word;
    while (getline(wordsFile, word)) {
        string cleaned = cleanWord(word);
        if (!cleaned.empty()) {
            vector<int> pos = KMPSearch(text, cleaned);
            if (!pos.empty()) {
                ht.insert(cleaned, pos);
            }
        }
    }
    wordsFile.close();

    cout << "Хеш-таблица готова!\n";
    cout << "Введите слово (пустая строка = выход):\n";

    string query;
    while (getline(cin, query)) {
        if (query.empty()) break;
        string cleaned = cleanWord(query);
        if (cleaned.empty()) {
            cout << -1 << endl;
            continue;
        }

        vector<int> pos = ht.search(cleaned);
        if (pos.empty()) {
            cout << -1 << endl;
        } else {
            for (size_t i = 0; i < pos.size(); ++i) {
                cout << pos[i];
                if (i + 1 < pos.size()) cout << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
