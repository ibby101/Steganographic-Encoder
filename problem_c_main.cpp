//#include "image_header.h"
//
//int main(int argc, char** argv) {
//
//	// argument checking and calling file_name function to obtain executable name for
//	// visual clarity. code for 'file_name' can be found in 'problem_a_main.cpp'
//
//	string file = file_name(argv[0]);
//
//	if (argc != 3) {
//		cerr << "Incorrect format, please use: " << file
//			<< " <input_image> <password>" << endl;
//		return -1;
//	}
//	
//	// storing image in variable input_image
//
//	Mat input_image = imread(argv[1], IMREAD_GRAYSCALE);
//
//	// reading in password as a constant, passing through the noise application
//	// function after recasting to unsigned character.
//
//	unsigned char* password = reinterpret_cast<unsigned char*>(argv[2]);
//	
//	//  cout << "Password Used: " << password << endl;
//
//	// storing output in variable noised_image, code can be found in 'problem_c_noise.cpp'
//	// for seeded_gaussian function.w
//
//	Mat noised_image = seeded_gaussian(input_image, password);
//
//	// general opencv functions for showing results
//
//	cout << "Password provided:" << password << endl;
//
//	imwrite("noise_apply_c.png", noised_image);
//	imshow("Input", input_image);
//	imshow("Noised", noised_image);
//	waitKey(0);
//
//	return 0;
//
//}