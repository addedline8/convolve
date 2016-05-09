#include <iostream>
using std::cout; using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string; using std::stol;
#include <fstream>
using std::ifstream; using std::ofstream;

#include "classImage.h"
#include "split.h"
/*
Convolve uses a quaduaple for loop to iterate through each pixel, while
getting the values of the pixels around it and the mask values. 

once it obtains each value it appends it to a new vector and then that 
vector to a vector. From there it uses the new 2D vector to update v_
*/
void Image::convolve(Image& i, vector<vector<long>> mask, long w, long h, long div, long whiten){
	long value=0;
	
	vector<long> values;
	
	vector<vector<long>> test;
	
	for (int row_num=0;row_num<i.height_;row_num++){//iterate through each row
		
		for(int col_num=0;col_num<i.width_;col_num++){//iterate through each element in the row
			
			value=0;
			
			for(int mask_row=0,r=(1-h)/2;mask_row<h;mask_row++,r++){//gets a row of the mask
				
				for(int mask_col=0,c=(1-w)/2;mask_col<w;mask_col++,c++){//gets the elments of a row of the mask
					
					if(row_num+r<i.height_&& row_num+r>=0 && col_num+c<i.width_&& col_num+c>=0){//check if the dimensions are in the matrix v_
						
						value+=mask[mask_row][mask_col]*i.v_[row_num+r][col_num+c];//calulate each value
						
					}
					
					else{value=value+0;}//outside the matrix the value is 0
					
				}
				
			}
		
			value /= div;
		
			value += whiten;
		
			if (value>i.max_val_){
			
				value=i.max_val_;
			
			}
		
			else if(value<0){
			
				value=0;
			
			}
		
			values.push_back(value);//adds each value to values(row) matrix
		
		}
		
		test.push_back(values);//adds each row to the test matrix
		
		values.clear();//resets the row
		
	}
	
	i.v_=test;//updates the images data member
}
/*
Reads the contents of the file, ignoring comments with #
Format: 
File type
Width Height (space seperated)
Maximum Value
Row 1 
...
Row Height
uses split to grab each number and stol to convert.
*/
Image::Image(string f_name){
	
	long count=0,w;
	
	int siz = 0;
	
	vector<string> size;
	
	vector<string> row_str;
	
	vector<long> row;
	
	ifstream ifs; 
	
	ifs.open(f_name);
	
	string line;
	
	if (ifs.is_open()){
		
		while ( getline (ifs,line) ){
			
			if (line[0] != '#' && line != ""){
				
		switch(count){
			
			case 0:
			
				//file type
				break;
				
			case 1:
			
				//width by height
				split(line,size,"0123456789");
				
				width_ = stol(size[0]); //stores width and height
				
				w=width_;
				
				height_ = stol(size[1]);
				
				size.clear();
				
				break;
				
			case 2:
			
				max_val_= stol(line);
				
				break;
				
			default: 
			
			//format: matrix (WxH)
				split(line,row_str,"0123456789");
				
				for (auto ele:row_str){
					
					row.push_back(stol(ele));
					
					siz++;
					
					if (siz==w){//Starts new row
						
						v_.push_back(row);
						
						row.clear();
						
						siz=0;
						
					}
					
				}
				row_str.clear();
		}
	
		count++;}}//count the lines
		
		ifs.close();	
		
	}
	
}
/*
Copy constructor 
*/
Image::Image(Image& new_img) {
	
	width_= new_img.width_; 
	
	height_= new_img.height_; 
	
	max_val_= new_img.max_val_; 
	
	v_= new_img.v_;
	
}
/*
Writes the data members of a Image to a 
file using the standard PMG format.
*/
void Image::write_image(string f_name){
	
	ofstream ofs;
	
	ofs.open (f_name);
	 
	long row_num=0;
	
	if (ofs.is_open()){
		for(int i=0;i<height_+3;i++){
			switch(i){
				case 0://first line
					ofs << "P2\n";
					break;
				case 1://second line
					ofs << width_<<" "<<height_<<"\n";
					break;
				case 2://third line
					ofs << max_val_<<"\n";
					break;
				default: 
					for(auto ele:v_[row_num]){
						ofs<<ele<<" ";
					}
					row_num++;
					ofs<<"\n";
			}
			
		}
	
		ofs.close();
	
	}
	
}
/*
uses the sharpen mask and calls convolove to edit the image
*/
Image Image::sharpen(){
	
	Image sharp_img(*this);
	
	vector<vector<long>> s_mask ={{0,-1,0},{-1,5,-1},{0,-1,0}};
	
	convolve(sharp_img,s_mask,3,3);
	
return sharp_img;	

}
/*
uses the edge detect mask and calls convolove to edit the image
*/
Image Image::edge_detect(){
	
	Image edge_img(*this);
	
	vector<vector<long>> edge_mask ={{0,1,0},{1,-4,1},{0,1,0}};
	
	convolve(edge_img,edge_mask,3,3,1,50);
	
return edge_img;

}
/*
uses the blur mask and calls convolove to edit the image
*/
Image Image::blur(){
	
	Image blur_img(*this);
	
	vector<vector<long>> blur_mask ={{1,1,1},{1,1,1},{1,1,1}};
	
	convolve(blur_img,blur_mask,3,3,9);
	
return blur_img;	

}
/*
uses the emboss mask and calls convolove to edit the image 
*/
Image Image::emboss(){
	
	Image emboss_img(*this);	
	
	vector<vector<long>> emboss_mask ={{-2,-1,0},{-1,1,1},{0,1,2}};
	
	convolve(emboss_img,emboss_mask,3,3);
	
return emboss_img;

}

