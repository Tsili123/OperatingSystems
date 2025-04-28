#include "hash.h"

unsigned int PJWHash(const void *_str) {
  unsigned int BitsInUnignedInt = (unsigned int) (sizeof(unsigned int) * 8);
  unsigned int ThreeQuarters = (unsigned int) ((BitsInUnignedInt * 3) / 4);
  unsigned int OneEighth = (unsigned int) (BitsInUnignedInt / 8);
  unsigned int HighBits =
    (unsigned int) (0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
  unsigned int hash = 0;
  unsigned int test = 0;
  char c;
  const char *str = _str;
  
  while ((c = *str++)) {
    hash = (hash << OneEighth) + c;
    if ((test = hash & HighBits) != 0) {
      hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }
  return (hash & 0x7FFFFFFF);
}

uint32_t murmur3_32(const void *_str)
{
	uint32_t h = seed;
	const char *key = _str;
	size_t	len=strlen(_str);
	
	if (len > 3) {
		size_t i = len >> 2;
		do {
			uint32_t k;
			memcpy(&k, key, sizeof(uint32_t));
			key += sizeof(uint32_t);
			k *= 0xcc9e2d51;
			k = (k << 15) | (k >> 17);
			k *= 0x1b873593;
			h ^= k;
			h = (h << 13) | (h >> 19);
			h = h * 5 + 0xe6546b64;
		} while (--i);
	}
	if (len & 3) {
		size_t i = len & 3;
		uint32_t k = 0;
		do {
			k <<= 8;
			k |= key[i - 1];
		} while (--i);
		k *= 0xcc9e2d51;
		k = (k << 15) | (k >> 17);
		k *= 0x1b873593;
		h ^= k;
	}
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

unsigned int sdbm(const void *_str) {
  unsigned int hash = 0;
  const char *str = _str;
  char c;
  while ((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;
  return hash;
}

int main()
{
	unsigned int k;
//	srand(time(0)); 
//	seed=rand();
//	k=murmur3_32("kappaaaadfweagftwqrgqwrtg");
	k=sdbm(12356);
//	k=PJWHash("katioliiiiiiiiiiiiiiiii");
	//k=abs(k);
	printf("%ld\n",k);
}

