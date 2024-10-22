#include "stb_image.h"
#include "stb_image_resize.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const char *script = "set term pdf size %fin,%fin\n"
                     "set size ratio %f\n"
                     "set output '%s'\n"
                     "unset key\n"
                     "unset tics\n"
                     "set xrange [0:%d]\n"
                     "set yrange [-%d:0]\n"
                     "plot '/proc/self/fd/%d' with points pointtype 7 pointsize %f\n";

#define OPTIONS "i:o:s:t:a:r:Idh"

void usage(const char *program_name) {
	fprintf(stderr,
	    "usage: %s -i infile -o outfile [-Idh] [-s point_size] [-t threshold] [-a "
	    "alpha_threshold] [-r dim]\n"
	    "    -i infile:          image to use as input. most common formats are supported.\n"
	    "    -o outfile:         PDF output file.\n"
	    "    -s point_size:      (default 0.25) size of each point in the plot.\n"
	    "    -t threshold:       (default 128) pixels with grayscale values below (default) or\n"
	    "                        above (with -I) this are plotted. 0-255.\n"
	    "\n"
	    "    -a alpha_threshold: (default 128) pixels with alpha values below this are not\n"
	    "                        plotted, no matter their grayscale value. 0-255.\n"
	    "\n"
	    "    -r max_dim:         if input image is larger than dim on one dimension, resize it to\n"
	    "                        fit within a dim x dim square, preserving aspect ratio.\n"
	    "\n"
	    "    -I:                 plot pixels above threshold instead of below.\n"
	    "    -d:                 output a data file instead of plotting anything. outfile is not\n"
	    "                        required; the default is stdout. each line is of the form 'x y'.\n"
	    "                        y coordinates are negative.\n"
	    "\n"
	    "    -h:                 display this help and exit.\n",
	    program_name);
}

char *infile = NULL, *outfile = NULL;
unsigned char *data;
int width, height;
uint8_t threshold = 128, alpha_threshold = 128;
bool invert = false;
int max_dimension = 0;

void cleanup(void) {
	if (infile) {
		free(infile);
	}
	if (outfile) {
		free(outfile);
	}
	if (data) {
		stbi_image_free(data);
	}
}

void read_image(const char *program_name) {
	// load image as grayscale + alpha (2 channels)
	data = stbi_load(infile, &width, &height, NULL, 2);
	if (!data) {
		fprintf(stderr, "%s: failed to decode input '%s': %s\n", program_name, infile,
		    stbi_failure_reason());
		cleanup();
		exit(1);
	}

	if (max_dimension > 0 && (width > max_dimension || height > max_dimension)) {
		int scaled_width, scaled_height;
		double ratio = 1.0 * width / height;
		if (width > height) {
			scaled_width = max_dimension;
			scaled_height = max_dimension / ratio;
		} else {
			scaled_height = max_dimension;
			scaled_width = scaled_height * ratio;
		}

		// set to 1 instead of 0
		if (!scaled_width) {
			scaled_width = 1;
		}
		if (!scaled_height) {
			scaled_height = 1;
		}

		printf("resizing %dx%d -> %dx%d\n", width, height, scaled_width, scaled_height);

		unsigned char *new_data
		    = (unsigned char *) malloc(scaled_width * scaled_height * 2 * sizeof(unsigned char));
		if (!new_data) {
			fprintf(stderr, "%s: failed allocating buffer for resized image\n", program_name);
			cleanup();
			exit(1);
		}

		int result = stbir_resize_uint8_srgb(
		    data, width, height, 0, new_data, scaled_width, scaled_height, 0, 2, 1, 0);

		if (!result) {
			fprintf(stderr, "%s: error while resizing image\n", program_name);
			cleanup();
			free(new_data);
			exit(1);
		}

		stbi_image_free(data);
		data = new_data;
		width = scaled_width;
		height = scaled_height;
	}
}

char *my_strdup(const char *s) {
	size_t len = strlen(s);
	char *s2 = (char *) malloc((len + 1) * sizeof(char));
	strncpy(s2, s, len + 1);
	return s2;
}

void write_data(int fd) {
	// iterate through pixels
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			unsigned char gray = data[2 * (y * width + x)], alpha = data[2 * (y * width + x) + 1];

			bool plot = invert ? (gray > threshold) : (gray < threshold);
			plot = plot && (alpha > alpha_threshold);
			if (plot) {
				// send it to the data stream
				// y is negated because gnuplot's positive y extends upwards
				dprintf(fd, "%d %d\n", x, -y);
			}
		}
	}
}

int main(int argc, char **argv) {
	bool data_output = false;
	double point_size = 0.25;

	if (argc == 1) {
		usage(argv[0]);
		return 1;
	}

	int opt = 0;
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
			case 'i':
				infile = my_strdup(optarg);
				if (!infile) {
					fprintf(stderr, "%s: failed allocating infile string\n", argv[0]);
					cleanup();
					return 1;
				}
				break;
			case 'o':
				outfile = my_strdup(optarg);
				if (!outfile) {
					fprintf(stderr, "%s: failed allocating infile string\n", argv[0]);
					cleanup();
					return 1;
				}
				break;
			case 's':
				point_size = strtod(optarg, NULL);
				if (point_size <= 0) {
					fprintf(
					    stderr, "%s: invalid point size %f. must be >0.\n", argv[0], point_size);
					cleanup();
					return 1;
				}
				break;
			case 't': {
				unsigned long t = strtoul(optarg, NULL, 10);
				if (t > 255) {
					fprintf(stderr, "%s: invalid threshold %lu: must be <= 255\n", argv[0], t);
					cleanup();
					return 1;
				} else {
					threshold = t;
				}
				break;
			}
			case 'a': {
				unsigned long a = strtoul(optarg, NULL, 10);
				if (a > 255) {
					fprintf(
					    stderr, "%s: invalid alpha threshold %lu: must be <= 255\n", argv[0], a);
					cleanup();
					return 1;
				} else {
					alpha_threshold = a;
				}
				break;
			}
			case 'r':
				max_dimension = atoi(optarg);
				if (max_dimension < 1) {
					fprintf(stderr, "%s: invalid max dimension %d: must be >= 1\n", argv[0],
					    max_dimension);
					cleanup();
					return 1;
				}
				break;
			case 'I': invert = true; break;
			case 'd': data_output = true; break;
			case 'h': usage(argv[0]); return 0;
			default: usage(argv[0]); return 1;
		}
	}

	if (!infile) {
		fprintf(stderr, "%s: error: no input specified\n", argv[0]);
		cleanup();
		return 1;
	}

	if (data_output) {
		int out_fd = STDOUT_FILENO;
		if (outfile) {
			out_fd = open(outfile, O_CREAT | O_TRUNC | O_WRONLY);
			if (out_fd < 0) {
				perror("error opening output file");
				cleanup();
				return 1;
			}
		}

		read_image(argv[0]);
		write_data(out_fd);
		close(out_fd);
	} else {
		if (!outfile) {
			fprintf(stderr, "%s: error: no output specified\n", argv[0]);
			cleanup();
			return 1;
		}

		read_image(argv[0]);

		// set up 2 pipes, one for the gnuplot script and one for the data
		int pipe_script[2], pipe_data[2];
		if (pipe(pipe_script) != 0) {
			perror("error with pipe");
			cleanup();
			return 1;
		}
		if (pipe(pipe_data) != 0) {
			perror("error with pipe");
			cleanup();
			return 1;
		}

		if (fcntl(pipe_script[0], F_SETFL, O_NONBLOCK) < 0) {
			perror("error with fcntl");
			cleanup();
			return 1;
		}
		if (fcntl(pipe_data[0], F_SETFL, O_NONBLOCK) < 0) {
			perror("error with fcntl");
			cleanup();
			return 1;
		}

		pid_t pid = fork();
		if (pid == 0) {
			// child
			// send script that we're gonna receive to stdin (which will be gnuplot's stdin)
			if (dup2(pipe_script[0], STDIN_FILENO) < 0) {
				perror("error with dup2");
				return 1;
			}
			// close our write ends (parent still has them open)
			close(pipe_script[1]);
			close(pipe_data[1]);
			// run gnuplot
			execlp("gnuplot", "gnuplot", (char *) NULL);
			// if we're here, execlp returned instead of replacing our code
			perror("error calling gnuplot");
		} else {
			// parent
			// close our read ends (child still has them open)
			close(pipe_script[0]);
			close(pipe_data[0]);
			// send the script to the child process
			double ratio = 1.0 * height / width, page_width = ratio < 1.0 ? 6.0 : 5.0 / ratio + 1.0,
			       page_height = ratio < 1.0 ? 5.0 * ratio + 1.0 : 6.0;
			dprintf(pipe_script[1], script, page_width, page_height, ratio, outfile, width, height,
			    pipe_data[0], point_size);
			close(pipe_script[1]);
			write_data(pipe_data[1]);
			// we're done
			close(pipe_data[1]);
			// wait for gnuplot to exit
			int wstatus;
			waitpid(pid, &wstatus, 0);
			if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0) {
				fprintf(stderr, "child process exited abnormally\n");
			}
		}
	}

	cleanup();
	return 0;
}
