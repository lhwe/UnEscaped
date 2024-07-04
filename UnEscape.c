// worlds worst developer here <3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "1.0"

void decode_hex_string(const char *input, const char *output_filename) {
    char *output = (char *)malloc(strlen(input) + 1);
    
    int len = strlen(input);
    int output_index = 0;
    int i = 0;
    
    while (i < len) {
        if (input[i] == '\\' && input[i + 1] == 'x') {
            int value = 0;
            sscanf(input + i + 2, "%2hhx", &value);
            output[output_index++] = (char)value;
            i += 4;
        } else {
            output[output_index++] = input[i++];
        }
    }
    
    output[output_index] = '\0';
    FILE *outfile = fopen(output_filename, "w");
    if (outfile == NULL) {
        perror("Error opening output file");
        free(output);
        return;
    }
    
    fprintf(outfile, "[Decoded with UnEscape V%s]\n\n", VERSION);
    fprintf(outfile, "%s\n", output);
    fclose(outfile);
    printf("Decoded string saved to %s\n", output_filename);
    free(output);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = (char *)malloc(file_size + 1);

    if (input == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }

    size_t read_size = fread(input, 1, file_size, file);
    if (read_size != file_size) {
        perror("Error reading file");
        fclose(file);
        free(input);
        return 1;
    }

    fclose(file);
    input[file_size] = '\0';
    char output_filename[1000];
    snprintf(output_filename, sizeof(output_filename), "%s-Unescaped.txt", argv[1]);
    decode_hex_string(input, output_filename);
    free(input);
    return 0;
}
