// MPI.cpp: ���������� ����� ����� ��� ����������� ����������.
//
//#include "pch.h" 
#include "stdafx.h"
#include "mpi.h" 
#include <stdio.h>
#include "func_matr.h"
#include <iostream> 
#include <fstream>

using namespace std; 

int main(int argc, char *argv[]) 
{ 
	int procNum, procRank; 
	//setlocale(LC_ALL,"Russian");
	//FILE* file = fopen("SUM_Matr.txt", "w");
	int row=10, column=10;

	MPI_Init(&argc, &argv); 

	MPI_Comm_rank(MPI_COMM_WORLD, &procRank); 
	MPI_Comm_size(MPI_COMM_WORLD, &procNum); 
	
	if(procRank==0) //� ������� ������
	{
		cout<<"Process # " << procRank << " started." << endl;
	
		int** matr=NULL;
		int* RES_SUM=NULL;

		matr = array_creation(row, column);	//�������� �������
		RES_SUM = result_creation(column);	//�������� ������� ����������

		filling_array(matr, row, column, 9);	//������������� �������
		out_display_matr(matr, row, column);	//����� �� ����� �������
		//inscription (file, "������� � \n");	//����� � ���� �������
		//out_file_matr(file, matr, row, column);	//����� � ���� �������

		if (procNum==1)
		{
			RES_SUM= result_sum(RES_SUM, matr, row, column);
			cout<<"\n"<<endl;
			out_display_res(RES_SUM, column);
			//inscription (file, "��������� \n");
			//out_file_res(file, RES_SUM, column);
		}
		else //���� �� ���� ����� 
		{
			int numproc;

			for(int i=0; i<column; i++)
			{
				numproc=1+i%(procNum-1); //��������� ������� �������� ����������
				//cout<< "num proc1  " << numproc<< endl;
				MPI_Send(matr[i], row, MPI_INT, numproc, 0, MPI_COMM_WORLD); //�������� �������
				//cout<< "send from 0" << endl;
				
			}

			for (int i = 0; i < column; i++)
			{	
					MPI_Status status;

					numproc = 1 + i % (procNum - 1);
					//cout<< "num proc2  " << numproc<< endl;
					MPI_Recv(RES_SUM + i, 1, MPI_INT, numproc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
					//cout<< "recv from proc" << endl;
					
			}

			cout<<"result"<<endl;
			out_display_res(RES_SUM, column);
			//inscription (file, "��������� \n");	
			//out_file_res(file, RES_SUM, column);

			delete_array(matr, column);
			delete_res(RES_SUM);
		}
	}
	else 
	{
		MPI_Status status;
		int* buf = new int[row];

		int count = column / (procNum - 1); //��������� ����� ����� ��� ������

		if (column % (procNum - 1) != 0) //���� ���� �������
		{
			if (procRank <= column % (procNum - 1)) 
			{
				count++;
			}
		}
		
		for (int i = 0; i < count; i++) 
		{
			MPI_Recv(buf, row, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //��������� �������
			//cout<< "recv from 0 in proc" << endl;

			int sum = 0;

			sum=result_column(buf,row);  //���������
			//cout<<"sum column"<<endl;

			MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); //�������� ��������� � ������� �����
			//cout<<"send from proc"<<endl;
		}

		delete[] buf;
	}
			
	MPI_Finalize(); 

//	fclose(file);	//�������� �����

}

