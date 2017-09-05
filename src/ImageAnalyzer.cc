// -*- C++ -*-

#include "ImageAnalyzer.hh"

#include <fstream>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace
{
  const std::string& class_name("ImageAnalyzer");
}

//______________________________________________________________________________
ImageAnalyzer::ImageAnalyzer( const cv::String& network,
			      const cv::String& caffe,
			      const cv::String& mean,
			      const cv::String& label,
			      const cv::String& input )
{
  static const std::string func_name("["+class_name+"::"+__func__+"()]");

  cv::dnn::initModule();

  m_network = cv::dnn::readNetFromCaffe(network, caffe);

  m_image_original = cv::imread(input);
  m_image_mean     = cv::imread(mean);

  m_label = ReadLabel(label);

  m_is_ready = ( CheckFile( m_network,        network ) &&
		 CheckFile( m_image_original, input   ) &&
		 CheckFile( m_image_mean,     mean    ) &&
		 CheckFile( m_label,          label   ) );

  return;
}

//______________________________________________________________________________
ImageAnalyzer::~ImageAnalyzer( void )
{
}

//______________________________________________________________________________
template <typename T>
bool
ImageAnalyzer::CheckFile( T object, const cv::String& name )
{
  static const std::string func_name("["+class_name+"::"+__func__+"()]");

  if( object.empty() )
    std::cerr << "#E " << func_name << " no such file : "
	      << name << std::endl;

  return !object.empty();
}

//______________________________________________________________________________
bool
ImageAnalyzer::Process( void )
{
  static const std::string func_name("["+class_name+"::"+__func__+"()]");

  if( !m_is_ready )
    return false;

  Resize( cv::Size(140, 140) );

  SubtractMeanImage();

  cv::dnn::Blob inputBlob = cv::dnn::Blob::fromImages(m_image_subtracted);
  m_network.setBlob(".data", inputBlob);        //set the network input
  m_network.forward();                          //compute output
  cv::dnn::Blob prob = m_network.getBlob("prob");   //gather output of "prob" layer
  double classProb;
  int classId = GetBestClass(prob, &classProb);//find the best class

  std::cout << "Best class: #" << classId << " '"
  	    << m_label.at(classId) << "'" << std::endl;
  std::cout << "Probability: " << classProb * 100 << "%" << std::endl;

  return true;
}

//______________________________________________________________________________
bool
ImageAnalyzer::Resize( cv::Size s )
{
  cv::resize( m_image_original, m_image_original, s );
  return true;
}

//______________________________________________________________________________
bool
ImageAnalyzer::SubtractMeanImage( void )
{
  static const std::string func_name("["+class_name+"::"+__func__+"()]");

  cv::Mat image_CV_16S;
  m_image_original.convertTo(image_CV_16S, CV_16S);
  cv::Mat mean_CV_16S;
  m_image_mean.convertTo(mean_CV_16S, CV_16S);

  cv::subtract(image_CV_16S, mean_CV_16S, m_image_subtracted);

  return true;
}


//______________________________________________________________________________
int
ImageAnalyzer::GetBestClass( cv::dnn::Blob &probBlob, double *classProb )
{
  static const std::string func_name("["+class_name+"::"+__func__+"()]");
  cv::Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
  cv::Point classNumber;
  cv::minMaxLoc( probMat, NULL, classProb, NULL, &classNumber );
  return classNumber.x;
}

//______________________________________________________________________________
std::vector<cv::String>
ImageAnalyzer::ReadLabel( const cv::String& file )
{
  static const std::string func_name("["+class_name+"::"+__func__+"()]");

  std::ifstream ifs( file.c_str() );

  std::string line;
  std::vector<cv::String> labels;

  while( ifs.good() && std::getline( ifs, line ) ){
    if( line.length()>0 )
      labels.push_back( line.substr(line.find(' ')+1) );
  }

  return labels;
}
