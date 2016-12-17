#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct json {
	char name[200];
	char value[200];
};

int 		json_check(char *text);
char 		*getval(char *text);
char 		*jdata(struct json *array, char *name, int length);
struct json *json_dec(char *text, int *lenr);

struct json *json_dec(char *text, int *lenr)
{
	struct 	json *rtn;
	char 	string[201];
	int 	i, ln=0, length=1;

	string[0] = '\0';
	rtn = (struct json *)malloc(sizeof(struct json) * 1);

	if(json_check(text))
	{
		for(i=1;i<strlen(text);i++)
		{
			if(text[i] == ':')
			{
				strcpy(rtn[ln].name, getval(string));
				memset(string, 0, 201);
				string[0] = '\0';
			}else if(text[i] == ',')
			{
				strcpy(rtn[ln].value, getval(string));
				length += 1;
				rtn = (struct json *)realloc(rtn, sizeof(struct json) * length);
				memset(string, 0, 201);
				string[0] = '\0';
				ln += 1;
			}else if(text[i] == '}' && strlen(text)-1 == i)
			{
				strcpy(rtn[ln].value, getval(string));
			}else
			{
				strncat(string, &text[i], 1);
			}
		}

		*lenr = length;
		return rtn;
	}else
	{
		strcpy(rtn[0].name, "Error");
		strcpy(rtn[0].value, "it's not json");
		*lenr = 1;
		return rtn;
	}
}

int json_check(char *text)
{
	int i, len = 0;

	if(text[0] == '{' && text[strlen(text) - 1] == '}')
	{
		for(i=0;i<strlen(text);i++)
			if(text[i] == '"')
				len += 1;

		if(len % 2 == 0)
			return 1;
		else
			return -1;
	}else
	{
		return -1;
	}
}

char *getval(char *text)
{
	int 	i, in=0, to=0, k=1;
	char 	*ret;
	ret = (char *)malloc(sizeof(ret) * 1);
	ret[0] = '\0';

	for(i=0;i<strlen(text);i++)
		if(text[i] == '"')
		{
			in = i;
			break;
		}

	for(i=in+1;i<strlen(text);i++)
		if(text[i] == '"')
		{
			to = i;
			break;
		}

	for(i=in+1;i<to;i++)
	{
		strncat(ret, &text[i], 1);
		k++;
		ret = (char *)realloc(ret, sizeof(char) * k+1);
	}

	return ret;
}

char *jdata(struct json *array, char *name, int length)
{
	int i;
	for(i=0;i<length;i++)
		if(strcmp(array[i].name, name) == 0)
			return array[i].value;
	return NULL;
}

int main()
{
	int i, length;
	struct json *ch = json_dec("{\"data\":\"2\", \"name\":\"game\", \"state\":\"Hello world!\"}", &length);

	for(i=0;i<length;i++)
	{
		printf("%s - %s\n", ch[i].name, ch[i].value);
	}

	printf("\n\n%s", jdata(ch, "state", length));

	free(ch);
	return 0;
}