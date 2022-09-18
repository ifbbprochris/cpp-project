#include <iostream>

#define MP_PAGE_SIZE 4096

struct mp_node_t {

  unsigned char *last;
  unsigned char *end;

  struct mp_node_t *next;
};

struct mp_large_t {

  struct mp_large_t *next;
  void *alloc;

};

struct mp_pool_t {

  size_t maxx;

  struct mp_large_t *large;
  struct mp_node_t *small;
};

mp_pool_t *mp_init(size_t size) {
  
}

// hook
void mp_malloc(mp_pool_t *pool, size_t size) {

  
}

void mp_free(void *ptr) {

}

int main() {

}
