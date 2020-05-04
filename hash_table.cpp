#include <iostream>
#include "hash_table.h"

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
    for (auto el : words) {
        hash_table1.find (el);
    }

    fclose (f_in);
    return 0;
}
