#include <stdint.h>
#include <stdio.h>
#include <math.h>

int main1(int argc, char** argv)
{
	{
		int i, q;
		int16_t i16_max = 0x7FFF;
		int16_t i16_min = (-0x7fff) - 1;
		float f_min_tab[16];
		float f_max_tab[16];
		for (i = 0; i < 16; i++)
		{
			q = 15 - i;
			//float div = pow(2.0, q);
			float div = 1L << q;
			f_max_tab[i] = ((float)i16_max) / div;
			f_min_tab[i] = ((float)i16_min) / div;
			printf("%f <= X <= %f\n", f_min_tab[i], f_max_tab[i]);
		}
	}
	//2. float x => x_q
	//x_q = x * 2^q
	{
		float x = 0.5;
		int16_t x_q = 0;
		int16_t q = 15;

		x_q = 0.5 * (1<<15);
		printf("x = %f => x_q = %d\n", x, x_q);
	}
	//3. q x_q => x
	//x = x_q * 2^(-q) 
	//x = x_q / (2^q)
	{
		int16_t x_q = 16834;
		int16_t q = 15;
		float x = 0;

		x = (float)x_q / (1<<q);
		printf("x_q = %d => x = %f\n", x_q, x);
	}
	//4.quantified add
	//z = x + y
	//zq*(2^-qz) = xq*(2^-qx) + yq*(2^-qy)
	//zq = (xq*(2^-qx) + yq*(2^-qy)) * (2^qz)
	//zq = (xq + yq*(2^(qx-qy))) * (2^(qz-qx))
	{
		float x = 0.5;
		float y = 3.1;
		float z = 0;
		int qx = 15;
		int qy = 13;
		int qz = 13;
		int x_q, y_q, z_q;
		int64_t tmp = 0;

		z = x + y;

		x_q = x * (1<<qx);
		y_q = y * (1<<qy);
		//z_q = (x_q + (y_q << (qx - qy))) << (qz - qx)
		z_q = (x_q + (y_q << (qx - qy))) >> (qx - qz); 
		//tmp = y_q << (qx - qy);
		//tmp = x_q + tmp;
		//z_q = (tmp >> (qx - qz));
		printf("z = x + y\n");

		printf("x = %f, y = %f, z = %f\n", x, y, z);
		printf("x_q = %d, y_q = %d, z_q = %d\n", x_q, y_q, z_q);

		printf("z_q = %d => z = %f\n", z_q, (float)z_q/(1<<qz));
	}

	//5.z = x - y
	{
		float x = 3.0;
		float y = 3.1;
		float z = 0;
		int qx = 13;
		int qy = 13;
		int qz = 15;
		int x_q, y_q, z_q;
		int64_t tmp = 0;

		z = x - y;

		x_q = x * (1<<qx);
		y_q = y * (1<<qy);
		z_q = (x_q - (y_q << (qx - qy))) << (qz - qx);

		printf("z = x - y\n");

		printf("x = %f, y = %f, z = %f\n", x, y, z);
		printf("x_q = %d, y_q = %d, z_q = %d\n", x_q, y_q, z_q);

		printf("z_q = %d => z = %f\n", z_q, (float)z_q/(1<<qz));
	}

	//6.add overflow
	{
		float x = 15000;
		float y = 20000;
		float z = 0;
		int16_t qx = 1;
		int16_t qy = 0;
		int16_t qz = 0;
		int16_t x_q, y_q, z_q;
		int64_t tmp = 0;

		z = x + y;

		x_q = x * (1<<qx);
		y_q = y * (1<<qy);
		//z_q = (x_q + (y_q << (qx - qy))) >> (qx - qz);
		tmp = y_q << (qx - qy);
		tmp = x_q + tmp;
		tmp = tmp >> (qx - qz); 
		z_q = tmp; //overflow  (int32_t)0x000088b8(35000) => (int16_t)0x88b8(-30536)

		printf("z = x + y overflow\n");

		printf("x = %f, y = %f, z = %f\n", x, y, z);
		printf("x_q = %d, y_q = %d, z_q = %d\n", x_q, y_q, z_q);

		printf("z_q = %d => z = %f\n", z_q, (float)z_q/(1<<qz));
	}
	//7.mul z = x*y
	/*
	z = x*y
	zq*(2^-qz) = xq*(2^-qx) * yq*(2^-qy)
	zq = (xq*yq)(2^(qz-(qx+qy)))
	zq = (xq*yq)(2^-(qx+qy-qz))
	
	int x,y,z;
	long tmp;
	tmp = (long)x;
	tmp = tmp * y;
	tmp = tmp>>(qx+qy-qz);
	*/
	{
		float x,y,z;
		int qx,qy,qz;
		int x_q,y_q,z_q;
		long tmp;

		x = 18.4;
		y = 36.8;
		z = x * y;

		qx = 10;
		qy = 9;
		qz = 5;

		x_q = x * (1<<qx);
		y_q = y * (1<<qy);

		tmp = (long)x_q;
		tmp = tmp * y_q;
		tmp = tmp>>(qx+qy-qz);
		z_q = tmp;
		
		printf("===============>\nz = x * y\n");

		printf("x = %f, y = %f, z = %f\n", x, y, z);
		printf("x_q = %d, y_q = %d, z_q = %d\n", x_q, y_q, z_q);

		printf("z_q = %d => z = %f\n", z_q, (float)z_q/(1<<qz));
	}
	//7.div z = x/y
	/*
	z = x/y
	zq*(2^-qz) = (xq*(2^-qx))/ (yq*(2^-qy))
	zq = (xq/yq)(2^(qz-qx+qy)))
	
	int x,y,z;
	long tmp;
	tmp = (long)x;
	tmp = tmp / y;
	tmp = tmp<<(qz-qx+qy);
	*/
	{
		float x,y,z;
		int qx,qy,qz;
		int x_q,y_q,z_q;
		long tmp;

		x = 18.4;
		y = 36.8;
		z = x / y;

		qx = 10;
		qy = 9;
		qz = 15;

		x_q = x * (1<<qx);
		y_q = y * (1<<qy);

		tmp = (long)x_q;
		tmp = tmp / y_q;
		tmp = tmp<<(qz-qx+qy);
		z_q = tmp;
		
		printf("===============>\nz = x / y\n");

		printf("x = %f, y = %f, z = %f\n", x, y, z);
		printf("x_q = %d, y_q = %d, z_q = %d\n", x_q, y_q, z_q);

		printf("z_q = %d => z = %f\n", z_q, (float)z_q/(1<<qz));
	}
	return 0;
}


/*
quantified table query
sin func
*/
#define N 512
#define pi 3.14159326
int sin_tab[512];

int main()
{
	{
		//init table
		int i;
		float x = pi;  
		float y = 0;
		int y_q;
		int index;
		for (i = 0; i < N; i++)
		{
			sin_tab[i] = (int)(32767*sin(2*pi*i/N));
		}
		//query table
		//query index float
		index = (int)(512 * x / (2*pi));
		printf("index = %d\n", index);
		y_q = sin_tab[index];
		{
			//query index quantified
			float tmp;
			int index_q, x_q, x_index_q;
			tmp = (512 / (2*pi));
			tmp = tmp * (1 << 8);
			index_q = tmp; //Q8
			printf("index_q = %d\n", index_q);
			tmp = x;
			tmp = tmp * (1 << 12); //Q12
			x_q = tmp;
			printf("x_q = %d\n", x_q);
			x_index_q = (x_q * index_q) >> (8 + 12 - 0); //Q0
			printf("x_index_q = %d\n", x_index_q);
		}
	}

	//log2 
	/*
	y = log2(x);
	0.5 <= x <= 1
	-1 <= y <= 0
	x q15
	y q15
	log2tab10
	*/
	{
		//1.init table
		#define N1 10
		int i;
		int log2tab10[10];
		
		printf("log2tab10:\n");
		for (i = 0; i < N1; i++)
		{
			int x_q, y_q;
			float x, y; 

			x = 0.5 + i*(1 - 0.5)/N1;
			y = log(x)/log(2);
			x_q = x * (1<<15);
			y_q = y * (1<<15);
			log2tab10[i] = y_q;
			printf("i = %d, x = %f, x_q = %d, y = %f, y_q = %d\n", i, x, x_q, y, y_q);
		}
		//2.query table
		//x x_q q15
		//x [0.5,1]
		//2*x-1 [0,1]
		//10*(2*x-1) [0, 10]
		{
			float x, y;
			int x_q, y_q;
			int index;
			x = 0.5;
			index = 10*(2*x-1); 
			y = (float)log2tab10[index] / (1<<15); //Q15
			printf("index = %d, x = %f, y = %f\n", index, x, y);
		}
		{
			//index = (x - 0.5) / 0.05;
			//index = (x - 0.5) * 20;
			//index * (2^Q15) = ((x - 0.5) * 20) * (2^Q15)  //两边乘以 2^Q15
			//index = (x*(2^Q15) - 0.5*(2^Q15)) * 20 / (2^Q15)
			//index = (x_q - base_q) * 20 / (2^Q15)
			float x, y;
			int x_q, y_q;
			int index;
			float tmp;
			x = 0.99;
			x_q = x * (1<<15); // Q15
			index = ((x_q - 16384) * 20) >> 15; //Q15 => Q0  //0.5 => 16384 (Q15)
			y = (float)log2tab10[index] / (1<<15); //Q15 => float
			printf("index = %d, x = %f, y = %f\n", index, x, y);
		}
		
	}	
	//

	return 0;
}

