#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <fstream>

#define IV_A	0x67452301
#define IV_B	0xefcdab89
#define IV_C	0x98badcfe
#define IV_D	0x10325476

#define NUM_ROUNDS 3
#define NUM_OPERATIONS 16
#define F(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define G(X, Y, Z) (((X) & (Y)) | ((X) & (Z)) | ((Y) & (Z)))
#define H(X, Y, Z) (((X) ^ (Y) ^ (Z)))
#define RLL32(X, C) (((X) << (C)) | ((X) >> ((32) - (C))))

#define K_0 0x00000000
#define K_1	0x5A827999
#define K_2	0x6ED9EBA1

#define BLOCKSIZE 512
#define DIGESTSIZE 128

std::string decToHex(uint32_t value);
uint32_t endiannessFix(uint32_t value);

struct md4Digest 
{
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
};

using namespace std;

int main(int argc, char *argv[])
{
    // leer el archivo hasta llenar 512 bits o terminar el archivo  
        // If 1: realizar hash
        // if 2: rellenar y realizar hash
    uint64_t filesize;          //64 bit por MD4
    struct stat results;        
    uint32_t numBlocks;
    bool paddingBlock = false;
    md4Digest digest = {IV_A,IV_B,IV_C,IV_D};

    uint32_t key [3] = {K_0, K_1, K_2};

    if (stat(argv[1], &results) == 0)
    {
        filesize = results.st_size * 8;
    }
    else 
    {
        cout << "Error al cargar el archivo";
    }

    // establece numBlocks al tamaño del archivo dividido por el tamaño del bloque
    numBlocks = filesize >> 9; 

    if ((filesize % 512) > 447)    // iterar por el bloque vacio
    {
        paddingBlock = true;        // existe un bloque de relleno
        numBlocks++;
    }

    ifstream fileToBeHashed (argv[1], ios::in | ios::binary);

    bool paddedBlock = false; 
    bool lastBlock = false;

    for (uint32_t i = 0; i < numBlocks+1; i++)
    { 
        char buffer[64] = {0}; 
        uint32_t messageBlock [16] = {}; // bloques de datos de mensajes que se utilizan en hash
        fileToBeHashed.seekg(i * 64);
        fileToBeHashed.read(buffer,64);
        if (!fileToBeHashed)
        {               // datos en el búfer, no en el bloque de mensajes
            buffer[fileToBeHashed.gcount()] = 0b10000000;
            if(!paddingBlock)
            {              // si no hay un bloque de relleno con todos 0
                for (uint16_t j = fileToBeHashed.gcount()+1; j < (512 - 64) / 8; j++)
                {
                    buffer[j] = 0;
                }
                lastBlock = true;
            }
            if(paddingBlock && !lastBlock)       // Si hay un bloque de relleno, primero debemos determinar si estamos en el bloque de relleno o no
            {
                if(paddedBlock)               
                {
                    for (uint16_t j = 0; j < (64); j++)
                    {
                        buffer[j] = 0;
                    }
                    lastBlock = true;
                }
                if(!paddedBlock)
                {
                    buffer[fileToBeHashed.gcount()] = 0b10000000; 
                    for (uint16_t j = fileToBeHashed.gcount()+1; j < 64; j++)
                    {
                        buffer[j] = 0;
                    }
                    paddedBlock = true;
                }
            }
        }
        for(int j = 0; j < 64; j++) 
        { 
            messageBlock[j/4] = (messageBlock[j/4] << 8) | uint8_t(buffer[j]);  
        }
        if(lastBlock)
        {
            messageBlock[15] = endiannessFix(uint32_t(((filesize) & 0xffffffff00000000) >> 32));
            messageBlock[14] = endiannessFix(uint32_t((filesize) & 0x00000000ffffffff));
        }

        md4Digest previousIter = digest; 

        for(int j = 0; j < NUM_ROUNDS; j++) 
        { 
            int messageOrder [16];
            int rollAmount [4];
            if (j == 0) 
            {
                int messageOrder_0 [16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15};
                int rollAmount_0 [4] = {3, 7, 11, 19};
                std::copy(messageOrder_0, messageOrder_0+16, messageOrder);
                std::copy(rollAmount_0, rollAmount_0+4, rollAmount);
            }
            if (j == 1) 
            {
                int messageOrder_1 [16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6 ,10, 14, 3, 7, 11, 15};
                int rollAmount_1 [4] = {3, 5, 9, 13};
                std::copy(messageOrder_1, messageOrder_1+16, messageOrder);
                std::copy(rollAmount_1, rollAmount_1+4, rollAmount);
            }
            if (j == 2) 
            {
                int messageOrder_2 [16] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
                int rollAmount_2 [4] = {3, 9, 11, 15};
                std::copy(messageOrder_2, messageOrder_2+16, messageOrder);
                std::copy(rollAmount_2, rollAmount_2+4, rollAmount);
            }
            for(int k = 0; k < NUM_OPERATIONS; k++) 
            {
                uint32_t temp_B = 0;
                if (j == 0)
                { 
                    temp_B = ( (uint32_t(digest.A) + uint32_t(uint32_t(F(uint32_t(digest.B), uint32_t(digest.C), uint32_t(digest.D))) + uint32_t(key[j]) + uint32_t(endiannessFix(messageBlock[messageOrder[k]])))));
                }
                if (j == 1) 
                {
                    temp_B = ( (digest.A + (G(digest.B, digest.C, digest.D) + key[j] + endiannessFix(messageBlock[messageOrder[k]]))));
                }
                if (j == 2) 
                {
                    temp_B = ( (digest.A + (H(digest.B, digest.C, digest.D) + key[j] + endiannessFix(messageBlock[messageOrder[k]]))));
                }
                temp_B = RLL32(temp_B, rollAmount[(k % 4)]);
                digest.A = digest.D;
                digest.D = digest.C;
                digest.C = digest.B;
                digest.B = temp_B;
            }
        }
        digest.A = previousIter.A + digest.A;
        digest.B = previousIter.B + digest.B;
        digest.C = previousIter.C + digest.C;
        digest.D = previousIter.D + digest.D;
    }
    fileToBeHashed.close();
    cout << decToHex(endiannessFix(digest.A)) << decToHex(endiannessFix(digest.B)) << decToHex(endiannessFix(digest.C)) << decToHex(endiannessFix(digest.D))<< endl;
    return 0;
}

std::string decToHex(uint32_t value)
{
    string hex = "";
    while(value > 0)
    {
        if ((value % 16) < 10)
        {
            hex += (value % 16) + 48;
        }
        if ((value % 16) > 9) 
        {
            hex += (value % 16) + 55;
        }
        value = value >> 4;
    }
    string endian_fix = "";
    for(uint8_t i = 1; i <= hex.length(); i++)
    {
        endian_fix += hex[hex.length() - i];
    }
    return endian_fix;
}

uint32_t endiannessFix(uint32_t value)
{
    return (((value & 0xff) << 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24));
}