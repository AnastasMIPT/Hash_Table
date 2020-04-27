#include <iostream>
#include "cache_list.h"
#include <string>
#include <functional>
#include <cstring>
#include <vector>
#include <memory>

const unsigned long long Max_val = 18446744073709551615U;

typedef unsigned long long hash_t;
typedef std::function<hash_t (const char*)> hash_func_t;

class HashTable {
    static hash_t default_hash (const char* string);
    size_t size;
    hash_func_t hash;
public:
    std::unique_ptr<list<const char*>[]> table;

    explicit HashTable (size_t size, const hash_func_t &hash_func = HashTable::default_hash) :
    size (size), table (new list<const char*>[size]), hash (hash_func) {}

    void SizesOfListsInTable (FILE* f_out);

    void LoadTambleFromVec (std::vector<char*> &vec);

    void insert (const char* str);
};

void cleaning_text (FILE* f_in, unsigned long long FileSize);

hash_t hash_len_of_word (const char* str);

hash_t hash_cycle (const char* str);

hash_t hash_const_one (const char* str);

hash_t hash_sum_ascii_mod_len (const char* str);

unsigned long long shr (unsigned long long val, unsigned int step);

unsigned long long int SizeOfFile (const char* NameOfFile);

void GetWords (std::vector<char*>& words, char* buf);


int main () {
    FILE* f_in = fopen ("input.txt", "r");
    FILE* f_out = fopen ("output.csv", "w");

    std::vector<char*> words;
    unsigned long long FileSize = SizeOfFile ("input.txt");
    std::unique_ptr<char> buf (new char[FileSize]);
    fread (buf.get(), sizeof (char), FileSize, f_in);
    GetWords (words, buf.get ());

    HashTable hash_table1 (1009, hash_const_one);
    HashTable hash_table2 (1009, hash_len_of_word);
    HashTable hash_table3 (1009, hash_sum_ascii_mod_len);
    HashTable hash_table4 (1009, hash_cycle);
    //HashTable hash_table5 (1009, hash_const_one);

    hash_table1.LoadTambleFromVec (words);
    hash_table2.LoadTambleFromVec (words);
    hash_table3.LoadTambleFromVec (words);
    hash_table4.LoadTambleFromVec (words);

    hash_table1.SizesOfListsInTable (f_out);
    hash_table2.SizesOfListsInTable (f_out);
    hash_table3.SizesOfListsInTable (f_out);
    hash_table4.SizesOfListsInTable (f_out);

    fclose (f_in);
    return 0;
}


void GetWords (std::vector<char*>& words, char* buf) {
    char* buf_p = buf;
    char* temp_str = buf;
    while (*buf_p != '\0') {
        if (*buf_p == ' ') {
            *buf_p = '\0';
            words.push_back (temp_str);
            temp_str = buf_p + 1;
        }
        ++buf_p;
    }
}

void HashTable::SizesOfListsInTable (FILE* f_out) {

    for (int i = 0; i < size; ++i) {
        fprintf (f_out, "%ld,", table[i].list_size ());
    }
    fprintf (f_out, "\n");
}


void HashTable::LoadTambleFromVec (std::vector<char*> &vec) {
    for (auto c : vec) {
        insert (c);
    }
}

void HashTable::insert (const char* str) {
    unsigned long long index = (hash (str)) % size;
    for (auto c : table[index]) {
        if (strcmp (str, c) == 0) {
            return;
        }
    }
    table[index].PushBack (str);
}

hash_t HashTable::default_hash (const char* string) {
    return 1;
}

hash_t hash_const_one (const char* str) {
    return 1;
}

hash_t hash_len_of_word (const char* str) {
    return strlen (str);
}

hash_t hash_sum_ascii_mod_len (const char* str) {
    hash_t hash = 0;
    const char* s_ptr = str;
    unsigned int col = 1;
    while (*s_ptr != '\0') {
        hash += *s_ptr;
        ++s_ptr;
        ++col;
    }
    return hash / col;
}

hash_t hash_cycle (const char* str) {
    hash_t hash = 0U;
    const char* s_ptr = str;

    while (*s_ptr != '\0') {
        hash = hash ^ *s_ptr;
        shr (hash, 1);
        ++s_ptr;
    }

    return hash;
}

unsigned long long shr (unsigned long long val, unsigned int step) {
    val = (val >> step) | ((val << (64 - step)) & Max_val);
    return val;
}

unsigned long long int SizeOfFile (const char* NameOfFile) {
    assert (NameOfFile != nullptr);

    FILE* f = fopen (NameOfFile, "r");
    fseek (f, 0, SEEK_END);
    unsigned long long FileSize = ftell (f);
    fseek (f, 0, SEEK_SET);

    fclose (f);
    return FileSize;
}

void cleaning_text (FILE* f_in, unsigned long long FileSize) {
    char* buf = new char[FileSize];
    char* new_buf = new char[FileSize];
    fread (buf, sizeof (char), FileSize, f_in);
    unsigned long long col = FileSize;
    char* new_buf_ptr = new_buf;
    char* buff = buf;
    while (col > 1) {
        if (*buf == ' ' || isalpha (*buf)) {
            *new_buf_ptr = *buf;
            ++new_buf_ptr;
        }
        ++buf;
        --col;
    }

    FILE* f_out = fopen ("output.txt", "w");
    fwrite (new_buf, sizeof (char), FileSize, f_out);
    delete[] (buff);
    delete[] (new_buf);
    fclose (f_out);
}