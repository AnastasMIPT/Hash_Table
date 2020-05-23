#include <iostream>
#include "hash_table.h"
#include <ctime>
#include <nmmintrin.h>

extern "C" int my_strcmp (const char* str1, const char* str2);

#define strcmp my_strcmp
const int NumTimes = 200;

hash_t CRC_32_fast (const unsigned char* string, unsigned long len);
void print_time_of_search (const HashTable<char*>& table, const std::vector<char*>& words, const char* massege);


int main () {
    
    FILE* f_in = fopen ("input.txt", "r");
    FILE* f_out = fopen ("output.csv", "w");

    std::vector<char*> words;
    unsigned long long FileSize = SizeOfFile ("input.txt");
    std::unique_ptr<char> buf (new char[FileSize]);
    fread (buf.get(), sizeof (char), FileSize, f_in);
    GetWords (words, buf.get ());

    HashTable <char*> hash_table1 (1009);
    hash_table1.LoadTambleFromVec (words);
    
    HashTable <char*> hash_table_sse (1009, CRC_32_fast);
    hash_table_sse.LoadTambleFromVec (words);

    HashTable <char*> hash_table_2 (1009, hash_cycle);
    hash_table_2.LoadTambleFromVec (words);

    HashTable <char*> hash_table_2_a (1009, hash_cycle_asm);
    hash_table_2_a.LoadTambleFromVec (words);
    

    fclose (f_in);


    printf ("%zu\n", words.size());
    
//    print_time_of_search (hash_table_2, words, "cycle_hash: ");
//    print_time_of_search (hash_table_2_a, words, "cycle_hash_asm: ");
    print_time_of_search (hash_table1, words, "CRC32 without SSE: ");
    print_time_of_search (hash_table_sse, words, "CRC32 with SSE: ");
  
    return 0;
}
void print_time_of_search (const HashTable<char*>& table, const std::vector<char*>& words, const char* massege) {
    clock_t time = clock();
    for (int i = 0; i < NumTimes; ++i) {
        for (auto el : words) {
            table.find (el);
        }
    }
    std::cout << massege << double (clock () - time) / CLOCKS_PER_SEC << std::endl;
    
}

hash_t CRC_32_fast (const unsigned char* string, unsigned long len) {
    hash_t hash = 0;
    if (len == 1) return _mm_crc32_u8  (hash, *string);
    if (len == 2) return _mm_crc32_u16 (hash, *(unsigned short*) string);
    unsigned int i = 0;
    while (len >= 4)
    {
        hash = _mm_crc32_u32 (hash, *(unsigned int*) (string + i));
        len -= 4;
        i += 4;
    }
    
    if (len >= 2) {
        hash = _mm_crc32_u16 (hash, *(unsigned short*) (string + i));
        len -= 2;
        i += 2;
    }

    if (len == 1) return _mm_crc32_u8  (hash, *string);

    return hash;
}

