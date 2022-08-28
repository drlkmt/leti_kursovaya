#include <stdio.h>
#include <stdlib.h>

#define iSRC 1
#define iDST 2
#define iSTR 3
#define iFILESTR 4
#define iFILEOUT 5
#define iFILESRC 6
#define iFILEDST 7
#define NOENTRY 1
#define ARGERRORINTYPE 2
#define ARGERRORINNUMBER 3
#define UNKNOWNERROR 4
#define SPECN "[\\n]"
#define SPECTAB "[\\t]"
#define SPECSPACE "[SPACE]"

int parcearguments(int argc, char *argv[]);
int findlen(char argv[]);
void copystr(const char src[], char dst[]);
int getentry(char s[]);
int isitword(const char s[], int i, char word[]);
int argstoint(const char argv[]);
int compareword(const char arg[], const char word[]);
void showerror(int i);
int processingstr(char s[]);
void inputtostr();

char SRC[] = "--src";
char DST[] = "--dst";
char STR[] = "--str";
char FILESTR[] = "--file-str";
char FILEOUT[] = "--file-out";
char FILESRC[] = "--file-src";
char FILEDST[] = "--file-dst";

int lensrc;
int lendst;
int lenstr;
int lenfiledst;

char *src;
char *dst;
char *str;
char *filedst;
char *repstr;

int main(int argc, char *argv[])
{
	int entrys;
	int hookerrorsparcearguments = parcearguments(argc, argv);
	if(hookerrorsparcearguments == -1)
	{
		showerror(ARGERRORINTYPE);
		return 0;
	}
	if(str == NULL)
	{
		inputtostr();
	}

	if (src != NULL && dst != NULL)
		entrys = getentry(str);
	else
	{
		showerror(UNKNOWNERROR);
		return 0;
	}

	if (entrys == 0)
	{
		showerror(NOENTRY);
		return 0;
	}

	printf("Source str -> %s\n", str);

	repstr = (char *) malloc(((lenstr + (lendst - lensrc) * entrys) + 1) * sizeof(char));
	char *pointernewrepstr = repstr;
	char *pointerstr = str;
	char *pointerdst = dst;

	int i, j;
	i = 0;
	j = 0;
	while (pointerstr[i] != '\0')
	{

		if (isitword(str, i, src))
		{
			int h = 0;
			while (pointerdst[h] != '\0')
			{
				pointernewrepstr[j] = pointerdst[h];
				++h;
				++j;
			}
			i += findlen(src);
		}
		else
		{
			pointernewrepstr[j] = pointerstr[i];
			++i;
			++j;
		}
	}
	pointernewrepstr[j] = '\0';
	printf("New str -> %s\n", pointernewrepstr);

	if (filedst != NULL)
	{
		FILE *filedstwrite = fopen(filedst, "w");
		fprintf(filedstwrite, "%s", pointernewrepstr);
	}

	free(pointernewrepstr);
	free(pointerstr);
	free(pointerdst);
	free(src);
	free(dst);
	free(str);
	free(repstr);
	return 0;
}

int parcearguments(int argc, char *argv[])
{
	if ((argc - 1) % 2 != 0)
		return -1;
	int i;
	if (argc > 1)
	{
		for (i = 2; i < argc; i += 2)
		{
			switch (argstoint(argv[i - 1]))
			{
				case iSRC:

					lensrc = findlen(argv[i]);
					src = (char *) malloc((lensrc + 1) * sizeof(char));
					copystr(argv[i], src);
					lensrc = processingstr(src);
					break;

				case iDST:
					lendst = findlen(argv[i]);
					dst = (char *) malloc((lendst + 1) * sizeof(char));
					copystr(argv[i], dst);
					lendst = processingstr(dst);
					break;

				case iSTR:
					lenstr = findlen(argv[i]);
					str = (char *) malloc((lenstr + 1) * sizeof(char));
					copystr(argv[i], str);
					break;

				case iFILESTR: lenstr = 0;
					FILE *fileread = fopen(argv[i], "r");
					while (getc(fileread) != EOF)
						lenstr++;
					str = (char *) malloc((lenstr + 1) * sizeof(char));
					//fgets(str, lenstr + 1, fileread);
					freopen(argv[i], "r", fileread);
					int l, c;
					for(l = 0; (c = getc(fileread)) != EOF; ++l)
						str[l] = (char)c;
					str[l] = '\0';
					fclose(fileread);
					break;

				case iFILEOUT: lenfiledst = findlen(argv[i]);
					filedst = (char *) malloc((lenfiledst + 1) * sizeof(char));
					copystr(argv[i], filedst);
					break;

				case iFILESRC: lensrc = 0;
					FILE *filereadsrc = fopen(argv[i], "r");
					while (getc(filereadsrc) != EOF)
						lensrc++;
					freopen(argv[i], "r", filereadsrc);
					src = (char *) malloc((lensrc + 1) * sizeof(char));
					int cxsrc, csrc;
					for(cxsrc = 0; (csrc = getc(filereadsrc)) != EOF; ++cxsrc)
						src[cxsrc] = (char)csrc;
					src[cxsrc] = '\0';
					fclose(filereadsrc);
					lensrc = processingstr(src);
					break;

				case iFILEDST: lendst = 0;
					FILE *filereaddst = fopen(argv[i], "r");
					while (getc(filereaddst) != EOF)
						lendst++;
					freopen(argv[i], "r", filereaddst);
					dst = (char *) malloc((lendst + 1) * sizeof(char));
					int cxdst, cdst;
					for(cxdst = 0; (cdst = getc(filereaddst)) != EOF; ++cxdst)
						dst[cxdst] = (char)cdst;
					dst[cxdst] = '\0';
					fclose(filereaddst);
					lendst = processingstr(dst);
					break;

				default: return -1;

			}
		}
		if (src == NULL)
		{
			lensrc = 1;
			src = (char *) malloc((1) * sizeof(char));
			src[0] = '\0';
		}
		if (dst == NULL)
		{
			lendst = 1;
			dst = (char *) malloc((1) * sizeof(char));
			dst[0] = '\0';
		}
	}

	else
		return ARGERRORINNUMBER;
	return 0;
}

int findlen(char argv[])
{
	int j = 0;
	char *argvarg = &argv[j];
	while (argvarg[j] != '\0')
		++j;
	return j;
}

void copystr(const char srcarr[], char dstarr[])
{
	int i;
	for (i = 0; srcarr[i] != '\0'; ++i)
		dstarr[i] = srcarr[i];
	dstarr[i] = '\0';
}

int isitword(const char s[], int i, char word[])
{
	int j = 0;
	char *newsrc = word;
	while (newsrc[j] != '\0' && newsrc[j] == s[i])
	{
		++i;
		++j;
	}

	if (j == findlen(word))
		return 1;
	else
		return 0;
}

int getentry(char s[])
{
	int i, cx;
	cx = 0;
	for (i = 0; i <= lenstr - lensrc; ++i)
	{
		if (isitword(s, i, src))
		{
			i += lensrc - 1;
			cx++;
		}
	}
	return cx;
}

int argstoint(const char argv[])
{
	if (compareword(argv, SRC))
		return iSRC;
	else if (compareword(argv, DST))
		return iDST;
	else if (compareword(argv, STR))
		return iSTR;
	else if (compareword(argv, FILESTR))
		return iFILESTR;
	else if (compareword(argv, FILEOUT))
		return iFILEOUT;
	else if (compareword(argv, FILESRC))
		return iFILESRC;
	else if (compareword(argv, FILEDST))
		return iFILEDST;
	else
		return -1;
}

int compareword(const char arg[], const char word[])
{

	int i, cx;
	cx = 0;
	for (i = 0; word[i] != '\0'; ++i)
		if (word[i] != arg[i])
			cx = 1;
	if (arg[i] != '\0')
		return 0;
	if (cx == 0)
		return 1;
	else
		return 0;
}

void showerror(int i)
{
	if (i == NOENTRY)
		printf("Entrys not find!");
	else if (i == ARGERRORINTYPE)
		printf(
			"Error in type arguments.\nRun programm as <program> --src <src word> --dst <dst word>/<empty> --str <str to change>\n");
	else if (i == ARGERRORINNUMBER)
		printf("Run programm as <program> --src <src word> --dst <dst word>/<empty> --str <str to change>\n");
	else if (i == UNKNOWNERROR)
	{
		printf("Unknown error!\n");
		//showerror(ARGERRORINTYPE);
	}
	free(src);
	free(dst);
	free(str);
}

int processingstr(char s[])
{
	int i, j = 0;
	char *sn = (char *) malloc((findlen(s) + 1) * sizeof (char ));

	for(i = 0; s[i] != '\0';++i)
		sn[i] = s[i];
	sn[i] = '\0';
	i = 0;
	while(sn[j] != '\0')
	{
		if(isitword(s,j,SPECN) == 1)
		{
			s[i] = '\n';
			i++;
			j += findlen(SPECN);
		}
		else if(isitword(s,j,SPECTAB) == 1)
		{
			s[i] = '\t';
			i++;
			j += findlen(SPECTAB);
		}
		else if(isitword(s,j,SPECSPACE) == 1)
		{
			s[i] = ' ';
			i++;
			j += findlen(SPECSPACE);
		}
		else
		{
			s[i] = sn[j];
			j++;
			i++;
		}
	}
	s[i] = '\0';
	return i;
}

void inputtostr()
{
	int memsize = 100;
	int i, c;
	str = (char *)malloc(memsize * sizeof(char));
	for(i = 0; (c = getchar()) != EOF; i++)
	{
		if(i == memsize - 1)
		{
			memsize += memsize;
			str = (char *)realloc(str,memsize * sizeof(char));
		}
		str[i] = (char)c;
	}
	str[--i] = '\0';
	//str[i] = '\0';
	str = (char *)realloc(str,(i+1) * sizeof(char));
	lenstr = i;
}