#include <iostream>
#include "hash_table.h"
#include <ctime>

const int NumTimes = 1000;
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
    hash_table1.SizesOfListsInTable (f_out);
    
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
    

    std::cout << double (clock () - time) / CLOCKS_PER_SEC << std::endl;
    fclose (f_in);
    return 0;
}
