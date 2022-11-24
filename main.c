/* Hex Dump
 * --------
 *
 * Simply print a 'pretty' hex dump of a specified file to stdout.
 *
 * Yeah this could be prettier but it gets the job done... */


#include <stdio.h>
#include <stdlib.h>


// all file related funcs will require user to close file handle
char* file_contents(FILE*); // return calloced str; assumes caller will free
size_t file_size(FILE*);


int main(int argc, char** argv)
{
  if(argc != 2) {
    fprintf(stderr, "Arg count invalid (%i)\n", argc);
    return 1;
  }

  FILE* file_in;
  if(!(file_in = fopen(argv[1], "r"))) { // file handle NULL return 1
    fprintf(stderr, "File cannot be opened \"%s\"\n", argv[1]);
    return 1;
  }

  file_in = fopen(argv[1], "rb");
  char* file_array = file_contents(file_in);

  // check file actually has contents
  if(!file_size(file_in)) {
    fprintf(stderr, "File has no contents\n");
    return 1;
  }

  // hex dump print and formatting of hex/ lines
  size_t j = 0;
  size_t k = 0;
  char readable[17] = {0};
  for(size_t i = 0; i < file_size(file_in); i++) {
    if(i == 16 * j) {
      if(!i) { // first row
	printf("%s", readable);
	printf("Offset     File: %s     Size: %lib\n%08lx | ",
	       argv[1],
	       file_size(file_in), i);
      }
      else { // other rows
	printf("| %s", readable);
	printf("\n%08lx | ", i);
      }
      k = 0; // reset counter for readable char array
      j++; // row offset multiplier
    }

    // append to readable char array for EOL print if a-z/ A-Z
    if(file_array[i] > 'A' && file_array[i] < 'Z' ||
       file_array[i] > 'a' && file_array[i] < 'z')
      readable[k] = file_array[i];
    else readable[k] = '.';
    ++k;

    printf("%02hhX ", file_array[i]); // avoid long 'F' padded hex
  }
  // (48 - k * 3) make final row match previous padding
  printf("%*s| %s\n", 48 - k * 3, "", readable);

  fclose(file_in);
  free(file_array);

  return 0;
}


char* file_contents(FILE* in_file)
{
  fseek(in_file, 0, SEEK_END);
  size_t file_len = ftell(in_file);
  rewind(in_file);

  char* farray = calloc(file_len, sizeof(char));

  for(size_t i = 0; i < file_len; i++) farray[i] = fgetc(in_file);
  farray[file_len] = '\0';

  return farray;
}


size_t file_size(FILE* in_file)
{
  fseek(in_file, 0, SEEK_END);
  size_t file_len = ftell(in_file);
  rewind(in_file);

  return file_len;
}
