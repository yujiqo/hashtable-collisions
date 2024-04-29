#include <iostream>
#include <string>
#include <vector>

using namespace std;


template<typename T>
class HTEntry {
public:
    string key;
    T val;

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

    int hash1(string key) {
        int hash = 0;

        for (char c : key)
            hash += static_cast<int>(c);

        return hash % this->capacity;
    }

    int hash2(string key) {
        int hash = 0;

        for (int i = 0; i < key.length(); i++)
            hash += static_cast<int>(key[i]) * (i + 1);

        return hash;
    }

    bool is_full() {
        bool flag = true;

        for (auto hte : this->table) {
            if (!hte) {
                flag = false;
                break;
            }
        }

        return flag;
    }
public:
    HashTable(int capacity) {
        this->capacity = capacity;
        this->table = vector<HTEntry<T>*>(capacity, nullptr);
    }

    ~HashTable() {
        for (auto hte : this->table)
            delete hte;

        delete[] this->table;
    }

    void lpinsert(string key, T val) {
        int index = hash1(key);

        HTEntry<T>* entry = new HTEntry<T>(key, val);

        if (this->table[index] && this->table[index]->key != key) {
            while(this->table[index]) {
                if (index >= this->capacity - 1) {
                    index = 0;
                    continue;
                }

                index++;
            }
        }

        this->table[index] = entry;
    }

    T* lpget(string key) {
        int index = hash1(key);

        if (!this->table[index])
            return nullptr;

        if (this->table[index]->key == key)
            return &this->table[index]->val;

        int tmp = index;
        index++;
        bool flag = false;
        while (true) {
            if (this->table[index]) {
                if (this->table[index]->key == key)
                    break;

                if (tmp == index && flag == true)
                    return nullptr;

                if (index >= this->capacity - 1) {
                    flag = true;
                    index = 0;
                    continue;
                }
            }

            index++;
        }

        return &this->table[index]->val;
    }

    void lpremove(string key) {
        int index = hash1(key);

        while (this->table[index]->key != key) {
            if (index + 1 >= this->capacity) {
                index = 0;
                continue;
            }

            index++;
        }

        HTEntry<T>* tmp = this->table[index];
        this->table[index] = nullptr;
        delete tmp;

        while (true) {
            if (index + 1 < this->capacity) {
                if (!this->table[index + 1]
                        || hash1(this->table[index + 1]->key) != hash1(key))
                    break;
            } else {
                if (!this->table[0]
                        || hash1(this->table[0]->key) != hash1(key))
                    break;

                this->table[index] = this->table[0];
                index = 0;
                continue;
            }

            this->table[index] = this->table[index + 1];
            index++;
        }

        if (index < this->capacity)
            this->table[index] = nullptr;
        else
            this->table[0] = nullptr;
    }

    void dhinsert(string key, T val) {
        int index = hash1(key);

        HTEntry<T>* entry = new HTEntry<T>(key, val);

        int offset;
        int i = 0;
        while(this->table[index] && i < 1000) {
            offset = hash2(this->table[index]->key);
            index = (index + offset) % this->capacity;
            i++;
        }

        if (i >= 1000) {
            cout << "We could not insert element!" << endl;
            return;
        }

        this->table[index] = entry;
    }

    T* dhget(string key) {
        int index = hash1(key);

        if (!this->table[index])
            return nullptr;

        int offset;
        while (this->table[index] && this->table[index]->key != key) {
            offset = hash2(this->table[index]->key);
            index = (index + offset) % this->capacity;
        }

        return &this->table[index]->val;
    }

    void dhremove(string key) {
        int index = hash1(key);

        int offset;
        while (this->table[index]->key != key) {
            offset = hash2(this->table[index]->key);
            index = (index + offset) % this->capacity;
        }

        HTEntry<T>* tmp = this->table[index];
        this->table[index] = nullptr;
        delete tmp;

        while (true) {
            int next_index = (index + offset) % this->capacity;

            if (!this->table[next_index]) {
                break;
            } else if (hash1(this->table[next_index]->key) != hash1(key)) {
                break;
            }

            HTEntry<T>* tmp = this->table[next_index];
            pair<string, T> copy = {tmp->key, tmp->val};
            this->table[next_index] = nullptr;
            this->dhinsert(copy.first, copy.second);
            delete tmp;

            offset = hash2(copy.first);
            index = next_index;
        }
    }

    void display() {
        cout << "[";
        for (int i = 0; i < this->capacity - 1; i++) {
            if (this->table[i])
                cout << "(" << this->table[i]->key << " ," << this->table[i]->val << "), ";
            else
                cout << "nullptr, ";
        }
        if (this->table[this->capacity - 1])
            cout << "(" << this->table[this->capacity - 1]->key << " ," << table[this->capacity - 1]->val << ")";
        else
            cout << "nullptr";
        cout << "]" << endl;
    }
};


int main() {
    HashTable<int>* hashtable = new HashTable<int>(10);

    cout << "-----------Linear Probing-----------" << endl;

    hashtable->lpinsert("one", 1);
    hashtable->lpinsert("neo", 2);
    hashtable->lpinsert("eno", 3);
    hashtable->lpinsert("eon", 4);

    cout << *hashtable->lpget("one") << endl;
    cout << *hashtable->lpget("neo") << endl;
    cout << *hashtable->lpget("eno") << endl;
    cout << *hashtable->lpget("eon") << endl;

    hashtable->display();

    hashtable->lpremove("eno");
    hashtable->display();
    hashtable->lpremove("one");
    hashtable->display();
    hashtable->lpremove("eon");
    hashtable->display();
    hashtable->lpremove("neo");
    hashtable->display();

    cout << endl << "-----------Double Hashing-----------" << endl;

    hashtable->dhinsert("one", 1);
    hashtable->dhinsert("neo", 2);
    hashtable->dhinsert("eno", 3);
    hashtable->dhinsert("eon", 4);

    cout << *hashtable->dhget("one") << endl;
    cout << *hashtable->dhget("neo") << endl;
    cout << *hashtable->dhget("eno") << endl;
    cout << *hashtable->dhget("eon") << endl;

    hashtable->display();

    hashtable->dhremove("one");
    hashtable->display();
    hashtable->dhremove("eno");
    hashtable->display();
    hashtable->dhremove("eon");
    hashtable->display();
    hashtable->dhremove("neo");
    hashtable->display();
}
