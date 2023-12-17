#include "hmac_sha256.hpp"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) 
{
    std::string dir = argv[1];
    std::ifstream file(dir);
    std::string text, clave;
    std::getline(file, text);
    // std::cout<<text<<'\n';
    std::getline(file, clave);
    // std::cout<<clave<<'\n';

    char* str_data = &text[0];
    char* str_key = &clave[0];
    uint8_t out[SHA256_HASH_SIZE];
    char out_str[SHA256_HASH_SIZE * 2 + 1];
    unsigned i;

    hmac_sha256(str_key, strlen(str_key), str_data, strlen(str_data), &out, sizeof(out));

    memset(&out_str, 0, sizeof(out_str));
    for (i = 0; i < sizeof(out); i++) 
    {
      snprintf(&out_str[i*2], 3, "%02x", out[i]);
    }

    printf("%s\n", out_str);

    assert(strncmp(out_str, "4b393abced1c497f8048860ba1ede46a23f1ff5209b18e9c428bddfbb690aad8", SHA256_HASH_SIZE * 2) == 0);
    return 0;
}