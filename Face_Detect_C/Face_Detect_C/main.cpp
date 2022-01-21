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
//���ó���

int width;
int height;
//����ͼ��ָ��
GLubyte* pixels;
GLint viewPort[4] = { 0 };


Decoder videoDecoder=Decoder(path);
FaceDetector faceDetector;
queue<cv::Mat> frames;
vector<dlib::rectangle> dets;

GLuint load_texture(cv::Mat frame)
{
    //OpenGL��������������ʾ
    GLuint texture_ID;

    //��ȡͼ��ָ��
    int pixellength = width * height * 3;
    pixels = new GLubyte[pixellength];
    memcpy(pixels, frame.data, pixellength * sizeof(char));

    //��texture_ID����Ϊ2D������Ϣ
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //����Ŵ���Сʹ�����Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //��ͼ���ڴ�����������Ϣ
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    free(pixels);
    return texture_ID;
}

void drawFrame(GLuint image) {
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

}
void drawFilter(vector<dlib::rectangle> dets) {
    for (int i = 0; i < dets.size(); i++)
    {
        //���������˾�
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

    // �����Ļ
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glDisable(GL_LIGHTING);
    //��ȡ�������
    GLuint image = load_texture(frames.front());

    //��������OpenGL���ڣ�ԭ��λ��Ϊ���Ͻǣ�x������ң�y����ϵ��£�����ֵ����������ֵ��ͬ
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);

    //��ʾ����
    glEnable(GL_TEXTURE_2D);    //����ʹ������
    glBindTexture(GL_TEXTURE_2D, image);    //ѡ���������
    glPushMatrix();
    
    drawFrame(image);

    glDisable(GL_TEXTURE_2D);    //��ֹʹ������
    glPopMatrix();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    drawFilter(dets);

    glDisable(GL_BLEND);

    //�ͷ�����
    glDeleteTextures(1,&image);
    //˫���潻����������ʾͼ��
    glutSwapBuffers();
}

void main(int argc, char** argv) {

    frames = videoDecoder.getFrames();
    faceDetector = FaceDetector();
    width = frames.front().cols;
    height = frames.front().rows;
    //��ʼ��GL
    glutInit(&argc, argv);
    //������ʾ����(˫���棬RGB��ʽ)
    glutInitDisplayMode(GLUT_RGBA);
    //���ô��ڳߴ磺width*height
    glutInitWindowSize(width, height);
    //���ô���λ�ã�����Ļ���Ͻ�����ֵ(100,100)��
    glutInitWindowPosition(100, 100);
    //���ô�������
    glutCreateWindow("Face_Detector");


	
	faceDetector.putIn(frames.front());
 	dets = faceDetector.putOut();
  

    //��ʾ������display�¼���Ҫ���б�д
    glutDisplayFunc(display);

    //�ظ�ѭ��GLUT�¼�
    glutMainLoop();

}