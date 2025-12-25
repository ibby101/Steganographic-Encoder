#include "image_header.h"


/*
* The purpose of this function is to take a message in the form of an image,
* an image that would have the message encoded into it, known as a "carrier image",
* and then slightly adjusts the pixels in the carrier image that correspond to the message image.
*/

Mat image_encoder_a(Mat& message, Mat& carrier) {

	// creating a new Mat image, copying the carrier's size
	// storing row and column data for future use.

	Mat encoded_carrier = carrier.clone();

	int rows = carrier.rows;
	int cols = carrier.cols;

	// validating inputs for images.
	// throws runtime error if inputs are invalid.

	if (carrier.empty() || message.empty()) {
		throw runtime_error("Carrier or Message image is empty.");
	}

	// checking that inputs are the same dimensions

	if (carrier.rows != message.rows || carrier.cols != message.cols) {
		throw runtime_error("Carrier & Message images do not have the same dimensions");
	}

	/*
	* Below we iterate over each pixel in the carrier and
	* message images simultaneously. Whenever it reaches 
	* a black pixel in the message image, encodes it into
	* the carrier image in the same position.	
	*/

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {

			// obtaining pixel value at position (i, j) for message img.

			uchar m_pix = message.at<uchar>(i, j);

			// obtaining pixel value at position (i, j) for carrier img.

			uchar c_pix = carrier.at<uchar>(i, j);

			// initialising the pixel variable for the encoded image.

			uchar encoded_pix;

			// if-else statement to check if the current message pixel is black or white,
			// denoting message or no message. if there is a message (black), then
			// adjust the least significant bit (LSB) of the new encoded carrier.
			// otherwise, encoded pixel is equal to the carrier pixel.

			if (m_pix != 0) {
				encoded_pix = c_pix;
			}
			else {
				encoded_pix = c_pix ^ 0x01; // using the XOR operator to flip LSB.
			}

			// updating the pixel data at the new position in encoded carrier image.

			encoded_carrier.at<uchar>(i, j) = encoded_pix;
		}
	}

	return encoded_carrier;
}