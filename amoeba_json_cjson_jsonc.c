#include "amoeba_json_cjson_jsonc.h"


json_object *convert_cjson_to_jsonc(cJSON *src)
{
	if (src == NULL || cJSON_IsNull(src) || cJSON_IsInvalid(src))
		return NULL;
	
	if (cJSON_IsNumber(src))
		return json_object_new_double(src->valuedouble);

	if (cJSON_IsString(src))
		return json_object_new_string(cJSON_GetStringValue(src));

	if (cJSON_IsBool(src))
		return json_object_new_boolean(cJSON_IsFalse(src) ? 0 : 1);

	if (cJSON_IsArray(src))
	{
		json_object *result = json_object_new_array();

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			json_object_array_add(result, convert_cjson_to_jsonc(item));
		}

		return result;
	}

	if (cJSON_IsObject(src))
	{
		json_object *result = json_object_new_object();

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			json_object_object_add(result, item->string, convert_cjson_to_jsonc(item));
		}

		return result;
	}

	return NULL;
}


cJSON *convert_jsonc_to_cjson(json_object *src)
{
	if (json_object_is_type(src, json_type_null))
		return cJSON_CreateNull();

	if (json_object_is_type(src, json_type_int))
		return cJSON_CreateNumber(json_object_get_int(src));

	if (json_object_is_type(src, json_type_double))
		return cJSON_CreateNumber(json_object_get_double(src));

	if (json_object_is_type(src, json_type_string))
		return cJSON_CreateString(json_object_get_string(src));

	if (json_object_is_type(src, json_type_boolean))
		return json_object_get_boolean(src) == 0 ? cJSON_CreateFalse() : cJSON_CreateTrue();

	if (json_object_is_type(src, json_type_array))
	{
		cJSON *result = cJSON_CreateArray();

		size_t size = json_object_array_length(src);
		size_t i = 0;

		for (i = 0; i < size; ++i)
		{
			json_object *item = json_object_array_get_idx(src, i);
			cJSON_AddItemToArray(result, convert_jsonc_to_cjson(item));
		}

		return result;
	}

	if (json_object_is_type(src, json_type_object))
	{
		cJSON *result = cJSON_CreateObject();

		json_object_iter iter;

		json_object_object_foreachC(src, iter)
//		json_object_object_foreach(src, key, item)
		{
			cJSON_AddItemToObject(result, iter.key, convert_jsonc_to_cjson(iter.val));
		}

		return result;
	}

	return cJSON_CreateNull();
}
