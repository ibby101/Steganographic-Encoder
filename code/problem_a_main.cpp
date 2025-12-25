#include "image_header.h"
//
// -------------- FUNCTION FOR VISUAL CLARITY ON EXECUTABLE ----------------------	

// removes the file path up to the ".exe" file shown in the error outputs,
// e.g., C:/users/you/folder1/folder1/main.exe removes the lines prior to the ".exe"
// this is to reduce the amount of visual clutter on the error output when showing
// the argument format for the main functions.

string file_name(const string& path) {
	size_t get_file_name = path.find_last_of("/\\");

	if (get_file_name == string::npos) {
		return path;
	}
	else {
		return path.substr(get_file_name + 1);
	}
}
//
//// -------------- MAIN FUNCTION FOR ENCODING AND DECODING PROBLEM A ----------------------	
//
//int main(int argc, char** argv) {
//
//	// checking if the format of the arguments is correct
//	// returning an error message that explains the format if wrong.
//
//	if (argc > 0) {
//		string file_path = argv[0];
//		string exe_name = file_name(file_path);
//		if (argc != 3) {
//			cerr << "Incorrect format, please use:\n Encoding:\t" << exe_name
//				<< " <message_image> <carrier_image>\n Decoding:\t" << exe_name
//				<< " <encoded_image> <carrier_image>" << endl;
//			return -1;
//		}
//	}
//
//	/*
//	* using try-catch block for error handling if any others occur
//	* stores message and carrier images in respective variables 
//	* stores encoded result in variable "encoded" 
//	* or decoded message is stored in variable "decoded"
//	* outputs decoded image and writes encoded output to new image.
//	* please uncomment the relevant try-catch block for encoding or decoding.
//	* make sure to uncomment the needed block when swapping using CTRL + K + U.
//	*/
//
//	// -------------- ENCODER BLOCK FOR MAIN FUNCTION ----------------------	
//
//	//try {
//	//	Mat message = imread(argv[1], IMREAD_GRAYSCALE);
//	//	Mat carrier = imread(argv[2], IMREAD_GRAYSCALE);
//
//	//	Mat encoded = image_encoder_a(message, carrier);
//
//	//	imwrite("encoded_image_a.png", encoded);
//	//	imshow("Message", message);
//	//	imshow("Carrier", carrier);
//	//	imshow("Encoded", encoded);
//	//	waitKey(0);
//	//}
//	//catch (const exception& e) {
//	//	cerr << "Error:" << e.what() << endl;
//	//}
//	
//
//	// -------------- DECODER BLOCK FOR MAIN FUNCTION ----------------------	
//
//	try {
//		Mat encoded = imread(argv[1], IMREAD_GRAYSCALE);
//		Mat carrier = imread(argv[2], IMREAD_GRAYSCALE);
//
//		Mat decoded = image_decoder_a(encoded, carrier);
//
//		imwrite("decoded_image_testing.png", decoded);
//		imshow("Encoded", encoded);
//		imshow("Decoded", decoded);
//		waitKey(0);
//	}
//	catch (const exception& e) {
//		cerr << "Error:" << e.what() << endl;
//	}
//
//	return 0;
//
//}