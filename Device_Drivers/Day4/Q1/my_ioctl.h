#define MY_TYPE 'x'

#define BUF_CNT _IOR(MY_TYPE,0,int)
#define BUF_CLR _IO(MY_TYPE,1)
#define BUF_DISP _IO(MY_TYPE,3)
