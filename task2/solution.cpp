#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main()
{
    // Load the three fragment images as grayscale — do not change these lines
    cv::Mat frag_a = cv::imread("./assets/fragment_a.png", cv::IMREAD_GRAYSCALE);
    cv::Mat frag_b = cv::imread("./assets/fragment_b.png", cv::IMREAD_GRAYSCALE);
    cv::Mat frag_c = cv::imread("./assets/fragment_c.png", cv::IMREAD_GRAYSCALE);

    if (frag_a.empty() || frag_b.empty() || frag_c.empty()) {
        cerr << "Could not load one or more fragment images from ./assets/\n";
        return -1;
    }
    // TODO: reverse the charm on each fragment, merge the recovered channels
    // into a single BGR image, save it as ./assets/memory_restored.png,
    // and print the number of non-zero pixels in the result.
    cv::Mat mat1;
   cv::Mat s1, s2, s3;
    // mat2.convertTo(s2, CV_32S);
    // mat3.convertTo(s3, CV_32S);
    // cv::Mat sum32=s1+s2+s3;
    // cv::Mat masked;
    // cv::bitwise_and(sum32, cv::Scalar(0xff), masked);//keep lower bits 8
    // cv:: Mat result_A;
    // masked.convertTo(result_A, CV_8U);
    //First charm
     // storing the dimensions of the image
    int height = frag_a.cols;
    int width = frag_a.rows;

    // get the x and y axis values for translation so that the image can be moved in both directions
    float tx = float(width) / 4;
    float ty = float(height) / 4;
    cv::Mat img2 = cv::Mat::zeros(cv::Size(height, width), CV_8UC1);
    
    // create the translation matrix using tx and ty
    float warp_values[] = { -1.0, 0.0, tx, 0.0, -1.0, ty };
    mat1 = cv::Mat(2, 3, CV_32F, warp_values);
    cout << mat1;
     mat1.convertTo(s1, CV_32S);
    // save the resulting image in translated_image matrix

    // apply affine transformation to the original image using the translation matrix
    cv::warpAffine(frag_a, s1, mat1, frag_a.size());

    for(int r = 0; r < height - ty; r++)
    {
        for(int c = 0; c < width - tx; c++)
        {
            img2.at<uchar>(c, r) = frag_a.at<uchar>(c + tx, r + ty);
        }
    }
    // display the original and the Translated images
    cv::imshow("Translated image", s1);
    cv::imshow("Translated Image Version", img2);
    cv::imshow("Original image", frag_a);
    cv::waitKey(0);
   

    return 0;
}
