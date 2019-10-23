#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>

struct Fragment
{
  int id;
  int posX;
  int posY;
  float angle;
};
 // faire une liste de struct fragment avec la lecture
int main(int argc, char** argv){

  // Read the fragment file
  std::string line;
  std::list<Fragment> listFragments;
  std::ifstream fragmentsFile("./bin/ressources/fragments.txt");

  // Create Fragment
  while(std::getline(fragmentsFile, line))
  {
    Fragment frag = {};
    sscanf(line.c_str(), "%d %d %d %f", &frag.id, &frag.posX, &frag.posY, &frag.angle);
    listFragments.push_back(frag);
  }

  // Affiche la liste de fragment
  // for(Fragment frag : listFragments)
  // {
  //   std::cout<<frag.id<<std::endl;
  // }

  cv::Mat res(775,1707,CV_8UC4, cv::Scalar(255,255,255,255));

  for(Fragment frag : listFragments)
  {

    std::string pathFragment = "./bin/ressources/frag_eroded/frag_eroded/frag_eroded_";
    cv::Mat matFrag = cv::imread(pathFragment+std::to_string(frag.id)+".png", cv::IMREAD_UNCHANGED);

    cv::Mat matFragRot;
    cv::Point2f rotPoint((matFrag.size().width)/2, (matFrag.size().height)/2);
    cv::Mat matRot = cv::getRotationMatrix2D(rotPoint, frag.angle,1);
    warpAffine(matFrag, matFragRot, matRot, matFrag.size());

    for (size_t i = 0; i < matFragRot.size().height; i++) {
      for (size_t j = 0; j < matFragRot.size().width; j++) {
        if(matFragRot.at<cv::Vec4b>(cv::Point(i,j))[3] == 255)
        {
          if(i+frag.posX - (matFragRot.size().width)/2 < 1707 && j+frag.posY - (matFragRot.size().height) < 775)
          {
              res.at<cv::Vec4b>(cv::Point(i+frag.posX - (matFragRot.size().width)/2, j+frag.posY - (matFragRot.size().height)/2)) = matFragRot.at<cv::Vec4b>(cv::Point(i,j));
          }
        }
      }
    }
  }


  cv::imshow("Result",res);

	// cv::Mat imageIn = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	// if(! imageIn.data )                              // Check for invalid input
  //   	{
  //       	std::cout <<  "Could not open or find the image" << std::endl ;
  //       	return -1;
  //  	  }

	// imshow( "Display window", imageIn );
	cv::waitKey(0);

	return 0;
}
