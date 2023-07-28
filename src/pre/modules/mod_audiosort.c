/*
 * foo-tools, a collection of utilities for glftpd users.
 * Copyright (C) 2003  Tanesha FTPD Project, www.tanesha.net
 *
 * This file is part of foo-tools.
 *
 * foo-tools is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * foo-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with foo-tools; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 * Module that runs pzs-ng's audiosort on pre release
 * Author, slv.
 * $Id: mod_audiosort.c,v 1.11 2020/01/25 21:00:00 slv Exp $
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// project includes
#include "mod_audiosort.h"
#include "../foo-pre.h"

// footools includes
#include <lib/stringtokenizer.h>
#include <collection/hashtable.h>
#include <collection/strlist.h>

// in foo-pre
hashtable_t *_mod_audiosort_cfg = 0;
hashtable_t *_mod_audiosort_env = 0;

void set_config(hashtable_t *cfg) {
	_mod_audiosort_cfg = cfg;
}
hashtable_t *get_config() {
	return _mod_audiosort_cfg;
}

void set_env(hashtable_t *env) {
	_mod_audiosort_env = env;
}
hashtable_t *get_env() {
	return _mod_audiosort_env;
}

/*
 * Gets configuration properties in the a|b|c|d format as a strlist.
 */
strlist_t * config_get_split_property(char *prop) {
        hashtable_t *cfg = get_config();
        char *tmp;
        stringtokenizer st;
        strlist_t *l = 0;

        tmp = ht_get(cfg, prop);

        st_initialize(&st, tmp, "|");
        while (st_hasnext(&st)) {
                tmp = st_next(&st);

                l = str_add(l, tmp);
        }

	return l;
}


// prototype for rel handling function.
int mod_audiosort_rel_func(char *dir, char *argv[]);

module_list_t mod_audiosort_info = {
	// module name
	"audiosorter",

	// module dir func
	0,

	// module file func
	0,

	// module rel func
	mod_audiosort_rel_func,

	// struct module_list entry
	0
};




// function to return module info of this module.
module_list_t *module_loader() {
	return &mod_audiosort_info;
}

int pre_replace(char *b, char *n, char *r) {
	char *t, *save;
	int i=0;

	while (t=strstr(b, n)) {
		save=(char*)malloc(strlen(t)-strlen(n)+1);
		strcpy(save, t+strlen(n));
		*t=0;
		strcat(b, r);
		strcat(b, save);
		free(save);
		i++;
	}
}


int mod_audiosort_rel_func(char *dir, char *argv[]) {
	char buf[1024], *tmp, *audiosort_bin, *section, *s_path;
	strlist_t *a_sections;
	strlist_iterator_t *i;
	FILE *f;
	int found = 0;
	section = ht_get(get_env(),"section");
	if (!section)
		return 1;

	a_sections = config_get_split_property(PROPERTY_MOD_AUDIOSORT_SECTIONS);
        for (i = str_iterator(a_sections); str_iterator_hasnext(i); ) {
                tmp = str_iterator_next(i);
		if (strcmp(tmp, section) == 0) {
			found++;
#ifdef DEBUG
			printf("MODULE-DEBUG: strcmp tmp=\"%s\" section=\"%s\" found=%i\n", tmp, section, found);
#endif
			break;
		}
	}
	str_close(a_sections);
	free(i);
	if (!found) {
#ifdef DEBUG
		printf("MODULE-DEBUG: no a_sections found=%i\n", found);
#endif
		return 1;
	}

#ifdef DEBUG
	printf("MODULE-DEBUG: dir=%s section=%s\n", dir, section);
#endif

	// get the dir of the section.
	sprintf(buf, "section.%s.%s", section, PROPERTY_SECTION_DIR);
#ifdef DEBUG
	printf("MODULE-DEBUG: buf=%s\n", buf);
#endif

	s_path = ht_get(get_config(), buf);

	// return if missing configuration.
	if (!s_path) {
#ifdef DEBUG
		printf("MODULE-DEBUG: missing cfg, s_path=%s\n", s_path);
#endif
		return 1;
	}

#ifdef DEBUG
	printf("MODULE-DEBUG: s_path=%s\n", s_path);
#endif

	// get the rlsname
	tmp = strrchr(dir, '/');
	if (!tmp)
		return 1;
	tmp++;
#ifdef DEBUG
	printf("MODULE-DEBUG: tmp=%s\n", tmp);
#endif

	audiosort_bin = ht_get(get_config(), PROPERTY_MOD_AUDIOSORT_BIN);
	if (!audiosort_bin)
		audiosort_bin = "/bin/audiosort";

#ifdef DEBUG
	printf("MODULE-DEBUG: audiosort_bin=%s\n", audiosort_bin);
#endif
	f = fopen(audiosort_bin, "r");
	if (!f)
		return 1;

#ifdef DEBUG
	printf("MODULE-DEBUG: dir=%s section=%s s_path=%s\n", dir, section, s_path);
	printf("MODULE-DEBUG: argv0=%s argv1=%s argv2=%s\n", argv[0], argv[1], argv[2]);
#endif

	time_t now;
	struct tm *tm_now;

	now = time(0);
	tm_now = localtime(&now);

	strftime(buf, 1024, "%d", tm_now);
	pre_replace(tmp, "%DD", buf);

	strftime(buf, 1024, "%m", tm_now);
	pre_replace(tmp, "%MM", buf);

	strftime(buf, 1024, "%Y", tm_now);
	pre_replace(tmp, "%YYYY", buf);

	strftime(buf, 1024, "%y", tm_now);
	pre_replace(tmp, "%YY", buf);

	strftime(buf, 1024, "%w", tm_now);
	pre_replace(tmp, "%WW", buf);

	strftime(buf, 1024, "%W", tm_now);
	pre_replace(tmp, "%WOY", buf);

	strftime(buf, 1024, "%V", tm_now);
	pre_replace(tmp, "%CW", buf);
	pre_replace(tmp, "%KW", buf);

	sprintf(buf, "%s '%s/%s' >/dev/null 2>&1", audiosort_bin, s_path, tmp);
#ifdef DEBUG
	printf("MODULE-DEBUG: %s\n", buf);
	f = fopen("mod_audiosort.log", "a");
	char fdate[12], ftime[10];
	strftime(fdate, 1024, "%Y-%m-%d", tm_now);
	strftime(ftime, 1024, "%H:%M:%S", tm_now);
	fprintf(f, "%s %s MODULE-DEBUG: %s\n", fdate, ftime, buf);
	fclose(f);
#endif
	if (system(buf) == -1)
		printf("audiosorting %s failed!\n", dir);

	return 1;
}

/* vim: set noai tabstop=8 shiftwidth=8 softtabstop=8 noexpandtab: */
