#include "stdafx.h"
#include "BinaryTree.h"
#include <allheaders.h>
#include <baseapi.h>
#include<fstream>
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>


#define char_s 256
#define RESIZE 32
#pragma comment(lib, "liblept168")
#pragma comment(lib, "libtesseract302")

using namespace std;
using namespace tesseract;
using namespace cv;

enum { WEBCAM = 1, ENCRYPTION, DECRYPTION, EXIT };
int ch[char_s][3][3] = { 0 };

typedef struct newch{   //mapping char to matrix

	int ch;
	int mat[3][3];

}newch;

void init_char(char*file) //read textfile
{
	ifstream fin;
	char buf[10];

	fin.open(file);
	for (int i = 0; i < char_s; i++){
		for (int j = 0; j < 11; j++){
			fin >> buf;
			if (j == 0 || buf[0] == ':' || buf[0] == '\0')
				continue;
			if (j < 5)
				ch[i][0][(j - 2) % 3] = buf[0]-48;
			else if (j<8)
				ch[i][1][(j - 2) % 3] = buf[0]-48;
			else if (j<11)
				ch[i][2][(j - 2) % 3] = buf[0]-48;
		}
	}
	fin.close();
}

string img_str(string img) // string by using tesseract
{
	TessBaseAPI *api = new TessBaseAPI();
	string temp;
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	// Open input image with leptonica library
	Pix *image = pixRead(img.c_str());
	image->xres = pixGetWidth(image);
	image->yres = pixGetHeight(image);
	api->SetImage(image);

	temp = api->GetUTF8Text();
	cout << "OCR output :" << endl << temp;

	// Destroy used object and release memory
	api->End();
	pixDestroy(&image);

	while (temp.at(temp.size() - 1) == '\n')
		temp.pop_back();
	return temp;
}

int main()
{
	srand(time(NULL));
	char * ch_text = "data.txt";
	int key;					//encryption & decryption key
	string img, temp;
	ofstream fout;
	char inputstr[1000];   
	char outputstr[1000];
	newch re_ch[1000];
	int choice = 0;
	int flag = 0;
	int ran = 0;
	int arr[100] = { 0 };
	
	// Initialize tesseract-ocr with English, without specifying tessdata path
	cout << "**********Wellcome to Text Encryption System!**********\n" << endl;
	while (1){
		cout << "   ---MENU--- " << endl;
		cout << "1. Webcam" << endl;
		cout << "2. Encryption" << endl;
		cout << "3. Decryption" << endl;
		cout << "4. Exit" << endl;

		cin >> choice;
		cout << endl;
		if (choice == WEBCAM){ // use webcam->save image
			VideoCapture cap(0);
			Mat save_img;
			cap >> save_img;

			char Esc = 0;

			while (Esc != 27 && cap.isOpened()) {
				bool Frame = cap.read(save_img);
				if (!Frame || save_img.empty()) {
					cout << "error: frame not read from webcam\n";
					break;
				}
				imshow("imgOriginal", save_img);
				Esc = waitKey(1);           //if press Esc -> save img
			}
			cvDestroyWindow("imgOriginal"); //close webcam
			imwrite("test.jpg", save_img); //save image
		}


		else if (choice == ENCRYPTION){  //encryption
			cout << "Input image, key value : "; 
			cin >> img >> key;

			init_char(ch_text);
			temp = img_str(img);

			strcpy(inputstr, temp.c_str());
			//char* encryption(char* inputstr,char* outputstr,int key)   key  1=pre, 2=in 3=post   order
			Encryption(inputstr, outputstr, key);

			//MK i = 1
			for (int i = 0; outputstr[i] != '\0'; i++) //mapping generated char to matrix
			{
				//MK
				re_ch[i].ch = (int)outputstr[i];
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						re_ch[i].mat[j][k] = ch[(int)outputstr[i]][j][k];

					}
				}
			}

			int matrix_size = ceil(sqrt(strlen(outputstr)));

			int * matrix = (int*)malloc(matrix_size * matrix_size * sizeof(int));

			for (int i = 0; i < matrix_size*matrix_size; i++){
				matrix[i] = 0;
			}
			Mat result;
			result = Mat::zeros(3 * matrix_size, 3 * matrix_size, CV_8UC3);

			int directions[4][2] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };

			int dir = 0;
			int dx = 0;
			int dy = 1;

			int x, y;

			x = y = 0;
			int a, b;

			for (int i = 0; i < matrix_size * matrix_size; i++)
			{
				// insert image
				// if i > length -> put "default"
				// start at x*3 , y*3
				// end at (x+1)*3 - 1, (y+1)*3 - 1
				if (i < strlen(outputstr)){
					for (int m = 0; m < 3; m++) {
						for (int n = 0; n < 3; n++) {
							a = re_ch[i].mat[m][n];
							if (a == 1) {
								//b = rand() % 255;
								result.at<Vec3b>(Point(x * 3 + m, y * 3 + n)).val[0] = 255;//b
								result.at<Vec3b>(Point(x * 3 + m, y * 3 + n)).val[1] = 255;//0
								result.at<Vec3b>(Point(x * 3 + m, y * 3 + n)).val[2] = 255;//b
							}
						}
					}
				}
				else{
					ran = rand() % 128 + 128;
					arr[i - strlen(outputstr)] = ran;
					for (int k = 0; k < i - strlen(outputstr); k++){
						if (arr[k] == ran){
							flag = 1;
						}
					}
					if (flag == 1){
						flag = 0;
						i = i - 1;
						continue;
					}

					for (int m = 0; m < 3; m++) {
						for (int n = 0; n < 3; n++) {
							a = ch[ran][m][n];
							if (a == 1) {
								//b = rand() % 255;
								result.at<Vec3b>(Point(x * 3 + m, y * 3 + n)).val[0] = 255;//b
								result.at<Vec3b>(Point(x * 3 + m, y * 3 + n)).val[1] = 255;//b
								result.at<Vec3b>(Point(x * 3 + m, y * 3 + n)).val[2] = 255;//0
							}
						}
					}
				}

				matrix[x*matrix_size + y] = 1;

				if (x + dx >= matrix_size || y + dy >= matrix_size || matrix[(x + dx) * matrix_size + y + dy] == 1) {  //change direction
					dir = (dir + 1) % 4;
					dx = directions[dir][0];
					dy = directions[dir][1];
				}

				x += dx;
				y += dy;
			}
			cv::Mat resized;

			cv::resize(result, resized, cv::Size(result.cols * RESIZE, result.rows * RESIZE), 0, 0, CV_INTER_NN); //

			imshow("QR code", resized);
			waitKey(0);
			imwrite("result.jpg", resized);
			cvDestroyWindow("QR code");
		}

		else if (choice == DECRYPTION){                              //decryption
			cout << "Input image, key value : ";
			cin >> img >> key;

			init_char(ch_text);
			/* DECRYPTION */
			Mat image_decrypt;
			image_decrypt = imread(img);
			if (image_decrypt.rows != image_decrypt.cols) {
				cout << "Invalid image!" << endl;
				return 0;
			}
			int new_size = image_decrypt.rows / RESIZE;

			Mat image_decrypt_resized;

			cv::resize(image_decrypt, image_decrypt_resized, cv::Size(new_size, new_size), 0, 0, CV_INTER_NN);

			int decrypt_length = (new_size / 3); //square

			int side_length = decrypt_length; //

			decrypt_length *= decrypt_length;

			//        decrypt_length = 9;

			char *decrypt_string = (char*)malloc(sizeof(char)*decrypt_length);

			int * flag_matrix = (int*)malloc(decrypt_length * sizeof(int));

			for (int i = 0; i < decrypt_length; i++) //init_flag_mat
				flag_matrix[i] = 0;

			int directions[4][2] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };

			int  dir, x, y, dx, dy;

			dir = x = y = dx = 0;

			dy = 1;

			int tmp_matrix[3][3] = { 0 };

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					tmp_matrix[m][n] = 0;

			for (int s = 0; s < decrypt_length; s++){ //get matrix from image->3x3 mat
				for (int m = 0; m < 3; m++) {
					for (int n = 0; n < 3; n++) {
						if (image_decrypt_resized.at<Vec3b>(Point(x * 3 + m, y * 3 + n)).val[0] > 0) {
							tmp_matrix[m][n] = 1;
						}
						else {
							tmp_matrix[m][n] = 0;
						}
					}
				}
				int c;
				for (int k = 0; k < char_s; k++) {//check mapping matrix to char
					c = 0;
					for (int i = 0; i < 3; i++){
						for (int j = 0; j < 3; j++){
							if (ch[k][i][j] == tmp_matrix[i][j])
								c++;
						}
					}
					if (c == 9) {
						decrypt_string[s] = k < 128 ? (char)k : '\0';
						c = 0;
						break;
					}
				}
				flag_matrix[x * side_length + y] = 1;
				if (x + dx >= side_length || y + dy >= side_length || flag_matrix[(x + dx) * side_length + y + dy] == 1) {
					dir = (dir + 1) % 4;
					dx = directions[dir][0];
					dy = directions[dir][1];
				}
				x += dx;
				y += dy;
			}
			char *decrypt_string_final = (char*)malloc(sizeof(char)*decrypt_length);
			Decryption(decrypt_string, decrypt_string_final, key);
			cout << endl << "Convert QRcode into text" << endl << "->" << decrypt_string_final << endl << endl;
		}
		else{
			return 0;
		}
	}
	return 0;

}
