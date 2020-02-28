#ifndef amoeba_json_cjson_lua__h
#define amoeba_json_cjson_lua__h


#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <lua.h>


/*
** returns 1 as nresults
*/
int convert_cjson_to_lua(lua_State *L, cJSON *src);

/*
** takes index
**/
cJSON *convert_lua_to_cjson(lua_State *L, int src);


#endif
