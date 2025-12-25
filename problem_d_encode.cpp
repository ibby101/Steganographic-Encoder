#include "image_header.h"

Mat image_encoder_d(Mat& message, Mat& carrier, unsigned char* pass) {

	// ----------------------- VALIDATION SECTION ------------------------------

	// validating if images exist

	if (message.empty() or carrier.empty()) {
		throw runtime_error("Carrier or Message image is empty.");
	}

	// checking that both message and carrier image are the same size

	if (message.size() != carrier.size()) {
		throw runtime_error("Carrier and Message image must be the same size.");
	}

	// if car rier image is not colour image,
	// throws runtime error explaining this.

	if (carrier.type() != CV_8UC3) {
		throw runtime_error("Carrier image is not colour");
	}

	// ----------------------- HASHING & ADDING NOISE ------------------------------
	
	// using password argument to provide seed

	unsigned long seed = password_hasher(pass);

	// applying RNG to seed 

	RNG rng(seed);

	// creating a noisy carrier image to start, using the password provided
	// not using seed as RNG is already used inside the seeded_gaussian function.

	Mat noise_carrier = seeded_gaussian(carrier, pass);

	// creating a copy of the noisy carrier image for the encoded output to use as a foundation.

	Mat encoded_image = noise_carrier.clone();

	// storing row and column data from the carrier image, can be message image too,
	// as same size is assumed.

	int rows = carrier.rows;
	int cols = carrier.cols;

	// storing total number of message pixels.

	long m_pixels = (long)rows * cols;

	// storing the number of carrier pixels and multiplying by 3, for BGR.

	long total_c_pixels = (long)rows * cols * 3;

	// checking that message pixels are not greater than the number of carrier pixels.

	if (m_pixels > total_c_pixels) {
		throw runtime_error("The number of message pixels is greater than carrier pixels.");
	}

	// ----------------------- TRACKING CHANGED PIXELS IN BGR ------------------------------
	
	// creating a BGR matrix filled with zeros as structure to store changed pixels.

	Mat store_changes = Mat::zeros(rows, cols, CV_8UC3);

	// counter used for incrementing

	long changes_made = 0;

	// ---------------------------- ENCODING THE MESSAGE -----------------------------------

	for (int m_rows = 0; m_rows < rows; ++m_rows) {
		for (int m_cols = 0; m_cols < cols; ++m_cols) {

			// stores pixel data from current message pixel

			uchar m_pixel = message.at<uchar>(m_rows, m_cols);

			// checks if the value for the pixel is 0 (black),
			// meaning it contains a message, sets bit to 1, otherwise 0.

			uchar m_bit = (m_pixel == 0) ? 1 : 0;

			// NEXT: finding a pixel across the 3 channels that can be encoded into.

			// r, c represent rows and columns, will later be used to apply RNG to them.

			int r, c, channel_idx;

			// boolean that is set to true once a suitable image slot is found.

			bool pixel_found = false;

			// counting the total number of encoding attempts across 3 channels

			int total_attempts = 0;

			// setting limit for number of attempts when encoding

			int max_attempt = total_c_pixels * 2;

			while (!pixel_found) {

				// ensuring that the attempts does not surpass the maximum amount set.

				if (++total_attempts > max_attempt) {
					throw runtime_error("Coudn't find a suitable pixel to encode to.");
				}

				// generating a random location for rows/cols

				r = rng.uniform(0, rows);
				c = rng.uniform(0, cols);

				// '3' represents the 3 channels for BGR

				channel_idx = rng.uniform(0, 3);

				// checks if the position in our matrix of stored encoded pixels has been used.

				bool already_encoded = (store_changes.at<Vec3b>(r, c)[channel_idx] != 0);

				// checks if the current position on our encoded image we want to modify is already white (255).

				bool is_overflowing = (encoded_image.at<cv::Vec3b>(r, c)[channel_idx] == 255);

				// if the above booleans are not true, i.e., the position hasn't already been encoded, and not 
				// already containing a value of 255, it will set the original boolean to true and use this pixel
				// for encoding.

				if (!already_encoded && !is_overflowing) {
					pixel_found = true;

				}
			}

			// updating the matrix that tracks any changes made to the carrier image.
			// increments change counter

			store_changes.at<Vec3b>(r, c)[channel_idx] = 1;
			++changes_made;

			uchar& selected_pixel = encoded_image.at<Vec3b>(r, c)[channel_idx];

			// clearing LSB, and applying chosen message bit to pixel position.

			selected_pixel = (selected_pixel & 0xFE) | m_bit;


		}
	}
	cout << "Total Pixels Encoded Across Channels:" << changes_made << endl;
	return encoded_image;

}