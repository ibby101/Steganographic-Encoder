#include "image_header.h"

/*
* The hash function written below is used in the random shuffling process
* of the encoder for problem b, it takes a string password and converts it 
* into an unsigned long, which a random number generator (RNG) can use to
* provide a way to scramble the points in our encoded image.
*/

unsigned long password_hasher(unsigned char* str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

// Hash function obtained from the following website: http://www.cse.yorku.ca/~oz/hash.html

// shuffling algorithm is defined below, created function to improve modularity and readability.
// takes a vector of (i, j) points stored during the encoding process of the carrier image
// as well as a pseudo-random number, generated from our hashed password.

void shuffling_algorithm(vector<Point>& locations, RNG& rng) {
	int n = locations.size(); 

	for (int i = n - 1; i > 0; --i) {
		int j = rng.uniform(0, i + 1);

		swap(locations[i], locations[j]);
	}
}


/*
* The function defined below takes 3 arguments, a message image,
* a carrier image, and a password in the form of an unsigned char																.
* the password is fed into a hashing function provided in the assignment specifications.
* we use the output from this hash function to scramble our pixel order
* this helps improve our encoding security.
*/

Mat image_encoder_b(Mat& message, Mat& carrier, unsigned char* pass) {

	// starting with input validation again

	if (message.empty() || carrier.empty()) {
		throw runtime_error("Carrier or Message image is empty.");
	}

	// checking that inputs are the same dimensions

	if (carrier.rows != message.rows || carrier.cols != message.cols) {
		throw runtime_error("Carrier & Message images do not have the same dimensions");
	}

	// creating a copy of the carrier image for our encoded image

	Mat encoded_image = carrier.clone();

	// defining row and column variables derived from carrier image.

	int c_rows = carrier.rows;
	int c_cols = carrier.cols;

	// storing the total number of pixels in another variable 

	int total_pixels = c_rows * c_cols;

	// generating the seed from the password hash

	unsigned long seed = password_hasher(pass);

	// creating a RNG using the generated seed

	RNG rng(seed);

	// creating a vector to hold the pixel  (i, j) locations (represented as Points)

	vector<Point> locations;

	// populating the vector with the pixel locations of the carrier image

	for (int i = 0; i < c_rows; ++i) {
		for (int j = 0; j < c_cols; ++j) {
			locations.push_back(Point(j, i));
		}
	}

	// shuffling the pixel locations based on the password hash
	// this is done using the following shuffling algorithm (fisher-yates).

	shuffling_algorithm(locations, rng);

	int message_index = 0;
	for (int i = 0; i < total_pixels; ++i) {
		Point coord = locations[i];

		// converting 1d index to 2d (row/col)
		int row = message_index / c_cols;
		int col = message_index % c_cols;

		// encode message pixel into shuffled location
		uchar msg_pixel_value = message.at<uchar>(row, col);

		// determining the single bit to hide (0 for black, 1 for non-black)
		uchar message_bit = (msg_pixel_value > 0) ? 1 : 0;

		// getting the current pixel value from the image we are modifying
		// (coord.y is row, coord.x is column for Point)
		uchar& target_pixel = encoded_image.at<uchar>(coord.y, coord.x);

		// Clear the LSB of the target pixel and set it to the message bit
		target_pixel = (target_pixel & 0xFE) | message_bit;

		// incrementing the index position

		++message_index;
	}

	return encoded_image;

}

