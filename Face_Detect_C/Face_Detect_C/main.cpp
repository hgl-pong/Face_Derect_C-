#pragma once
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include<Windows.h>
#include<GL/glut.h>

#include"decoder.h"
#include"face_detector.h"
using namespace face_detect;


string path = "../video/test.mp4";
//设置长宽

int width;
int height;
//设置图像指针
GLubyte* pixels;
GLint viewPort[4] = { 0 };


Decoder videoDecoder=Decoder(path);
FaceDetector faceDetector;
queue<cv::Mat> frames;
vector<dlib::rectangle> dets;

GLuint load_texture(cv::Mat frame)
{
    //OpenGL纹理用整型数表示
    GLuint texture_ID;

    //获取图像指针
    int pixellength = width * height * 3;
    pixels = new GLubyte[pixellength];
    memcpy(pixels, frame.data, pixellength * sizeof(char));

    //将texture_ID设置为2D纹理信息
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //纹理放大缩小使用线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //将图像内存用作纹理信息
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    free(pixels);
    return texture_ID;
}

void drawFrame(GLuint image) {
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

}
void drawFilter(vector<dlib::rectangle> dets) {
    for (int i = 0; i < dets.size(); i++)
    {
        //画出人脸滤镜
        glPushMatrix();
        glBegin(GL_QUADS);
        glColor4f(0.2f, 1.0f, 0.2f, 0.4);
        glVertex2f(dets[i].left(), dets[i].top());
        glVertex2f(dets[i].left(), dets[i].top() + dets[i].height());
        glVertex2f(dets[i].left() + dets[i].width(), dets[i].top() + dets[i].height());
        glVertex2f(dets[i].left() + dets[i].width(), dets[i].top());
        glEnd();
        glPopMatrix();
    }
}
void display()
{

    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glDisable(GL_LIGHTING);
    //获取纹理对象
    GLuint image = load_texture(frames.front());

    //重新设置OpenGL窗口：原点位置为左上角，x轴从左到右，y轴从上到下，坐标值与像素坐标值相同
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);

    //显示纹理
    glEnable(GL_TEXTURE_2D);    //允许使用纹理
    glBindTexture(GL_TEXTURE_2D, image);    //选择纹理对象
    glPushMatrix();
    
    drawFrame(image);

    glDisable(GL_TEXTURE_2D);    //禁止使用纹理
    glPopMatrix();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    drawFilter(dets);

    glDisable(GL_BLEND);

    //释放纹理
    glDeleteTextures(1,&image);
    //双缓存交换缓存以显示图像
    glutSwapBuffers();
}

void main(int argc, char** argv) {

    frames = videoDecoder.getFrames();
    faceDetector = FaceDetector();
    width = frames.front().cols;
    height = frames.front().rows;
    //初始化GL
    glutInit(&argc, argv);
    //设置显示参数(双缓存，RGB格式)
    glutInitDisplayMode(GLUT_RGBA);
    //设置窗口尺寸：width*height
    glutInitWindowSize(width, height);
    //设置窗口位置：在屏幕左上角像素值(100,100)处
    glutInitWindowPosition(100, 100);
    //设置窗口名称
    glutCreateWindow("Face_Detector");


	
	faceDetector.putIn(frames.front());
 	dets = faceDetector.putOut();
  

    //显示函数，display事件需要自行编写
    glutDisplayFunc(display);

    //重复循环GLUT事件
    glutMainLoop();

}