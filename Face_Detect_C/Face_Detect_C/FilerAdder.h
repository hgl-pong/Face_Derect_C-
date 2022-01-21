#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "windows.h"
#include <GL/glut.h>
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;

//OpenCV读取图像
cv::Mat I = cv::imread("0001.jpg");
//设置长宽
int width = I.cols;
int height = I.rows;
//设置图像指针
GLubyte* pixels;
GLint viewPort[4] = { 0 };


GLuint load_texture()
{
    //OpenGL纹理用整型数表示
    GLuint texture_ID;

    //获取图像指针
    int pixellength = width * height * 3;
    pixels = new GLubyte[pixellength];
    memcpy(pixels, I.data, pixellength * sizeof(char));
    //imshow("OpenCV", I);

    //将texture_ID设置为2D纹理信息
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //纹理放大缩小使用线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //纹理水平竖直方向外扩使用重复贴图
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //纹理水平竖直方向外扩使用边缘像素贴图(与重复贴图二选一)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //将图像内存用作纹理信息
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    free(pixels);
    return texture_ID;
}
typedef GLbyte* bytePt;
int arrLen = width * height * 3;
GLbyte* colorArr = new GLbyte[arrLen];


void saveColorData2img(bytePt& _pt, string _str) {
    glGetIntegerv(GL_VIEWPORT, viewPort);
    glReadPixels(viewPort[0], viewPort[1], viewPort[2], viewPort[3], GL_RGB, GL_UNSIGNED_BYTE, colorArr);
    printf("color data read !\n");
    cv::Mat img;
    vector<cv::Mat> imgPlanes;
    img.create(height, width, CV_8UC3);
    cv::split(img, imgPlanes);

    for (int i = 0; i < height; i++) {
        UCHAR* plane0Ptr = imgPlanes[0].ptr<UCHAR>(i);
        UCHAR* plane1Ptr = imgPlanes[1].ptr<UCHAR>(i);
        UCHAR* plane2Ptr = imgPlanes[2].ptr<UCHAR>(i);
        for (int j = 0; j < width; j++) {
            int k = 3 * (i * width + j);
            plane2Ptr[j] = _pt[k];
            plane1Ptr[j] = _pt[k + 1];
            plane0Ptr[j] = _pt[k + 2];
        }
    }
    cv::merge(imgPlanes, img);
    cv::flip(img, img, 0); // !!!
    cv::imwrite(_str.c_str(), img);

    printf("opencv save opengl img done! \n");
}

void display()
{

    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glDisable(GL_LIGHTING);
    //获取纹理对象
    GLuint image = load_texture();

    //重新设置OpenGL窗口：原点位置为左上角，x轴从左到右，y轴从上到下，坐标值与像素坐标值相同
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);

    //显示纹理
    glEnable(GL_TEXTURE_2D);    //允许使用纹理
    glBindTexture(GL_TEXTURE_2D, image);    //选择纹理对象
    glPushMatrix();
    //原始完全填充四边形
    glBegin(GL_POLYGON);    //设置为多边形纹理贴图方式并开始贴图
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);    //纹理左上角对应窗口左上角
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);    //纹理左下角对应窗口左下角
    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);    //纹理右下角对应窗口右下角
    glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);    //纹理右上角对应窗口右上角
    glEnd();    //结束贴图*/

                /*//三角形
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
                glEnd();*/

                /*    //多边形
                    glBegin(GL_POLYGON);
                    glTexCoord2f(0, 0); glVertex2f(0, 0);
                    glTexCoord2f(0, 1); glVertex2f(0, height/3);
                    glTexCoord2f(1, 1); glVertex2f(width/4, height/3);
                    glTexCoord2f(1, 0); glVertex2f(width/4, 0);
                    glTexCoord2f(0.0f, 0.5f); glVertex2f(0, height/2);
                    glEnd();*/

                    /*//任意变换
                    glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f); glVertex2f(width/4, height/4);
                    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
                    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height*2/3);
                    glTexCoord2f(1.0f, 0.0f); glVertex2f(width*4/5, 50);
                    glEnd();*/

                    //边缘贴图效果
                    /*glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
                    glTexCoord2f(0.0f, 2.0f); glVertex2f(0, height);
                    glTexCoord2f(2.0f, 2.0f); glVertex2f(width, height);
                    glTexCoord2f(2.0f, 0.0f); glVertex2f(width, 0);
                    glEnd();*/

    glDisable(GL_TEXTURE_2D);    //禁止使用纹理
    glPopMatrix();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor4f(0.2f, 1.0f, 0.2f, 0.4);
    glVertex2f(0, 0);
    glVertex2f(0, 100);
    glVertex2f(100, 100);
    glVertex2f(100, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);

    //双缓存交换缓存以显示图像
    glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'c':
        saveColorData2img(colorArr, "tmpcolor.jpg");
    default:
        break;
    }
}


void main(int argc, char** argv)
{
    //初始化GL
    glutInit(&argc, argv);
    //设置显示参数(双缓存，RGB格式)
    glutInitDisplayMode(GLUT_RGBA);
    //设置窗口尺寸：width*height
    glutInitWindowSize(width, height);
    //设置窗口位置：在屏幕左上角像素值(100,100)处
    glutInitWindowPosition(100, 100);
    //设置窗口名称
    glutCreateWindow("OpenGL");
    //显示函数，display事件需要自行编写
    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    //重复循环GLUT事件
    glutMainLoop();
}