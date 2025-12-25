#include "image_header.h"

/*
* Using gaussian noise, to provide a good distribution of changes across the image.
*/

Mat seeded_gaussian(Mat& image, unsigned char* password) {

	// checking image validity

	if (image.empty()) {
		throw runtime_error("Noise: Image file is empty.");
	}

	// storing password in variable for later use

	unsigned long seed = password_hasher(password);

	// using opencv's global RNG, to provide further consistency.

	theRNG().state = seed;

	// variable that will hold final result of image noise application

	Mat output_image;

	// variable that stores the value for the mean of gaussian blurring parameters

	double noise_mean = 0.0;

	// storing the standard deviation of the blur, and using seeded password.

	double sigma = (seed % 10) + 10.0;

	// creating a clone of the input image for consistency

	Mat noised_image = image.clone();

	// applying seeded noise to new noise image

	randn(noised_image, noise_mean, sigma);

	// saving to output image.

	add(image, noised_image, output_image);

	return output_image;
}