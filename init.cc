#include <nan.h>

NAN_METHOD(vips) {
  printf("foobar!");
}

NAN_MODULE_INIT(init) {
  NAN_EXPORT(target, vips);
}

NODE_MODULE(utils, init)
