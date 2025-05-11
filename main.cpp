
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "utilities.hpp"


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

    //adjustHue(HSVImage, 390.0f, height, width);
    //adjustSaturation(HSVImage, 100.0f, height, width);
    adjustValue(HSVImage, 0.3f, height, width);

    unsigned char* identity = convertHSVToRGBImage(HSVImage, height, width, channels);

    stbi_write_jpg("identity_test.jpg", width, height, channels, identity, 100);


    //identityTest(image, identity, height, width, channels);



    delete[] HSVImage;
    delete[] identity; 


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