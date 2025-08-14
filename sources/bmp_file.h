/*
//
// Copyright (C) 2025 Jean-François DEL NERO
//
// This file is part of PCB Annot.
//
// PCB Annot may be used and distributed without restriction provided
// that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// PCB Annot is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// PCB Annot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with PCB Annot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/

#pragma pack(1)

typedef struct bitmap_data_
{
	int16_t xsize;
	int16_t ysize;
	int16_t nb_color;
	uint8_t * palette;
	void    * data;
}bitmap_data;

typedef struct tagBITMAPFILEHEADER_HXC {
	uint16_t   bfType;
	uint32_t   bfSize;
	uint16_t   bfReserved1;
	uint16_t   bfReserved2;
	uint32_t   bfOffBits;
} BITMAPFILEHEADER_HXC;

typedef struct tagBITMAPINFOHEADER_HXC {
	uint32_t    biSize;
	uint32_t    biWidth;
	uint32_t    biHeight;
	uint16_t    biPlanes;
	uint16_t    biBitCount;
	uint32_t    biCompression;
	uint32_t    biSizeImage;
	uint32_t    biXPelsPerMeter;
	uint32_t    biYPelsPerMeter;
	uint32_t    biClrUsed;
	uint32_t    biClrImportant;
} BITMAPINFOHEADER_HXC;

typedef struct tagRGBQUAD_HXC {
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
} RGBQUAD_HXC;

#pragma pack()

int bmp_load(char * file,bitmap_data * bdata);
int bmpRLE8b_write(char * file,bitmap_data * bdata);
int bmp24b_write(char * file,bitmap_data * bdata);
int bmp16b_write(char * file,bitmap_data * bdata);

unsigned char getPixelCode(uint32_t pix,uint32_t * pal,int * nbcol);
