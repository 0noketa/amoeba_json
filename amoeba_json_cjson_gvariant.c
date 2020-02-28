#include "amoeba_json_cjson_gvariant.h"


/**
 * convert_cjson_to_gvariant: 
 * @src: read-only cJSON.
 * Returns: GVariant, caller should delete. converts every null to empty string.
 * Stability: Stable
 */
GVariant *convert_cjson_to_gvariant(cJSON *src)
{
	if (src == NULL || cJSON_IsNull(src))
		return g_variant_new_string("");

	if (cJSON_IsNumber(src))
		return g_variant_new_double(src->valuedouble);

	if (cJSON_IsString(src))
		return g_variant_new_string(cJSON_GetStringValue(src));

	if (cJSON_IsBool(src))
		return g_variant_new_boolean(cJSON_IsTrue(src) ? 1 : 0); //?

	if (cJSON_IsArray(src))
	{
		int size = cJSON_GetArraySize(src);

		GVariant **objs = malloc(size * sizeof(GVariant*));

		if (objs == NULL)
			return g_variant_new_string("");

		int i;

		for (i = 0; i < size; ++i)
		{
			GVariant *item = convert_cjson_to_gvariant(cJSON_GetArrayItem(src, i));

			objs[i] = g_variant_new_variant(item);
		}

		GVariant *result = g_variant_new_array(G_VARIANT_TYPE_VARIANT, objs, size);

		free(objs);

		return result;
	}

	if (cJSON_IsObject(src))
	{
		int size = cJSON_GetArraySize(src);

		GVariant **objs = calloc(size, sizeof(GVariant*));

		if (objs == NULL)
			return g_variant_new_string("");

		cJSON *item;
		int i = 0;

		cJSON_ArrayForEach(item, src)
		{
			GVariant *key = g_variant_new_string(item->string);
			GVariant *item2 = convert_cjson_to_gvariant(item);
			GVariant *item3 = g_variant_new_variant(item2);

			objs[i++] = g_variant_new_dict_entry(key, item3);
		}

		GVariant *result = g_variant_new_array(G_VARIANT_TYPE("{sv}"), objs, size);
		
		free(objs);

		return result;
	}

	return g_variant_new_string("");
}


/**
 * convert_gvariant_to_cjson: 
 * @src: read-only GVariant.
 * Returns: cJSON, caller should delete.
 * Stability: Stable
 */
cJSON *convert_gvariant_to_cjson(GVariant *src)
{
	if (src == NULL)
		return cJSON_CreateNull();

	switch (g_variant_classify(src))
	{
	case G_VARIANT_CLASS_BYTE:
		return cJSON_CreateNumber(g_variant_get_byte(src));
	case G_VARIANT_CLASS_INT16:
		return cJSON_CreateNumber(g_variant_get_int16(src));
	case G_VARIANT_CLASS_UINT16:
		return cJSON_CreateNumber(g_variant_get_uint16(src));
	case G_VARIANT_CLASS_INT32:
		return cJSON_CreateNumber(g_variant_get_int32(src));

	// warrning: large numbers
	case G_VARIANT_CLASS_UINT32:
		return cJSON_CreateNumber(g_variant_get_uint32(src));
	case G_VARIANT_CLASS_INT64:
		return cJSON_CreateNumber(g_variant_get_int64(src));
	case G_VARIANT_CLASS_UINT64:
		return cJSON_CreateNumber(g_variant_get_uint64(src));

	case G_VARIANT_CLASS_DOUBLE:
		return cJSON_CreateNumber(g_variant_get_double(src));

	case G_VARIANT_CLASS_STRING:
		return cJSON_CreateString(g_variant_get_string(src, NULL));

	case G_VARIANT_CLASS_BOOLEAN:
		return g_variant_get_boolean(src) ? cJSON_CreateTrue() : cJSON_CreateFalse();

	case G_VARIANT_CLASS_ARRAY:
		if (g_variant_check_format_string(src, "a{sv}", TRUE))
		{
			cJSON *result = cJSON_CreateObject();

			GVariantIter *iter = g_variant_iter_new(src);
			const gchar *key = NULL;
			GVariant *item;

			while (g_variant_iter_loop(iter, "{sv}", &key, &item))
			{
				cJSON *item2 = convert_gvariant_to_cjson(item);

				cJSON_AddItemToObject(result, key, item2);
			}

			return result;
		}
		// else
	case G_VARIANT_CLASS_TUPLE:
		if (g_variant_check_format_string(src, "av", TRUE))
		{
			cJSON *result = cJSON_CreateArray();

			GVariantIter *iter = g_variant_iter_new(src);
			GVariant *item;

			while (g_variant_iter_loop(iter, "v", &item))
			{
				cJSON *item2 = convert_gvariant_to_cjson(item);

				cJSON_AddItemToArray(result, item2);
			}

			return result;
		}

		return cJSON_CreateArray();

	default:
		return cJSON_CreateNull();
	}
}