/* Hex Dump
 * --------
 *
 * Simply print a 'pretty' hex dump of a specified file to stdout. */


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

  // logic for dynamic padding depending on size of file (no ugly padding in hex dump)
  size_t fsize = file_size(file_in) / 16;
  int units_padding = 0;
  while(fsize) {
    fsize /= 10; // decrement until 0 to count units (1000, 100, 10, 1)
    ++units_padding; // printf padding to correspond to times div/ units in file size
  } // this will repeat until div as much as possible

  // check file actually has contents
  if(!file_size(file_in)) {
    fprintf(stderr, "File has no contents\n");
    return 1;
  }

  // hex dump print and formatting of hex/ lines
  size_t j = 0;
  for(size_t i = 0; i < file_size(file_in); i++) {
    if(i == 16 * j) {
      // ("%*li", x) '*' <- syntax allows var param to set format specifier
      // really cool, in this example dynamically pad for number of units in file size
      if(!i) printf("%*sRW\tFile: %s\tSize: %lib\n%*li | ",
		    units_padding - 1,
		    " ",
		    argv[1],
		    file_size(file_in),
		    units_padding + 1, j + 1); // 1st line of file
      else printf("\n%*li | ", units_padding + 1, j + 1); // proceeding lines
      j++;
    }
    printf("%#04hhX ", file_array[i]); // pretty hex pad 5 0s
  }
  printf("\n");

  // clean up
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
