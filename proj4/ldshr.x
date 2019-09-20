
/* structure definitions, no enumerations needed */


struct node {
  float value;
  struct node * next;
};


struct values{
	int n;
	int m;
	int s;

};

/* program definition, no union or typdef definitions needed */
program RDBPROG { /* could manage multiple servers */
	version RDBVERS {
		double GETLOAD() = 1;
		double SUMQROOT_GPU(values) = 2 ;
		double SUMQROOT_LST(node) = 3;
		
	} = 1;
} = 0x20000001;  /* program number ranges established by ONC */
