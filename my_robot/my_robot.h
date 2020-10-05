#pragma once
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#define ACCURACY 0.001

class MyRobot
{
    cv::Point2f m_center;
    cv::Size2i m_area;
    float m_angle;
    float m_width;
    float m_height;
    float m_wheelWidth;
    float m_wheelDiameter;
    float m_speedX;
    float m_speedY;
    float m_rotationSpeed;
    cv::Mat m_backend;

//    int move(float speedX, float speedY, float angularSpeed);
    int rotate();
    cv::Point2f localToGlob (float x, float y);
    cv::Point2f globalToLoc (float xGlob, float yGlob);
    void checkArea(float & x, float & y);
public:
    MyRobot()=default;
    ~MyRobot()=default;
    MyRobot(float width, float height,
            float wheelWidth, float wheelDiameter,
            float rotation_speed = 0);

    int motion();
    int move(float speedX, float speedY, float angularSpeed = 0);
    int move_to(float xGlob, float yGlob, float angularSpeed = 0);
    bool stop();

    //сеттеры
    void setSpeed(float speedX, float speedY);
    void setAngularSpeed(float angle);
    void setArea(cv::Size2i area);
    void setBackend(cv::Mat wallpaper);
    void setStop(bool flag);
    int setArea(cv::Mat image);
    int setCenter(cv::Mat image);
    void setAngle (float angle);
    int setCenter(float x, float y);

    //геттеры
    float getSpeed();
    float getAngularSpeed();

    //отрисовка
    int draw();
};
