#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bmp.h"

bool _check_bmp_header_error(BMPHeader* bmp_hdr, FILE* fp, char** error);
void _error_message(char** error, char* message);
long int _find_num_chars_file(FILE* fp);
long int _offset_crop(long int x, long int y, long int width, long int height);

long int _find_num_chars_file(FILE* fp){
	long int num_chars = 0;
	fseek(fp, 0, SEEK_SET);
	while(true){
		fgetc(fp);
		if(feof(fp)){
			break;
		}
		num_chars += 1;
	}
	return num_chars;
}

void _error_message(char** error, char* message){
	*error = malloc(sizeof(**error) * (strlen(message) + 1));
	strcpy(*error, message);
}

BMPImage* read_bmp(FILE* fp, char** error)
{
	BMPImage* image = malloc(sizeof(*image));
	if(image == NULL){
		_error_message(error, "Error: could not allocate space for image");
		free(image);
		return NULL;
	}
	fseek(fp, 0, SEEK_SET);

	int read_check = fread(image, BMP_HEADER_SIZE, 1, fp);
	if(read_check != 1){
		_error_message(error, "Error: could not read file header");
		free(image);
		return NULL;
	}
	long int total_chars = _find_num_chars_file(fp);
	long int num_pixel_bytes = total_chars - BMP_HEADER_SIZE;
	if(num_pixel_bytes == 0){
		_error_message(error, "Error: image file only contains a header");
		free(image);
		return NULL;
	}
	image->data = malloc(sizeof(*image->data) * num_pixel_bytes);
	if(image->data == NULL){
		_error_message(error, "Error: could not allocate space for image data");
		free(image);
		return NULL;
	}
	fseek(fp, BMP_HEADER_SIZE, SEEK_SET);
	for(long int i = 0; i < num_pixel_bytes; i++){
		read_check = fread(&(image->data[i]), 1, 1, fp);
		if(read_check != 1){
			_error_message(error, "Error: could not read pixel data in file");
			free_bmp(image);
			return NULL;
		}
	}
	
	bool ok = _check_bmp_header_error(&image->header, fp, error);
	if(ok == false){
		free_bmp(image);
		return NULL;
	}

	return image;
}

bool check_bmp_header(BMPHeader* bmp_hdr, FILE* fp){
	char* error = NULL;
	bool ok = _check_bmp_header_error(bmp_hdr, fp, &error);
	return ok;
}

bool _check_bmp_header_error(BMPHeader* bmp_hdr, FILE* fp, char** error){
	//1
	if(bmp_hdr -> type != 0x4d42){
		_error_message(error, "Error: BMP header file does not have correct identifier 0x4d42");
		return false;
	}
	//2
	if(bmp_hdr -> offset != BMP_HEADER_SIZE){
		_error_message(error, "Error: BMP header file contains the incorrect offset value");
		return false;
	}
	//3
	if(bmp_hdr -> dib_header_size != DIB_HEADER_SIZE){	
		_error_message(error, "Error: BMP header file contains the incorrect DIB header size");
		return false;
	}
	//4
	if(bmp_hdr -> num_planes != 1){
		_error_message(error, "Error: BMP header file contains the incorrect number of planes");
		return false;
	}
	//5
	if(bmp_hdr -> compression != 0){
		_error_message(error, "Error: BMP header file contains compression value that is not zero");
		return false;
	}
	//6
	if(bmp_hdr -> num_colors != 0){
		_error_message(error, "Error: BMP header file contains the incorrect value for num_colors");
		return false;
	}
	//6.5
	if(bmp_hdr -> important_colors != 0){
		_error_message(error, "Error: BMP header file contains the incorrect value for important_colors");
		return false;
	}
	//7
	if(bmp_hdr -> bits_per_pixel != 24){
		_error_message(error, "Error: BMP header file contains a bit-per-pixel value that is not 24");
		return false;
	}
	//8
	long int byte_width = bmp_hdr -> width_px * 3;
	while(byte_width % 4 != 0){
		byte_width += 1;
	}
	long int height = bmp_hdr -> height_px;
	long int pixel_bytes = height * byte_width;
	long int theoretical_size = bmp_hdr -> size;
	long int num_chars = _find_num_chars_file(fp);
	fseek(fp, 0, SEEK_SET);
	for(int i = 0; i < BMP_HEADER_SIZE; i++){
		fgetc(fp);
	}
	long int num_pixel_bytes = 0;
	while(true){
		fgetc(fp);
		if(feof(fp)){
			break;
		}
		num_pixel_bytes += 1;
	}

	if(num_chars != theoretical_size || bmp_hdr->image_size_bytes != pixel_bytes || num_pixel_bytes != pixel_bytes || num_pixel_bytes != bmp_hdr->image_size_bytes || theoretical_size != pixel_bytes + BMP_HEADER_SIZE){
		_error_message(error, "Error: BMP header file contains size dimensions that do not correctly correspond to the dimensions of the image file");
		return false;
	}
	//incorrect padding
	int num_padding_zeros = byte_width - bmp_hdr->width_px * 3;
	fseek(fp, BMP_HEADER_SIZE, SEEK_SET);
	for(long int i = 0; i < height; i++){
		for(long int i2 = 0; i2 < bmp_hdr->width_px * 3; i2++){
			fgetc(fp);
		}
		for(int i3 = 0; i3 < num_padding_zeros; i3++){
			unsigned char checker = fgetc(fp);
			if(checker != '\0'){
				_error_message(error, "Error: the image file was not padded with zeros correctly");
				return false;
			}
		}
	}

	return true;
}

bool write_bmp(FILE* fp, BMPImage* image, char** error){
	int wrote_ok1 = fwrite(image, image->header.offset, 1, fp);
	int wrote_ok2 = fwrite(image->data, image->header.image_size_bytes, 1, fp);
	bool ok = true;
	if(wrote_ok1 != 1 || wrote_ok2 != 1){
		_error_message(error, "Error: could not properly write to file");
		ok = false;
	}
	
	return ok;
}

void free_bmp(BMPImage* image){
	free(image->data);
	free(image);
}

BMPImage* crop_bmp(BMPImage* image, int x, int y, int w, int h, char** error){
	if(x + w > image->header.width_px || y + h > image->header.height_px){
		_error_message(error, "Error: crop is outside of the image boundaries");
		return NULL;
	}
	if(w < 1){
		_error_message(error, "Error: width must be a positive integer");
		return NULL;
	}
	if(h < 1){
		_error_message(error, "Error: height must be a positive integer");
	}


	BMPImage* cropped = malloc(sizeof(*cropped));
	if(cropped == NULL){
		_error_message(error, "Error: could not allocate space for the cropped image");
		free(cropped);
		return NULL;
	}
	int padding_zeros = (w * 4 - w * 3) % 4;
	long int byte_width = 3 * w + padding_zeros;
	long int num_data_bytes = byte_width * h;
	cropped->data = malloc(sizeof(*cropped->data) * num_data_bytes);
	if(cropped->data == NULL){
		_error_message(error, "Error: could not allocate space for the cropped image data");
		free_bmp(cropped);
		return NULL;
	}
	cropped->header = image->header;
	cropped->header.size = BMP_HEADER_SIZE + num_data_bytes;
	cropped->header.width_px = w;
	cropped->header.height_px = h;
	cropped->header.image_size_bytes = num_data_bytes;
	long int offset_crop = 0;
	for(long int i1 = h - 1; i1 >= 0; i1--){
		for(long int i2 = 0; i2 < w * 3; i2++){
			long int offset_orig = _offset_crop(x * 3 + i2, y + i1, image->header.width_px, image->header.height_px);
			cropped->data[offset_crop] = image->data[offset_orig - BMP_HEADER_SIZE];
			offset_crop += 1;
		}
		for(int i3 = 0; i3 < padding_zeros; i3++){
			cropped->data[offset_crop] = '\0';
			offset_crop += 1;
		}
	}

	return cropped;
}

long int _offset_crop(long int x, long int y, long int width, long int height){
	int padding_bytes = (width * 4 - width * 3) % 4;
	long int byte_width = width * 3 + padding_bytes;
	long int last_row_offset = byte_width * (height - 1) + BMP_HEADER_SIZE;
	long int offset = last_row_offset - byte_width * y + x; //x is in bytes, not pixels

	return offset;
}
