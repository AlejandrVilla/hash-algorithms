#pragma once

#include <stdint.h>
#include <stdio.h>
#include <memory.h>

typedef struct 
{
    uint64_t length;
    uint32_t state[8];
    uint32_t curlen;
    uint8_t buf[64];
}   Sha256Context;

#define SHA256_HASH_SIZE (256 / 8)

typedef struct 
{
    uint8_t bytes[SHA256_HASH_SIZE];
}   SHA256_HASH;

void Sha256Initialise(Sha256Context* Context);

void Sha256Update(Sha256Context* Context, void const* Buffer, uint32_t BufferSize);

void Sha256Finalise(Sha256Context* Context, SHA256_HASH* Digest);

void Sha256Calculate(void const* Buffer, uint32_t BufferSize, SHA256_HASH* Digest);

#define ror(value, bits) (((value) >> (bits)) | ((value) << (32 - (bits))))

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define STORE32H(x, y)                     \
  {                                        \
    (y)[0] = (uint8_t)(((x) >> 24) & 255); \
    (y)[1] = (uint8_t)(((x) >> 16) & 255); \
    (y)[2] = (uint8_t)(((x) >> 8) & 255);  \
    (y)[3] = (uint8_t)((x)&255);           \
  }

#define LOAD32H(x, y)                                                         \
  {                                                                           \
    x = ((uint32_t)((y)[0] & 255) << 24) | ((uint32_t)((y)[1] & 255) << 16) | \
        ((uint32_t)((y)[2] & 255) << 8) | ((uint32_t)((y)[3] & 255));         \
  }

#define STORE64H(x, y)                     \
  {                                        \
    (y)[0] = (uint8_t)(((x) >> 56) & 255); \
    (y)[1] = (uint8_t)(((x) >> 48) & 255); \
    (y)[2] = (uint8_t)(((x) >> 40) & 255); \
    (y)[3] = (uint8_t)(((x) >> 32) & 255); \
    (y)[4] = (uint8_t)(((x) >> 24) & 255); \
    (y)[5] = (uint8_t)(((x) >> 16) & 255); \
    (y)[6] = (uint8_t)(((x) >> 8) & 255);  \
    (y)[7] = (uint8_t)((x)&255);           \
  }

static const uint32_t K[64] = 
{
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
    0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
    0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
    0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
    0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
    0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
    0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
    0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
    0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
    0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define BLOCK_SIZE 64

#define Ch(x, y, z) (z ^ (x & (y ^ z)))
#define Maj(x, y, z) (((x | y) & z) | (x & y))
#define S(x, n) ror((x), (n))
#define R(x, n) (((x)&0xFFFFFFFFUL) >> (n))
#define Sigma0(x) (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x) (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0(x) (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x) (S(x, 17) ^ S(x, 19) ^ R(x, 10))

#define Sha256Round(a, b, c, d, e, f, g, h, i)    \
  t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i]; \
  t1 = Sigma0(a) + Maj(a, b, c);                  \
  d += t0;                                        \
  h = t0 + t1;

static void TransformFunction(Sha256Context* Context, uint8_t const* Buffer) 
{
    uint32_t S[8];
    uint32_t W[64];
    uint32_t t0;
    uint32_t t1;
    uint32_t t;
    int i;

    // Copy state into S
    for (i = 0; i < 8; i++) 
    {
        S[i] = Context->state[i];
    }

    // Copy the state into 512-bits into W[0..15]
    for (i = 0; i < 16; i++) 
    {
        LOAD32H(W[i], Buffer + (4 * i));
    }

    // Fill W[16..63]
    for (i = 16; i < 64; i++) 
    {
        W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];
    }

    // Compress
    for (i = 0; i < 64; i++) 
    {
        Sha256Round(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], i);
        t = S[7];
        S[7] = S[6];
        S[6] = S[5];
        S[5] = S[4];
        S[4] = S[3];
        S[3] = S[2];
        S[2] = S[1];
        S[1] = S[0];
        S[0] = t;
    }

    // Feedback
    for (i = 0; i < 8; i++) 
    {
        Context->state[i] = Context->state[i] + S[i];
    }
}

void Sha256Initialise(Sha256Context* Context) 
{
    Context->curlen = 0;
    Context->length = 0;
    Context->state[0] = 0x6A09E667UL;
    Context->state[1] = 0xBB67AE85UL;
    Context->state[2] = 0x3C6EF372UL;
    Context->state[3] = 0xA54FF53AUL;
    Context->state[4] = 0x510E527FUL;
    Context->state[5] = 0x9B05688CUL;
    Context->state[6] = 0x1F83D9ABUL;
    Context->state[7] = 0x5BE0CD19UL;
}

void Sha256Update(Sha256Context* Context, void const* Buffer, uint32_t BufferSize) 
{
    uint32_t n;

    if (Context->curlen > sizeof(Context->buf)) 
    {
       return;
    }

    while (BufferSize > 0) 
    {
        if (Context->curlen == 0 && BufferSize >= BLOCK_SIZE) 
        {
            TransformFunction(Context, (uint8_t*)Buffer);
            Context->length += BLOCK_SIZE * 8;
            Buffer = (uint8_t*)Buffer + BLOCK_SIZE;
            BufferSize -= BLOCK_SIZE;
        } 
        else 
        {
            n = MIN(BufferSize, (BLOCK_SIZE - Context->curlen));
            memcpy(Context->buf + Context->curlen, Buffer, (size_t)n);
            Context->curlen += n;
            Buffer = (uint8_t*)Buffer + n;
            BufferSize -= n;
            if (Context->curlen == BLOCK_SIZE) 
            {
                TransformFunction(Context, Context->buf);
                Context->length += 8 * BLOCK_SIZE;
                Context->curlen = 0;
            }
        }
    }
}

void Sha256Finalise(Sha256Context* Context, SHA256_HASH* Digest) 
{
    int i;

    if (Context->curlen >= sizeof(Context->buf)) 
    {
        return;
    }

    Context->length += Context->curlen * 8;

    Context->buf[Context->curlen++] = (uint8_t)0x80;

    if (Context->curlen > 56) 
    {
        while (Context->curlen < 64) 
        {
            Context->buf[Context->curlen++] = (uint8_t)0;
        }
        TransformFunction(Context, Context->buf);
        Context->curlen = 0;
    }

    while (Context->curlen < 56) 
    {
        Context->buf[Context->curlen++] = (uint8_t)0;
    }

    STORE64H(Context->length, Context->buf + 56);
    TransformFunction(Context, Context->buf);

    for (i = 0; i < 8; i++) 
    {
        STORE32H(Context->state[i], Digest->bytes + (4 * i));
    }
}

void Sha256Calculate(void const* Buffer, uint32_t BufferSize, SHA256_HASH* Digest) 
{
    Sha256Context context;

    Sha256Initialise(&context);
    Sha256Update(&context, Buffer, BufferSize);
    Sha256Finalise(&context, Digest);
}