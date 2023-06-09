#ifndef MD5_H
#define MD5_H

/*------------ md5 prototypes -------------*/

typedef unsigned long word32;
typedef word32 md5_uint32;

struct MD5Context {
	md5_uint32 buf[4];
	md5_uint32 bits[2];
	unsigned char in[64];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, unsigned char const *buf,
	       unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);
void MD5Transform(md5_uint32 buf[4], const unsigned char in[64]);

typedef struct MD5Context MD5_CTX;

#endif /* !MD5_H */
