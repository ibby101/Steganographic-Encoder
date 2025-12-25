#include "image_header.h"


// -------------- MAIN FUNCTION FOR ENCODING AND DECODING PROBLEM E ----------------------	

int main(int argc, char** argv) {

	// checking if the format of the arguments is correct
	// returning an error message that explains the format if wrong.

	if (argc > 0) {
		string file_path = argv[0];
		string exe_name = file_name(file_path);
		if (argc != 5) {
			cerr << "Incorrect format, please use:\n"
				<< " Encoding:\t" << exe_name << " <carrier_image> <message_file> <output_path> <password>\n"
				<< " Decoding:\t" << exe_name << " <encoded_image> <original_carrier> <output_path> <password>" << endl;
			return -1;
		}
	}

	// assigning input arguments to clearly named variables for readability
	const char* input1_path = argv[1];  // could be carrier image (encode) or encoded image (decode)
	const char* input2_path = argv[2];  // could be message file (encode) or original carrier image (decode)
	const char* output_path = argv[3];  // output file path (image or decoded file)
	unsigned char* password = reinterpret_cast<unsigned char*>(argv[4]); // password (used for seeding)

	cout << "Main: Password used = " << password << endl;


	// -------------- ENCODER BLOCK FOR MAIN FUNCTION ----------------------	

	
	//try {
	//	cout << "Main: [Encoding Mode Active]" << endl;

	//	Mat carrier = imread(input1_path, IMREAD_COLOR);
	//	if (carrier.empty()) {
	//		cerr << "Main Error: Failed to load carrier image from '" << input1_path << "'" << endl;
	//		return -1;
	//	}

	//	string message_file_path = input2_path;
	//	Mat encoded = image_encoder_e(message_file_path, carrier, password);

	//	cout << "Main: Saving encoded image to '" << output_path << "'..." << endl;
	//	if (!imwrite(output_path, encoded)) {
	//		cerr << "Main Error: Failed to save encoded image to '" << output_path << "'" << endl;
	//		return -1;
	//	}

	//	cout << "Main: Encoding completed successfully." << endl;
	//}
	//catch (const exception& e) {
	//	cerr << "Main Encoding Error: " << e.what() << endl;
	//	return -1;
	//}
	


	// -------------- DECODER BLOCK FOR MAIN FUNCTION ----------------------	


	
	try {
		cout << "Main: [Decoding Mode Active]" << endl;

		Mat encoded = imread(input1_path, IMREAD_COLOR);
		Mat original_carrier = imread(input2_path, IMREAD_COLOR);
		string output_file_path = output_path;

		if (encoded.empty()) {
			cerr << "Main Error: Failed to load encoded image from '" << input1_path << "'" << endl;
			return -1;
		}
		if (original_carrier.empty()) {
			cerr << "Main Error: Failed to load original carrier image from '" << input2_path << "'" << endl;
			return -1;
		}

		bool success = image_decoder_e(encoded, original_carrier, password, output_file_path);

		if (success) {
			cout << "Main: Decoding completed. Output saved to '" << output_file_path << "'" << endl;
		} else {
			cerr << "Main Error: Decoding failed." << endl;
			return -1;
		}
	}
	catch (const exception& e) {
		cerr << "Main Decoding Error: " << e.what() << endl;
		return -1;
	}
	

	return 0;
}
