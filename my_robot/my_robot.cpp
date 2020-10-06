#include "my_robot.h"


MyRobot::MyRobot(float width, float height,
                 float wheelWidth, float wheelDiameter,
                 float rotation_speed)
    : m_width(width)
    , m_height(height)
    , m_wheelWidth(wheelWidth)
    , m_wheelDiameter(wheelDiameter)
    , m_rotationSpeed(rotation_speed)
{setSpeed(0, 0);}


void MyRobot::setSpeed(float speedX, float speedY)
{
    this->m_speedX = speedX;
    this->m_speedY = speedY;
}

void MyRobot::setAngularSpeed(float angle) {this->m_rotationSpeed = angle;}

int MyRobot::setArea(cv::Mat image)
{
    if (image.empty())
        return -1;
    this->m_area.height = image.rows;
    this->m_area.width = image.cols;
    return 1;
}

void MyRobot::setArea(cv::Size2i area) {this->m_area = area;}

float MyRobot::getSpeed() {return (this->m_speedX + this->m_speedY)/2;}

float MyRobot::getAngularSpeed() {return this->m_rotationSpeed;}

int MyRobot::draw()
{
    this->setAngle(this->m_angle + this->m_rotationSpeed);
  
    cv::Point2f vertices[4];
    vertices[0] = localToGlob(this->m_height / 2, -this->m_width / 2);
    vertices[1] = localToGlob(this->m_height / 2, this->m_width / 2);
    vertices[3] = localToGlob(-this->m_height / 2, -this->m_width / 2);
    vertices[2] = localToGlob(-this->m_height / 2, this->m_width / 2);
    setCenter(((vertices[0].x + vertices[2].x)/2.0f), ((vertices[0].y + vertices[2].y)/2.0f));
    cv::Mat background = this->m_backend.clone();
    for (int i = 0; i < 4; i++)
    {
        cv::line(background, vertices[i], vertices[(i+1)%4], cv::Scalar(0,255,0), 2);
        // Задний бампер
        if (i == 0)
            cv::line(background, vertices[i], vertices[(i+1)%4], cv::Scalar(255,0,0), 2);
        // Передний бампер
        if (i == 2)
            cv::line(background, vertices[i], vertices[(i+1)%4], cv::Scalar(0,0,255), 2);
    }
    cv::imshow("robot", background);
    return 0;
}

int MyRobot::setCenter(cv::Mat image)
{
    if (image.empty())
        return -1;
    if ((image.cols >= m_area.width) || (image.rows >= m_area.height))
        return -2;
    this->m_center.x = image.rows / 2;
    this->m_center.y = image.cols / 2;
    return 1;
}

int MyRobot::setCenter(float x, float y)
{
    if ((x > m_area.width) || (y > m_area.height))
        return -1;
    this->m_center.x = x;
    this->m_center.y = y;
    return 1;
}

int MyRobot::move(float speedX, float speedY, float angularSpeed)
{
    setAngularSpeed(this->m_rotationSpeed + angularSpeed);
    setSpeed(this->m_speedX + speedX, this->m_speedY + speedY);
    this->setAngle(this->m_angle + this->m_rotationSpeed);
    return 0;
}


void MyRobot::setBackend(cv::Mat wallpaper)
{
     this->m_backend = wallpaper.clone();
}

void MyRobot::setAngle (float angle)
{
    this->m_angle = angle;
}

cv::Point2f MyRobot::localToGlob (float yLoc, float xLoc)
{
    float yLoc_ = yLoc + this->m_speedY;
    float xLoc_ = xLoc + this->m_speedX;

    float xGlob = xLoc_ * std::cos(this->m_angle) + this->m_center.x - yLoc_ * std::sin(this->m_angle);
    float yGlob = xLoc_ * std::sin(this->m_angle) + this->m_center.y + yLoc_ * std::cos(this->m_angle);

    //проверка выхода за границы

    checkArea(xGlob, yGlob);
    cv::Point2f out(xGlob, yGlob);
    return out;
}


cv::Point2f MyRobot::globalToLoc(float xGlob, float yGlob)
{
    float xLoc = xGlob * std::cos(m_angle) + yGlob * std::sin(m_angle) - m_center.x * std::cos(m_angle) - m_center.y * std::sin(m_angle);
    float yLoc = yGlob * std::cos(m_angle) - xGlob * std::sin(m_angle) - m_center.y * std::cos(m_angle) + m_center.x * std::sin(m_angle);
    cv::Point2f out(xLoc, -yLoc);
    return out;
}

void MyRobot::checkArea(float & x, float & y)
{
    if ((x >= this->m_area.width) || (x <= 0) || (y >= this->m_area.height) || (y <= 0))
    {
        this->stop();
        return;
    }
}


bool MyRobot::stop()
{
    setSpeed(0, 0);
    setAngularSpeed(0.0);
    return 1;
}

float regulator(int err)
{
    float out = 0;
    if (err == 0)
    {
        out = 0;
        return out;
    }
    out = std::signbit(err) ? -0.1 : 0.1;
    return out;
}

float regulator(float err)
{
    float out = 0;
    if (std::abs(err) <= ACCURACY)
    {
        out = 0;
        return out;
    }
    out = std::signbit(err) ? -0.01 : 0.01;
    return out;
}


int MyRobot::move_to(float xGlob, float yGlob, float angularSpeed)
{
    cv::Point3f dest(xGlob, yGlob, angularSpeed);
    cv::Point3f pose(m_center.x, m_center.y, m_angle);
    cv::Point3f err = dest - pose;

    while (int(err.x) != 0 || int(err.y) != 0 || std::abs(err.z) >= ACCURACY)
    {
        pose = cv::Point3f(m_center.x, m_center.y, m_angle);
        err = dest - pose;
        float xSpeed = regulator(int(err.x));
        float ySpeed = regulator(int(err.y));
        float aSpeed = regulator(err.z);
        setSpeed(xSpeed, ySpeed);
        setAngularSpeed(aSpeed);
        draw();
        cv::waitKey(1);
    }
    stop();
    return 0;

}


