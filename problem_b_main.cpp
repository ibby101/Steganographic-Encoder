#include "image_header.h"

// -------------- MAIN FUNCTION FOR ENCODING AND DECODING PROBLEM B ----------------------	

//int main(int argc, char** argv) {
//
//	// checking if the format of the arguments is correct
//	// returning an error message that explains the format if wrong.
//
//	if (argc > 0) {
//		string file_path = argv[0]; // storing executable file name after input
//		string exe_name = file_name(file_path);
//		if (argc != 4) {
//			cerr << "Incorrect format, please use:\n Encoding:\t" << exe_name
//				<< " <message_image> <carrier_image> <password>\n Decoding:\t" << exe_name
//				<< " <encoded_image> <carrier_image> <password>" << endl;
//			return -1;
//		}
//	}
//
//	/*
//	* This time, the encoder function takes 3 arguments,
//	* a message image, a carrier image, and a password
//	* the password is used to scramble the encoding process
//	* without the password, the image cannot be decoded.
//	*/
//
//	// -------------- ENCODER BLOCK FOR MAIN FUNCTION ----------------------	
//
//	//try {
//	//	Mat message = imread(argv[1], IMREAD_GRAYSCALE);
//	//	Mat carrier = imread(argv[2], IMREAD_GRAYSCALE);
//
//	//	unsigned char* password = reinterpret_cast<unsigned char*>(argv[3]);
// 
//	//  cout << "Password Used: " << password << endl;
//
//
//
//	//	Mat encoded = image_encoder_b(message, carrier, password);
//
//	//	imwrite("encoded_image_b.png", encoded);
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
//	//try {
//	//	Mat encoded = imread(argv[1], IMREAD_GRAYSCALE);
//	//	Mat carrier = imread(argv[2], IMREAD_GRAYSCALE);
//
//	//	unsigned char* password = reinterpret_cast<unsigned char*>(argv[3]);
// 
//	//  cout << "Password Used: " << password << endl;
//
//	//	Mat decoded = image_decoder_b(encoded, carrier, password);
//
//	//	imwrite("decoded_image_b.png", decoded);
//	//	imshow("Encoded", encoded);
//	//	imshow("Decoded", decoded);
//	//	waitKey(0);
//	//}
//	//catch (const exception& e) {
//	//	cerr << "Error:" << e.what() << endl;
//	//}
//
//	return 0;
//
//}