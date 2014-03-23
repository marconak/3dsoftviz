#include "Kinect/KinectRecognition.h"
#include "QDebug"

using namespace Kinect;

Kinect::KinectRecognition::KinectRecognition()
{
	mOpenniStatus=openni::OpenNI::initialize();
	if(mOpenniStatus != openni::STATUS_OK)
	{
		qDebug() << "Openni initialization failed";
	}
	mOpenniStatus=device.open(openni::ANY_DEVICE);
	if(mOpenniStatus != openni::STATUS_OK)
	{
		qDebug() << "Openni Device not Open";
	}

}

Kinect::KinectRecognition::~KinectRecognition()
{
	openni::OpenNI::shutdown();
}

cv::Mat Kinect::KinectRecognition::colorImageCvMat(openni::VideoFrameRef &colorFrame)
{
	cv::Mat frame;
	const openni::RGB888Pixel* imageBuffer = (const openni::RGB888Pixel*)colorFrame.getData();

	frame.create(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3);
	memcpy( frame.data, imageBuffer, 3*colorFrame.getHeight()*colorFrame.getWidth()*sizeof(uint8_t) );

	cv::cvtColor(frame,frame,CV_BGR2RGB);
	return frame;
}

cv::Mat Kinect::KinectRecognition::depthImageCvMat(openni::VideoFrameRef &colorFrame)
{
	openni::VideoMode mode = colorFrame.getVideoMode();
	cv::Mat image = cv::Mat(mode.getResolutionY(), mode.getResolutionX(), CV_8UC3, (char*)colorFrame.getData());
	cv::cvtColor(image, image, CV_RGB2BGR);
	return image;

}

QImage Kinect::KinectRecognition::colorImageQImage(openni::VideoFrameRef &colorFrame)
{
	QImage image = QImage((uchar*)colorFrame.getData(),
						  colorFrame.getWidth(), colorFrame.getHeight(),
						  QImage::Format_RGB888);
	image = image.rgbSwapped();
	return image;

}

QImage Kinect::KinectRecognition::deptImageQImage(openni::VideoFrameRef &colorFrame)
{
	cv::Mat depthMat = depthImageCvMat(colorFrame);
			QImage image = QImage(depthMat.data,
								  colorFrame.getWidth(), colorFrame.getHeight(),
								  QImage::Format_Indexed8);
	return image.convertToFormat(QImage::Format_RGB32);
}
