﻿#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <filesystem>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world460d.lib")
#else
#pragma comment(lib, "opencv_world460.lib")
#endif

using namespace std;

const string images_directory_path = "./images";

string PixelToAscii(int intensity) {
    const string ASCII = " .,-+!=#@$%";
    string s;
    int num = ASCII.length() - (int)(intensity * ASCII.length() / 256) - 1;
    if (num >= ASCII.length()) s = ASCII.back();
    else s = ASCII.substr(num, 1);
    return s;
}

int main()
{
    string file_name, text_file_name;

    filesystem::directory_iterator it = filesystem::directory_iterator(images_directory_path);
    vector<string> image_files;

    int unnecessary_len = images_directory_path.length() + 1;

    for (auto e: it) {
        image_files.push_back(e.path().string().substr(unnecessary_len));
    }

    cout << "[image files]" << endl;
    for (auto e: image_files) {
        cout << "・" << e << endl;
    }

    cout << "\n" << "Please input image file name >>";
    cin >> file_name;
    cout << "Please input text file name >>";
    cin >> text_file_name;

    string image_path = "images\\" + file_name;
    cv::Mat image = cv::imread(image_path, 0);

    if (image.empty()) {
        cout << "Can't open file [" << file_name << "]" << endl;
        return 1;
    }

    double WIDTH_PIXEL{ 0 }, HEIGHT_PIXEL{ 0 };
    double width_size_rate, height_size_rate;
    
    while (true) {
        cout << "Input number of beside pixels (200 ~ 900)" << endl << ">>";
        cin >> WIDTH_PIXEL;
        if (200 <= WIDTH_PIXEL && WIDTH_PIXEL <= 900) {
            HEIGHT_PIXEL = WIDTH_PIXEL / 2;
            break;
        }
        else {
            cout << "Please input 200 ~ 900" << endl;
        }
    }

    width_size_rate = (WIDTH_PIXEL / image.cols);
    height_size_rate = (HEIGHT_PIXEL / image.rows);

    cout << "width:" << image.cols << endl << "height:" << image.rows << endl;
    cout << "width_rate:" << width_size_rate << endl << "height_rate:" << height_size_rate << endl;

    cv::resize(image, image, cv::Size(), 
        width_size_rate, height_size_rate);

    fstream file;
    file.open("output_text\\" + text_file_name + ".txt", ios::out);

    if (!file.is_open()) {
        cout << "Can't create text file..." << endl;
        return 1;
    }

    for (int i = 0; i < image.rows; i++) {
        string ASCII_string;
        for (int j = 0; j < image.cols; j++) {
            ASCII_string += PixelToAscii(image.at<unsigned char>(i, j));
        }
        file << ASCII_string << endl;
    }
    file.close();

    cout << "\n======== creation ASCII file successful !! ========" << endl;
    this_thread::sleep_for(chrono::milliseconds(1500));

    return 0;
}