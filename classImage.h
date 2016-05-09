#ifndef IMAGE_CLASS
#define IMAGE_CLASS

#include <vector>
using std::vector;
#include<string>
using std::string;

class Image{
private:
  vector<vector<long> > v_;
  long height_;
  long width_;
  long max_val_;
  void convolve(Image& i, vector<vector<long> > mask, long w, long h, long div=1, long whiten=0);

public:
  Image()=default;
  Image (string f_name);
  Image (Image& new_img);
  void write_image(string f_name);
  Image sharpen();
  Image edge_detect();
  Image blur();
  Image emboss();

};

#endif
