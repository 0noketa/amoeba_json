#include "amoeba_json_cjson_lua.h"


/*
** returns 1 as nresults
*/
int convert_cjson_to_lua(lua_State *L, cJSON *src)
{
	if (L == NULL)
		return 0;
	if (src == NULL || cJSON_IsNull(src))
		lua_pushnil(L);
	if (cJSON_IsNumber(src))
		lua_pushnumber(L, src->valuedouble);
	if (cJSON_IsString(src))
		lua_pushstring(L, cJSON_GetStringValue(src));
	if (cJSON_IsBool(src))
		lua_pushboolean(L, cJSON_IsFalse(src) ? 0 : 1);

	if (cJSON_IsArray(src) || cJSON_IsObject(src))
	{
		int isobj = cJSON_IsObject(src) ? 1 : 0;

		int size = cJSON_GetArraySize(src);

		lua_createtable(L, size, 0);  // -- table
		int result = lua_gettop(L);

		int i = 1;
		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			if (isobj)
				lua_pushstring(L, item->string);
			else
				lua_pushinteger(L, i);
	
			convert_cjson_to_lua(L, item);
	
			lua_settable(L, result);  // key value --
		}
	}

	return 1;
}


/*
** returns -1 if table is record
*/
static int size_of_array(lua_State *L, int table)
{
	#if LUA_VERSION_NUM > 501
		lua_len(L, table);  // -- len
		int narr = lua_tointeger(L, -1);

		lua_pop(L, 1);
	#endif

	int i;
	lua_pushnil(L);

	for (i = 0; lua_next(L, table) != 0; ++i)  // 2: (k0 -- k1 v1), 0: (k0 --)
	{
		lua_pop(L, 1);


		if (
			#if LUA_VERSION_NUM > 501
				!lua_isinteger(L, -1)
			#else
				!lua_isnumber(L, -1)
			#endif
			|| lua_tointeger(L, -1) != i + 1)
		{
			lua_pop(L, 1);

			return -1;
		}
	}

	#if LUA_VERSION_NUM > 501
		return i == narr ? narr : -1;
	#else
		return i;
	#endif
}

/*
** takes index
**/
cJSON *convert_lua_to_cjson(lua_State *L, int src)
{
	if (L == NULL)
		return cJSON_CreateNull();

	// accepts negative index like lua_*(L, idx)
	if (src < 0)
		src = lua_gettop(L) + src + 1;

	if (src <= 0 || lua_isnil(L, src))
		return cJSON_CreateNull();
	#if LUA_VERSION_NUM > 501
		if (lua_isinteger(L, src))
			return cJSON_CreateNumber(lua_tointeger(L, src));
	#endif
	if (lua_isnumber(L, src))
		return cJSON_CreateNumber(lua_tonumber(L, src));
	if (lua_isstring(L, src))
		return cJSON_CreateString(lua_tostring(L, src));
	if (lua_isboolean(L, src))
		return lua_toboolean(L, src) ? cJSON_CreateTrue() : cJSON_CreateFalse();

	if (lua_istable(L, src))
	{
		cJSON *result;

		int size = size_of_array(L, src);

		if (size >= 0)
		{
			result = cJSON_CreateArray();

			int i;

			for (i = 1; i <= size; ++i)
			{
				lua_pushinteger(L, i);
				lua_gettable(L, src);  // i -- src[i]

				cJSON *item2 = convert_lua_to_cjson(L, -1);

				lua_pop(L, 1);

				cJSON_AddItemToArray(result, item2);
			}

			return result;
		}

		// record

		result = cJSON_CreateObject();

		lua_pushnil(L);

		while (lua_next(L, src) > 0)
		{
			cJSON *item2 = convert_lua_to_cjson(L, -1);

			lua_pop(L, 1);

			if (!lua_isstring(L, -1)) continue;

			const char *key2 = lua_tostring(L, -1);

			cJSON_AddItemToObject(result, key2, item2);
		}

		return result;
	}

	return cJSON_CreateNull();
}

