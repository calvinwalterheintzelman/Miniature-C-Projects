#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
	FILE* fp_6x6 = fopen("6x6_24bit.bmp", "r");
	char* error = NULL;
	BMPImage* image1 = read_bmp(fp_6x6, &error);
	bool ok = check_bmp_header(&image1->header, fp_6x6);
	if(error != NULL || ok == false)
	{
		fprintf(stderr, "%s\n", error); //should not occur
		free(error);
		return EXIT_FAILURE;
	}
	else
	{
		fprintf(stdout, "-=-=-=-=-=-=-=-=- 6x6_24bit.bmp -=-=-=-=-=-=-=-=-\nThe magic number is 0x%x.\nThe following is the image binary file in hexadecimal format:\n", image1->header.type);
		fseek(fp_6x6, BMP_HEADER_SIZE + 1, SEEK_SET);
		int new_line_check = 0;
		for(long int i = 0; !feof(fp_6x6); i++)
		{
			fgetc(fp_6x6);
			fprintf(stdout, "0x%.2x ", image1->data[i]);
			if(new_line_check == 7)
			{
				new_line_check = 0;
				fprintf(stdout, "\n");
			}
			else
			{
				new_line_check += 1;
			}
		}
	}
	fclose(fp_6x6);
	
	error = NULL;
	FILE* fp_corrupt_car = fopen("corrupt_carw.bmp", "r");
	BMPImage* image2 = read_bmp(fp_corrupt_car, &error);
	if(error != NULL)
	{
		fprintf(stdout, "\n-=-=-=-=-=-=-=- currupt_carw.bmp -=-=-=-=-=-=-=-\nThere should be an error message following this line:\n%s\n\n", error);
		free(error);
	}
	else
	{
		free_bmp(image2);
		return EXIT_FAILURE;
	}
	fclose(fp_corrupt_car);

	FILE* airplane = fopen("airplane.bmp", "r");
	error = NULL;
	BMPImage* image3 = read_bmp(airplane, &error);
	ok = check_bmp_header(&image3->header, airplane);
	if(error != NULL || ok == false)
	{
		fprintf(stderr, "%s\n", error); //should not occur
		free(error);
		return EXIT_FAILURE;
	}
	else
	{
		fprintf(stdout, "-=-=-=-=-=-=-=- airplane.bmp -=-=-=-=-=-=-=-\nfile size: 0x%x, reserved1: 0x%x, compression: 0x%x,\nimportant colors: 0x%x, bits per pixel: 0x%x, reserved2: 0x%x, num_colors: 0x%x,\nwidth: 0x%x, height: 0x%x, planes: 0x%x, DIB header size: 0x%x, offset: 0x%x,\nand image size: 0x%x.\n", image3->header.size, image3->header.reserved1, image3->header.compression, image3->header.important_colors, image3->header.bits_per_pixel, image3->header.reserved2, image3->header.num_colors, image3->header.width_px, image3->header.height_px, image3->header.num_planes, image3->header.dib_header_size, image3->header.offset, image3->header.image_size_bytes);
	}
	fclose(airplane);

	BMPImage* image4 = malloc(sizeof(*image4));
	image4->header = image1->header;
	image4->header.width_px = 5;
	image4->header.height_px = 5;
	image4->header.image_size_bytes = (5 * 3 + 1) * 5;
	image4->header.size = BMP_HEADER_SIZE + image4->header.image_size_bytes;
	image4->data = malloc(sizeof(*image4->data) * image4->header.image_size_bytes);
	int num_until_padding = 15;
	for(int i = 0; i < image4->header.image_size_bytes; i++)
	{
		if(num_until_padding == 0)
		{
			num_until_padding = 15;
			image4->data[i] = 0x00;
		}
		else
		{
			image4->data[i] = 0xff;
			num_until_padding -= 1;
		}
	}//image4 is a 5x5 white picture

	FILE* write_test = fopen("write_test.bmp", "w");
	error = NULL;
	write_bmp(write_test, image4, &error);
	if(error != NULL)
	{
		fprintf(stderr, "%s\n", error); //should not occur
		free(error);
		return EXIT_FAILURE;
	}
	fclose(write_test);

	fprintf(stdout, "\n-=-=-=-=-=-=-=- Cropped black image (3x3) -=-=-=-=-=-=-=-=\n");
	error = NULL;
	BMPImage* image5 = crop_bmp(image4, 0, 0, 3, 3, &error);
	if(error != NULL)
	{
		fprintf(stderr, "%s\n", error); //should not occur
		free(error);
		return EXIT_FAILURE;
	}
	int new_line_check = 0;
	for(int i = 0; i < 36; i++)
	{
		fprintf(stdout, "0x%.2x ", image5->data[i]);
		if(new_line_check != 7)
		{
			new_line_check += 1;
		}
		else
		{
			new_line_check = 0;
			fprintf(stdout, "\n");
		}
	}

	fprintf(stdout, "\n\n -=-=-=-=-=-=- Cropped colorful image (2x3) -=-=-=-=-=-=-\n");
	error = NULL;
	BMPImage* image6 = crop_bmp(image1, 1, 3, 2, 3, &error);
	if(error != NULL)
	{
		fprintf(stderr, "%s\n", error); //should not occur
		free(error);
		return EXIT_FAILURE;
	}
	new_line_check = 0;
	for(int i = 0; i < 24; i++)
	{
		fprintf(stdout, "0x%.2x ", image6->data[i]);
		if(new_line_check != 7)
		{
			new_line_check += 1;
		}
		else
		{
			new_line_check = 0;
			fprintf(stdout, "\n");
		}
	}

	error = NULL;
	BMPImage* image7 = crop_bmp(image1, 23432, 23553, 436463, 345433, &error);
	if(error != NULL)
	{
		fprintf(stdout, "\n-=-=-=-=-=-=-=- Cropped Image Error -=-=-=-=-=-=-=-\nThere should be an error message on the following line:\n%s\n", error);
		free(error);
	}
	else
	{
		free_bmp(image7);
		return EXIT_FAILURE;
	}

	free_bmp(image1);
	free_bmp(image3);
	free_bmp(image4);
	free_bmp(image5);
	free_bmp(image6);

	return EXIT_SUCCESS;
}
