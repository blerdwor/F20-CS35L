#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

  // check for argument errors
  if (argc == 3) {
    int sizeFrom = strlen(argv[1]);
    int sizeTo = strlen(argv[2]);
    if (sizeFrom != sizeTo) {
      fprintf(stderr, "from and to are not the same length.\n");
      exit(1);
    }

    for (int i = 0; i < sizeFrom; i++) {
      for (int j = i+1; j < sizeFrom; j++) {
        if (argv[1][i] == argv[1][j]) {
          fprintf(stderr, "from contains duplicate errors.\n");
          exit(1);
        }
      }
    }
  }
  else {
    fprintf(stderr, "wrong number of arguments.\n");
    exit(1);
  }

  // print out the words
  int read = getchar();
  while (read != EOF) {
    for (int i = 0; i < strlen(argv[1]); i++) {
      if (read == argv[1][i]) {
        read=argv[2][i];
        break;
      }
    }
    putchar(read);
    read = getchar();
  }

  return 0;
}
