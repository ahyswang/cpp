#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

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

int main_sin_func()
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
			x = 0.54;
			index = 10*(2*x-1); 
			y = (float)log2tab10[index] / (1<<15); //Q15
			printf("===> index = %d, x = %f, y = %f\n", index, x, y);
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
			
			printf("====================>\n");
			int count = 20;
			for (int i = 0; i < count; i++)
			{
				x = 0.5 + ((1 - 0.5) / count ) * i;
				x_q = x * (1<<15); // Q15
				index = ((x_q - 16384) * 20) >> 15; //Q15 => Q0  //0.5 => 16384 (Q15)
				y = (float)log2tab10[index] / (1<<15); //Q15 => float
				printf("i = %d, index = %d, x = %f, y = %f\n", i, index, x, y);
			}
			
		}
		
	}	
	// 混合法
	{
		/*
		y_hat = y0 + dot_y
		dot_y = (dot_y0/dot_x0) * dot_x
		y_hat 作为y的近似值
		y0 每段的起始值、制作一个表查询
		dot_y0 dot_x0 每段的斜率是恒定值、制作一个表查询
		dot_x 用到的横坐标起始值做一个表
		*/
		//1.init table
		#define N1 10
		int i;
		int logtab0[N1]; //存储每段起点对数值的表
		int logtab1[N1]; //存储每段y0/x0值的表
		int logtab2[N1]; //存储每段输入起始值x0的表
		
		printf("logtab0:\n");
		for (i = 0; i < N1; i++)
		{
			int x_q, y_q;
			float x, y; 

			x = 0.5 + i*(1 - 0.5)/N1;
			y = log(x)/log(2);
			x_q = x * (1<<15);
			y_q = y * (1<<15);
			logtab0[i] = y_q;
			printf("i = %d, x = %f, x_q = %d, y = %f, y_q = %d\n", i, x, x_q, y, y_q);
		}
		printf("logtab1:\n");
		for (i = 0; i < N1; i++)
		{
			
			float x0,y0,x0_1,y0_1;
			float slope;
			int slope_q;
			
			x0 = 0.5 + i*(1 - 0.5)/N1;
			y0 = log(x0)/log(2);
			x0_1 = 0.5 + (i+1)*(1 - 0.5)/N1;
			y0_1 = log(x0_1)/log(2);

			slope = (y0_1 - y0) / (x0_1 - x0);
			slope_q = slope*(1<<13); //Q13

			//printf("dot_y_q = %d, dot_x_q = %d, tmp = %f\n", dot_y_q, dot_x_q, tmp);
			
			logtab1[i] = slope_q;
			 
			printf("i = %d, %d\n", i, logtab1[i]);
		}
		printf("logtab2:\n");
		for (i = 0; i < N1; i++)
		{
			float x0;
			
			x0 = 0.5 + i*(1 - 0.5)/N1;
			
			logtab2[i] = x0 * (1<<15); //Q15
			 
			printf("i = %d, %d\n", i, logtab2[i]);
		}
		/*
		1.根据输入值、计算查找地址：
		index = ((x - 16384)*20) >> 15;
		2.查表得 y0 = logtab0[index];
		3.计算 dotx = x - logtab2[index];
		4.计算 doty = (dotx * logtab1[index]) >> 13;
		5.计算得到结果 y = y0 + doty;
		*/
		{
			float x = 0.54;
			float y = 0;
			int x_q, y_q, y0, dotx, doty;
			int index;
			//准确值
			y = log(0.54) / log(2);
			printf("1.x = %f, y = %f\n", x, y);

			//近似值
			//1.定标Q15
			x_q = x * (1 << 15);
			//2.表地址
			index = ((x_q - 16384) * 20) >> 15;
			//查表y0
			y0 = logtab0[index];
			//计算dotx
			dotx = x_q - logtab2[index];
			//计算doty
			doty = (dotx * logtab1[index]) >> 13; //Q15*Q13 => Q15
			//计算结构
			y_q = y0 + doty; //Q15
			y = ((float)y_q) / (1<<15); //浮点化
			printf("2.x_q = %d, index = %d, y0 = %d, dotx = %d, doty = %d, y_q = %d, y = %f\n",
				x_q, index, y0, dotx, doty, y_q, y);
			printf("3.y0 = %d, y0_f = %f\n", y0, (float)y0/(1<<15));
		}
	}
	return 0;
}


/*
C语言中变量以补码形式存放在内存中，正数的补码与原码相同，负数求补码方式为（符号位不变，其余各位取反，最后末尾加1）；
32位机器：int 32位，short 16位。
x = 127，正数，原码：0111 1111，补码：0111 1111，扩展到32位高位补0，结果为0000007FH;
Y = -9，负数，原码：1000 1001，补码：1111 0111，扩展到16位高位补1，结果为FFF7H；
z = x + y = 118，原码：0111 0110，补码：0111 0110，扩展到32位高位补0，结果为00000076H。
注意：扩展时，符号位不变。
*/
int print_hex(uint8_t* p, uint32_t size)
{
	int i;
	printf("0x");
	for (i = size - 1; i >= 0; i--)
	{
		printf("%02x", p[i]);
	}
	printf("\n");

	return 0;
}


int main3()
{

	//int转short截断（有问题）
	{
		int16_t i16_1, i16_2;
		int32_t i32_1, i32_2;

		i32_1 = 0x00ff0101;   //mov 导致截断
		i32_2 = 0x00000101;

		i16_1 = i32_1;
		i16_2 = i32_2;

		printf("1.int2short---------------------------------\n");

		printf("i16_1=%d\n", i16_1);
		printf("i16_2=%d\n", i16_2);

		print_hex((uint8_t *)&i16_1, sizeof(i16_1));
		print_hex((uint8_t *)&i16_2, sizeof(i16_2));
		
		printf("i32_1=%d\n", i32_1);
		printf("i32_2=%d\n", i32_2);

		print_hex((uint8_t *)&i32_1, sizeof(i32_1));
		print_hex((uint8_t *)&i32_2, sizeof(i32_2));
	}
	//short转int扩展(不会有问题)
	{
		int16_t i16_1, i16_2;
		int32_t i32_1, i32_2;

		i16_1 = 127;
		i16_2 = -9;
		
		i32_1 = i16_1;  //正数高位补0
		i32_2 = i16_2;  //负数高位补1

		printf("2.short2int---------------------------------\n");

		printf("i16_1=%d\n", i16_1);
		printf("i16_2=%d\n", i16_2);

		print_hex((uint8_t *)&i16_1, sizeof(i16_1));
		print_hex((uint8_t *)&i16_2, sizeof(i16_2));
		
		printf("i32_1=%d\n", i32_1);
		printf("i32_2=%d\n", i32_2);

		print_hex((uint8_t *)&i32_1, sizeof(i32_1));
		print_hex((uint8_t *)&i32_2, sizeof(i32_2));
	}

	return 0;
}

int f32_mem_sign(uint32_t num)
{
	int sign = num & (1 << 31);
	return sign == 0 ? 1 : -1;
}

int f32_mem_exp(uint32_t num)
{
	uint32_t exp = 0;
	int i;
	for (i = 23; i < 31; i++)
	{
		exp |= (num & (1 << i));
	}
	exp = exp >> 23;
	exp = exp - 127;
	return exp;
}



int f32_mem_toint(float f32)
{
	uint32_t num;
	int sign, exp;
	memcpy(&num, &f32, sizeof(float));
	sign = f32_mem_sign(num);
	exp = f32_mem_exp(num);  //获取float的指数部分
	printf("sign = %d, exp = %d\n", sign, exp);
	if (exp < 0)
	{
		return 0;
	}
	else 
	{
		int res =  num & ((1<<23) - 1); //保留mantissa 注意运算符的优先级
		res |= 1<<23; //将小数点前面的1补上
		//1.10001 * 2^3
		//100010...0  补1前
		//1100010..0  补1后 
		//1100  整数部分
		res >>= (23-exp); //整数部分右移到合适位置
		printf("res = %d\n", res);
		return sign * res;
	}

	return 0;
}


struct f32_struct_t
{
	unsigned int mantissa:23;
	unsigned int exponent:8;
	unsigned int sign:1;
};

int f32_struct_toint(float f32)
{
	struct f32_struct_t f32_s;
	int sign, exp, res;
	f32_s = *(struct f32_struct_t *)(&f32);
	sign = f32_s.sign==0?1:-1;
	exp =f32_s.exponent - 127;
	if (exp < 0) 
	{
		return 0;
	}
	else 
	{
		int res = f32_s.mantissa;
		res |= (1<<23);
		res = res>>(23-exp);
		res = sign*res;
		return res;
	}
	return 0;
}
//convrt float to int 
int main_convert()
{
	//float mem binary
	{
		float f32_1 = -12.25; //memcpy
		char s[32];
		int i32_1 = 0;

		i32_1 = f32_1;
	
		itoa(*(int*)&f32_1, s, 2);
		printf("binary --> %s\n", s);
		itoa(*(int*)&f32_1, s, 16);
		printf("hex --> %s\n", s);
	}
	//float to int by memcpy
	{
		float f32_1 = .0;
		int f32_1_int = 0;

		f32_1 = -12.25;
		f32_1_int = f32_mem_toint(f32_1);
		printf("f32_1 = %f, f32_1_int = %d\n", f32_1, f32_1_int);

		f32_1 = -100.85;
		f32_1_int = f32_mem_toint(f32_1);
		printf("f32_1 = %f, f32_1_int = %d\n", f32_1, f32_1_int);
	}
	//float to int by struct
	{
		float f32_1 = .0;
		int f32_1_int = 0;

		f32_1 = -12.25;
		f32_1_int = f32_struct_toint(f32_1);
		printf("f32_1 = %f, f32_1_int = %d\n", f32_1, f32_1_int);

		f32_1 = -100.85;
		f32_1_int = f32_struct_toint(f32_1);
		printf("f32_1 = %f, f32_1_int = %d\n", f32_1, f32_1_int);
	}
	return 0;
}

float sigmoid_f(float x)
{
	return 1.0 / (1.0 + exp(-x));
}

int main_sigmoid()
{
	//制表
	//x Q8 
	//y Q30
	/*
	x [-16, 16] Q8(步长=(float)1/(1<<8)) 点数 = (2*2^4)/(1/(1<<8)) = 2*2^4*2^8=2^13
	y [0, 1]   Q30 [-1,1] 满足范围
	*/
	{
		#define N_POINT (8192)
		int step = 1<<8;
		int points = (2*16)/(1.0/(1<<8)); //2^13
		int sig_tab[N_POINT];
		int i,index ;
		int x_q, y_q;
		float x, y;
		printf("points = %d, N_POINT = %d\n", points, N_POINT);
		for (i = 0; i < N_POINT; i++)
		{
			x = -16 + i * (1.0/(1<<8)); 
			x_q = x * (1<<8); //Q8
			y = sigmoid_f(x);
			y_q = y * (1<<30); //Q30
			if (i<10)
			 	printf("x = %f, y = %f, x_q = %d, y_q = %d\n", x, y, x_q, y_q);
			sig_tab[i] = y_q;
		}
		
		{
			x = 2.1;
			x_q = x * (1<<10); //Q10  
			printf("Q10 x = %f, x_q = %d\n", x, x_q);
			x_q = x_q/(1<<(10-8)); //Q10->Q8
			printf("Q10 x = %f, x_q = %d\n", x, x_q);
			index = 4096 + x_q;
			y_q = sig_tab[index]; 
			y = (float)y_q/(1<<30);
			printf("x = %f, y = %f, sigmod_f(x) = %f\n",x , y, sigmoid_f(x));
		}
		
	}

	return 0;
}

int main()
{
	/*
	负数右移高位保持为1，为了保持符号位。
	在c 中左移也就是所说的逻辑移位，右端补0，而右移是算数移位，左端补齐的是最高位的符号位。
	*/

	{
		int i32_1 = -1024;
		int i;
		char s[64];
		printf("i32_1 = %d, 0x%x\n", i32_1, i32_1);
		for (i = 0; i < 32; i++)
		{
			i32_1 = i32_1>>1;
			itoa(*(int *)(&i32_1), s, 2);
			printf(">>%d, i32_1 = %d, 0x%x, 0b%s\n", i, i32_1, i32_1, s);
		}
		i32_1 = -1024;
		for (i = 0; i < 32; i++)
		{
			i32_1 = i32_1<<1;
			itoa(*(int *)(&i32_1), s, 2);
			printf("<<%d, i32_1 = %d, 0x%x, 0b%s\n", i, i32_1, i32_1, s);
		}
	}
	{
		int i32_1 = -1024;
		int i;
		char str[36];
		printf("i32_1 = %d, 0x%x\n", i32_1, i32_1);
		for (i = 0; i < 32; i++)
		{
			i32_1 = i32_1<<1;
			//itoa(str, (int *)(&i32_1), 2);
			printf("<<%d, i32_1 = %d, 0x%x\n", i, i32_1, i32_1);
		}
	}
	//移位主动进1
	/*
	if (shift > 0) {
		result = result >> (shift - 1);
		result = (result & 0x1) + (result >> 1);
	}
	*/
	{
		int shift = 1;
		int result = 0b100111;
		char s[64];
		itoa(*(int *)(&result), s, 2);
		printf("0. result = %d, 0x%x, 0b%s\n", result, result, s);
		if (shift > 0) {
			result = result >> (shift - 1);
			result = (result & 0x1) + (result >> 1);  //右移一位相当于除以2、如果是1除以2结果为0.5、然后主动进移位。
		}
		itoa(*(int *)(&result), s, 2);
		printf("0. result = %d, 0x%x, 0b%s\n", result, result, s);
	}
	/*
	ceil(x)返回不小于x的最小整数值（然后转换为double型）。
	floor(x)返回不大于x的最大整数值。
	round(x)返回x的四舍五入整数值。
	*/
	{
		float num = 1.4999;
		printf("ceil(%f) is %f\n", num, ceil(num));
		printf("floor(%f) is %f\n", num, floor(num));
		printf("round(%f) is %f\n", num, round(num));
		num = -1.4999;
		printf("ceil(%f) is %f\n", num, ceil(num));
		printf("floor(%f) is %f\n", num, floor(num));
		printf("round(%f) is %f\n", num, round(num));
	}

	{
		typedef struct __int128_t1
		{

		};
		__int128_t x = 1100000000000000L;
	}
}




