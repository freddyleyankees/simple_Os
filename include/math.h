#ifndef _MATHS_H
#define _MATHS_H

#define abs(n)							\
			(n = (n<0 ? -n : n),		\
				n)
#define cos(n)

#define isDigit(n)					\
				(( n > '0' && n < '9' ))	
#define divround(n, m)	({		\
							unsigned int _r, _q;	\
							_r = n%m;	\
							_q = n/m;	\
							_r>0?_q+1:_q;	\
						})
#endif

int pow(int n){
	if(n == 1)
		return n;
	return 10*pow(--n);
}