#include "image_header.h"

/*
* The function defined below takes 3 arguments, a message image,
* a carrier image, and a password in the form of an unsigned char																.
* we essentially carry out similar steps to reach the shuffled state of pixels
* then sequentially read the encoded image pixels, applies decoding steps
* outputs to new decoding image
*/

Mat image_decoder_b(Mat& encoded, Mat& carrier, unsigned char* pass) {

	// starting with input validation again

	if (encoded.empty() || carrier.empty()) {
		throw runtime_error("Carrier or Encoded image is empty.");
	}

	// checking that inputs are the same dimensions

	if (carrier.rows != encoded.rows || carrier.cols != encoded.cols) {
		throw runtime_error("Carrier & Encoded images do not have the same dimensions");
	}

	// creating decoded encoded image, using carrier image to set the size
	// set to grayscale

	Mat decoded_message = Mat::zeros(carrier.size(), CV_8UC1);

	int cols = carrier.cols;
	int rows = carrier.rows;

	// obtaining the seed again, using the same password hashing method

	unsigned long seed = password_hasher(pass);
	

	// using a RNG on the obtained seed

	RNG rng(seed);

	// creating a vector to hold the pixel (i, j) locations (represented as Points)

	vector<Point> locations;

	// populating the vector with the pixel locations of the carrier image

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			locations.push_back(Point(j, i));
		}
	}

	// here, the process diverges, and we "de-shuffle".
	// we read the encoded image using the shuffled locations,
	// then write the outputs to the decoded image.

	shuffling_algorithm(locations, rng);

	int message_index = 0;
	for (int i = 0; i < locations.size(); ++i) {
		Point coord = locations[i];

		// converting row and column data back to 2d

		int row = message_index / cols;
		int col = message_index % cols;


		// storing encoded pixel value at shuffled position
			
		uchar encoded_pixel = encoded.at<uchar>(coord.y, coord.x);

		// storing carrier pixel value at shuffled position

		uchar carrier_pixel = carrier.at<uchar>(coord.y, coord.x);

		// storing LSB of carrier_pixel for comparison

		uchar carrier_lsb = carrier_pixel & 0xFE;

		// comparing carrier with LSB reset, using "0xFE", to encoded pixel
		// we reset the LSB to standardise the carrier image.

		uchar extracted_bit;
		if (encoded_pixel == carrier_lsb) {
			extracted_bit = 0; // they are equal only if the hidden bit was 0
		}
		else {
			extracted_bit = 1; // they are different if the hidden bit was 1
		}

		// mapping the extracted bit to the output pixel value
		// if they are the same, outputs white pixel for that position
		// otherwise, black pixel for decoded message.
		uchar decoded_pixel_value = (extracted_bit == 0) ? 0 : 255;
		decoded_message.at<uchar>(row, col) = decoded_pixel_value;


		// incrementing the index

		++message_index;
	}	

	return decoded_message;

}

