// Used for pffft lib, have no idea why it didnt work without it.


#include <malloc.h>

extern "C" {
void* pffft_aligned_malloc(size_t nb_bytes)
{
#ifdef _WIN32
  return _aligned_malloc(nb_bytes, 16);
#else
  void* p;
  if (posix_memalign(&p, 16, nb_bytes))
    return 0;
  return p;
#endif
}

void pffft_aligned_free(void* p)
{
#ifdef _WIN32
  _aligned_free(p);
#else
  free(p);
#endif
}
}