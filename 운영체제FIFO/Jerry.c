#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    char * chatTtoJ = "./chatTtoJ"; 
    char * chatJtoT = "./chatJtoT";
    char user[80], other[80];
    int i, n;
    int fdw, fdr;
	
	mkfifo(chatTtoJ, 0666);
    mkfifo(chatJtoT, 0666);
    //접근 권한이 0666이라는 chatTtoJ라는 FIFO를 생성했다.
    //성공시 0 반환 실패시 -1반환
    //0666권한은 (owner, group, user)모두 읽기 쓰기 권한을 준 것

    //FIFO를 사용하기 위해는 열어야한다!
    //FIFO를 개방하기 전에는 반드시 쓰기 권한이 있어야 한다.
    //최초 쓰기 개방 전에는 모든 읽기 개방을 block해버리기 떄문

	fdw = open(chatTtoJ, O_WRONLY);
    fdr = open(chatJtoT, O_RDONLY);
	//chatTtoj를 쓰기 모드로! , chatJtoT는 읽기 모드로!
    //위 변수는 최초개방이므로 O_RDWR 읽기 쓰기 모두 가능 모드로 개방했다.

    while(1){
        i=0;
        while((n==read(fdr, &other[i], 1))>0){
            if(other[i]=='\0') break;
            if(i==78){
                other[++i]='\0';
                break;
            }
            i++;
        }
        if(n<=0){
            printf("상대가 종료하였습니다.\n");
            break;
        }
        printf("other: %s\n", other);
        if(strcmp(other, "exit\n")==0){
            printf("exit이므로 break합니다.\n");
            break;
        }
        printf(">>>");
        fgets(user, 80, stdin);
        write(fdw, user, strlen(user)+1);
        if(strcmp(user, "exit\n")==0){
            printf("break합니다\n");
            break;
        }
    }
    close(fdw);
    close(fdr);
    return 0;
    //pipe 특성상 정해진 바이트를 읽을때 까지 대기를 해서 정해진 사이즈까지 읽으면 출력한다    
}