#pragma once

#include <opencv2/opencv.hpp>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>



using namespace std;
using namespace cv;

string file_name(const string& path);

unsigned char getBitN(unsigned char val, int n);

void set_bit(unsigned char& byte, int bit_index, unsigned char bit_value);

unsigned long password_hasher(unsigned char* str);

Mat seeded_gaussian(Mat& image, unsigned char* password);

void shuffling_algorithm(vector<Point>& locations, RNG& rng);

Mat image_encoder_a(Mat& message, Mat& carrier);

Mat image_decoder_a(Mat& encoded, Mat& carrier);

Mat image_encoder_b(Mat& message, Mat& carrier, unsigned char* pass);

Mat image_decoder_b(Mat& encoded, Mat& carrier, unsigned char* pass);

Mat image_encoder_d(Mat& message, Mat& carrier, unsigned char* pass);

Mat image_decoder_d(Mat& encoded, Mat& carrier, unsigned char* pass);

Mat image_encoder_e(string& message_file_path, Mat& carrier, unsigned char* pass);

bool image_decoder_e(const cv::Mat& encodedImage, const cv::Mat& originalCarrier, unsigned char* password, const std::string& outputFilePath);

