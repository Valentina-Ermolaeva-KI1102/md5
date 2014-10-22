#include <iostream>
#include <fstream>
#include <math.h>
#pragma warning(disable : 4996)

#define uint unsigned int
#define uchar unsigned char

uint F(uint x, uint y, uint z)
{
	return ((x & y) | ((~x) & z));
}

uint G(uint x, uint y, uint z)
{
	return ((x & z) | (y & (~z)));
}

uint H(uint x, uint y, uint z)
{
	return (x ^ y ^ z);
}

uint I(uint x, uint y, uint z)
{
	return (y ^ (x | (~z)));
}

uint Cyclic_shift(uint x, int y) 
{
	return x << y | x >> (32-y);
}

uchar* md5(std::ifstream *in)
{
	int lenght, size = 64, rest, i;
	in->seekg(0, std::ios::end);
	lenght = in->tellg();
	in->seekg(0, std::ios::beg);
	bool end = false;
	rest = lenght % 64;
	uchar *msg_for_hash = new uchar[size];
	char buf;
	uint A = 0x67452301, B = 0xefcdab89, C = 0x98badcfe, D = 0x10325476, AA, BB, CC, DD;
	uint T[64];
	for(int l = 0; l < size; l++) 
		T[l]= pow(2,32)*fabs(sin(l + 1));
	uint *X=(uint*)(msg_for_hash);
	while(!in->eof() && !end)
	{
		for(i = 0; i < size; i++)
		{
			in->get(buf);
			if(in->eof())
			{
				if(i < 55)
				{
					msg_for_hash[i] = 0x80;
					for(i++; i < 56; i++)
						msg_for_hash[i] = 0;
					_int64 bit_length = (uint)(lenght)*8; //длина сообщения в битах.
					for(int i = 0; i < 8; i++) //последние 8 байт
						msg_for_hash[size - 8 + i] = (unsigned char)(bit_length >> i * 8);
					break;
				}
				else
				{
					msg_for_hash[i] = 0x80;
					for(i++; i < size; i++)
						msg_for_hash[i] = 0;
					end = true;
					break;
				}
			}
			msg_for_hash[i] = buf;
		}
		if(end)
		{
			for(i++; i < 56; i++)
				msg_for_hash[i] = 0;
			_int64 bit_length = (uint)(lenght)*8; //длина сообщения в битах.
			for(int i = 0; i < 8; i++) //последние 8 байт
				msg_for_hash[size - 8 + i] = (unsigned char)(bit_length >> i * 8);
			end = false;
		}
		for (int i = 0; i < size/16; i++)
		{
			AA = A;
			BB = B;
			CC = C;
			DD = D;
			//round 1
			for(int j = 0; j < 4; j++)
			{			
				A = B + Cyclic_shift((A + F(B,C,D) + X[i + (j * 4) % 16] + T[j * 4]),  7);
				D = A + Cyclic_shift((D + F(A,B,C) + X[i + (j * 4 + 1) % 16] + T[j * 4 + 1]), 12);
				C = D + Cyclic_shift((C + F(D,A,B) + X[i + (j * 4 + 2) % 16] + T[j * 4 + 2]), 17);
				B = C + Cyclic_shift((B + F(C,D,A) + X[i + (j * 4 + 3) % 16] + T[j * 4 + 3]), 22);
			}

			//round 2
			for(int j = 0; j < 4; j++)
			{		
				A = B + Cyclic_shift((A + G(B,C,D) + X[i + (j * 4 + 1) % 16 ] + T[j * 4 + 16]),  5);
				D = A + Cyclic_shift((D + G(A,B,C) + X[i + (j * 4 + 6) % 16] + T[j * 4 + 17]),  9);
				C = D + Cyclic_shift((C + G(D,A,B) + X[i + (j * 4 + 11) % 16] + T[j * 4 + 18]), 14);
				B = C + Cyclic_shift((B + G(C,D,A) + X[i + (j * 4) % 16] + T[j * 4 + 19]), 20);
			}

			//round 3
			for(int j = 0; j < 4; j++)
			{
				A = B + Cyclic_shift((A + H(B,C,D) + X[i + (j * 4 + 5) % 16] + T[j * 4 + 32]),  4);
				D = A + Cyclic_shift((D + H(A,B,C) + X[i + (j * 4 + 8) % 16] + T[j * 4 + 33]), 11);
				C = D + Cyclic_shift((C + H(D,A,B) + X[i + (j * 4 + 11) % 16] + T[j * 4 + 34]), 16);
				B = C + Cyclic_shift((B + H(C,D,A) + X[i + (j * 4 + 14) % 16] + T[j * 4 + 35]), 23);
			}

			//round 4
			for(int j = 0; j < 4; j++)
			{
				A = B + Cyclic_shift((A + I(B,C,D) + X[i + (j * 4) % 16] + T[j * 4 + 48]),  6);
				D = A + Cyclic_shift((D + I(A,B,C) + X[i + (j * 4 + 7) % 16] + T[j * 4 + 49]), 10);
				C = D + Cyclic_shift((C + I(D,A,B) + X[i + (j * 4 + 14) % 16] + T[j * 4 + 50]), 15);
				B = C + Cyclic_shift((B + I(C,D,A) + X[i + (j * 4 + 5) % 16] + T[j * 4 + 51]), 21);
			}

			// recording result of rounds;
			A += AA;
			B += BB;
			C += CC;
			D += DD;
		}
	}
	uchar *MD5 =new uchar[16];
	MD5[0] = A % 256;
	MD5[1] = (A /= 256) % 256;
	MD5[2] = (A /= 256) % 256;
	MD5[3] = (A / 256);
	MD5[4] = B % 256;
	MD5[5] = (B /= 256) % 256;
	MD5[6] = (B /= 256) % 256;
	MD5[7] = B / 256;
	MD5[8] = C % 256;
	MD5[9] = (C /= 256) % 256;
	MD5[10] = (C /= 256) % 256;
	MD5[11] = (C / 256);
	MD5[12] = D % 256;
	MD5[13] = (D /= 256) % 256;
	MD5[14] = (D /= 256) % 256;
	MD5[15] = D / 256;
	return MD5;
}

int main(int argc, char **argv)
{
	char in_name[512], out_name[512];
	if (argc > 2)
	{
		strcpy(in_name, argv[1]);
		strcpy(out_name, argv[2]);
	}
	else if (argc == 2)
	{
		if(!strcmp("-h", argv[1]) || !strcmp("-H", argv[1]) || !strcmp("-help", argv[1]))
		{
			std::cout<<"this is program are created for calculate hash md5\nExample of using:\n md5.exe input_file_neme out_file_name\n md5.exe input_file_name\n md5.exe\n";
			return 1;
		}
		strcpy(in_name, argv[1]);
		std::cout<<"Enter the output file: ";
		std::cin>>out_name;
	}
	else 
	{
		std::cout<<"Enter the input file: ";
		std::cin>>in_name;
		std::cout<<"Enter the output file: ";
		std::cin>>out_name;
	}
	std::ifstream in(in_name, std::ios_base::in);
	if(!in)	return 1;
	std::ofstream out(out_name, std::ios_base::out);
	uchar *MD5 = md5(&in);
	for(int i = 0; i < 16; i++)
		out<<std::hex<<std::uppercase<<uint(MD5[i]);
	in.close();
	out.close();
	return 0;
}

