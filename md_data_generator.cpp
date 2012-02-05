#include <iostream>
#include <ctime>

int main (int argc, char **argv)
{
	int num_coordinates = 100;
	if (argc > 1) {
		puts(argv[1]);
		sscanf(argv[1], "%d", &num_coordinates);
	}

	srand(time(NULL));
	for (int i = 0; i < num_coordinates; i++)
	{
		printf("%d\t%d\t%d\n", rand(), rand(), rand());
	}

	return 0;
}
