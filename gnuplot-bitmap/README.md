# gnuplot-bitmap

Small experiment looking at displaying a black and white image with gnuplot.

## Usage

Prerequisites:

- A C compiler. clang is the default, but it also works with GCC; just change `CC` in the `Makefile`.
- GNU make
- gnuplot

Steps:

- Clone this repository with `--recurse-submodules` in order to fetch the STB header.
	- If you already cloned without that option, run `git submodule update --init --recursive` to fetch it after the fact.
- Compile:

```sh
$ make
```

- Now run `./gnuplot-bitmap` with whichever options you like.

```
usage: ./gnuplot-bitmap -i infile -o outfile [-Idh] [-s point_size] [-t threshold] [-a alpha_threshold] [-r dim]
    -i infile:          image to use as input. most common formats are supported.
    -o outfile:         PDF output file.
    -s point_size:      (default 0.25) size of each point in the plot.
    -t threshold:       (default 128) pixels with grayscale values below (default) or
                        above (with -I) this are plotted. 0-255.

    -a alpha_threshold: (default 128) pixels with alpha values below this are not
                        plotted, no matter their grayscale value. 0-255.

    -r max_dim:         if input image is larger than dim on one dimension, resize it to
                        fit within a dim x dim square, preserving aspect ratio.

    -I:                 plot pixels above threshold instead of below.
    -d:                 output a data file instead of plotting anything. outfile is not
                        required; the default is stdout. each line is of the form 'x y'.
                        y coordinates are negative.

    -h:                 display this help and exit.
```

## Intended future features

- Use a temporary file instead of a pipe to send the data into gnuplot (should fix execution on macOS).
- Support dithering the image instead of using a simple threshold.
