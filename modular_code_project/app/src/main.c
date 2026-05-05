#include "file.h"
#include <minwindef.h>
int main(void) {
  wchar_t path[MAX_PATH];
  wchar_t dirpath[MAX_PATH];
  if (get_path(path, dirpath) != 0) {
    return -1;
  }
  if (create_directory(dirpath) != 0) {
    return -1;
  }
  if (write_file(path) != 0) {
    return -1;
  }
  if (read_file(path)) {
    return -1;
  }
  return 0;
}
