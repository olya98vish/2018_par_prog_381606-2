#include <ctime>
#include <iostream>
#include <Windows.h>

using std::cout;
using std::endl;

int** array_creation (int row, int column)
{
	int** matr = new int*[column];
	for (int i=0; i<column; i++)
		matr[i]=new int[row];
	return matr;
}
int* result_creation (int column)
{
	int* res= new int[column];
	for (int i=0; i<column; i++)
		res[i]=0;
	return res;
}
void filling_array (int** arr, int row, int column, int del)
{
	for (int i=0; i<column; i++)
	{
		for (int j=0; j<row; j++)
		{
			arr[i][j]=rand()%del;
		}
	}
}
void out_display_matr(int** arr, int row, int column)
{
	for (int i=0; i<row; i++)
	{
		for (int j=0; j<column; j++)
		{
			cout<<arr[j][i]<<"\t";
		}
		cout<<endl;
	}
}
/*void filling_res(int* res_sums, int column) 
{
	for (int i=0; i<column; i++) 
	{
		res_sums[i] = 0;
	}
}*/
void out_display_res(int* res, int column)
{
	for(int i=0; i<column; i++)
		cout<<res[i]<<"\t";
}
void out_file_matr(FILE* file, int** arr, int row, int column)
{
	for(int i=0; i<column; i++)
	{
		for (int j=0; j<row; j++)
		{
			fprintf(file,"%d\t",arr[j][i]);
		}
		fprintf(file, "\n");
	}
}
void out_file_res(FILE* file, int* res, int column)
{
	for (int i=0; i<column; i++)
		fprintf(file,"%d\t",res[i]);
}
void inscription (FILE* file, char* row)
{
	fprintf(file,"%s",row);
}
void delete_array (int** arr, int column)
{
	for (int i=0; i<column; i++)
		delete[] arr[i];
	delete[] arr;
}
void delete_res(int* res)
{
	delete[] res;
}
int* result_sum (int* res, int** matr, int row, int column)
{
	for (int j=0; j<column; j++)
		for (int i=0; i<row; i++)
			res[j]+=matr[j][i];
	return res;
}
int result_column (int* buf, int row)
{
	int sum=0;
	for(int i=0; i<row; i++)
	{
		sum+=buf[i];
	}
	return sum;
}