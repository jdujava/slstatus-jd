/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * dwmst.c
 * Copyright (C) 2014 Jente Hidskes <hjdskes@gmail.com>
 *
 * dwmst is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dwmst is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <mpd/client.h>
#include <alsa/asoundlib.h>
#include <linux/wireless.h>

#include "../util.h"

const char *
get_mpd(const char *unused) {
	struct mpd_connection *con;
	struct mpd_status *status;
	struct mpd_song *song;
	int status_type;
	const char *res;
	const char *artist = NULL, *title = NULL, *state = NULL, *ellipsis = "";
	int elapsed_time, full_time;

	con = mpd_connection_new(NULL, 0, 30000);
	if(mpd_connection_get_error(con)) {
		mpd_connection_free(con);
		return "";
	}

	mpd_command_list_begin(con, true);
	mpd_send_status(con);
	mpd_send_current_song(con);
	mpd_command_list_end(con);

	status = mpd_recv_status(con);
	status_type = mpd_status_get_state(status);
	if(!status || (status_type != MPD_STATE_PLAY && status_type != MPD_STATE_PAUSE)) {
		mpd_connection_free(con);
		return "";
	}

	mpd_response_next(con);
	song = mpd_recv_song(con);
	title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
	if(!title)
		title = mpd_song_get_uri(song);
	artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
	elapsed_time = mpd_status_get_elapsed_time(status);
	full_time = mpd_status_get_total_time(status);
	int elapsed_time_m = elapsed_time/60;
	int elapsed_time_s = elapsed_time%60;
	int full_time_m = full_time/60;
	int full_time_s = full_time%60;

	if(strlen(title) > 50)
		ellipsis="...";

	state = (status_type == MPD_STATE_PLAY)? "^c#9ee9c0^^d^":"^c#9ee9c0^^f1^^d^";
	res = bprintf(" %.30s - %.50s%s %s %d:%02d/%d:%02d", artist, title, ellipsis, state, elapsed_time_m, elapsed_time_s, full_time_m, full_time_s);

	mpd_song_free(song);
	mpd_response_finish(con);
	mpd_status_free(status);
	mpd_connection_free(con);
	return res;
}
