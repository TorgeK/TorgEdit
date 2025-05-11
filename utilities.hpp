#include <string>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <cmath>

struct HSV {
    float hue;
    float saturation;
    float value;

    HSV() : hue(0.0f), saturation(0.0f), value(0.0f) {};
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
    
    if (delta < 1e-5){
        return HSV(0.0, 0.0, value);
    }

    float hue;
    const float saturation = (delta / colorMax);

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

    return HSV(hue, saturation, value);
};

/*
    Convert image from RGB to HSV format

    @param[in] image      RGB format image
    @param[in] height     Image height
    @param[in] width      Image width
    @param[in] channels   Image channels per pixel

    @return    HSVImage   Hue-saturation-value format image
*/
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

/*
    Convert HSV pixel to equivalent rgb

    @param[in]       pixel  HSV pixel of hue, saturation and value
    @param[in/out]   red    Red pixel
    @param[in/out]   green  Green pixel
    @param[in/out]   blue   Blue pixel
*/
void HSVToRGB(const HSV& pixel, unsigned char& red, unsigned char& green, unsigned char& blue){

//    const float chroma = pixel.value * pixel.saturation;

    const float value = pixel.value;
    const float saturation = pixel.saturation;

    const float chroma = value * saturation;

    // const float hueSegment = std::fmod(pixel.hue / 60.0f, 2.0f);
    // const float X = color * (1 - std::fabs(hueSegment - 1));

    float hue = std::fmod(pixel.hue, 360.0f);
    if (hue < 0.0f) hue += 360.0f;

    const float huePrime = hue / 60.0f;
    const float X = chroma * (1 - std::fabs(std::fmod(huePrime, 2.0f) - 1));

    float redSubOne = 0.0f;
    float greenSubOne = 0.0f;
    float blueSubOne = 0.0f;

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

    const float min = value - chroma;

    // red = static_cast<unsigned char>((redSubOne + min) * 255);
    // green = static_cast<unsigned char>((greenSubOne + min) * 255);
    // blue = static_cast<unsigned char>((blueSubOne + min) * 255);

    red   = static_cast<unsigned char>(std::clamp((redSubOne + min) * 255.0f, 0.0f, 255.0f));
    green = static_cast<unsigned char>(std::clamp((greenSubOne + min) * 255.0f, 0.0f, 255.0f));
    blue  = static_cast<unsigned char>(std::clamp((blueSubOne + min) * 255.0f, 0.0f, 255.0f));
};


/*
    Check image pixel integrity after converting from rgb-hsv-rgb within +/- 1 for each rgb value

    @param[in] original    starting image pre-conversion
    @param[in] identity    image post-conversion
    @param[in] height      image height
    @param[in] width       image width
    @param[in] channels    image channels per pixel

    @return    bool        image within +/- 1 sameness
*/
bool identityTest(const unsigned char* original, const unsigned char* identity, const int height, const int width, const int channels ){
    for (int i = 0; i < height * width * channels; ++i){
        if (original[i] - identity[i] > 1 || original[i] - identity[i] < -1){
            std::cout << "Pixel " << i / 3 << "different" << int(original[i]) << " : " << int(identity[i]);
            return false;
        }
    }
    std::cout << "Image within +/-1 rgb value";
    return true;
}

/*
    Convert Hue-Saturation-Value image to 3 channel RGB

    @param[in] HSVImage   Image to be converted
    @param[in] height     Image height
    @param[in] width      Image width
    @param[in] channels   Image channels per pixel

    @return    rgbImage   3 channel rgb image 

*/
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

/*
    Adjust image hue 

    @param[in/out] image          HSV image
    @param[in]     hueAdjustment  Degrees of hue adjustment [-360.0, 360.0]
    @param[in]     height         Image height
    @param[in]     width          Image width
*/
void adjustHue(HSV* image, const float hueAdjustment, const int height, const int width){

    for (int i = 0; i < height * width; ++i){
        if (hueAdjustment > 0){
            image[i].hue = std::fmod(image[i].hue + hueAdjustment, 360.0f);
        }
        else{
            image[i].hue = std::fmod(image[i].hue + hueAdjustment, 360.0f) + 360.0f;
        }
    }
}

/*
    Adjust image saturation

    @param[in/out] image          HSV image
    @param[in]     saturationAdjustment  Amount of saturation adjustment 
    @param[in]     height         Image height
    @param[in]     width          Image width
*/
void adjustSaturation(HSV* image, const float saturationAdjustment, const int height, const int width){

    for (int i = 0; i < height * width; ++i){
        image[i].saturation = std::clamp(image[i].saturation + saturationAdjustment, 0.0f, 100.0f);
    }
}
