#pragma once
#ifndef amoeba_json_cjson_jansson__h
#define amoeba_json_cjson_jansson__h


#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <jansson.h>


extern json_t *convert_cjson_to_jansson(cJSON *src);
extern cJSON *convert_jansson_to_cjson(json_t *src);


#endif
