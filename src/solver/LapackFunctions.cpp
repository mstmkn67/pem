#include "LapackFunctions.h"

namespace lap{

//void solve_linear_equations_band(){}

int solve_linear_equations_general(int n,double* A,double* B){
	int nrhs=1,lda=n,ldb=n,info;
	int* ipiv=new int[n];
	dgesv_(&n,&nrhs,A,&lda,ipiv,B,&ldb,&info);
	delete[] ipiv;
	return info;
}

int solve_linear_equations_general(int n,double* A,double* B,double* X){
	for(int i=0;i<n;i++)X[i]=B[i];
	int nrhs=1,lda=n,ldb=n,info;
	int* ipiv=new int[n];
	dgesv_(&n,&nrhs,A,&lda,ipiv,X,&ldb,&info);
	delete[] ipiv;
	return info;
}

int get_inverse_matrix_symmetric(int n,double* A){
	int info;
	char U='U';
	dpptrf_(&U,&n,A,&info);
	if(info!=0)return info;
	dpptri_(&U,&n,A,&info);
	return info;
}

int get_inverse_matrix_general(int n,double* A){
	int info;
	int* ipiv=new int[n];
	dgetrf_(&n,&n,A,&n,ipiv,&info);
	if(info!=0)return info;
	int lwork=2*n;
	double* work=new double[lwork];
	dgetri_(&n,A,&n,ipiv,work,&lwork,&info);
	delete[] work;
	delete[] ipiv;
	return info;
}

int get_Cholesky_decomposition(int n,double* A){
	int info;
	char U='U';
	dpptrf_(&U,&n,A,&info);
	return info;
}

int get_eigenvalues_symmetric(int n,double* A,double* W){
	int info;
	int lwork=3*n;
	double* work=new double[lwork];
	char N='N',U='U';
	dsyev_(&N,&U,&n,A,&n,W,work,&lwork,&info);
	delete[] work;
	return info;
}

int get_eigenvalues_and_vectors_symmetric(int n,double* A,double* W){
	int info;
	int lwork=3*n;
	double* work=new double[lwork];
	char V='V',U='U';
	dsyev_(&V,&U,&n,A,&n,W,work,&lwork,&info);
	delete[] work;
	return info;
}


} // end of namespace lap
