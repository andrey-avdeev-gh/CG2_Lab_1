#include <iostream>
#include <string>
#include <iomanip>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int task_1();
Scalar getMSSIM(const Mat& i1, const Mat& i2);


int main()
{ 
    int opt;

    cout 
        << "   Main Menu    " << endl
        << endl << "Write 1 for task_1" << endl
        << endl << "Write 2 for task_2" << endl
        << endl << "Write 3 for task_3" << endl
        << endl
        << endl << "Write 0 to exit" << endl
        << endl
        ;

    cin >> opt;
    cout << "\n" << endl;

    switch (opt)
    {
    case 1:
        task_1();
        break;
    case 2:
        //Code
        break;
    case 3:
        //Code
        break;
    case 0:
        exit(1);
    default:
        break;
    }


    return 0;
}

int task_1()
{
    string image1_path = samples::findFile("1.jpg");
    string image2_path = samples::findFile("2.jpg");

    Mat img = imread(image1_path, IMREAD_COLOR);
    Mat img1 = imread(image2_path, IMREAD_COLOR);
    if (img.empty() || img1.empty())
    {
        cout << "Could not read the image: " << image1_path << std::endl;
        return 1;
    }

    namedWindow("Reference window", WINDOW_AUTOSIZE);
    namedWindow("Test window", WINDOW_AUTOSIZE);
    moveWindow("Test window", 400, 0);

    imshow("Reference window", img);
    imshow("Test window", img1);

    Scalar mssimvV = getMSSIM(img, img1);
    cout << "MSSIM: "
        << " R " << setiosflags(ios::fixed) << setprecision(2) << mssimvV.val[2] * 100 << '%'
        << " G " << setiosflags(ios::fixed) << setprecision(2) << mssimvV.val[1] * 100 << '%'
        << " B " << setiosflags(ios::fixed) << setprecision(2) << mssimvV.val[0] * 100 << '%'
        << endl
        ;

    int l = waitKey(0); // Wait for a keystroke in the window
    if (l == 'b')
    {

        imwrite("1.png", img);
        imwrite("2.png", img1);
    }
}



Scalar getMSSIM(const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    // INITS
    int d = CV_32F;

    Mat I1, I2;
    i1.convertTo(I1, d);           // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    Mat I2_2 = I2.mul(I2);        // I2^2
    Mat I1_2 = I1.mul(I1);        // I1^2
    Mat I1_I2 = I1.mul(I2);        // I1 * I2

    // PRELIMINARY COMPUTING

    Mat mu1, mu2;   //
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);

    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);

    Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    // FORMULA
    Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    Mat ssim_map;
    divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

    Scalar mssim = mean(ssim_map); // mssim = average of ssim map
    return mssim;
}