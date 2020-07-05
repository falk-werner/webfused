#include "webfused/daemon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <signal.h>

#include <unistd.h>
#include <getopt.h>

#include <webfuse/webfuse.h>
#include "webfused/config/config.h"
#include "webfused/config/factory.h"
#include "webfused/log/log.h"
#include "webfused/log/logger.h"
#include "webfused/log/stderr_logger.h"
#include "webfused/change_user.h"

#define WFD_SERVICE_TIMEOUT (1 * 1000)
#define WFD_DEFAULT_CONFIG_FILE ("/etc/webfused.conf")

struct args
{
	char * config_file;
	bool show_help;
};

static bool shutdown_requested = false;

static void show_help(void)
{
	printf(
		"webfused, Copyright (c) 2019-2020, webfused authors <https://github.com/falk-werner/webfused>\n"
		"Websocket file system daemon\n"
		"\n"
		"Usage: webfused [-f <config file>] | -h\n"
		"\n"
		"Options:\n"
		"\t-f, --config-file Path to config file (default: /etc/webfuse.conf)\n"
		"\t-h, --help        Print this message and terminate\n"
		"\n");
}

static int parse_arguments(int argc, char * argv[], struct args * args)
{
	static struct option const options[] =
	{
		{"config-file", required_argument, NULL, 'f'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	args->config_file = strdup(WFD_DEFAULT_CONFIG_FILE);
	args->show_help = false;

	bool result = EXIT_SUCCESS;
	bool finished = false;
	optind = 0;
	while ((!finished) && (EXIT_SUCCESS == result))
	{
		int option_index = 0;
		int const c = getopt_long(argc, argv, "f:h", options, &option_index);

		switch (c)
		{
			case -1:
				finished = true;
				break;
			case 'h':
				args->show_help = true;
				finished = true;
				break;
			case 'f':
				free(args->config_file);
				args->config_file = strdup(optarg);
				break;
			default:
				fprintf(stderr, "error: unknown argument\n");
				result = EXIT_FAILURE;
				break;
		}
	}

	if (EXIT_SUCCESS != result)
	{
		args->show_help = true;
	}

	return result;
}

static void on_interrupt(int signal_id)
{
	(void) signal_id;

	shutdown_requested = true;
}

int wfd_daemon_run(int argc, char * argv[])
{
	wfd_stderr_logger_init(WFD_LOGLEVEL_ALL, NULL);

	struct args args;
	int result = parse_arguments(argc, argv, &args);
	
	if (!args.show_help)
	{
		signal(SIGINT, on_interrupt);

		struct wfd_config * config = wfd_config_load_file(args.config_file);
		bool success = (NULL != config);

		if (!success)
		{
			fprintf(stderr, "fatal: failed to load server config\n");
			result = EXIT_FAILURE;
		}

		if (success)
		{
			success = wfd_change_user(
				wfd_config_get_user(config),
				wfd_config_get_group(config));
		}

		if (success)
		{
			struct wf_server_config * server_config = wfd_config_get_server_config(config);
			struct wf_server * server = wf_server_create(server_config);
			if (NULL != server)
			{
				while (!shutdown_requested)
				{
					wf_server_service(server);
				}

				wf_server_dispose(server);			
			}
			else
			{
				fprintf(stderr, "fatal: unable start server\n");
				result = EXIT_FAILURE;
			}
		}

		if (NULL != config)
		{
			wfd_config_dispose(config);
		}
	}
	else
	{
		show_help();
	}

	wfd_logger_close();
	free(args.config_file);
	shutdown_requested = false;
	return result;
}

