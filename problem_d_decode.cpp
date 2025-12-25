#include "image_header.h"

Mat image_decoder_d(Mat& encoded_image, Mat& original_carrier, unsigned char* pass) {

	// ----------------------- VALIDATION SECTION ------------------------------

	// check if input images are valid
	if (encoded_image.empty() || original_carrier.empty()) {
		throw runtime_error("Encoded or Carrier image is empty.");
	}

	// check that both images are of the same size
	if (encoded_image.size() != original_carrier.size()) {
		throw runtime_error("Encoded and Carrier image must be the same size.");
	}

	// check if images are in colour format
	if (encoded_image.type() != CV_8UC3 || original_carrier.type() != CV_8UC3) {
		throw runtime_error("Encoded and Carrier images must be colour (CV_8UC3).");
	}

	// ----------------------- HASHING & RECREATING NOISE ------------------------------

	// generate seed from password
	unsigned long seed = password_hasher(pass);

	// set up RNG using the seed
	RNG rng(seed);

	// regenerate the noisy carrier image using the same password
	Mat noise_carrier = seeded_gaussian(original_carrier, pass);

	// extract dimensions of image
	int rows = encoded_image.rows;
	int cols = encoded_image.cols;

	// compute total pixels for message and available slots
	long m_pixels = (long)rows * cols;
	long total_c_pixels = (long)rows * cols * 3;

	// ----------------------- TRACKING DECODED PIXELS ------------------------------

	// structure to track decoded pixels
	Mat store_changes = Mat::zeros(rows, cols, CV_8UC3);
	long decoded_count = 0;

	// create an empty grayscale image to store decoded message
	Mat decoded_msg = Mat::zeros(rows, cols, CV_8UC1);

	// ---------------------------- DECODING THE MESSAGE -----------------------------------

	for (int m_rows = 0; m_rows < rows; ++m_rows) {
		for (int m_cols = 0; m_cols < cols; ++m_cols) {

			// declaring variables to store encoded pixel info
			int r, c, channel_idx;
			bool pixel_found = false;
			int total_attempts = 0;
			int max_attempt = total_c_pixels * 2;

			while (!pixel_found) {
				// break the loop if too many failed attempts
				if (++total_attempts > max_attempt) {
					throw runtime_error("Decoder failed to find a valid slot.");
				}

				// generate random location and channel
				r = rng.uniform(0, rows);
				c = rng.uniform(0, cols);
				channel_idx = rng.uniform(0, 3);

				// check if already used
				bool already_used = (store_changes.at<Vec3b>(r, c)[channel_idx] != 0);

				// check for overflow condition
				bool is_overflowing = (noise_carrier.at<Vec3b>(r, c)[channel_idx] == 255);

				if (!already_used && !is_overflowing) {
					pixel_found = true;
				}
			}

			// mark the pixel as used
			store_changes.at<Vec3b>(r, c)[channel_idx] = 1;
			++decoded_count;

			// extract the encoded pixel and isolate LSB to recover bit
			uchar encoded_val = encoded_image.at<Vec3b>(r, c)[channel_idx];

			uchar noisy_val = noise_carrier.at<Vec3b>(r, c)[channel_idx];


			uchar recovered_bit = noisy_val & 0xFE;

			// comparison between the encoded pixel value, and the pixel value for the noisy carrier.

			uchar decoded_bit;
			if (encoded_val == recovered_bit) {
				decoded_bit = 0;
			}
			else {
				decoded_bit = 1;
			}

			// maps bit to grayscale pixel (0 = white, 1 = black, reverse of encoder)
			decoded_msg.at<uchar>(m_rows, m_cols) = (decoded_bit == 1) ? 0 : 255;
		}
	}

	cout << "Total Decoded Pixels Across Channels: " << decoded_count << endl;
	return decoded_msg;
}