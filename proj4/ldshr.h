/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _LDSHR_H_RPCGEN
#define _LDSHR_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct node {
	float value;
	struct node *next;
};
typedef struct node node;

struct values {
	int n;
	int m;
	int s;
};
typedef struct values values;

#define RDBPROG 0x20000001
#define RDBVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define GETLOAD 1
extern  double * getload_1(void *, CLIENT *);
extern  double * getload_1_svc(void *, struct svc_req *);
#define SUMQROOT_GPU 2
extern  double * sumqroot_gpu_1(values *, CLIENT *);
extern  double * sumqroot_gpu_1_svc(values *, struct svc_req *);
#define SUMQROOT_LST 3
extern  double * sumqroot_lst_1(node *, CLIENT *);
extern  double * sumqroot_lst_1_svc(node *, struct svc_req *);
extern int rdbprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define GETLOAD 1
extern  double * getload_1();
extern  double * getload_1_svc();
#define SUMQROOT_GPU 2
extern  double * sumqroot_gpu_1();
extern  double * sumqroot_gpu_1_svc();
#define SUMQROOT_LST 3
extern  double * sumqroot_lst_1();
extern  double * sumqroot_lst_1_svc();
extern int rdbprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_node (XDR *, node*);
extern  bool_t xdr_values (XDR *, values*);

#else /* K&R C */
extern bool_t xdr_node ();
extern bool_t xdr_values ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_LDSHR_H_RPCGEN */
