#pragma once
#ifndef amoeba_json_cjson_parson__h
#define amoeba_json_cjson_parson__h


#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <parson.h>


extern JSON_Value *convert_cjson_to_parson(cJSON *src);
extern cJSON *convert_parson_to_cjson(JSON_Value *src);


#endif
