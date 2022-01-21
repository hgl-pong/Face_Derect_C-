#pragma once
#include <dlib/opencv.h>
#include "opencv2/opencv.hpp"
#include <vector>
#include <ctime>


//using namespace dlib;
using namespace std;
//using namespace cv;

namespace face_detect {


	class FaceDetector {
	private:
		cv::Mat frame;
		cv::Mat bgr;
		cv::Mat dst;
		dlib::shape_predictor sp;
		dlib::array2d<dlib::bgr_pixel> dimg;
		std::vector<dlib::rectangle> dets;//人脸区域
		//dlib的人脸识别器
		dlib::frontal_face_detector detector;

		void getBGR() {
			cv::cvtColor(frame, bgr, CV_YUV2BGR);
		}

		void getGray() {
			cv::cvtColor(bgr, dst, CV_BGR2GRAY);
		}

		void load_shape_predictor() {
			//加载人脸形状探测器
			dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
		}

		void Mat2Matrix() {
			//Mat转化为dlib的matrix
			dlib::assign_image(dimg, dlib::cv_image<uchar>(dst));
		}


		void getFace() {
			dets = detector(dimg);

			if (dets.size() == 0)
				cout << "未检测到人脸!" << endl;
		}
	public:
		FaceDetector() {
			detector = dlib::get_frontal_face_detector();
			load_shape_predictor();
		}

		void print(int index) {
			//获取一系列人脸所在区域
			cout << "第" << index << "帧人脸检测数量: " << dets.size() << endl;
		}

		
		void putIn(cv::Mat& frame) {
			dimg.clear();

			this->frame = frame;
			getBGR();
			getGray();
			//矩阵换算
			Mat2Matrix();
			//获取人脸
			getFace();
		}

		vector<dlib::rectangle> putOut() {
			return dets;
		}

	};

	
}