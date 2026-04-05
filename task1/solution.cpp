#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>              // To time execution - Benchmarking
using namespace std;
using namespace cv;

/*
    Specifying Namespaces
    Namespace is a declarative region that provides a scope to the identifiers (the names of types, functions, variables, etc) inside it. 
*/

/// @brief : Convolution will help apply different kernels to your images yielding different results
/// @param   Original_image
/// @param   kernel
/// @return : An Output Image with the kernel applied
cv::Mat convolve(cv::Mat original_image, cv::Mat kernel){
    
    cv::Mat kernel_inv{kernel.size(), kernel.type()}, temp_kernel{kernel.size(), kernel.type()}, image_pad;
    cv::Mat resultant_image{original_image.size(), original_image.type()};
    
    // Using For Loops
    // for(int i = 0 ; i < kernel.rows ; i++ ){ cout<<"k"<<endl; kernel_inv.row(i) = kernel.row(kernel.rows - i - 1).clone(); }
    // temp_kernel = kernel_inv.clone();
    // for(int i = 0 ; i < kernel.cols ; i++ ){ kernel_inv.col(i) = temp_kernel.col(kernel.cols - i - 1).clone();}

    // Method 2: Simply using in-built functions
    // flip(kernel, kernel_inv, -1);  

    // Viewing channels for each image
    // cout<<original_image.channels()<<endl;

    // Adding Padding to the Image 
    cv::copyMakeBorder(original_image, image_pad, 1, 1, 1, 1, cv::BORDER_REPLICATE, cv::Scalar(0)); 

    for (int i = 1; i < image_pad.rows - 1; i++) 
    {   
        for (int j = 1; j < image_pad.cols - 1 ; j++) 
        {
            for(int ch = 0 ; ch < original_image.channels() ; ch++){

                double tmp = 0.0;
                for (int k = 0; k < kernel_inv.rows; k++) 
                {
                    for (int l = 0; l < kernel_inv.cols; l++) 
                    {
                        int x = j - 1 + l;
                        int y = i - 1 + k;  

                        if ((x >= 0 && x < image_pad.cols) && (y >= 0 && y < image_pad.rows)){
                            tmp += (double)image_pad.at<cv::Vec3b>(y, x).val[ch] * (double)kernel.at<double>(k, l);
                        }
                    }
                }

                resultant_image.at<cv::Vec3b>(i-1, j-1).val[ch] = cv::saturate_cast<uchar>(tmp);        // Why Unsigned? 
            }
        }
    }

    return resultant_image;
}
int main()
{
      auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    cv::Mat intermediate;
    cv::Mat output;
    cv::Mat intermediate2;
    cv::Mat output2;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    // The two kernels — do not change these
    cv::Mat K1 = (cv::Mat_<double>(3, 3) <<
         1,  2,  1,
         0,  0,  0,
        -1, -2, -1
    );

    cv::Mat K2 = (cv::Mat_<double>(3, 3) <<
         1,  0, -1,
         2,  0, -2,
         1,  0, -1
    );

    cv::Mat img = cv::imread("./assets/hogwarts.png", cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        cerr << "Could not load image at ./assets/hogwarts.png\n";
        return -1;
    }

    cv::Mat img_f;
    img.convertTo(img_f, CV_64FC1);
// filter2D(img_f, output, -1, K1); 
//                  // Convolve with original 'full' kernel
//      filter2D(img_f, output2, -1, K2);
    // Declare matrix to store intermediate image
    // TODO: apply K1 and K2 to img_f with as few multiplications per pixel as possible.
    // Print the number of multiplications your approach uses per pixel.
cv::Mat K1a = (cv::Mat_<double>(3, 1) <<
         1,  
         0,
         -1
    );
cv::Mat K1b = (cv::Mat_<double>(1, 3) <<
         1,  2, 1
    );
    cv::Mat K2a = (cv::Mat_<double>(3, 1) <<
         1, 
         2, 
         1
    );
    cv::Mat K2b = (cv::Mat_<double>(1, 3) <<
         1,  0, -1
    );
    
    
    // Making All Windows resizable
 

    // Now to actually convolve
    start = std::chrono::high_resolution_clock::now();  // Start clock to time execution
    intermediate = convolve(img_f, K1a);         // Convolve Vertically
    output = convolve(intermediate, K1b);        // Convolve Horizontally
    stop = std::chrono::high_resolution_clock::now();   // Stop clock

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Output" << duration.count() << " microseconds.\n";

    

    start = std::chrono::high_resolution_clock::now();
    cv::filter2D(img_f, intermediate, -1, K1a);   // Convolve with vertical 'half' kernel
    cv::filter2D(intermediate, output, -1, K1b);  // Convolve with horizontal 'half' kernel
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Separated convolution took " << duration.count() << " microseconds.\n";

    //KERNEL2:
    // Declare matrix to store intermediate image
    
    // Making All Windows resizable
  

    // Now to actually convolve
    start = std::chrono::high_resolution_clock::now();  // Start clock to time execution
    intermediate2 = convolve(output, K2a);         // Convolve Vertically
    output2 = convolve(intermediate2, K2b);        // Convolve Horizontally
    stop = std::chrono::high_resolution_clock::now();   // Stop clock

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Output" << duration.count() << " microseconds.\n";

    start = std::chrono::high_resolution_clock::now();
    cv::filter2D(output, intermediate2, -1, K2a);   // Convolve with vertical 'half' kernel
    cv::filter2D(intermediate2, output2, -1, K2b);  // Convolve with horizontal 'half' kernel
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Separated convolution took " << duration.count() << " microseconds.\n";

    cv::waitKey(0);
    cv::namedWindow("Output of Horizontal Convolution", cv::WINDOW_NORMAL);
    cv::imshow("Output of Horizontal Convolution", output2);
    cv::waitKey(0);
    return 0;
}
