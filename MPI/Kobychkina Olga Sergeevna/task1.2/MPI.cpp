// MPI.cpp: ���������� ����� ����� ��� ����������� ����������.
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

	if(procRank==0) //� ������� ������
	{
		cout<<"Process # " << procRank << " started." << endl;	//
	
		A = result_creation(row*column); //array_creation(row, column);	//�������� �������
		COLUMN_SUM = result_creation(column);	//�������� ������� ����������
		
		filling_array(A, row, column, 9);	//������������� �������
		out_display_matr(A, row, column);	//����� �� ����� �������

		inscription (file, "������� � \n");	//����� � ���� �������
		out_file_matr(file, A, row, column);	//����� � ���� �������
	
		cout<<"result"<<endl;  //����� �� ����� �������
		inscription (file, "��������� \n");
	}

	cel = column/procNum;		//��������� ������ ������

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

	ost = column % procNum;			//�����
	cout<<"ost   "<<ost<<endl;
	
	if(ost != 0)	//���� ���� �����, �� �������� �� ����� ������� ������� ������
	{
		int num_ost=cel*procNum*row;	//����� ��������� ����������
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
		out_display_res(COLUMN_SUM, column);	//����� ���������� �� ����� 
		out_file_res(file, COLUMN_SUM, column);
	}

	MPI_Finalize(); 

	if(procRank==0)
	{   
		delete_res(A);		//�������� ������
		delete_res(COLUMN_SUM);
		fclose(file);
	}
	return 0;
}
