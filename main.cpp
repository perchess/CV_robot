#include <QCoreApplication>
#include <opencv2/highgui.hpp>
#include <my_robot/my_robot.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cv::Mat img(cv::Size(300, 300), CV_8UC3,cv::Scalar(255,255,255));
    MyRobot bot1(20, 30, 5, 5);
    bot1.setArea(img);
    bot1.setCenter(100, 100);
    bot1.setBackend(img);
    bot1.draw();
    char key = 0;
    while (key != 'q')
    {
        key = char(cv::waitKey(30));
        switch (key)
        {
        case 'w' :
            bot1.move(0, -1);
            break;
        case 's' :
            bot1.move(0, 1);
            break;
        case 'a' :
            bot1.move(-1, 0);
            break;
        case 'd' :
            bot1.move(1, 0);
            break;
        case 'c':
            bot1.move(0,0,0.01f);
            break;
        case 'z':
            bot1.move(0,0,-0.01f);
            break;
        case 'x':
            bot1.stop();
            break;
        case 'g':
            bot1.move_to(100, 110, 0.5f);
//            bot1.move_to(200, 110);
//            bot1.move_to(200, 210);
//            bot1.move_to(100, 210);
            break;
//        default:
//            bot1.move(0, 0);
//            break;
        }
    bot1.draw();
    }
    cv::destroyWindow("robot");

    return 0;
}
