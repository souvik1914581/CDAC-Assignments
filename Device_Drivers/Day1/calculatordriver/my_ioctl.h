#define MY_TYPE 'x'

#define READ_ADD _IOR(MY_TYPE,0,int)
#define READ_SUB _IOR(MY_TYPE,1,int)
#define  READ_MUL _IOR(MY_TYPE,2,int)
#define READ_DIV _IOR(MY_TYPE,3,int)

#define WRITE_ADD _IOW(MY_TYPE,4,int)
#define WRITE_SUB _IOW(MY_TYPE,5,int)
#define WRITE_MUL _IOW(MY_TYPE,6,int)
#define WRITE_DIV _IOW(MY_TYPE,7,int) 
