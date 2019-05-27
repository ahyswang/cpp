/* a.c */
extern int shared;

int main()
{
	int a = 100;
	swap( &a, &shared ); //隐式申明函数
}
