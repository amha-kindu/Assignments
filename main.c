#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <json-c/json.h>


#include "TLV/tlv_box.h"
#include "hashtable.h"


int main(int argc, char **argv){
  // Initialize required variables
  FILE *file_stream, *output_stream;

  struct json_object *parsed_json;
  enum json_type type;

  char * line = NULL;
  size_t len = 0, counter=1;
  
  // A hashtable to store the key-value pair of the json objects
  hashtable* key_hashtable = hashtable_create();

  // Open the json file stream, and a binary file to store the tlv encoding binary stream
  file_stream = fopen("test.json", "r");
  output_stream = fopen("binary_tlv_format.bin", "wb");
    
  if (file_stream == NULL)
      exit(EXIT_FAILURE);

  // Streaming each record from the json file
  while (getline(&line, &len, file_stream) != -1) {
    tlv_box_t *val_box = tlv_box_create();   

    parsed_json = json_tokener_parse(line);
    
		json_object_object_foreach(parsed_json, key, val) {

      void* value = hashtable_get(key_hashtable, key);
      if (value == NULL) {
        int* new_num = malloc(sizeof(int));
        *new_num = counter;

        hashtable_set(key_hashtable, key, new_num);
        value = new_num;
        counter++;
      }
      
			type = json_object_get_type(val);
      int* tag = (int*) value;
      
			switch (type) {
  			case json_type_int: 
          tlv_box_put_int(val_box, *tag, (int)json_object_get_int(val));
  				break;
  			case json_type_boolean: 
          tlv_box_put_short(val_box, *tag, (short)json_object_get_boolean(val));
  				break;
  			case json_type_string: 
          tlv_box_put_string(val_box, *tag, (char*)json_object_get_string(val)); 
  				break;
        default:
          printf("unknown data type!");
          break;
      }
    }
    if (tlv_box_serialize(val_box) != 0) {
        printf("boxes serialize failed !\n"); 
        return -1;
    }

    tlv_box_t *val_parsedBox = tlv_box_parse(tlv_box_get_buffer(val_box), tlv_box_get_size(val_box));
    unsigned char* val_binary = tlv_box_get_buffer(val_parsedBox);
    
    fwrite(val_binary, 1, tlv_box_get_size(val_parsedBox), output_stream);

    tlv_box_destroy(val_box);
    tlv_box_destroy(val_parsedBox);
  }
  if (line)
      free(line);

  
  hashtable_destroy(key_hashtable);
  fclose(file_stream);
  fclose(output_stream);

  exit(EXIT_SUCCESS);
}