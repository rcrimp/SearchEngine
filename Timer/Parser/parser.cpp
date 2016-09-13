#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#include <iostream>
#include "../Util/timer.h"

const int BUFFER_SIZE = 2048;

char *cleanToken(char *t) {
	int s = strlen(t);
	int i = s;
	// remove trailing white space
	while (--i > 0) {
		if (t[i] == '>' || isalpha(t[i]))
			break;
		else
			t[i] = '\0';
	}

	// to lower case
	do {
		if (isupper(t[i]))
			t[i] = tolower(t[i]);
	} while (--i >= 0);

	// remove leading white space
	while (*t != '\0') {
		if (*t == '<' || isalpha(*t))
			break;
		else
			t++;
	}
	return t;
}

void parseToken(char* t) {
	if (strcmp(t, "") == 0 || strcmp(t, "\n") == 0)
		return;
	if (strcmp(t, "&amp;") == 0)
		return;
	// printf("&\n");
	else
		printf("%s\n", t);
}

int main(void) {
	/* open file */
	const char *filename = "wsj.xml";
	FILE* file = fopen(filename, "r");
	if (NULL == file) {
		fprintf(stderr, "Unable to open file %s\n", filename);
		return EXIT_FAILURE;
	}

	char line[BUFFER_SIZE];
	char *token = NULL;

	Timer tmr;

	/* get next line */
	while (fgets(line, BUFFER_SIZE, file) != NULL) {
		/* get first token */
		token = strtok(line, " ");
		while (token != NULL) {
			token = cleanToken(token);

			/* if token contains a tag */
			char *st = strchr(token, '<');
			if (st != NULL) {
				/* find the end of the tag */
				char *en = strchr(token, '>');

				/* if the tag is preceded by a token */
				if (st != token) {
					char c = *st;
					*st = '\0';
					token = cleanToken(token);
					parseToken(token);
					*st = c;
				}

				/* if the token ends a doc */
				if (strncmp("</DOC>", st, 6) == 0)
					putchar('\n');

				/* if the tag is followed by a token */
				if (en != (token + strlen(token) - 1)) {
					char* test = cleanToken(en + 1);
					parseToken(en + 1);
				}
			}
			else {
				/* token doesn't contain a XML tag */
				parseToken(token);
			}

			/* get next token */
			token = strtok(NULL, " ");
		}
	}

	std::cerr << tmr.elapsed() << " seconds" << std::endl;
	getchar();

	return EXIT_SUCCESS;
}