#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <vector>

unsigned char* loadImage(const char* filename, int* width, int* height, int* channels) {
    unsigned char* img = stbi_load(filename, width, height, channels, 0);
    if (!img) {
        std::cerr << "Error loading image!" << std::endl;
        exit(1);
    }
    return img;
}

void saveImage(const char* filename, unsigned char* img, int width, int height, int channels) {
    stbi_write_jpg(filename, width, height, channels, img, 90);
}

void adjustBrightness(unsigned char* img, int width, int height, int channels, int value) {
    for (int i = 0; i < width * height * channels; i++) {
        int newVal = img[i] + value;
        img[i] = (newVal > 255) ? 255 : (newVal < 0) ? 0 : newVal;
    }
}

void convertToGrayscale(unsigned char* img, int width, int height, int channels) {
    for (int i = 0; i < width * height; i++) {
        int r = img[i * channels];
        int g = img[i * channels + 1];
        int b = img[i * channels + 2];
        int gray = 0.299f * r + 0.587f * g + 0.114f * b;
        for (int c = 0; c < channels; c++) {
            img[i * channels + c] = gray;
        }
    }
}

void applyBlur(unsigned char* img, int width, int height, int channels) {
    std::vector<unsigned char> temp(img, img + width * height * channels);
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        sum += temp[(y + ky) * width * channels + (x + kx) * channels + c];
                    }
                }
                img[y * width * channels + x * channels + c] = sum / 9;
            }
        }
    }
}

void showMenu() {
    std::cout << "Image Processor\n";
    std::cout << "1. Adjust Brightness\n";
    std::cout << "2. Convert to Grayscale\n";
    std::cout << "3. Apply Blur\n";
    std::cout << "4. Save & Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    int width, height, channels;
    const char* inputPath = "input.jpg";
    const char* outputPath = "output.jpg";
    unsigned char* img = loadImage(inputPath, &width, &height, &channels);

    int choice;
    do {
        showMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int brightness;
                std::cout << "Enter brightness value (-255 to 255): ";
                std::cin >> brightness;
                adjustBrightness(img, width, height, channels, brightness);
                break;
            }
            case 2:
                convertToGrayscale(img, width, height, channels);
                break;
            case 3:
                applyBlur(img, width, height, channels);
                break;
            case 4:
                saveImage(outputPath, img, width, height, channels);
                std::cout << "Image saved as 'output.jpg'!\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    stbi_image_free(img);
    return 0;
}