#include "image_header.h"

// -------------- FUNCTION FOR IMAGE DECODING ----------------------	

// decodes a random binary file from a colour image encoded using
// randomised steganography. The encoding process involves bit-level manipulation
// using LSB (Least Significant Bit) and a synchronised RNG seeded with a hashed password.
//
// Inputs:
//   - encodedImage   : colour image (CV_8UC3) containing hidden data.
//   - originalCarrier: The original carrier image used during encoding for RNG sync
//   - password       : user inputs the password assumed to be used for encoding
//   - outputFilePath : path where the decoded binary data will be saved.
//
// Returns:
//   - true if decoding succeeds, false otherwise.
// ----------------------------------------------------------------------------------------------
bool image_decoder_e(const cv::Mat& encodedImage, const cv::Mat& originalCarrier,
    unsigned char* password, const std::string& outputFilePath) {

    // -------------- INPUT VALIDATION ----------------------	
    //
    // Checks that the encoded image and original carrier are valid.
    // Throws an error if any of the images are empty, have incorrect types, 
    // or mismatched sizes.

    if (encodedImage.empty() || originalCarrier.empty()) {
        throw std::runtime_error("Decoder_E Error: Encoded or carrier image is empty.");
    }

    if (encodedImage.type() != CV_8UC3 || originalCarrier.type() != CV_8UC3) {
        throw std::runtime_error("Decoder_E Error: Both images must be 8-bit colour (CV_8UC3).");
    }

    if (encodedImage.size() != originalCarrier.size()) {
        throw std::runtime_error("Decoder_E Error: Encoded and carrier images must match in size.");
    }

    // -------------- SETUP RNG AND TRACK USED SLOTS ----------------------
    //
    // Hashes the password to generate the seed for the RNG.
    // Uses the same RNG as during encoding for proper decoding synchronization.
    // Creates a noisy carrier to check the overflow of slots and prevent mismatches.

    unsigned long seed = password_hasher(password);
    cv::RNG rng(seed);  // synchronise the RNG with the encoding process

    // re-generating noisy carrier used in encoding for overflow-checking
    cv::Mat noisyCarrier = seeded_gaussian(const_cast<cv::Mat&>(originalCarrier), password);

    int rows = encodedImage.rows;
    int cols = encodedImage.cols;
    uint64_t totalSlots = static_cast<uint64_t>(rows) * cols * 3;  // Total bit slots in the image

    cv::Mat used = cv::Mat::zeros(rows, cols, CV_8UC3);  // Tracks which channels have been used
    long decodedBits = 0;  // Counter for successfully decoded bits

    // -------------- BIT-LEVEL DECODING HELPER ----------------------
    //
    // This lambda function decodes a single bit from a random slot in the image.
    // It ensures that unused and valid slots are chosen by checking their conditions.
    // Tracks the number of attempts to find a valid slot and raises an error if the 
    // decoding process is unsuccessful within a maximum number of attempts.

    auto decode_bit = [&]() -> unsigned char {
        int r, c, ch;
        bool found = false;
        int attempts = 0;
        const uint64_t maxAttempts = totalSlots * 2;  // max number of attempts to find a valid bit

        while (!found) {
            if (++attempts > maxAttempts) {
                throw std::runtime_error("Decoder_E Error: Failed to sync with encoded sequence.");
            }

            r = rng.uniform(0, rows);
            c = rng.uniform(0, cols);
            ch = rng.uniform(0, 3);

            // skipping used channels or slots that are overflowed

            if (used.at<cv::Vec3b>(r, c)[ch] != 0) continue;
            if (noisyCarrier.at<cv::Vec3b>(r, c)[ch] == 255) continue;

            found = true;
        }

        // marking this channel as used

        used.at<cv::Vec3b>(r, c)[ch] = 1; 
        decodedBits++;  

        return encodedImage.at<cv::Vec3b>(r, c)[ch] & 1;  // extracting LSB
        };

    // -------------- DECODE FILE SIZE ----------------------
    //
    // Decodes the file size from the first 64 bits of the image.
    // Uses the decode_bit function to retrieve individual bits and
    // reconstructs the file size in bytes.

    std::cout << "Decoder_E: Decoding file size (64-bit header)..." << std::endl;

    uint64_t fileSize = 0;
    unsigned char byteBuffer = 0;
    int bitCount = 0;

    for (int i = 0; i < 64; ++i) {
        unsigned char bit = decode_bit();
        set_bit(byteBuffer, bitCount % 8, bit);
        bitCount++;

        if (bitCount % 8 == 0) {
            fileSize |= (static_cast<uint64_t>(byteBuffer) << ((bitCount / 8 - 1) * 8));
            byteBuffer = 0;
        }
    }

    std::cout << "Decoder_E: File size decoded = " << fileSize << " bytes." << std::endl;

    // -------------- DECODE FILE DATA ----------------------
    //
    // Decodes the file data, writing it to the specified output file.
    // The file size was previously decoded, so we can now decode and write
    // the corresponding number of bits (bytes) to the output file.

    uint64_t remaining = totalSlots - 64;
    if (fileSize * 8 > remaining) {
        std::cerr << "Decoder_E Warning: File size exceeds remaining capacity. Output may be truncated." << std::endl;
    }

    std::cout << "Decoder_E: Decoding " << fileSize * 8 << " bits to '" << outputFilePath << "'..." << std::endl;

    std::ofstream outFile(outputFilePath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Decoder_E Error: Could not open output file: " + outputFilePath);
    }

    for (uint64_t i = 0; i < fileSize; ++i) {
        unsigned char decodedByte = 0;
        for (int j = 0; j < 8; ++j) {
            unsigned char bit = decode_bit();
            set_bit(decodedByte, j, bit);  // Reconstruct byte from bits
        }

        outFile.write(reinterpret_cast<const char*>(&decodedByte), 1);

        if (!outFile) {
            outFile.close();
            throw std::runtime_error("Decoder_E Error: Failed to write output data.");
        }
    }

    outFile.close();

    // Final status messages, including the total number of bits decoded,
    // and the output file location.

    std::cout << "Decoder_E: Decoding complete. Total bits read: " << decodedBits << std::endl;
    std::cout << "Decoder_E: Output file saved to '" << outputFilePath << "'" << std::endl;

    return true;
}
