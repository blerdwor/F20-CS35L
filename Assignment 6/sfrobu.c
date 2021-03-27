#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>

/* returns an int result that is negative, zero, or positive depending 
 * on whether a is less than, equal to, or greater than b.*/
int frobcmp(int const *a, int const *b, int ignore_case) {
  while (*a!=' ' && *b!=' ')
  {
    int deobfus_a = *a ^ 42, deobfus_b = *b ^ 42;
    if (deobfus_a != deobfus_b) {
      // -f was enabled
      if (ignore_case == 1) {
        // if the two characters are letters
        if (isalpha(deobfus_a) && isalpha(deobfus_b)) {
          if (toupper(deobfus_a) != toupper(deobfus_b))
            return toupper(deobfus_a) < toupper(deobfus_a) ? -1: 1;
        }
        else
          return deobfus_a < deobfus_b ? -1: 1;
      }
      else // -f was not enabled
        return deobfus_a < deobfus_b ? -1: 1;
    }
    a++; b++;
  }
  
  if (*a==' ' && *b==' ')
    return 0;
  else if (*a==' ')
    return -1;
  return 1;
}

/*compare function if -f was specified*/
int cmp_with_f(const void * a, const void * b) {
   return frobcmp(*(int const**) a,*(int const**) b, 1);
}

/*compare function it -f was not specified*/
int cmp_without_f(const void * a, const void * b) {
   return frobcmp(*(int const**) a,*(int const**) b, 0);
}

int main(int argc, char *argv[]) {
  
  int fFlag = 0; 
  // error checking for no -f flag
  if (argc == 2) { // error checking for -f flag
    if (argv[1][0] != '-' && argv[1][1] != 'f') {
      fprintf(stderr, "option was not formatted correctly.\n");
      exit(1);
    }
    fFlag = 1;
  }
  else if (argc != 1) {
    fprintf(stderr, "additional arguments provided.\n");
    exit(1);
  }
 
  // create buffer
  struct stat fileData;
  size_t bufferSize = 0;
  char* buffer;

  if (fstat(0, &fileData) == -1) {
    fprintf(stderr, "fstat error");
    exit(1);
  }

  if (S_ISREG(fileData.st_mode))
    bufferSize = fileData.st_size;
  else
    bufferSize = 8192;
  
  buffer = (char *)malloc(sizeof(char) * bufferSize); 
   
  // read input to buffer
  char ch;
  int count = 0;
  size_t input = read(0,&ch,1);

  while (input != 0) {
    if (count > bufferSize/2) {
      bufferSize *= 2;
      buffer = (char *)realloc(buffer, sizeof(char) * bufferSize);
    }
    if (input < 0) {
      fprintf(stderr, "read error");
      exit(1);
    }
    buffer[count] = ch;
    count++;
    input = read(0,&ch,1);
  }
  buffer[count] = ' ';
  buffer[count+1] = '\0';

  // read input from buffer part
  int **word_arr;
  int word_len = 0, word_count = 0;
   
  // cast to double pointer, total size of 1 pointer (8 bytes)
  word_arr = (int **)malloc(sizeof(int *));
  
  // check if memory was not allocated successfully
  if (word_arr == NULL) {
    fprintf(stderr, "Memory not allocated.\n");
    exit(1);
  }
  
  // initialize first pointer to NULL
  word_arr[0] = NULL;

  // read in words
  for (int i = 0; buffer[i] != '\0'; i++) {
   
    // space char = end of word, allocate memory for new row
    if (buffer[i] == ' ') {
      word_arr[word_count] = (int *)realloc(word_arr[word_count], sizeof(int)*(word_len+1));
      word_arr[word_count][word_len] = ' ';
  
      // if the input had a space at the end then break
      if (buffer[i+1] == '\0')
        break;
      else {
        word_len = 0;
        word_count += 1;
        int **tmp = (int **)realloc(word_arr, sizeof(int *)*(word_count+1));
        if (tmp) {
          word_arr = tmp;
          word_arr[word_count] = (int *)malloc(sizeof(int *));
        }
        else {
          fprintf(stderr, "Memory not allocated. \n");
          exit(1);
        }
      }
    }
    else { // add char to current word, allocates memory for new col
      word_len += 1;
      word_arr[word_count] = (int *)realloc(word_arr[word_count], sizeof(int)*word_len);
      word_arr[word_count][word_len-1] = buffer[i];
    
      // if the input doesn't have a space at the end
      if (buffer[i+1] == '\0') {
        word_arr[word_count] = (int *)realloc(word_arr[word_count], sizeof(int)*(word_len+1));
        word_arr[word_count][word_len] = ' ';
        break;
      }
    }
  }
  
  // sort the words
  if (fFlag == 1)
    qsort(word_arr, word_count+1, sizeof(int *), cmp_with_f);
  else
    qsort(word_arr, word_count+1, sizeof(int *), cmp_without_f); 

  // print out array of words
  for (int i = 0; i <= word_count; i++) {
    for (int j = 0; word_arr[i][j] != ' '; j++)
       write(1,&word_arr[i][j],1);
    write(1," ",1);
  }

  // free memory
  for (int i = 0; i <= word_count; i++)
    free(word_arr[i]);
  free(word_arr);
  
  free(buffer);

  return 0;
}  
