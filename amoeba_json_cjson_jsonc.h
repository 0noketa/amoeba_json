#pragma once
#ifndef amoeba_json_cjson_jsonc__h
#define amoeba_json_cjson_jsonc__h


#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <json-c/json.h>


extern json_object *convert_cjson_to_jsonc(cJSON *src);
extern cJSON *convert_jsonc_to_cjson(json_object *src);


#endif
