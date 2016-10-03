#include <stdio.h>
#include <vector>

const long long max_w = 2000;

void w2vToJSON(char *binPath, char *jsonPath) {

  FILE *fi = fopen(binPath, "rb");
  // FILE *fWordIndex = fopen(wordIndexPath, "wb");
  FILE *fJson = fopen(jsonPath, "wb");
  
  long long words, dimensions;
  fscanf(fi, "%lld", &words);
  fscanf(fi, "%lld", &dimensions);
  fscanf(fi, "%*[ ]");
  fscanf(fi, "%*[\n]");

  // *numberOfDimensionsPerVector = dimensions;

  char word[max_w];
  char ch;
  float value;
  int b, a;

  // Start outer array.
  // fprintf(fJson, "%s\n,", "[");

  for (b = 0; b < words; b++) {
    if(feof(fi))
      break;

    word[0] = 0;
    fscanf(fi, "%[^ ]", word);
    fscanf(fi, "%c", &ch);

    // fprintf(fWordIndex, "%s:%d\n", word, b);
    fprintf(fJson, "{\"word\": \"%s\", \"vector\": [", word);
    // fprintf(fJson, "%s\n", "[");

    for (a = 0; a < dimensions; a++) {
      fread(&value, sizeof(float), 1, fi);
      if (a < dimensions - 1) {
        fprintf(fJson, "\"%f\",", value);
      }
      else {
        fprintf(fJson, "\"%f\"", value);
      }
    }

    fprintf(fJson, "%s\n", "]}");

    fscanf(fi, "%*[\n]");
  }

  // End outer array.
  // fprintf(fJson, "%s\n,", "]");

  fclose(fi);
  fclose(fJson);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: w2v-to-json <word2vec index path> <JSON path>\n");
    return -1;
  }

  char *binPath = argv[1];
  char *jsonPath = argv[2];

  w2vToJSON(binPath, jsonPath);  
}
