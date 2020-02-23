
#include "amoeba_json_cjson_neko.h"


value convert_cjson_to_neko(cJSON *src)
{
	if (src == NULL || cJSON_IsNull(src))
		return val_null;

	if (cJSON_IsNumber(src))
		return alloc_float(src->valuedouble);

	if (cJSON_IsString(src))
		return alloc_string(cJSON_GetStringValue(src));

	if (cJSON_IsBool(src))
		return cJSON_IsFalse(src) ? val_false : val_true;

	if (cJSON_IsArray(src))
	{
		size_t size = cJSON_GetArraySize(src);
		value result = alloc_array(size);

		size_t i;

		for (i = 0; i < size; ++i)
		{
			val_array_ptr(result)[i] = convert_cjson_to_neko(cJSON_GetArrayItem(src, i));
		}

		return result;
	}

	if (cJSON_IsObject(src))
	{
		value result = alloc_object(val_null);

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			alloc_field(result, val_id(item->string), convert_cjson_to_neko(item));
		}

		return result;
	}

	neko_error();
}


typedef struct {
	size_t depth;
	cJSON *result;
} field_converter_arg;

static cJSON *convert_neko_to_cjson__internal(value src, size_t depth);

static void field_converter(value v, field f, void *arg0)
{
	field_converter_arg *arg = arg0;
	value key = val_field_name(f);

	if (!val_is_string(key)) return;

	cJSON *val = convert_neko_to_cjson__internal(v, arg->depth + 1);

	cJSON_AddItemToObject(arg->result, val_string(key), val);
}

static cJSON *convert_neko_to_cjson__internal(value src, size_t depth)
{
	if (depth >= AMOEBA_JSON_DEPTH_LIM)
		return cJSON_CreateNull();

	if (val_is_float(src))
		return cJSON_CreateNumber(val_float(src));

	if (val_is_any_int(src))
		return cJSON_CreateNumber(val_any_int(src));

	if (val_is_string(src))
		return cJSON_CreateString(val_string(src));

	if (val_is_bool(src))
		return val_bool(src) ? cJSON_CreateTrue() : cJSON_CreateFalse();

	if (val_is_array(src))
	{
		cJSON *result = cJSON_CreateArray();

		size_t size = val_array_size(src);
		size_t i;

		for (i = 0; i < size; ++i)
		{
			cJSON *val = convert_neko_to_cjson__internal(val_array_ptr(src)[i], depth + 1);

			cJSON_AddItemToArray(result, val);
		}

		return result;
	}

	if (val_is_object(src))
	{
		cJSON *result = cJSON_CreateObject();

		field_converter_arg arg = {
			.depth = depth,
			.result = result
		};

		val_iter_fields(src, field_converter, result);

		return result;
	}

	return cJSON_CreateFalse();
}

cJSON *convert_neko_to_cjson(value src)
{
	return convert_neko_to_cjson__internal(src, 0);
}

