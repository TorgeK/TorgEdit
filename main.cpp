
#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct HSV {
    float hue;
    float saturation;
    float value;

    HSV() : hue(0), saturation(0), value(0) {};
    HSV(float hue, float saturation, float value) : hue(hue), saturation(saturation), value(value) {};
};


/*
    Strips file extension from the end of a filename

    @param[in/out] filename Image filename
*/
void removeFileExtension(std::string& filename) {
    size_t lastDotPosition = filename.find_last_of('.');
    if (lastDotPosition == std::string::npos){
        return;
    }
    filename = filename.substr(0, lastDotPosition);
};


/*
    Adjust image pixels based on given adjustments for each RGB value

    @param[in/out]  image             Image buffer
    @param[in]      redAdjustment     Red adjustment amount
    @param[in]      greenAdjustment   Green adjustment amount
    @param[in]      blueAdjustment    Blue adjustment amount
    @param[in]      height            Image height
    @param[in]      width             Image width
    @param[in]      channels          Number of channels per pixel 
*/
void adjustRGB(unsigned char* image, const int redAdjustment, const int greenAdjustment, const int blueAdjustment, const int height, const int width, const int channels) {
    
    for (int y=0; y < height; ++y){
        for (int x=0; x < width; ++x){
            int pixelIndex = (y * width + x) * channels;

            image[pixelIndex]     = std::clamp(image[pixelIndex]     + redAdjustment, 0, 255); // Red
            image[pixelIndex + 1] = std::clamp(image[pixelIndex + 1] + greenAdjustment, 0, 255); // Green
            image[pixelIndex + 2] = std::clamp(image[pixelIndex + 2] + blueAdjustment, 0, 255); // Blue
        }
    }
};

/*
    Adjust image brightness

    @param[in/out]  image                   Image buffer
    @param[in]      brightnessAdjustment    brightness adjustment amount
    @param[in]      height                  Image height
    @param[in]      width                   Image width
    @param[in]      channels                Number of channels per pixel 

*/
void adjustBrightness(unsigned char* image, const int brightnessAdjustment, const int height, const int width, const int channels) {
    
    for (int y=0; y < height; ++y){
        for (int x=0; x < width; ++x){
            int pixelIndex = (y * width + x) * channels;

            image[pixelIndex]     = std::clamp(image[pixelIndex]     + brightnessAdjustment, 0, 255); // Red
            image[pixelIndex + 1] = std::clamp(image[pixelIndex + 1] + brightnessAdjustment, 0, 255); // Green
            image[pixelIndex + 2] = std::clamp(image[pixelIndex + 2] + brightnessAdjustment, 0, 255); // Blue
        }
    }
};

/*
    Adjust image contrast

    @param[in/out]  image                   Image buffer
    @param[in]      contrastFactor      Contrast factor [0.5 - 1.5]
    @param[in]      height                  Image height
    @param[in]      width                   Image width
    @param[in]      channels                Number of channels per pixel 

*/
void adjustContrast(unsigned char* image, const double contrastFactor, const int height, const int width, const int channels) {
    
    const int MIDPOINT = 128;

    for (int y=0; y < height; ++y){
        for (int x=0; x < width; ++x){
            int pixelIndex = (y * width + x) * channels;

            image[pixelIndex] = std::clamp(
                int((image[pixelIndex] - MIDPOINT) * contrastFactor + MIDPOINT), 
                0, 255); // Red
            image[pixelIndex + 1] = std::clamp(
                int((image[pixelIndex + 1] - MIDPOINT) * contrastFactor + MIDPOINT), 
                0, 255); // Green
            image[pixelIndex + 2] = std::clamp(
                int((image[pixelIndex + 2] - MIDPOINT) * contrastFactor + MIDPOINT), 
                0, 255); // Blue
        }
    }
};


/*
    convert RGB pixel value to Hue, Saturation, Value

    @param[in] red   Pixel's red value
    @param[in] green Pixel's green value
    @param[in] blue  Pixel's blue value 

    @return HSV      Hue, saturation, value struct 
*/
HSV convertPixelToHSV(const int red, const int green, const int blue) {

    // normalize rgb values to be fit range [0.0, 1.0]
    const float normalizedRed   = red / 255.0;
    const float normalizedGreen = green / 255.0;
    const float normalizedBlue  = blue / 255.0;

    const float colorMax = std::max( { normalizedRed, normalizedGreen, normalizedBlue});
    const float colorMin = std::min( { normalizedRed, normalizedGreen, normalizedBlue});

    const float delta = colorMax - colorMin;
    const float value = colorMax;    
    
    if (colorMax == colorMin){
        return HSV(0.0, 0.0, value);
    }

    float hue;
    const float saturation = (delta / colorMax) * 100;

    const float newRed   = (colorMax - normalizedRed) / delta;
    const float newGreen = (colorMax - normalizedGreen) / delta;
    const float newBlue  = (colorMax - normalizedBlue) / delta;

    if (normalizedRed == colorMax){
        hue = 0.0 + newBlue - newGreen;
    }
    else if (normalizedGreen == colorMax){
        hue = 2.0 + newRed - newBlue;
    }
    else {
        hue = 4.0 + newGreen - newRed;
    }

    if (hue < 0) {
        hue += 6.0;
    }
    hue = (hue / 6.0) * 360;

    return HSV(hue, saturation, value * 100);
};

HSV* convertImageToHSV(unsigned char* image, const int height, const int width, const int channels){
    HSV* HSVImage = new HSV[height * width];

    auto start = std::chrono::high_resolution_clock::now();

    for (int y=0; y < height; ++y){
        for (int x=0; x < width; ++x){
            int pixelIndex = (y * width + x) * channels;
            int HSVIndex = y * width + x;
            
            HSV newPixel = convertPixelToHSV(image[pixelIndex], image[pixelIndex + 1], image[pixelIndex + 2]);

            HSVImage[HSVIndex] = newPixel;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Processing took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms\n";

    return HSVImage;
}

void HSVToRGB(const HSV& pixel, unsigned char& red, unsigned char& green, unsigned char& blue){

    const float chroma = pixel.value * pixel.saturation;

    // const float hueSegment = std::fmod(pixel.hue / 60.0f, 2.0f);
    // const float X = color * (1 - std::fabs(hueSegment - 1));

    const float huePrime = pixel.hue / 60.0f;
    const float X = chroma * (1 - std::fabs(std::fmod(huePrime, 2.0f) - 1));

    int redSubOne;
    int greenSubOne;
    int blueSubOne;

    if (huePrime >= 0 && huePrime < 1){
        redSubOne = chroma;
        greenSubOne = X;
        blueSubOne = 0;
    }
    else if (huePrime >= 1 && huePrime < 2){
        redSubOne = X;
        greenSubOne = chroma;
        blueSubOne = 0;
    }
    else if (huePrime >= 2 && huePrime < 3){
        redSubOne = 0;
        greenSubOne = chroma;
        blueSubOne = X;
    }
    else if (huePrime >= 3 && huePrime < 4){
        redSubOne = 0;
        greenSubOne = X;
        blueSubOne = chroma;
    }
    else if (huePrime >= 4 && huePrime < 5){
        redSubOne = X;
        greenSubOne = 0;
        blueSubOne = chroma;
    }
    else if (huePrime >= 5 && huePrime < 6){
        redSubOne = chroma;
        greenSubOne = 0;
        blueSubOne = X;
    }

    const float min = pixel.value - (chroma / 2);

    red = redSubOne + min;
    green = greenSubOne + min;
    blue = blueSubOne + min;
};


unsigned char* convertHSVToRGBImage(HSV* HSVImage, const int height, const int width, const int channels){
    unsigned char* rgbImage = new unsigned char[height * width * channels];

    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            int pixelIndex = (y * width + x) * channels;
            int HSVIndex = y * width + x;

            HSVToRGB(HSVImage[HSVIndex], rgbImage[pixelIndex], rgbImage[pixelIndex + 1], rgbImage[pixelIndex + 2]);
        }
    }

    return rgbImage;
}

int main(int argc, char* argv[]){

    int width;
    int height;
    int channels;

    unsigned char* image = stbi_load(argv[1], &width, &height, &channels, 0);

    if (image == NULL){
        std::cout << "Error loading image\n";
        std::exit(1);
    }

    std::cout << "Filename: " << argv[1] << "\n";
    std::cout << "Output type: " << argv[2] << "\n";
    std::cout << "Width: " << width << "\nHeight: " << height << "\nChannels: " << channels << "\n";

    // if (argc > 3){
    //     adjustRGB(image, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), height, width, channels);
    //     adjustContrast(image, atof(argv[6]), height, width, channels);
    // }

    HSV* HSVImage = convertImageToHSV(image, height, width, channels);

    unsigned char* identity = convertHSVToRGBImage(HSVImage, height, width, channels);

    stbi_write_jpg("identity_test.jpg", width, height, channels, identity, 100);


    delete HSVImage;
    delete identity; 


    // if (argv[2] == std::string("jpg")){
    //     std::cout << "outputting jpg...\n";
    //     std::string newFileName = argv[1];
    //     removeFileExtension(newFileName);
    //     newFileName += "_new.jpg"; 
        
    //     stbi_write_jpg(newFileName.c_str(), width, height, channels, image, 100);
    // }
    // if (argv[2] == std::string("png")){
    //     std::cout << "outputting png...\n";
    //     std::string newFileName = argv[1];
    //     removeFileExtension(newFileName);
    //     newFileName += "_new.png"; 
        
    //     stbi_write_png(newFileName.c_str(), width, height, channels, image, width * channels);
    // }


    stbi_image_free(image);

    return 0;
}