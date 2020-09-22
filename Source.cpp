#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
using namespace std;

// We'll store image info as globals; not great programming practice
// but ok for this short program.
int size;
bool** image;

void renderPixel(int x, int y, int r) {
	assert(x >= 0 && y >= 0 && x <= ::size && y <= ::size);
	image[x][y] = 1;
	image[y][x] = 1;
	//this statement lights up the pixels for the larger radius
	if (r == 150)
	{
		image[x][300 - y] = 1;
		image[y][300 - x] = 1;
	}
	//this statement lights up the pixels for the smaller radius
	else if (r == 100)
	{
		image[300 - x][y] = 1;
		image[300 - y][x] = 1;
	}
	// TODO:  light up the pixel's symmetric counterpart
}

void rasterizeArc(int radius) {
	// TODO:  rasterize the arc using renderPixel to light up pixels
	int x = 0;
	int y = radius;
	int r = radius;
	int diameter = 1 - radius;
	//this initializes the first renderpixel
	renderPixel(x + 150, y + 150, r);
	//this uses the midpoint circle scan, based on how close it is to the new pixel
	while (y > x)
	{
		if (diameter < 0)
		{
			diameter += 2 * x + 3;
		}
		else
		{
			diameter += 2 * (x - y) + 5;
			y--;
		}
		x++;
		renderPixel(x + 150, y + 150, r);

	}

}

// You shouldn't need to change anything below this point.

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " circleSize\n";

		return 0;
	}

#ifdef _WIN32
	sscanf_s(argv[1], "%d", &::size);
#else
	sscanf(argv[1], "%d", &::size);
#endif
	if (::size <= 0) {
		cout << "Image must be of positive size.\n";
		return 0;
	}

	// reserve image as 2d array
	image = new bool* [::size + 1];
	for (int i = 0; i <= ::size; i++) image[i] = new bool[::size + 1];

	//i changed this to call both radii
	rasterizeArc(100);
	rasterizeArc(150);

	char filename[50];
#ifdef _WIN32
	sprintf_s(filename, 50, "circle%d.ppm", ::size);
#else
	sprintf(filename, "circle%d.ppm", ::size);
#endif

	ofstream outfile(filename);
	outfile << "P3\n# " << filename << "\n";
	outfile << ::size + 1 << ' ' << ::size + 1 << ' ' << 1 << endl;

	for (int i = 0; i <= ::size; i++)
		for (int j = 0; j <= ::size; j++)
			outfile << image[::size - i][j] << " 0 0\n";

	// delete image data
	for (int i = 0; i <= ::size; i++) delete[] image[i];
	delete[] image;

	return 0;
}