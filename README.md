# convolve
This class (Image) uses a convolution mask to iterate across an image to perform simple image processing tasks.

It is used with PGM ascii files, in which each pixel is represented by a number between 0 and 255. 0 represents pure black and 255 represents pure white, with the integer values in between representing shades of grey. 

When you perform a convolution, a new image is generated from the old image based on operations described below.

The operations are represented by a matrix, the convolution mask. For each pixel, you multiply each old pixel's neighbor by its mask value and sum those values up for the new pixel value. 

For edge pixels that do not have the required neighbors to make the calculation, we treat the missing neighbors as 0s.

The methods included are sharpen, edge_detect, blur, and emboss, which simply have the convolution mask associated with the process hard-coded into the method. Each of these methods call convolve, which creates a new image and takes in a convolution mask with dimensions w and h. This mask is applied to the old image, and since the new image is passed by reference it is updated in the caller.
