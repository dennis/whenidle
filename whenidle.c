/*
"THE BEER-WARE LICENSE" (Revision 42):
<dennis@moellegaard.dk> wrote this file. As long as you retain this notice you
can do whatever you want with this stuff. If we meet some day, and you think
this stuff is worth it, you can buy me a beer in return.
*/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>

static long idle_time_threshold = 30; /* seconds */
static char *command = NULL;

void parse_options(int argc, char *argv[]) {
	char opt;
	struct option long_opts[] = {
		{ "idle-threshold", required_argument, NULL, 't' },
		{ NULL, 0, NULL, 0 }
	};

	char *err;
	while((opt = getopt_long(argc, argv, "t:", long_opts, NULL)) != -1) {
		switch(opt) {
			case 't':
				idle_time_threshold = atoi(optarg);
				break;
			default: 
				exit(2);
		}
	}

	if(optind >= argc) {
		fprintf(stderr, "Expected argument after options\n");
		exit(1);
	}

	command = argv[optind];
}

int main(int argc, char* argv[]) {
	Display *d;
	XScreenSaverInfo *xssi;
	int xss_event_basep, xss_error_basep;
	int gone_idle;
	char *display = getenv("DISPLAY");

	parse_options(argc, argv);

	if(!(d = XOpenDisplay(display))) {
		fprintf(stderr, "Could not open X display \"%s\"\n", display);
		exit(1);
	}
	else if(!XScreenSaverQueryExtension(d, &xss_event_basep, &xss_error_basep)) {
		fprintf(stderr, "X Screen Saver extension is not available!\n");

		exit(1);
	}
	else if(!(xssi = XScreenSaverAllocInfo())) {
		fprintf(stderr, "Failed to allocate X Screen Saver information memory!\n");
		exit(1);
	}
	else {
		while(1) {
			if (!XScreenSaverQueryInfo(d, DefaultRootWindow(d), xssi)) {
				fprintf(stderr, "Failed to get X Screen Saver information!\n");
				break;
			}

			if( (xssi->idle/1000) > idle_time_threshold ) {
				system(command);
				break;
			}
			sleep(1);
		}
	}

	XCloseDisplay(d);
}
