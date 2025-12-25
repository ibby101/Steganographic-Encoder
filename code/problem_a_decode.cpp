#include "image_header.h"

/*
* The purpose of this function is to take an encoded image,
* compare it against the default "carrier" image
* then produce a message image from the decoded pixels.
*/

Mat image_decoder_a(Mat& encoded, Mat& carrier) {

	// creating a decoded image skeleton, using the carrier image to set
	// the size. storing row and column data for later use.

	Mat decoded_message = cv::Mat::zeros(carrier.size(), CV_8UC1);

	int rows = carrier.rows;
	int cols = carrier.cols;

	// validating input for images.
	// throws runtime error if inputs are invalid.

	if (carrier.empty()) {
		throw runtime_error("Carrier image is empty.");
	}
	if (encoded.empty()) {
		throw runtime_error("Encoded image is empty.");
	}

	// checking that inputs are the same dimensions

	if (carrier.rows != encoded.rows || carrier.cols != encoded.cols) {
		throw runtime_error("Carrier & Encoded images do not have the same dimensions");
	}

	/*
	* Below we iterate over each pixel in the carrier and
	* encoded images simultaneously. Whenever it reaches
	* a pixel that differs in the encoded image, outputs a 1
	* to the message image we will produce.
	*/

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {

			// obtaining pixel value at position (i, j) for encoded img.

			uchar e_pix = encoded.at<uchar>(i, j);

			// obtaining pixel value at position (i, j) for carrier img.

			uchar c_pix = carrier.at<uchar>(i, j);

			// compares the encoded pixel and carrier pixel, if they are the same, returns white
			// otherwise, returns black for the message.

			decoded_message.at<uchar>(i, j) = (e_pix == c_pix)? 255 : 0;

		}
	}

	return decoded_message;
}