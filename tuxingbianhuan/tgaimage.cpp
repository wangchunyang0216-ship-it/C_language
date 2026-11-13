#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "tgaimage.h"

#include <cstdlib>  // 对于整数类型
#include <cmath>    // 对于浮点类型
#include <vector>
#include <algorithm>  // 这里需要包含 <algorithm>


// 透视投影函数
TGAColor TGAImage::project_and_draw_triangle(float x0, float y0, float z0,
                                             float x1, float y1, float z1,
                                             float x2, float y2, float z2,
                                             TGAColor color, bool fill = false) {
    // 进行透视投影，将3D坐标转换为2D
    float s = 1.0f;  // 透视投影的缩放因子，通常为1
    int x0_proj = (int)((x0 / (z0 + 1e-6f)) * s);
    int y0_proj = (int)((y0 / (z0 + 1e-6f)) * s);
    int x1_proj = (int)((x1 / (z1 + 1e-6f)) * s);
    int y1_proj = (int)((y1 / (z1 + 1e-6f)) * s);
    int x2_proj = (int)((x2 / (z2 + 1e-6f)) * s);
    int y2_proj = (int)((y2 / (z2 + 1e-6f)) * s);

    // 调用原始的绘制三角形函数
    draw_triangle(x0_proj, y0_proj, x1_proj, y1_proj, x2_proj, y2_proj, color, fill);
}

TGAImage::TGAImage() : data(NULL), width(0), height(0), bytespp(0) {
}

TGAImage::TGAImage(int w, int h, int bpp) : data(NULL), width(w), height(h), bytespp(bpp) {
	unsigned long nbytes = width*height*bytespp;
	data = new unsigned char[nbytes];
	memset(data, 0, nbytes);
}

TGAImage::TGAImage(const TGAImage &img) {
	width = img.width;
	height = img.height;
	bytespp = img.bytespp;
	unsigned long nbytes = width*height*bytespp;
	data = new unsigned char[nbytes];
	memcpy(data, img.data, nbytes);
}

TGAImage::~TGAImage() {
	if (data) delete [] data;
}

TGAImage & TGAImage::operator =(const TGAImage &img) {
	if (this != &img) {
		if (data) delete [] data;
		width  = img.width;
		height = img.height;
		bytespp = img.bytespp;
		unsigned long nbytes = width*height*bytespp;
		data = new unsigned char[nbytes];
		memcpy(data, img.data, nbytes);
	}
	return *this;
}

bool TGAImage::read_tga_file(const char *filename) {
	if (data) delete [] data;
	data = NULL;
	std::ifstream in;
	in.open (filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "can't open file " << filename << "\n";
		in.close();
		return false;
	}
	TGA_Header header;
	in.read((char *)&header, sizeof(header));
	if (!in.good()) {
		in.close();
		std::cerr << "an error occured while reading the header\n";
		return false;
	}
	width   = header.width;
	height  = header.height;
	bytespp = header.bitsperpixel>>3;
	if (width<=0 || height<=0 || (bytespp!=GRAYSCALE && bytespp!=RGB && bytespp!=RGBA)) {
		in.close();
		std::cerr << "bad bpp (or width/height) value\n";
		return false;
	}
	unsigned long nbytes = bytespp*width*height;
	data = new unsigned char[nbytes];
	if (3==header.datatypecode || 2==header.datatypecode) {
		in.read((char *)data, nbytes);
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	} else if (10==header.datatypecode||11==header.datatypecode) {
		if (!load_rle_data(in)) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	} else {
		in.close();
		std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
		return false;
	}
	if (!(header.imagedescriptor & 0x20)) {
		flip_vertically();
	}
	if (header.imagedescriptor & 0x10) {
		flip_horizontally();
	}
	std::cerr << width << "x" << height << "/" << bytespp*8 << "\n";
	in.close();
	return true;
}

bool TGAImage::load_rle_data(std::ifstream &in) {
	unsigned long pixelcount = width*height;
	unsigned long currentpixel = 0;
	unsigned long currentbyte  = 0;
	TGAColor colorbuffer;
	do {
		unsigned char chunkheader = 0;
		chunkheader = in.get();
		if (!in.good()) {
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		if (chunkheader<128) {
			chunkheader++;
			for (int i=0; i<chunkheader; i++) {
				in.read((char *)colorbuffer.raw, bytespp);
				if (!in.good()) {
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				for (int t=0; t<bytespp; t++)
					data[currentbyte++] = colorbuffer.raw[t];
				currentpixel++;
				if (currentpixel>pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		} else {
			chunkheader -= 127;
			in.read((char *)colorbuffer.raw, bytespp);
			if (!in.good()) {
				std::cerr << "an error occured while reading the header\n";
				return false;
			}
			for (int i=0; i<chunkheader; i++) {
				for (int t=0; t<bytespp; t++)
					data[currentbyte++] = colorbuffer.raw[t];
				currentpixel++;
				if (currentpixel>pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);
	return true;
}

bool TGAImage::write_tga_file(const char *filename, bool rle) {
	unsigned char developer_area_ref[4] = {0, 0, 0, 0};
	unsigned char extension_area_ref[4] = {0, 0, 0, 0};
	unsigned char footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
	std::ofstream out;
	out.open (filename, std::ios::binary);
	if (!out.is_open()) {
		std::cerr << "can't open file " << filename << "\n";
		out.close();
		return false;
	}
	TGA_Header header;
	memset((void *)&header, 0, sizeof(header));
	header.bitsperpixel = bytespp<<3;
	header.width  = width;
	header.height = height;
	header.datatypecode = (bytespp==GRAYSCALE?(rle?11:3):(rle?10:2));
	header.imagedescriptor = 0x20; // top-left origin
	out.write((char *)&header, sizeof(header));
	if (!out.good()) {
		out.close();
		std::cerr << "can't dump the tga file\n";
		return false;
	}
	if (!rle) {
		out.write((char *)data, width*height*bytespp);
		if (!out.good()) {
			std::cerr << "can't unload raw data\n";
			out.close();
			return false;
		}
	} else {
		if (!unload_rle_data(out)) {
			out.close();
			std::cerr << "can't unload rle data\n";
			return false;
		}
	}
	out.write((char *)developer_area_ref, sizeof(developer_area_ref));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.write((char *)extension_area_ref, sizeof(extension_area_ref));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.write((char *)footer, sizeof(footer));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.close();
	return true;
}

// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
bool TGAImage::unload_rle_data(std::ofstream &out) {
	const unsigned char max_chunk_length = 128;
	unsigned long npixels = width*height;
	unsigned long curpix = 0;
	while (curpix<npixels) {
		unsigned long chunkstart = curpix*bytespp;
		unsigned long curbyte = curpix*bytespp;
		unsigned char run_length = 1;
		bool raw = true;
		while (curpix+run_length<npixels && run_length<max_chunk_length) {
			bool succ_eq = true;
			for (int t=0; succ_eq && t<bytespp; t++) {
				succ_eq = (data[curbyte+t]==data[curbyte+t+bytespp]);
			}
			curbyte += bytespp;
			if (1==run_length) {
				raw = !succ_eq;
			}
			if (raw && succ_eq) {
				run_length--;
				break;
			}
			if (!raw && !succ_eq) {
				break;
			}
			run_length++;
		}
		curpix += run_length;
		out.put(raw?run_length-1:run_length+127);
		if (!out.good()) {
			std::cerr << "can't dump the tga file\n";
			return false;
		}
		out.write((char *)(data+chunkstart), (raw?run_length*bytespp:bytespp));
		if (!out.good()) {
			std::cerr << "can't dump the tga file\n";
			return false;
		}
	}
	return true;
}

TGAColor TGAImage::get(int x, int y) {
	if (!data || x<0 || y<0 || x>=width || y>=height) {
		return TGAColor();
	}
	return TGAColor(data+(x+y*width)*bytespp, bytespp);
}

bool TGAImage::set(int x, int y, TGAColor c) {
	if (!data || x<0 || y<0 || x>=width || y>=height) {
		return false;
	}
	memcpy(data+(x+y*width)*bytespp, c.raw, bytespp);
	return true;
}

int TGAImage::get_bytespp() {
	return bytespp;
}

int TGAImage::get_width() {
	return width;
}

int TGAImage::get_height() {
	return height;
}

bool TGAImage::flip_horizontally() {
	if (!data) return false;
	int half = width>>1;
	for (int i=0; i<half; i++) {
		for (int j=0; j<height; j++) {
			TGAColor c1 = get(i, j);
			TGAColor c2 = get(width-1-i, j);
			set(i, j, c2);
			set(width-1-i, j, c1);
		}
	}
	return true;
}

bool TGAImage::flip_vertically() {
	if (!data) return false;
	unsigned long bytes_per_line = width*bytespp;
	unsigned char *line = new unsigned char[bytes_per_line];
	int half = height>>1;
	for (int j=0; j<half; j++) {
		unsigned long l1 = j*bytes_per_line;
		unsigned long l2 = (height-1-j)*bytes_per_line;
		memmove((void *)line,      (void *)(data+l1), bytes_per_line);
		memmove((void *)(data+l1), (void *)(data+l2), bytes_per_line);
		memmove((void *)(data+l2), (void *)line,      bytes_per_line);
	}
	delete [] line;
	return true;
}

unsigned char *TGAImage::buffer() {
	return data;
}

void TGAImage::clear() {
	memset((void *)data, 0, width*height*bytespp);

//	 for (int i = 0; i < width; i++) {
//        for (int j = 0; j < height; j++) {
//            set(i, j, TGAColor(128, 128, 128, 255)); // 设置为灰色背景
//        }
//    }
}


void TGAImage::fill(TGAColor color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            set(i, j, color); // 设置每个像素为背景色
        }
    }
}



bool TGAImage::scale(int w, int h) {
	if (w<=0 || h<=0 || !data) return false;
	unsigned char *tdata = new unsigned char[w*h*bytespp];
	int nscanline = 0;
	int oscanline = 0;
	int erry = 0;
	unsigned long nlinebytes = w*bytespp;
	unsigned long olinebytes = width*bytespp;
	for (int j=0; j<height; j++) {
		int errx = width-w;
		int nx   = -bytespp;
		int ox   = -bytespp;
		for (int i=0; i<width; i++) {
			ox += bytespp;
			errx += w;
			while (errx>=(int)width) {
				errx -= width;
				nx += bytespp;
				memcpy(tdata+nscanline+nx, data+oscanline+ox, bytespp);
			}
		}
		erry += h;
		oscanline += olinebytes;
		while (erry>=(int)height) {
			if (erry>=(int)height<<1) // it means we jump over a scanline
				memcpy(tdata+nscanline+nlinebytes, tdata+nscanline, nlinebytes);
			erry -= height;
			nscanline += nlinebytes;
		}
	}
	delete [] data;
	data = tdata;
	width = w;
	height = h;
	return true;
}




void TGAImage::draw_line(int x0, int y0, int x1, int y1, TGAColor color) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int error = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            set(y, x, color);
        } else {
            set(x, y, color);
        }
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}
void TGAImage::draw_circle(int cx, int cy, int r, TGAColor color, bool filled) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y) {
        if (filled) {
            // Fill the area inside the circle
            draw_line(cx - x, cy + y, cx + x, cy + y, color);
            draw_line(cx - x, cy - y, cx + x, cy - y, color);
            draw_line(cx - y, cy + x, cx + y, cy + x, color);
            draw_line(cx - y, cy - x, cx + y, cy - x, color);
        } else {
            // Only the boundary
            set(cx + x, cy + y, color);
            set(cx - x, cy + y, color);
            set(cx + x, cy - y, color);
            set(cx - x, cy - y, color);
            set(cx + y, cy + x, color);
            set(cx - y, cy + x, color);
            set(cx + y, cy - x, color);
            set(cx - y, cy - x, color);
        }

        x++;
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            y--;
            d += 4 * (x - y) + 10;
        }
    }
}
void TGAImage::draw_rectangle(int x0, int y0, int x1, int y1, TGAColor color, bool filled) {
    if (filled) {
        for (int x = x0; x <= x1; x++) {
            for (int y = y0; y <= y1; y++) {
                set(x, y, color);
            }
        }
    } else {
        draw_line(x0, y0, x1, y0, color);
        draw_line(x1, y0, x1, y1, color);
        draw_line(x1, y1, x0, y1, color);
        draw_line(x0, y1, x0, y0, color);
    }
}



void TGAImage::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color, bool fill = false) {
    // 绘制三角形的边框（连接三个点）
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);

    if (fill) {
        // 找到三角形的上下边界
        int minY = std::min(std::min(y0, y1), y2);  // 计算最小Y
        int maxY = std::max(std::max(y0, y1), y2);  // 计算最大Y

        // 遍历从最小Y到最大Y的所有扫描线
        for (int y = minY; y <= maxY; ++y) {
            // 计算交点坐标
            std::vector<int> xIntersections;

            // 计算y与每条边的交点，确保不重复计算已绘制的边界
            if (y > std::min(y0, y1) && y < std::max(y0, y1)) {
                int xIntersect = (x1 - x0) * (y - y0) / (float)(y1 - y0) + x0;
                xIntersections.push_back(xIntersect);
            }
            if (y > std::min(y1, y2) && y < std::max(y1, y2)) {
                int xIntersect = (x2 - x1) * (y - y1) / (float)(y2 - y1) + x1;
                xIntersections.push_back(xIntersect);
            }
            if (y > std::min(y2, y0) && y < std::max(y2, y0)) {
                int xIntersect = (x0 - x2) * (y - y2) / (float)(y0 - y2) + x2;
                xIntersections.push_back(xIntersect);
            }

            // 对交点进行排序
            std::sort(xIntersections.begin(), xIntersections.end());

            // 在两交点之间填充像素
            for (size_t i = 0; i < xIntersections.size(); i += 2) {
                int xStart = xIntersections[i];
                int xEnd = xIntersections[i + 1];

                if (xStart > xEnd) std::swap(xStart, xEnd);

                // 填充两个交点之间的区域
                for (int x = xStart; x <= xEnd; ++x) {
                    // 仅填充内部区域，不再绘制边框
                    set(x, y, color);
                }
            }
        }
    }
}




//// 绘制三角形，支持填充或仅绘制边框
//void TGAImage::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color, bool fill=false) {
//    // 绘制三角形边框（连接三个点）
//    draw_line(x0, y0, x1, y1, color);
//    draw_line(x1, y1, x2, y2, color);
//    draw_line(x2, y2, x0, y0, color);
//
//    if (fill) {
//        // 使用扫描线填充三角形
//        // 计算三角形的上下边界
//       int minY = std::min(std::min(y0, y1), y2); // 找到最小的Y坐标
//int maxY = std::max(std::max(y0, y1), y2); // 找到最大的Y坐标
//
//
//        // 遍历每一行
//        for (int y = minY; y <= maxY; ++y) {
//            // 找到三角形边界交点
//            int xStart = 0, xEnd = 0;
//
//            // 计算交点的 x 坐标（通过线段的插值法）
//            if ((y >= y0 && y <= y1) || (y >= y1 && y <= y2) || (y >= y2 && y <= y0)) {
//                if (y1 != y0) {
//                    int xIntersect = (int)((x1 - x0) * (y - y0) / (float)(y1 - y0) + x0);
//                    if (y == y0) xStart = xIntersect;
//                    else if (y == y1) xEnd = xIntersect;
//                }
//                if (y2 != y1) {
//                    int xIntersect = (int)((x2 - x1) * (y - y1) / (float)(y2 - y1) + x1);
//                    if (y == y1) xStart = xIntersect;
//                    else if (y == y2) xEnd = xIntersect;
//                }
//                if (y2 != y0) {
//                    int xIntersect = (int)((x2 - x0) * (y - y0) / (float)(y2 - y0) + x0);
//                    if (y == y0) xStart = xIntersect;
//                    else if (y == y2) xEnd = xIntersect;
//                }
//            }
//            // 扫描填充行
//            if (xStart > xEnd) std::swap(xStart, xEnd);
//            for (int x = xStart; x <= xEnd; ++x) {
//                set(x, y, color);  // 填充颜色
//            }
//        }
//    }
//}


//void TGAImage::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color, bool filled) {
//    // Draw the edges of the triangle
//    draw_line(x0, y0, x1, y1, color);
//    draw_line(x1, y1, x2, y2, color);
//    draw_line(x2, y2, x0, y0, color);
//
//    if (filled) {
//        // Fill the triangle using a scanline approach (or other methods)
//    }
//}

