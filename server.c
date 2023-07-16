#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <pthread.h>

typedef struct User{
	int userID;
	char name[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}User;

typedef struct jointUser{
	int userID;
	char name1[30];
	char name2[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}jointUser;

typedef struct admin{
	int userID;
	char username[30];
	char password[10];
}admin;

User getUser(int ID){
	int i=ID-1000;
	User curUser;
	int fd=open("Ufile",O_RDONLY,0744);
	
	int fl;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    	     //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&curUser,sizeof(User));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return curUser;
}

jointUser getJointUser(int ID){
	int i=ID-1000;
	jointUser curUser;
	int fd=open("JUfile",O_RDONLY,0744);
	
	int fl;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&curUser,sizeof(jointUser));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return curUser;
}

admin getAdmin(int ID){
	int i=ID-1000;
	admin curUser;
	int fd=open("ADfile",O_RDONLY,0744);
	int fl;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     //nth record
	lock.l_len=sizeof(admin);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&curUser,sizeof(admin));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return curUser;
}

bool checkUser(User curUser){
	int i=curUser.userID-1000;
	int fd=open("Ufile",O_RDONLY,0744);
	bool result;
	User temp;
	
	int fl;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    	     //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(User));
	if(!strcmp(temp.password,curUser.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkJointUser(jointUser curUser){
	int i=curUser.userID-1000;
	int fd=open("JUfile",O_RDONLY,0744);
	bool result;
	jointUser temp;
	
	int fl;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(jointUser));
	if(!strcmp(temp.password,curUser.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkAdmin(admin curUser){
	int i=curUser.userID-1000;
	int fd=open("ADfile",O_RDONLY,0744);
	bool result;
	admin temp;
	
	int fl;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     //nth record
	lock.l_len=sizeof(admin);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(admin));
	if(!strcmp(temp.password,curUser.password))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool depositMoney(int accType,int ID,float amt){
	int i=ID-1000;
	if(accType==1){
		int fd=open("Ufile",O_RDWR,0744);
		bool result;
		int fl;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(User);    //nth record
		lock.l_len=sizeof(User);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		User curUser;
		lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&curUser,sizeof(User));
		
		if(!strcmp(curUser.status,"ACTIVE")){
			curUser.balance+=amt;
			lseek(fd,sizeof(User)*(-1),SEEK_CUR);
			write(fd,&curUser,sizeof(User));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;		
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser curUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&curUser,sizeof(jointUser));
		
		if(!strcmp(curUser.status,"ACTIVE")){
			curUser.balance+=amt;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&curUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
	}
	return false;
}

bool withdrawMoney(int accType,int ID,float amt){
	int i=ID-1000;
	if(accType==1){
		int fd=open("Ufile",O_RDWR,0744);
		bool result;
		int fl;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(User);    //nth record
		lock.l_len=sizeof(User);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		User curUser;
		lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&curUser,sizeof(User));
		
		if(!strcmp(curUser.status,"ACTIVE") && curUser.balance>=amt){
			curUser.balance-=amt;
			lseek(fd,sizeof(User)*(-1),SEEK_CUR);
			write(fd,&curUser,sizeof(User));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser curUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&curUser,sizeof(jointUser));
		
		if(!strcmp(curUser.status,"ACTIVE") && curUser.balance>=amt){
			curUser.balance-=amt;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&curUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

float getBalance(int accType,int ID){
	int i=ID-1000;
	float result;
	if(accType==1){
		int i=ID-1000;
		int fd=open("Ufile",O_RDONLY,0744);
		User temp;
	
		int fl;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(User);    	     //nth record
		lock.l_len=sizeof(User);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&temp,sizeof(User));
		if(!strcmp(temp.status,"ACTIVE"))	result=temp.balance;
		else					result=0;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2){
		int i=ID-1000;
		int fd=open("JUfile",O_RDONLY,0744);
		jointUser temp;
	
		int fl;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    	     //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&temp,sizeof(jointUser));
		if(!strcmp(temp.status,"ACTIVE"))	result=temp.balance;
		else					result=0;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return 0;
}

bool alterPassword(int accType,int ID,char newPwd[10]){
	int i=ID-1000;
	if(accType==1){
		int fd=open("Ufile",O_RDWR,0744);
		bool result;
		int fl;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(User);    //nth record
		lock.l_len=sizeof(User);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		User curUser;
		lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&curUser,sizeof(User));
		
		if(!strcmp(curUser.status,"ACTIVE")){
			strcpy(curUser.password,newPwd);
			lseek(fd,sizeof(User)*(-1),SEEK_CUR);
			write(fd,&curUser,sizeof(User));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser curUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&curUser,sizeof(jointUser));
		
		if(!strcmp(curUser.status,"ACTIVE")){
			strcpy(curUser.password,newPwd);
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&curUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

bool addUser(User record){
	int fd=open("Ufile",O_RDWR,0744);
	bool result;
	
	int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	User endUser;
	lseek(fd,(-1)*sizeof(User),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(User));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(User));
	if(j!=0)	result=true;
	else	result=false;
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool addJointUser(jointUser record){
	int fd=open("JUfile",O_RDWR,0744);
	bool result;
	
	int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser endUser;
	lseek(fd,(-1)*sizeof(jointUser),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(jointUser));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(jointUser));
	if(j!=0)	result=true;
	else	result=false;
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteUser(int ID){
	int i=ID-1000;
	int fd=open("Ufile",O_RDWR,0744);
	bool result;
	
	int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	User curUser;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&curUser,sizeof(User));
	
	if(!strcmp(curUser.status,"ACTIVE")){	
		strcpy(curUser.status,"CLOSED");
		curUser.balance=0;
		
		lseek(fd,(-1)*sizeof(User),SEEK_CUR); 
		int j=write(fd,&curUser,sizeof(User));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteJointUser(int ID){
	int i=ID-1000;
	int fd=open("JUfile",O_RDWR,0744);
	bool result;
	
	int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser curUser;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&curUser,sizeof(jointUser));
	
	if(!strcmp(curUser.status,"ACTIVE")){	
		strcpy(curUser.status,"CLOSED");
		curUser.balance=0;
		
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR); 
		int j=write(fd,&curUser,sizeof(jointUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modifyUser(User modUser){
	int i=modUser.userID-1000;
	int fd=open("Ufile",O_RDWR,0744);
	bool result=false;
	
	int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	User curUser;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&curUser,sizeof(User));
	
	if(!strcmp(curUser.status,"ACTIVE") && (modUser.account_no==curUser.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(User),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(User));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modifyJointUser(jointUser modUser){
	int i=modUser.userID-1000;
	int fd=open("JUfile",O_RDWR,0744);
	bool result=false;
	
	int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser curUser;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&curUser,sizeof(jointUser));
	
	if(!strcmp(curUser.status,"ACTIVE")  && (modUser.account_no==curUser.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(jointUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

void serverTask(int nsd){
	int msgLength,select,type,option,accType,curUserID;
	bool result;
	while(1){
		read(nsd,&option,sizeof(option));
		printf("Option : %d\n",option);

		if(option==1){
			User curUser1;
			accType=1;
			msgLength=read(nsd,&curUser1,sizeof(User));
			printf("Username : %d\n",curUser1.userID);
			printf("Password : %s\n",curUser1.password);
			curUserID=curUser1.userID;
			result=checkUser(curUser1);
			write(nsd,&result,sizeof(result));
		}
		else if(option==2){
			jointUser curUser2;
			accType=2;
			msgLength=read(nsd,&curUser2,sizeof(jointUser));
			curUserID=curUser2.userID;
			printf("Username : %d\n",curUser2.userID);
			printf("Password : %s\n",curUser2.password);
			result=checkJointUser(curUser2);
			write(nsd,&result,sizeof(result));
		}
		else if(option==3){
			admin curUser3;
			accType=3;
			msgLength=read(nsd,&curUser3,sizeof(admin));
			curUserID=curUser3.userID;
			printf("Username : %d\n",curUser3.userID);
			printf("Password : %s\n",curUser3.password);
			result=checkAdmin(curUser3);
			write(nsd,&result,sizeof(result));
		}
		else{
			result=false;
			write(nsd,&result,sizeof(result));
		}
		if(result)	break;		
	}

	while(1){
		read(nsd,&select,sizeof(int));
		if(option==1 || option==2){
			if(select==1){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=depositMoney(accType,curUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==2){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=withdrawMoney(accType,curUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==3){
				float amt;
				amt=getBalance(accType,curUserID);
				write(nsd,&amt,sizeof(float));
			}
			else if(select==4){
				char pwd[10];
				read(nsd,pwd,sizeof(pwd));
				result=alterPassword(accType,curUserID,pwd);
				write(nsd,&result,sizeof(result));
			}
			else if(select==5){
				if(option==1){
					User user1=getUser(curUserID);
					write(nsd,&user1,sizeof(User));
				}
				else if(option==2){
					jointUser user2=getJointUser(curUserID);
					write(nsd,&user2,sizeof(jointUser));
				}
			}
			else if(select==6)	break;
		}
		else if(option==3){
			read(nsd,&type,sizeof(int));
			if(select==1){
				if(type==1){
					User newUser1;
					read(nsd,&newUser1,sizeof(User));
					result=addUser(newUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					jointUser newUser2;
					read(nsd,&newUser2,sizeof(jointUser));
					result=addJointUser(newUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==2){
				if(type==1){
					int delUserID1;
					read(nsd,&delUserID1,sizeof(int));
					result=deleteUser(delUserID1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
					read(nsd,&delUserID2,sizeof(int));
					result=deleteJointUser(delUserID2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==3){
				if(type==1){
					User modUser1;
					read(nsd,&modUser1,sizeof(User));
					result=modifyUser(modUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					jointUser modUser2;
					read(nsd,&modUser2,sizeof(jointUser));
					result=modifyJointUser(modUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==4){
				if(type==1){
					User searchUser1;
					int userID1;
					read(nsd,&userID1,sizeof(int));
					searchUser1=getUser(userID1);
					write(nsd,&searchUser1,sizeof(User));
				}
				else if(type==2){
					jointUser searchUser2;
					int userID2;
					read(nsd,&userID2,sizeof(int));
					searchUser2=getJointUser(userID2);
					write(nsd,&searchUser2,sizeof(jointUser));
				}
			}
			else if(select==5)	break;
		}
	}
	close(nsd);
	write(1,"Ended client session.....\n",sizeof("Ended client session.....\n"));
	return;
}

void *connection_handler(void *nsd) {
	int nsfd = *(int*)nsd;
	serverTask(nsfd);
}


int main(){
	struct sockaddr_in server,client;
	int sd,nsd,clientLen;
	pthread_t threads;
	bool result;
	sd=socket(AF_INET,SOCK_STREAM,0);

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(5555);

	bind(sd,(struct sockaddr *)&server,sizeof(server));
	listen(sd,5);	//queue length :: 5 clients per connection
	
	write(1,"Waiting for the client.....\n",sizeof("Waiting for the client.....\n"));
	while(1){
		clientLen=sizeof(client);
		nsd=accept(sd,(struct sockaddr *)&client,&clientLen);

		write(1,"Connected to the client.....\n",sizeof("Connected to the client.....\n"));
		if(pthread_create(&threads,NULL,connection_handler,(void*) &nsd)<0){
			perror("could not create thread");
			return 1;		
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}