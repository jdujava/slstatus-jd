/* See LICENSE file for copyright and license details. */
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <wordexp.h>

#include "../slstatus.h"
#include "../util.h"

const char *
num_files(const char *path)
{
	struct dirent *dp;
	DIR *dir;
	int num;

	wordexp_t exp_path;
	wordexp(path, &exp_path, 0);

	if (!(dir = opendir(exp_path.we_wordv[0]))) {
		warn("opendir '%s':", path);
		return NULL;
	}

	num = 0;
	while ((dp = readdir(dir))) {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
			continue; /* skip self and parent */

		num++;
	}

	closedir(dir);
	wordfree(&exp_path);

	// return bprintf("%d", num);
	return (num>0)? bprintf("^c#ffffff^%d^d^", num):bprintf("%d", num);
}
