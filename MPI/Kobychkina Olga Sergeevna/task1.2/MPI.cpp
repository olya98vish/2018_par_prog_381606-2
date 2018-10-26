// MPI.cpp: определяет точку входа для консольного приложения.
//
//#include "pch.h" 
#include "stdafx.h"
#include "mpi.h" 
#include "func_matr.h"
#include <iostream> 
#include <fstream>

using namespace std; 

int main(int argc, char *argv[]) 
{ 
	int procNum, procRank; 
	setlocale(LC_ALL,"Russian");
	FILE* file = fopen("SUM_Matr.txt","w");
	int row = 10, column = 10;
	int* A = NULL;
	int* COLUMN_SUM = NULL;
	int sum=0;
	int cel=0;
	int ost=0;

	MPI_Init(&argc, &argv); 

	MPI_Comm_rank(MPI_COMM_WORLD, &procRank); 
	MPI_Comm_size(MPI_COMM_WORLD, &procNum); 

	if(procRank==0) //в главном потоке
	{
		cout<<"Process # " << procRank << " started." << endl;	//
	
		A = result_creation(row*column); //array_creation(row, column);	//создание массива
		COLUMN_SUM = result_creation(column);	//создание вектора результата
		
		filling_array(A, row, column, 9);	//генерирование массива
		out_display_matr(A, row, column);	//вывод на экран массива

		inscription (file, "матрица А \n");	//вывод в файл надписи
		out_file_matr(file, A, row, column);	//вывод в файл массива
	
		cout<<"result"<<endl;  //вывод на экран надписи
		inscription (file, "результат \n");
	}

	cel = column/procNum;		//вычисляем размер пакета

	int* A1=new int[row*cel];
	int* res = result_creation(column);

	MPI_Scatter(A, row*cel, MPI_INT, A1, row*cel, MPI_INT, 0, MPI_COMM_WORLD);

	for(int i=0;i<cel;i++)
	{
		sum=0;
		for(int j=0; j<row; j++)
		{
			sum+=A1[i*row+j];
		}
		res[i] = sum;
	}

	MPI_Gather(res, cel, MPI_INT, COLUMN_SUM, cel, MPI_INT, 0, MPI_COMM_WORLD);

	ost = column % procNum;			//хвост
	cout<<"ost   "<<ost<<endl;
	
	if(ost != 0)	//если есть хвост, то отправим по одном столбцу каждому потоку
	{
		int num_ost=cel*procNum*row;	//число элементов отработано
		int num_res=cel*procNum;

		if(procRank==0)
		{
			for (int i=0; i<ost; i++)
			{
				sum=0;
				for(int j=0; j<row; j++)
				{
					sum+=A[(num_ost+i*row)+j];
				}
				COLUMN_SUM[num_res+i]=sum;
			}
		}
	}

	if(procRank==0)
	{
		out_display_res(COLUMN_SUM, column);	//вывод результата на экран 
		out_file_res(file, COLUMN_SUM, column);
	}

	MPI_Finalize(); 

	if(procRank==0)
	{   
		delete_res(A);		//очищение памяти
		delete_res(COLUMN_SUM);
		fclose(file);
	}
	return 0;
}
