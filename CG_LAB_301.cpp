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
int task_2a();
int task_2b();
int task_3a();
int task_3b();
int task_3d_rgb();
int task_3d_hsv();
Scalar getMSSIM(const Mat& i1, const Mat& i2);


int main()
{ 
    int opt;

    cout
        << endl
        << "   Main Menu    " << endl
        << endl << "Write 1 for task_1" << endl
        << endl << "Write 2 for task_2a" << endl
        << endl << "Write 3 for task_2b" << endl
        << endl << "Write 4 for task_3a" << endl
        << endl << "Write 5 for task_3b" << endl
        << endl << "Write 6 for task_3d_rgb" << endl
        << endl << "Write 7 for task_3d_hsv" << endl
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
        //Кирилл
        break;
    case 3:
        //Кирилл
        break;
    case 4:
        //Кирилл
        break;
    case 5:
        task_3b();
        break;
    case 6:
        task_3d_rgb();
        break;
    case 7:
        task_3d_hsv();
        break;
    case 0:
        exit(1);
    default:
        break;
    }


    return 0;
}

Mat chooseimg() 
{
    cout << endl << "Enter full name of the file: " << endl;
    string imagename = "";
    cin >> imagename;

    try
    {
        Mat img = imread(samples::findFile(imagename), IMREAD_COLOR); //Поиск изображения 1.jpg происходит в папке samples директории OpenCV
        if (img.empty())
        {
            cout << "Image cannot be loaded..!!" << endl;
            exit(0);
        }

        return img;
    }
    catch (cv::Exception)
    {
        cout << "Image cannot be loaded..!!" << endl;
        exit(0);
    }


}
//Functions
int task_1()
{
    Mat img = chooseimg();
    Mat img1 = chooseimg();

    if (img.size != img1.size)
    {
        cout << "Images have different sizes, that's error: " << endl;
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

    waitKey(0);
    destroyAllWindows();
    main();
}

//Кирилл впишет свою часть сюда

int task_3b()
{
    Mat img1, img2, img3;
    img1 = chooseimg();

    cvtColor(img1, img2, COLOR_RGB2HSV);
    cvtColor(img2, img3, COLOR_HSV2BGR);

    namedWindow("origin", WINDOW_AUTOSIZE);
    namedWindow("rgb2hsv", WINDOW_AUTOSIZE);
    namedWindow("hsv2rgb", WINDOW_AUTOSIZE);

    imshow("origin", img1);
    imshow("rgb2hsv", img2);
    imshow("hsv2rgb", img3);

    waitKey(0); // Wait for a keystroke in the window
    destroyAllWindows();
    main();
}

int task_3d_rgb()
{
    Mat img = chooseimg();

        int brightness = 0;
        cout << endl << "Please, enter multiplier for brightness: ";
        cin >> brightness;

        Mat img_new_brightness;
        img.convertTo(img_new_brightness, -1, 1, brightness);

        namedWindow("New Brightness", WINDOW_AUTOSIZE);
        imshow("New Brightness", img_new_brightness);


        waitKey(0); //wait for key press
        destroyAllWindows();
        main();
}

int task_3d_hsv()
{
    Mat img1, img2;
    img1 = chooseimg();

    cvtColor(img1, img2, COLOR_RGB2HSV);

    int brightness = 0;
    cout << endl << "Please, enter multiplier for brightness: ";
    cin >> brightness;

    Mat img_new_brightness;
    img2.convertTo(img_new_brightness, -1, 1, brightness);

    namedWindow("New Brightness", WINDOW_AUTOSIZE);
    imshow("New Brightness", img_new_brightness);


    waitKey(0); //wait for key press
    destroyAllWindows();
    main();

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