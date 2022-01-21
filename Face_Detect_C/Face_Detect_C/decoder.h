#pragma once
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>
using namespace std;



namespace face_detect {
	class Decoder {
	private:
		queue<cv::Mat>frames;
		cv::VideoCapture cap;
		int framesNum;
		
		void open(string& path) {
			cap = cv::VideoCapture(path,cv::CAP_FFMPEG);
			//isOpen�ж���Ƶ�Ƿ�򿪳ɹ�
			if (!cap.isOpened())
			{
				cerr << "Movie Open Error!" << endl;
			}
			else {
				framesNum = cap.get(cv::CAP_PROP_FRAME_COUNT);//֡����ȡ
				clog << "Movie Open Success!" << endl;
			}
		}
		void getFrameData() {
			double position = 600;
			
			cap.set(CV_CAP_PROP_POS_FRAMES, position);
			while (1)
			{
				cv::Mat frame;
				//��ȡ��Ƶ֡
				if (!cap.read(frame))
					break;

				frames.push(frame);
				frame.release();
			}

		}
	public:
		Decoder(string& path) {
			open(path);
			getFrameData();
		}
		~Decoder() {
			cap.release();
		}
		queue<cv::Mat> getFrames() {
			return frames;
		}
		int getFramesNum() {
			framesNum = frames.size();
			return framesNum;
		}
	};


}