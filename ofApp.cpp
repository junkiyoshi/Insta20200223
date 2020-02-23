#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->cap.open(0);
	this->cap.set(cv::CAP_PROP_FPS, 30);
	this->cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

	this->frame_img.allocate(this->cap.get(cv::CAP_PROP_FRAME_WIDTH), this->cap.get(cv::CAP_PROP_FRAME_HEIGHT), OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->frame_img.getHeight(), this->frame_img.getWidth(), CV_MAKETYPE(CV_8UC3, this->frame_img.getPixels().getNumChannels()), this->frame_img.getPixels().getData(), 0);

	this->edge_img.allocate(this->cap.get(cv::CAP_PROP_FRAME_WIDTH), this->cap.get(cv::CAP_PROP_FRAME_HEIGHT), OF_IMAGE_GRAYSCALE);
	this->edge = cv::Mat(this->edge_img.getHeight(), this->edge_img.getWidth(), CV_MAKETYPE(CV_8U, this->edge_img.getPixels().getNumChannels()), this->edge_img.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	if (!this->cap.isOpened()) {
		
		return;
	}

	this->cap >> this->frame;

	if (this->frame.empty()) { 
		return; 
	}

	cv::cvtColor(this->frame, this->frame, cv::COLOR_BGR2RGB);
	cv::flip(this->frame, this->frame, 1);

	if (ofGetFrameNum() == 0) { this->frame.copyTo(this->save_frame); }

	cv::Canny(this->frame, this->edge, 50, 200);

	for (int y = 0; y < this->edge.rows; y += 1) {

		unsigned char* value = &this->edge.at<unsigned char>(y, 0);
		for (int x = 0; x < this->edge.cols; x += 1) {

			if (*value > 0) {

				this->frame.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
			}
			else {

				this->frame.at<cv::Vec3b>(y, x) = this->save_frame.at<cv::Vec3b>(y, x);
			}
			value++;
		}
	}

	this->frame_img.update();
	this->edge_img.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->frame_img.draw(0, 0);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}