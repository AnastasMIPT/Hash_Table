#include <iostream>
#include "cache_list.h"
#include <string>
#include <functional>
#include <cstring>
#include <vector>
#include <memory>

const unsigned long long Max_val = 18446744073709551615U;

typedef unsigned int hash_t;
typedef std::function<hash_t (const unsigned char*, unsigned long)> hash_func_t;



template <typename Tp>
class HashTable {
    static hash_t default_hash (const unsigned char* string, unsigned long len);
    size_t size;
    hash_func_t hash;
public:
    std::unique_ptr<list<std::pair<const char*, Tp >>[]> table;

    explicit HashTable (size_t size, const hash_func_t &hash_func = HashTable::default_hash) :
    size (size), table (new list<std::pair<const char*, Tp >> [size]), hash (hash_func) {}

    void SizesOfListsInTable (FILE* f_out);

    void LoadTambleFromVec (std::vector<char*> &vec);

    void insert (const char* str, Tp value);
};

void cleaning_text (FILE* f_in, unsigned long long FileSize);

hash_t hash_len_of_word (const unsigned char* str, unsigned long len);

hash_t hash_cycle (const unsigned char* str, unsigned long len);

hash_t hash_const_one (const unsigned char* str, unsigned long len);

hash_t hash_sum_ascii_mod_len (const unsigned char* str, unsigned long len);

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

     HashTable <char*> hash_table1 (1009);
    // HashTable hash_table2 (1009, hash_len_of_word);
    // HashTable hash_table3 (1009, hash_sum_ascii_mod_len);
    // HashTable hash_table4 (1009, hash_cycle);
    //HashTable hash_table5 (1009, hash_const_one);

    hash_table1.LoadTambleFromVec (words);
    // hash_table2.LoadTambleFromVec (words);
    // hash_table3.LoadTambleFromVec (words);
    // hash_table4.LoadTambleFromVec (words);

    hash_table1.SizesOfListsInTable (f_out);
    // hash_table2.SizesOfListsInTable (f_out);
    // hash_table3.SizesOfListsInTable (f_out);
    // hash_table4.SizesOfListsInTable (f_out);

    printf ("Hello!\n");
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

template <typename Tp>
void HashTable<Tp>::SizesOfListsInTable (FILE* f_out) {

    for (int i = 0; i < size; ++i) {
        fprintf (f_out, "%ld,", table[i].list_size ());
    }
    fprintf (f_out, "\n");
}

template <typename Tp>
void HashTable<Tp>::LoadTambleFromVec (std::vector<char*> &vec) {
    for (auto c : vec) {
        insert (c, c);
    }
}

template <typename Tp>
void HashTable<Tp>::insert (const char* str, Tp value) {
    unsigned long long index = (hash ((unsigned char*) str, strlen (str)) % size);
    for (auto c : table[index]) {
        if (strcmp (str, c.first) == 0) {
            return;
        }
    }
    table[index].PushBack ({str, value});
}

// hash_t HashTable::default_hash (const char* string) {
//     return 1;
// }

hash_t hash_const_one (const unsigned char* str, unsigned long len) {
    return 1;
}

hash_t hash_len_of_word (const unsigned char* str, unsigned long len) {
    return len;
}

hash_t hash_sum_ascii_mod_len (const unsigned char* str, unsigned long len) {
    hash_t hash = 0;
    const unsigned char* s_ptr = str;
    unsigned int col = 1;
    while (*s_ptr != '\0') {
        hash += *s_ptr;
        ++s_ptr;
        ++col;
    }
    return hash / col;
}

hash_t hash_cycle (const unsigned char* str, unsigned long len) {
    hash_t hash = 0U;
    const unsigned char* s_ptr = str;

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

template <typename Tp>
hash_t HashTable<Tp>::default_hash (const unsigned char* buf, unsigned long len)
{
	unsigned long crc_table[256];
	unsigned long crc;
	for (int i = 0; i < 256; i++)
	{
		crc = i;
		for (int j = 0; j < 8; j++) {
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
        }
		crc_table[i] = crc;
	}
	crc = 0xFFFFFFFFUL;
	while (len--) {
        crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
    }
	return crc ^ 0xFFFFFFFFUL;
}
