#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

typedef enum {
	invalid_fun, discretize_fun, undiscretize_fun, extract_coordinates_fun
} function_selector;

void usage () { 
	puts("discretize (-d|-u|-e) [-s STEP] [INPUT_FILE OUTPUT_FILE]");
}

int discretize (double f, double step);
double undiscretize (int n, double step);
double square_error (double f, double step);

int main (int argc, char **argv)
{
	static const char opts[] = "dues:";

	double step = 0.00001;
	function_selector f = invalid_fun;
	int opt = getopt(argc, argv, opts);
	while (opt != -1)
	{
		switch (opt)
		{
			case 'd':
				f = discretize_fun;
				break;
			case 'u':
				f = undiscretize_fun;
				break;
			case 'e':
				f = extract_coordinates_fun;
				break;
			case 's':
				sscanf(optarg, "%lf", &step);
				printf("%lf\n", step);
				break;
			default:
				usage();
				exit(1);
				break;
		}
		opt = getopt(argc, argv, opts);
	}

	FILE *in = stdin;
	FILE *out = stdout;
	if (optind < argc)
	{
		in = fopen(argv[optind], "r");
		out = fopen(argv[optind + 1], "w");
		if (!in || !out)
		{
			puts("Cannot find file.");
			exit(1);
		}
	}

	int count = 0;
	if (f == extract_coordinates_fun)
	{
		char first_line[100];
		fgets(first_line, 100, in);

		char type[100];
		int i, j, k;
		double x, y, z;
		double u, v;
		while (true)
		{
			fscanf(in, "%s %d %d %d %lf %lf %lf %lf %lf", type, &i, &j, &k, &x, &y, &z, &u, &v);
			if (feof(in))
			{
				break;
			}
			fprintf(out, "%lf\t%lf\t%lf\n", x, y, z);
		}
	}
	else if (f == discretize_fun)
	{
		double square_error_sum = 0;

		double x, y, z;
		while (true)
		{
			fscanf(in, "%lf\t%lf\t%lf", &x, &y, &z);
			if (feof(in))
			{
				break;
			}
			fprintf(out, "%d\t%d\t%d\n", discretize(x, step), 
				discretize(y, step), discretize(z, step));
			square_error_sum += square_error(x, step) + square_error(y, step) +
				square_error(z, step);
			count++;
		}
		double avg_error = sqrt(square_error_sum) / count;
		fprintf(stderr, "Average error: %lf\n", avg_error);
	} 
	else if (f = undiscretize_fun)
	{
		int x, y, z;
		while (true)
		{
			fscanf(in, "%d\t%d\t%d", &x, &y, &z);
			if (feof(in))
			{
				break;
			}
			fprintf(out, "%lf\t%lf\t%lf\n", undiscretize(x, step), 
				undiscretize(y, step), undiscretize(z, step));
		}
	}
}

int discretize (double f, double step)
{
	return (int)(f / step);
}

double undiscretize (int n, double step)
{
	return (double)n * step;
}

double square_error (double f, double step)
{
	return pow(f - undiscretize(discretize(f, step), step), 2);
}
