#include "amoeba_json_cjson_tcl.h"


static cJSON *convert_tcl_to_cjson__internal(Tcl_Interp *interp, Tcl_Obj *src, size_t depth)
{
	if (depth >= AMOEBA_JSON_REC_DEPTH_LIM || interp == NULL || src == NULL)
		return cJSON_CreateNull();

	const char *name = src != NULL
		&& src->typePtr != NULL
		? src->typePtr->name
		: NULL;

	if (name == NULL)
		return cJSON_CreateString(Tcl_GetStringFromObj(src, NULL));

	if (!strcmp(name, "int"))
	{
		int i;

		Tcl_GetIntFromObj(interp, src, &i);

		return cJSON_CreateNumber(i);
	}

	if (!strcmp(name, "double"))
	{
		double d;

		Tcl_GetDoubleFromObj(interp, src, &d);

		return cJSON_CreateNumber(d);
	}

	if (!strcmp(name, "bytearray"))
	{
		cJSON *result;

		int size;
		unsigned char *bytes = Tcl_GetByteArrayFromObj(src, &size);

		int *a = malloc(size * sizeof(int));

		if (a == NULL)
			return cJSON_CreateArray();

		int i;

		for (i = 0; i < size; ++i)
			a[i] = bytes[i];

		result = cJSON_CreateIntArray(a, size);

		free(a);

		return result;
	}

	if (!strcmp(name, "list"))
	{
		cJSON *result = cJSON_CreateArray();

		int size;
		//Tcl_ListObjLength(interp, src, &size);

		Tcl_Obj **objs = NULL;

		Tcl_ListObjGetElements(interp, src, &size, &objs);

		int i;

		for (i = 0; i < size; ++i)
		{
			cJSON *val = convert_tcl_to_cjson__internal(interp, objs[i], depth + 1);

			cJSON_AddItemToArray(result, val);
		}

		return result;
	}

	if (!strcmp(name, "dict"))
	{
		cJSON *result = cJSON_CreateObject();

		Tcl_DictSearch search;
		Tcl_Obj *key, *item;
		int done;

		Tcl_DictObjFirst(interp, src, &search, &key, &item, &done);

		while (!done)
		{
			const char *key2 = Tcl_GetStringFromObj(src, NULL);
			cJSON *item2 = convert_tcl_to_cjson__internal(interp, item, depth + 1);

			cJSON_AddItemToObject(result, key2, item2);

			Tcl_DictObjNext(&search, &key, &item, &done);
		}

		Tcl_DictObjDone(&search);

		return result;
	}

	return cJSON_CreateString(Tcl_GetStringFromObj(src, NUL));
}

cJSON *convert_tcl_to_cjson(Tcl_Interp *interp, Tcl_Obj *src)
{
	return convert_tcl_to_cjson__internal(interp, src, 0);
}


Tcl_Obj *convert_cjson_to_tcl(Tcl_Interp *interp, cJSON *src)
{
	//?
	if (interp == NULL || src == NULL || cJSON_IsNull(src))
		return Tcl_NewIntObj(0);

	if (cJSON_IsString(src))
	{
		const char *s = cJSON_GetStringValue(src);
		size_t len = strlen(s);

		return Tcl_NewStringObj(s, len);
	}

	if (cJSON_IsNumber(src))
		return Tcl_NewDoubleObj(src->valuedouble);

	if (cJSON_IsBool(src))
	{
		int b = cJSON_IsFalse(src) ? 0 : 1;

		return Tcl_NewBooleanObj(b);
	}

	if (cJSON_IsArray(src))
	{
		int size = cJSON_GetArraySize(src);

		Tcl_Obj **objs = malloc(size * sizeof(Tcl_Obj*));

		if (objs == NULL)
			return Tcl_NewIntObj(0);

		cJSON *item;
		size_t i = 0;

		cJSON_ArrayForEach(item, src)
		{
			objs[i++] = convert_cjson_to_tcl(interp, item);
		}

		Tcl_Obj *result = Tcl_NewListObj(size, objs);

		free(objs);

		return result;
	}

	if (cJSON_IsObject(src))
	{
		Tcl_Obj *result = Tcl_NewDictObj();

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			const char *key = item->string;
			Tcl_Obj *key2 = Tcl_NewStringObj(key, strlen(key));

			Tcl_DictObjPut(interp, result, key2, convert_cjson_to_tcl(interp, item));
		}

		return result;
	}

	return Tcl_NewIntObj(0);
}
