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
		std::vector<dlib::rectangle> dets;//��������
		//dlib������ʶ����
		dlib::frontal_face_detector detector;

		void getBGR() {
			cv::cvtColor(frame, bgr, CV_YUV2BGR);
		}

		void getGray() {
			cv::cvtColor(bgr, dst, CV_BGR2GRAY);
		}

		void load_shape_predictor() {
			//����������״̽����
			dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
		}

		void Mat2Matrix() {
			//Matת��Ϊdlib��matrix
			dlib::assign_image(dimg, dlib::cv_image<uchar>(dst));
		}


		void getFace() {
			dets = detector(dimg);

			if (dets.size() == 0)
				cout << "δ��⵽����!" << endl;
		}
	public:
		FaceDetector() {
			detector = dlib::get_frontal_face_detector();
			load_shape_predictor();
		}

		void print(int index) {
			//��ȡһϵ��������������
			cout << "��" << index << "֡�����������: " << dets.size() << endl;
		}

		
		void putIn(cv::Mat& frame) {
			dimg.clear();

			this->frame = frame;
			getBGR();
			getGray();
			//������
			Mat2Matrix();
			//��ȡ����
			getFace();
		}

		vector<dlib::rectangle> putOut() {
			return dets;
		}

	};

	
}