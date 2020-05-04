#include <iostream>
#include "hash_table.h"
#include <ctime>
#include <nmmintrin.h>

const int NumTimes = 100;

hash_t CRC_32_fast (const unsigned char* string, unsigned long len);

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
    hash_table1.LoadTambleFromVec (words);
    
    fclose (f_in);


    printf ("%zu\n", words.size());
    
    clock_t time = clock();
    //int col = 0;
    for (int i = 0; i < NumTimes; ++i) {
        for (auto el : words) {
            //printf ("%s ", el);
            //col++;
            //printf ("%d\n", col);
            hash_table1.find (el);
        }
    }
    

    std::cout << "Without SSE: " << double (clock () - time) / CLOCKS_PER_SEC << std::endl;
    
    clock_t time2 = clock();
    //int col = 0;
    for (int i = 0; i < NumTimes; ++i) {
        for (auto el : words) {
            //printf ("%s ", el);
            //col++;
            //printf ("%d\n", col);
            hash_table_sse.find (el);
        }
    }
    

    std::cout << "With SSE: " << double (clock () - time2) / CLOCKS_PER_SEC << std::endl;

    return 0;
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