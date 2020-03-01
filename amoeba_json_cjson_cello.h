#ifndef amoeba_json_cjson_cello__h
#define amoeba_json_cjson_cello__h


#include <cJSON/cJSON.h>
#include <Cello.h>


extern cJSON *convert_cello_to_cjson(var src);
extern var convert_cjson_to_cello(cJSON *src);


#endif
