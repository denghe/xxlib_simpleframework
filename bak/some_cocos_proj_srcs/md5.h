//:! THIS FILE IS SHRIMPS ADDED

#if !defined(WW_MD5_H)
#define WW_MD5_H

#ifdef __cplusplus
extern "C" {
#endif

/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int WW_MD5_u32plus;

typedef struct {
	WW_MD5_u32plus lo, hi;
	WW_MD5_u32plus a, b, c, d;
	unsigned char buffer[64];
	WW_MD5_u32plus block[16];
} WW_MD5_CTX;

void WW_MD5_Init(WW_MD5_CTX *ctx);
void WW_MD5_Update(WW_MD5_CTX *ctx, void *data, unsigned long size);
void WW_MD5_Final(unsigned char *result, WW_MD5_CTX *ctx);

#ifdef __cplusplus
}
#endif
    
#endif
