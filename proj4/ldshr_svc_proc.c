#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "ldshr.h"
#include <math.h>

double qroot(double value);
double  reduction(int n, int mean, int seed);
double sum_function(double input, double total);
double reduce(double(*func)(double, double), node * head);
void map(double(*func)(double), node * head);


double * getload_1_svc(void * nothing,struct svc_req *rqp)
{
	double *load = malloc(sizeof(double));
	getloadavg(load,3);
	return load;
};

double * sumqroot_gpu_1_svc(values * input, struct svc_req *rqp)
{
	double *value = malloc(sizeof(double));
	*value = reduction(input->n,input->m,input->s);
	return value;
};

double * sumqroot_lst_1_svc(node * head, struct svc_req * rqp)
{
	double *sum = malloc(sizeof(double));

	map(&qroot, head);

	*sum = reduce(&sum_function, head);
	
	return sum;
};




void map(double(*func)(double), node * head)
{
	while(head != NULL)
	{
		head->value = (*func)(head->value);
		head = head->next;
	}
}

double qroot(double value)
{
	return sqrt(sqrt(value));
}



double reduce(double(*func)(double, double), node * head)
{
	double total = 0;
	while(head != NULL)
	{
		total = (*func)(head->value, total);
		head = head->next;
	}
	return total;
	
}


double sum_function(double input, double total)
{
	return input + total;
}
















