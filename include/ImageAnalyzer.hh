// -*- C++ -*-

#ifndef IMAGE_ANALYZER_HH
#define IMAGE_ANALYZER_HH

#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>

//______________________________________________________________________________
class ImageAnalyzer
{
public:
  ImageAnalyzer( const cv::String& network,
		 const cv::String& caffe,
		 const cv::String& mean,
		 const cv::String& label,
		 const cv::String& input );
  ~ImageAnalyzer( void );

private:
  ImageAnalyzer( ImageAnalyzer& );
  ImageAnalyzer& operator =( const ImageAnalyzer& );

private:
  bool         m_is_ready;
  std::vector<cv::String> m_label;
  cv::dnn::Net m_network;
  cv::Mat      m_image_original;
  cv::Mat      m_image_mean;
  cv::Mat      m_image_subtracted;

public:
  bool IsReady( void ) const { return m_is_ready; }
  bool Process( void );
  bool Resize( cv::Size s );
  bool SubtractMeanImage( void );
  int  GetBestClass( cv::dnn::Blob& probBlob, double *classProb );
  std::vector<cv::String> ReadLabel( const cv::String& file );
  void Print( const cv::String& arg="" );

private:
  template <typename T>
  bool CheckFile( T object, const cv::String& name );

};

#endif
