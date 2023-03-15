
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define _defer(i)                                                              \
  {                                                                            \
    rtn = 1;                                                                   \
    goto defer;                                                                \
  }

int main(int argc, const char **argv) {
  int rtn = 0;

  // 2 files and the binary
  if (argc != 3)
    return 1;

  FILE *record = NULL;
  FILE *sample = NULL;

  record = fopen(argv[1], "r");
  if (record == NULL)
    _defer(-1);

  sample = fopen(argv[2], "r");
  if (sample == NULL)
    _defer(-1);

  int rec, sam;
  while ((rec = fgetc(record)) != EOF) {
    if ((sam = fgetc(sample)) == EOF || rec != sam) {
      _defer(-1);
    }
  }

defer:
  if (record)
    fclose(record);
  if (sample)
    fclose(sample);

  return rtn;
}
