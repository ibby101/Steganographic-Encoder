#pragma once

#include <opencv2/opencv.hpp>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;
using namespace cv;

string file_name(const string& path);

unsigned long password_hasher(unsigned char* str);

Mat seeded_gaussian(Mat& image, unsigned char* password);

void shuffling_algorithm(vector<Point>& locations, RNG& rng);

Mat image_encoder_a(Mat& message, Mat& carrier);

Mat image_decoder_a(Mat& encoded, Mat& carrier);

Mat image_encoder_b(Mat& message, Mat& carrier, unsigned char* pass);

Mat image_decoder_b(Mat& encoded, Mat& carrier, unsigned char* pass);

Mat image_encoder_d(Mat& message, Mat& carrier, unsigned char* pass);

Mat image_decoder_d(Mat& encoded, Mat& carrier, unsigned char* pass);

