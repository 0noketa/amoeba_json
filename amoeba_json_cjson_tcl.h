#pragma once
#ifndef amoeba_json_cjson_tcll__h
#define amoeba_json_cjson_tcll__h


#include <stdlib.h>
#include <string.h>
#include <cJSON/cJSON.h>
#include <tcl.h>


#ifndef AMOEBA_JSON_REC_DEPTH_LIM
#define AMOEBA_JSON_REC_DEPTH_LIM 256
#endif


extern cJSON *convert_tcl_to_cjson(Tcl_Interp *interp, Tcl_Obj *src);
extern Tcl_Obj *convert_cjson_to_tcl(Tcl_Interp *interp, cJSON *src);



#endif
