#include "image_header.h"

Mat image_encoder_e(string& message_file_path, Mat& carrier, unsigned char* pass) {
    // check if the carrier image is loaded
    if (carrier.empty()) {
        throw runtime_error("Carrier image is empty");
    }

    // check that the carrier image is a colour image (3 channels)
    if (carrier.type() != CV_8UC3) {
        throw runtime_error("Carrier image is not in colour");
    }

    // read the message file
    std::ifstream message_file(message_file_path, std::ios::binary | std::ios::ate);
    if (!message_file.is_open()) {
        throw std::runtime_error("Encoder_E Error: Cannot open message file: " + message_file_path);
    }

    std::streamsize message_file_size_stream = message_file.tellg();
    message_file.seekg(0, std::ios::beg);

    if (message_file_size_stream < 0) {
        throw std::runtime_error("Encoder_E Error: Error reading file size.");
    }

    if (static_cast<unsigned long long>(message_file_size_stream) > std::numeric_limits<uint64_t>::max()) {
        throw std::runtime_error("Encoder_E Error: Message file size exceeds maximum representable value (uint64_t).");
    }

    uint64_t message_file_size_bytes = static_cast<uint64_t>(message_file_size_stream);
    std::vector<unsigned char> message_file_data(message_file_size_bytes);

    if (!message_file.read(reinterpret_cast<char*>(message_file_data.data()), message_file_size_bytes)) {
        message_file.close();
        throw std::runtime_error("Encoder_E Error: Error reading file data from: " + message_file_path);
    }

    message_file.close();
    std::cout << "Encoder_E: Message file '" << message_file_path << "' size: " << message_file_size_bytes << " bytes." << std::endl;

    // check if the carrier image has enough capacity
    int carrier_rows = carrier.rows;
    int carrier_cols = carrier.cols;

    // getting total carrier size, by multiplying rows/cols by 3 for colour channels

    uint64_t total_carrier_slots = static_cast<uint64_t>(carrier_rows) * carrier_cols * 3;
    uint64_t required_bits = (sizeof(uint64_t) * 8) + (message_file_size_bytes * 8);

    if (required_bits > total_carrier_slots) {
        throw std::runtime_error("Encoder_E Error: Carrier image does not have enough slots (" + std::to_string(total_carrier_slots) +
            ") to encode the message file (" + std::to_string(required_bits) + " bits required).");
    }

    std::cout << "Encoder_E: Required bits: " << required_bits << ", Available slots: " << total_carrier_slots << std::endl;

    // setup RNG and matrices
    unsigned long hashed_seed = password_hasher(pass);
    cv::RNG rng(hashed_seed);

    cv::Mat noisy_carrier = seeded_gaussian(const_cast<Mat&>(carrier), pass);
    cv::Mat encoded_image = noisy_carrier.clone();
    cv::Mat used_slots_map = cv::Mat::zeros(carrier_rows, carrier_cols, CV_8UC3);
    long encoded_bit_counter = 0;

    // lambda to encode a single bit
    auto encode_single_bit = [&](unsigned char bit_to_encode) {
        int row_index, col_index, channel_index;
        bool slot_found = false;
        int attempt_counter = 0;
        uint64_t max_attempts = total_carrier_slots * 2;

        while (!slot_found) {
            if (++attempt_counter > max_attempts) {
                throw std::runtime_error("Encoder_E Error: Could not find an unused, non-overflow slot after many attempts.");
            }

            row_index = rng.uniform(0, carrier_rows);
            col_index = rng.uniform(0, carrier_cols);
            channel_index = rng.uniform(0, 3);

            bool already_used = (used_slots_map.at<cv::Vec3b>(row_index, col_index)[channel_index] != 0);
            bool overflow = (encoded_image.at<cv::Vec3b>(row_index, col_index)[channel_index] == 255);

            if (!already_used && !overflow) {
                slot_found = true;
            }
        }

        used_slots_map.at<cv::Vec3b>(row_index, col_index)[channel_index] = 1;
        uchar& pixel_channel = encoded_image.at<cv::Vec3b>(row_index, col_index)[channel_index];
        pixel_channel = (pixel_channel & 0xFE) | bit_to_encode;
        encoded_bit_counter++;
        };

    // encode the message file size (64 bits)
    std::cout << "Encoder_E: Encoding file size (" << sizeof(uint64_t) * 8 << " bits)..." << std::endl;
    for (int byte_index = 0; byte_index < sizeof(uint64_t); ++byte_index) {
        unsigned char size_byte = (message_file_size_bytes >> (byte_index * 8)) & 0xFF;
        for (int bit_index = 0; bit_index < 8; ++bit_index) {
            encode_single_bit(getBitN(size_byte, bit_index));
        }
    }

    // encode the actual message file data
    std::cout << "Encoder_E: Encoding file data (" << message_file_size_bytes * 8 << " bits)..." << std::endl;
    for (unsigned char data_byte : message_file_data) {
        for (int bit_index = 0; bit_index < 8; ++bit_index) {
            encode_single_bit(getBitN(data_byte, bit_index));
        }
    }

    std::cout << "Encoder_E: Successfully encoded " << encoded_bit_counter << " bits into the image." << std::endl;

    return encoded_image;
}
