#include "amoeba_json_cjson_tcl.h"


cJSON *convert_tcl_to_cjson(Tcl_Interp *interp, Tcl_Obj *src)
{
	Tcl_ObjType t;
	Tcl_ConvertToType(interp, src, &t);

	if (!strcmp(t.name, "int"))
	{
		int i;

		Tcl_GetIntFromObj(interp, src, &i);

		return cJSON_CreateNumber(i);
	}

	if (!strcmp(t.name, "double"))
	{
		double d;

		Tcl_GetDoubleFromObj(interp, src, &d);

		return cJSON_CreateNumber(d);
	}

	if (!strcmp(t.name, "char *"))
	{
		int len;
		const char *s = Tcl_GetStringFromObj(src, &len);

		return cJSON_CreateString(s);
	}

	if (!strcmp(t.name, "dict"))
	{
		cJSON *result = Tcl_NewDictObj();

		Tcl_DictSearch search;
		Tcl_Obj *key, *val;
		int done;

		Tcl_DictObjFirst(interp, src, &search, &key, &val, &done);

		while (!done)
		{
			int len;
			const char *key2 = Tcl_GetStringFromObj(src, &len);

			cJSON_AddItemToObject(result, key2, convert_tcl_to_cjson(val));

			Tcl_DictObjNext(&search, &key, &val, &done);
		}

		Tcl_DictObjDone(&search);

		return result;
	}

	return NULL;
}

Tcl_Obj *convert_cjson_to_tcl(Tcl_Interp *interp, cJSON *src)
{
	//?
	if (src == NULL || cJSON_IsNull(src))
		return NULL;

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
		Tcl_Obj *result = Tcl_NewListObj();

		cJSON *item;
		size_t i = 0;

		cJSON_ArrayForEach(item, src)
		{
			Tcl_SetListObj(result, i, convert_cjson_to_tcl(item));
			++i;
		}

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

			Tcl_DictObjPut(interp, result, key2, convert_cjson_to_tcl(item));
		}

		return result;
	}

	return NULL;
}
