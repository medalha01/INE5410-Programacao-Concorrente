#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "args.h"


config_t parse(int argc, char **argv) {
    int c;
    config_t config = { 1, 10, 10, 18, 15 };

    while ((c = getopt(argc, argv, "c:b:s:o:x:m:h")) != -1) {
        switch (c) {
            case 'c':
                config.sushi_chefs = atoi(optarg);
                break;
            case 'b':
                config.conveyor_belt_capacity = atoi(optarg);
                break;
            case 'o':
                config.opening_time = atoi(optarg);
                break;
            case 'x':
                config.closing_time = atoi(optarg);
                break;
            case 'm':
                config.clock_speed_multiplier = atoi(optarg);
                break;
            case 'h':
                help(argv);
                exit(EXIT_SUCCESS);
            default:
                exit(EXIT_FAILURE);
        }
    }

    int abort = FALSE;
    simulation_summary(&config);

    if (config.sushi_chefs < 1) {
        fprintf(stdout, BROWN "[ABORTING] Number of sushi chefs (-c) must be equal or larger than 1.\n" NO_COLOR);
        abort = TRUE;
    }

    if (config.conveyor_belt_capacity < 10) {
        fprintf(stdout, BROWN "[ABORTING] The conveyor belt capacity (-b) must not be lower than 10.\n" NO_COLOR);
        abort = TRUE;
    }

    if (config.opening_time >= config.closing_time) {
        fprintf(stdout, BROWN "[ABORTING] Opening time (-o) must happen before the closing time (-x).\n" NO_COLOR);
        abort = TRUE;
    }

    if (config.clock_speed_multiplier < 1) {
        fprintf(stdout, BROWN "[ABORTING] Clock speed multiplier (-m) must be equal or larger than 1.\n" NO_COLOR);
        abort = TRUE;
    }

    if (abort == TRUE) {
        exit(EXIT_SUCCESS);
    }

    return config;
}

void help(char** argv) {
    title();
    description();
    usage(argv[0]);
    version();
    bar();
}

void simulation_summary(config_t* config) {
    title();
    configuration(config);
    separator();
}

void title() {
    fprintf(stdout, MAGENTA TITLE NO_COLOR);
}

void bar() {
    fprintf(stdout, MAGENTA BAR NO_COLOR);
}

void separator() {
    fprintf(stdout, MAGENTA SEPARATOR NO_COLOR);
}

void usage(char* program_name) {
    fprintf(stdout, MAGENTA "Usage: " NO_COLOR "%s [OPTIONS]\n", program_name);
    fprintf(stdout, MAGENTA "Options:\n" NO_COLOR);
    fprintf(stdout, "  -c  Number of sushi chefs (default=2).\n");
    fprintf(stdout, "  -b  Conveyor belt capacity (default=50).\n");
    fprintf(stdout, "  -o  Opening time (default=10).\n");
    fprintf(stdout, "  -x  Closing time (default=18).\n");
    fprintf(stdout, "  -m  Clock speed multiplier (default=60).\n");
    fprintf(stdout, "  -h  Prints this help.\n");
}

void configuration(config_t* config) {
    fprintf(stdout, MAGENTA "Simulation configuration parameters:\n" NO_COLOR);
    fprintf(stdout, GREEN "  Sushi Chefs             " NO_COLOR "%d\n", config->sushi_chefs);
    fprintf(stdout, GREEN "  Conveyor Belt Capacity  " NO_COLOR "%d\n", config->conveyor_belt_capacity);
    fprintf(stdout, GREEN "  Opening time            " NO_COLOR "%dh\n", config->opening_time);
    fprintf(stdout, GREEN "  Closing time            " NO_COLOR "%dh\n", config->closing_time);
    fprintf(stdout, GREEN "  Clock speed multiplier  " NO_COLOR "%dx\n", config->clock_speed_multiplier);
}

void description() {
    fprintf(stdout, MAGENTA "Description: " NO_COLOR);
    fprintf(stdout, "This is the first concurrent programming assignment for ine5410 2022.2!\n");
}

void version() {
    fprintf(stdout, MAGENTA "Version: " GRAY "%s\n" NO_COLOR, __PROGRAM_VERSION__);
}

