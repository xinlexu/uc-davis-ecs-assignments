#include<iostream>
using namespace std;
#include<string>
int main()
{
  int numb; int times = 0;
  char str[10];
  cout << "请输入一个整数：" << endl;
  cin >> numb;
  for(int i = numb ; i > 0 ; i--)
  {
	  sprintf(str,"%d",i);//将数字转换为字符串
	  for(int j = 0; str[j] != '\0'; j++)//遍历转换后的字符串
	  {
		  if(str[j] == '1')//查找字符串中是否含'1',此处可将'1'换成其他字符
		  {
		    times++;
		  }
	  }
  }
  cout << "所有小于等于" << numb << "的非负整数中数字 1 出现的个数为：" << endl;
  cout << times << endl;
  system("pause");
  return 0;
}