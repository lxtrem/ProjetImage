#pragma once

#include <vector>
#include "image.h"

Image fusionCouleurs(std::vector<ImageGris> images);

std::vector<ImageGris> splitCouleurs(Image img);

ImageGris convolution(ImageGris img, std::vector<std::vector<int>> noyau);

Image convolution(Image img, std::vector<std::vector<int>> noyau);