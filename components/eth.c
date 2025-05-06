/* See LICENSE file for copyright and license details. */
#include <ifaddrs.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util.h"
#include <limits.h>

const char *
eth(const char *interface)
{
	char *p;
	char path[PATH_MAX];
	char status[5];
	FILE *fp;

	if (esnprintf(path, sizeof(path), "/sys/class/net/%s/operstate",
				  interface) < 0) {
		return NULL;
	}
	if (!(fp = fopen(path, "r"))) {
		warn("fopen '%s':", path);
		return NULL;
	}
	p = fgets(status, 5, fp);
	fclose(fp);
	if (!p || strcmp(status, "up\n") != 0) {
		/* esnprintf(path, sizeof(path), "/sys/class/net/enp0s31f6/operstate"); */
		/* fp = fopen(path, "r"); */
		/* p = fgets(status, 5, fp); */
		/* fclose(fp); */
		/* if (!p || strcmp(status, "up\n") != 0) { */
		/* 	return bprintf(""); */
		/* } else { */
		/* 	return bprintf(""); */
		/* } */
		return bprintf("^c#7c8390^^d^");
	}
	return bprintf("^c#e7a6d0^^d^");
}
