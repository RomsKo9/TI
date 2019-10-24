#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

struct Fragment
{
  int id;
  int posX;
  int posY;
  float angle;
  float size;
};

int calculFragSize(cv::Mat matFrag)
{
  return matFrag.size().height*matFrag.size().width;
}

int main(int argc, char** argv){

  std::string line;
  std::list<Fragment> listFragments;
  std::list<Fragment> listSolutions;
  std::ifstream fragmentsFile("./ressources/fragments.txt");
  std::ifstream solutionFile("./ressources/solution.txt");

  int fragSizeTotalFresque = 0;
  int fragSizeOutFresque = 0;
  int fragSizeInFresque = 0;
  int nbFragOK = 0;
  int nbFragOutFresque = 0;
  float precision = 0;

  std::string pathFragment = "./ressources/frag_eroded/frag_eroded/frag_eroded_";

  while(std::getline(fragmentsFile, line))
  {
    Fragment frag = {};
    sscanf(line.c_str(), "%d %d %d %f", &frag.id, &frag.posX, &frag.posY, &frag.angle);
    cv::Mat matFrag = cv::imread(pathFragment+std::to_string(frag.id)+".png", cv::IMREAD_UNCHANGED);
    frag.size = calculFragSize(matFrag);
    listFragments.push_back(frag);
    fragSizeTotalFresque += frag.size;
  }

  while (std::getline(solutionFile, line)) {
    Fragment frag = {};
    sscanf(line.c_str(), "%d %d %d %f", &frag.id, &frag.posX, &frag.posY, &frag.angle);
    cv::Mat matFrag = cv::imread(pathFragment+std::to_string(frag.id)+".png", cv::IMREAD_UNCHANGED);
    frag.size = calculFragSize(matFrag);
    listSolutions.push_back(frag);
  }


  for(Fragment frag : listFragments)
  {

    for(Fragment fragSol : listSolutions)
    {
      if (fragSol.posX == frag.posX && fragSol.posY == frag.posY && fragSol.angle == frag.angle) {

        if(abs(fragSol.posX-frag.posX)<1 && abs(fragSol.posY-frag.posY)<1 && abs(fragSol.angle-frag.angle)<1)
        {
          nbFragOK++;
          fragSizeInFresque += frag.size;
        }
        break;
      }
    }
  }

  for(Fragment fragSol : listSolutions)
  {
    bool trouve = false;

    for(Fragment frag : listFragments)
    {
      if (fragSol.posX == frag.posX && fragSol.posY == frag.posY && fragSol.angle == frag.angle) {
        trouve = true;
        break;
      }
    }
    if(!trouve)
    {
      nbFragOutFresque++;
      fragSizeOutFresque += fragSol.size;
    }
  }

  precision = (float)(fragSizeInFresque-fragSizeOutFresque)/(float)fragSizeTotalFresque;

  std::cout << "Fragments Correctement placés : "<<nbFragOK<<" -- Taille : "<<fragSizeInFresque << '\n';
  std::cout << "Fragments Mal Placés : "<<nbFragOutFresque<<" -- Taille : "<<fragSizeOutFresque << '\n';
  std::cout << "Taille total des fragments de la Fresque :  "<<fragSizeTotalFresque << '\n';
  std::cout << "Précision : "<<precision*100.<<"%" << '\n';

	return 0;
}
