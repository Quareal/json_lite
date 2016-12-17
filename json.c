#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
// Структура 
struct json {
	char name[200];
	char value[200];
};
// Прототипы 
int 		json_check(char *text);
char 		*getval(char *text);
char 		*jdata(struct json *array, char *name, int length);
struct json 	*json_dec(char *text, int *lenr);
// Функция раскодирования json
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
				ln++;
			}else if(text[i] == '}' && strlen(text)-1 == i)
			{
				strcpy(rtn[ln].value, getval(string));
			}else
			{
				if(strlen(string) < 200)
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
// Проверка валидности json
int json_check(char *text)
{
	int i, len = 0;

	if(text[0] == '{' && text[strlen(text) - 1] == '}')
	{
		for(i=0;i<strlen(text);i++)
			if(text[i] == '"')
				len++;

		if(len % 2 == 0)
			return 1;
		else
			return -1;
	}else
	{
		return -1;
	}
}
// Получение точного значения элемента
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
// Функция возвращает значение определенного элемента массива json ( по имени )
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
	// Переменная i - для цикла, length - для количества элементов массива
	int i, length;
	// В переменную ch записывается массив json
	struct json *ch = json_dec("{\"data\":\"2\", \"name\":\"game\", \"state\":\"Hello world!\"}", &length);

	for(i=0;i<length;i++)
	{
		printf("%s - %s\n", ch[i].name, ch[i].value);
	}
	// Возвращаем значение элемента с именем state, ответ будет - Hello world!
	printf("\n\n%s", jdata(ch, "state", length));

	free(ch);
	return 0;
}
