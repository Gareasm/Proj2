
#ifndef PROJECT2_FUN_H
#define PROJECT2_FUN_H

#endif //PROJECT2_FUN_H
#include "functions.cpp"


#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "functions.cpp"

/////////////////////////

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};
struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

class Image{
public:
    vector<Pixel> image_data;
    Header header_data;
    Image(const std::vector<Pixel>& image_data, const Header& header_data)
            : image_data(image_data), header_data(header_data) {}
};

Image readTGA(string filename) {
    string fullFilePath = "input/" + filename;
    fstream file(fullFilePath, ios::binary | ios::in | ios::out);
    if (!file.is_open()) {
        cout << "Failed to open the file." << endl;
    } else {
        cout << "file opened succesfully" << endl;
    }
    Header tgaheader;
    //reading header
    file.read(reinterpret_cast<char *>(&tgaheader.idLength), sizeof(char));
    file.read(reinterpret_cast<char *>(&tgaheader.colorMapType), sizeof(char));
    file.read(reinterpret_cast<char *>(&tgaheader.dataTypeCode), sizeof(char));
    file.read(reinterpret_cast<char *>(&tgaheader.colorMapOrigin), sizeof(short));
    file.read(reinterpret_cast<char *>(&tgaheader.colorMapLength), sizeof(short));
    file.read(reinterpret_cast<char *>(&tgaheader.colorMapDepth), sizeof(char));
    file.read(reinterpret_cast<char *>(&tgaheader.xOrigin), sizeof(short));
    file.read(reinterpret_cast<char *>(&tgaheader.yOrigin), sizeof(short));
    file.read(reinterpret_cast<char *>(&tgaheader.width), sizeof(short));
    file.read(reinterpret_cast<char *>(&tgaheader.height), sizeof(short));
    file.read(reinterpret_cast<char *>(&tgaheader.bitsPerPixel), sizeof(char));
    file.read(reinterpret_cast<char *>(&tgaheader.imageDescriptor), sizeof(char));
    //reading pixels
    vector<Pixel> pixels;
    Pixel pixel;
    while (file.read(reinterpret_cast<char *>(&pixel.blue), sizeof(pixel.blue)) &&
           file.read(reinterpret_cast<char *>(&pixel.green), sizeof(pixel.green)) &&
           file.read(reinterpret_cast<char *>(&pixel.red), sizeof(pixel.red))) {
        pixel.blue = std::min(std::max(0, static_cast<int>(pixel.blue)), 255);
        pixel.green = std::min(std::max(0, static_cast<int>(pixel.green)), 255);
        pixel.red = std::min(std::max(0, static_cast<int>(pixel.red)), 255);
        pixels.push_back(pixel);
    }

    // Close the file
    file.close();
    Image img(pixels, tgaheader);
    return img;

}
//creating new file
void file_creator(Header tgaheader, const vector<Pixel>& pixels, string name){
    string final_name;
    ofstream new_file(name, ios::binary);
    if (!new_file.is_open()) {
        cout << "Error opening the file." << endl;
    }
    //copy header
    new_file.write(reinterpret_cast<char *>(&tgaheader.idLength), sizeof(char));
    new_file.write(reinterpret_cast<char *>(&tgaheader.colorMapType), sizeof(char));
    new_file.write(reinterpret_cast<char *>(&tgaheader.dataTypeCode), sizeof(char));
    new_file.write(reinterpret_cast<char *>(&tgaheader.colorMapOrigin), sizeof(short));
    new_file.write(reinterpret_cast<char *>(&tgaheader.colorMapLength), sizeof(short));
    new_file.write(reinterpret_cast<char *>(&tgaheader.colorMapDepth), sizeof(char));
    new_file.write(reinterpret_cast<char *>(&tgaheader.xOrigin), sizeof(short));
    new_file.write(reinterpret_cast<char *>(&tgaheader.yOrigin), sizeof(short));
    new_file.write(reinterpret_cast<char *>(&tgaheader.width), sizeof(short));
    new_file.write(reinterpret_cast<char *>(&tgaheader.height), sizeof(short));
    new_file.write(reinterpret_cast<char *>(&tgaheader.bitsPerPixel), sizeof(char));
    new_file.write(reinterpret_cast<char *>(&tgaheader.imageDescriptor), sizeof(char));


    for (const Pixel &pixel: pixels) {
        new_file.write(reinterpret_cast<const char *>(&pixel.blue), sizeof(pixel.blue));
        new_file.write(reinterpret_cast<const char *>(&pixel.green), sizeof(pixel.green));
        new_file.write(reinterpret_cast<const char *>(&pixel.red), sizeof(pixel.red));

    }
    new_file.close();
}


vector<float> normalizePixels(std::vector<Pixel>& pixels) {
  vector<float> normalized_values;

    for (Pixel& pixel : pixels) {
        // Normalize each color component and store in the vector
        float normalized_blue = static_cast<float>(pixel.blue) / 255.0f;
        float normalized_green = static_cast<float>(pixel.green) / 255.0f;
        float normalized_red = static_cast<float>(pixel.red) / 255.0f;

        normalized_values.push_back(normalized_blue);
        normalized_values.push_back(normalized_green);
        normalized_values.push_back(normalized_red);
    }

    return normalized_values;
}
/*
vector<Pixel> multiply(Image top_layer, Image bottom_layer){
    vector<Pixel> result;
    vector<Pixel> top_data = top_layer.image_data;
    vector<Pixel> bottom_data = bottom_layer.image_data;
    vector<float> top_normalized = normalizePixels(top_data);
    vector<float> bottom_normalized = normalizePixels(bottom_data);
    //pixel data is now in the range of 0-1
    for (size_t i = 0; i < top_normalized.size(); ++i) {
        top_normalized[i] = (top_normalized[i] * bottom_normalized[i]);
    }

    for (size_t i = 0; i < top_normalized.size(); i += 3) {
        Pixel pixel;

        // Convert the normalized values back to unsigned char range [0, 255] with rounding
        pixel.blue = static_cast<unsigned char>((top_normalized[i] * 255.0f) + 0.5f);
        pixel.green = static_cast<unsigned char>((top_normalized[i + 1] * 255.0f) + 0.5f);
        pixel.red = static_cast<unsigned char>((top_normalized[i + 2] * 255.0f) + 0.5f);

        // Add the pixel to the vector
        result.push_back(pixel);
    }
    return result;
}

*/
vector<Pixel> multiply(const Image& top_layer, const Image& bottom_layer) {
    vector<Pixel> result;
    vector<Pixel> final_result;

    // Get pixel data for both layers
    const vector<Pixel>& top_data = top_layer.image_data;
    const vector<Pixel>& bottom_data = bottom_layer.image_data;

    // Multiply corresponding pixels of top and bottom layers
    for (size_t i = 0; i < top_data.size(); ++i) {
        Pixel pixel;

        // Multiply channels of individual pixels of top layer with bottom layer
        pixel.blue = ((top_data[i].blue / 255) * bottom_data[i].blue / 255);
        pixel.green = ((top_data[i].green / 255) * (bottom_data[i].green / 255));
        pixel.red = ((top_data[i].red / 255) * (bottom_data[i].red / 255));

        // Add the pixel to the result vector
        result.push_back(pixel);
    }
    for (Pixel& pixel : result) {
        // Add 0.5f to the normalized value multiplied by 255, before converting to an unsigned char
        pixel.blue = static_cast<unsigned char>((pixel.blue * 255.0f) + 0.5f);
        pixel.green = static_cast<unsigned char>((pixel.green * 255.0f) + 0.5f);
        pixel.red = static_cast<unsigned char>((pixel.red * 255.0f) + 0.5f);
        final_result.push_back(pixel);
    }

    return final_result;
}



void task_one(){
    Image top = readTGA("layer1.tga");
    Image car = readTGA("car.tga");
    Image bottom = readTGA("pattern1.tga");
    vector<Pixel> new_data = multiply(top, bottom);

    file_creator(car.header_data, car.image_data, "output/carcopytest.tga");
    cout << "FILE CREATED" << endl;
    file_creator(top.header_data, new_data, "output/part1.tga");
    cout << "FILE CREATED" << endl;




}