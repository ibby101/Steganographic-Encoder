//#include "image_header.h"
//
//int main(int argc, char** argv) {
//
//	// as previous main files have done, validating,
//	// visual clarity.
//
//	string file = file_name(argv[0]);
//
//	if (argc != 4) {
//		cerr << "Incorrect format, please use: " << file
//			<< "Encoder: <message_image> <carrier_image> <password>\n" <<
//			"Decoder: <encoded_image> <carrier_image> <password>" << endl;
//		return -1;
//	}
//
//	// storing message and carrier images as grayscale and color respectively,
//	// color in this case represents BGR, which opencv uses instead of RGB.
//
//	//try {
//	//	Mat message = imread(argv[1], IMREAD_GRAYSCALE);
//	//	Mat carrier = imread(argv[2], IMREAD_COLOR);
//
//	//	// storing password taken from arguments
//
//	//	unsigned char* password = reinterpret_cast<unsigned char*>(argv[3]);
// 
//	//  cout << "Password Used: " << password << endl;
//
//	//	// sends information to problem d's image encoder, which outputs an encoded image
//	//	// image stored in variable
//
//	//	Mat encoded_image = image_encoder_d(message, carrier, password);
//
//	//	// outputting results
//
//	//	imwrite("encoded_image_d.png", encoded_image);
//	//	imshow("Message", message);
//	//	imshow("Carrier", carrier);
//	//	imshow("Encoded", encoded_image);
//	//	waitKey(0);
//	//}
//	//catch (const exception& e) {
//	//	cerr << "Error:" << e.what() << endl;
//	//}
//
//	try {
//		Mat encoded = imread(argv[1], IMREAD_COLOR);
//		Mat carrier = imread(argv[2], IMREAD_COLOR);
//
//		// storing password taken from arguments
//
//		unsigned char* password = reinterpret_cast<unsigned char*>(argv[3]);
// 
//		cout << "Password Used: " << password << endl;
//
//		// sends information to problem d's image decoder, which outputs an decoded image
//		// image stored in variable
//
//		Mat decoded_image = image_decoder_d(encoded, carrier, password);
//
//		// outputting results
//
//		imwrite("decoded_image_d.png", decoded_image);
//		imshow("Encoded", encoded);
//		imshow("Carrier", carrier);
//		imshow("Decoded", decoded_image);
//		waitKey(0);
//	}
//	catch (const exception& e) {
//		cerr << "Error:" << e.what() << endl;
//	}
//
//
//	return 0;
//
//
//}