#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "windows.h"
#include <GL/glut.h>
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;

//OpenCV��ȡͼ��
cv::Mat I = cv::imread("0001.jpg");
//���ó���
int width = I.cols;
int height = I.rows;
//����ͼ��ָ��
GLubyte* pixels;
GLint viewPort[4] = { 0 };


GLuint load_texture()
{
    //OpenGL��������������ʾ
    GLuint texture_ID;

    //��ȡͼ��ָ��
    int pixellength = width * height * 3;
    pixels = new GLubyte[pixellength];
    memcpy(pixels, I.data, pixellength * sizeof(char));
    //imshow("OpenCV", I);

    //��texture_ID����Ϊ2D������Ϣ
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //����Ŵ���Сʹ�����Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //����ˮƽ��ֱ��������ʹ���ظ���ͼ
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //����ˮƽ��ֱ��������ʹ�ñ�Ե������ͼ(���ظ���ͼ��ѡһ)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //��ͼ���ڴ�����������Ϣ
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

    // �����Ļ
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glDisable(GL_LIGHTING);
    //��ȡ�������
    GLuint image = load_texture();

    //��������OpenGL���ڣ�ԭ��λ��Ϊ���Ͻǣ�x������ң�y����ϵ��£�����ֵ����������ֵ��ͬ
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);

    //��ʾ����
    glEnable(GL_TEXTURE_2D);    //����ʹ������
    glBindTexture(GL_TEXTURE_2D, image);    //ѡ���������
    glPushMatrix();
    //ԭʼ��ȫ����ı���
    glBegin(GL_POLYGON);    //����Ϊ�����������ͼ��ʽ����ʼ��ͼ
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);    //�������ϽǶ�Ӧ�������Ͻ�
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);    //�������½Ƕ�Ӧ�������½�
    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);    //�������½Ƕ�Ӧ�������½�
    glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);    //�������ϽǶ�Ӧ�������Ͻ�
    glEnd();    //������ͼ*/

                /*//������
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
                glEnd();*/

                /*    //�����
                    glBegin(GL_POLYGON);
                    glTexCoord2f(0, 0); glVertex2f(0, 0);
                    glTexCoord2f(0, 1); glVertex2f(0, height/3);
                    glTexCoord2f(1, 1); glVertex2f(width/4, height/3);
                    glTexCoord2f(1, 0); glVertex2f(width/4, 0);
                    glTexCoord2f(0.0f, 0.5f); glVertex2f(0, height/2);
                    glEnd();*/

                    /*//����任
                    glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f); glVertex2f(width/4, height/4);
                    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
                    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height*2/3);
                    glTexCoord2f(1.0f, 0.0f); glVertex2f(width*4/5, 50);
                    glEnd();*/

                    //��Ե��ͼЧ��
                    /*glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
                    glTexCoord2f(0.0f, 2.0f); glVertex2f(0, height);
                    glTexCoord2f(2.0f, 2.0f); glVertex2f(width, height);
                    glTexCoord2f(2.0f, 0.0f); glVertex2f(width, 0);
                    glEnd();*/

    glDisable(GL_TEXTURE_2D);    //��ֹʹ������
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

    //˫���潻����������ʾͼ��
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
    //��ʼ��GL
    glutInit(&argc, argv);
    //������ʾ����(˫���棬RGB��ʽ)
    glutInitDisplayMode(GLUT_RGBA);
    //���ô��ڳߴ磺width*height
    glutInitWindowSize(width, height);
    //���ô���λ�ã�����Ļ���Ͻ�����ֵ(100,100)��
    glutInitWindowPosition(100, 100);
    //���ô�������
    glutCreateWindow("OpenGL");
    //��ʾ������display�¼���Ҫ���б�д
    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    //�ظ�ѭ��GLUT�¼�
    glutMainLoop();
}