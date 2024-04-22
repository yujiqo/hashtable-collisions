#include <iostream>
#include <string>
#include <vector>

using namespace std;


template<typename T>
class HTEntry {
public:
    string key;
    T val;
    HTEntry<T>* next = nullptr;

    HTEntry(string key, T val) {
        this->key = key;
        this->val = val;
    }
};

template<typename T>
class HashTable {
private:
    int capacity;
    vector<HTEntry<T>*> table;

    int hash(string key) {
        unsigned int hash = 0;

        for (char c : key)
            hash += static_cast<int>(c);

        return hash % this->capacity;
    }
public:
    HashTable(int capacity) {
        this->capacity = capacity;
        table.resize(capacity);
    }

    ~HashTable() {
        for (auto hte : this->table)
            delete hte;

        delete[] this->table;
    }

    T* get(string key) {
        int index = hash(key);

        if (!this->table[index])
            return nullptr;

        if (this->table[index]->key != key) {
            HTEntry<T>* current = this->table[index]->next;

            while (current) {
                if (current->key == key)
                    return &current->val;

                current = current->next;
            }

            return nullptr;
        }

        return &this->table[index]->val;
    }

    void insert(string key, T val) {
        int index = hash(key);

        HTEntry<T>* entry = new HTEntry<T>(key, val);

        if (this->table[index]) {
            HTEntry<T>* current = this->table[index];

            while (current) {
                if (!current->next) {
                    current->next = entry;

                    break;
                }

                current = current->next;
            }

            return;
        }

        this->table[index] = entry;
    }

    void remove(string key) {
        int index = hash(key);

        if (!this->table[index]) {
            cout << "There is no element by " << key << endl;

            return;
        }

        if (this->table[index]->key != key) {
            HTEntry<T>* previous = this->table[index];
            HTEntry<T>* current = this->table[index]->next;

            while (current) {
                if (current->key == key) {
                    previous->next = current->next ? current->next : nullptr;
                    delete current;

                    return;
                }

                current = current->next;
            }

            cout << "There is no element by " << key << endl;

            return;
        }

        HTEntry<T>* tmp = this->table[index]->next;
        delete this->table[index];
        if (tmp)
            this->table[index] = tmp;
    }
};


int main() {
    HashTable<int>* hashtable = new HashTable<int>(5);

    hashtable->insert("one", 1);
    hashtable->insert("neo", 2);
    hashtable->insert("eno", 3);

    cout << *hashtable->get("one") << endl;
    cout << *hashtable->get("neo") << endl;
    cout << *hashtable->get("eno") << endl;

    hashtable->remove("neo");
    hashtable->remove("one");
    hashtable->remove("eno");
}
