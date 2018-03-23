/*
   SERVER/CLIENTE UDP COM LISTA DE ENVIO COM CUSTO
   HENRIQUE JOSE DALLA CORTE
   gcc comunicacao.c -o cliente -lpthread -pthread
   ./cliente
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

int BUFLEN = 130; //100 da mensagem + 20 dos ids
pthread_t tids[1000]; //ids das threads
int PORT; //porta do server

typedef struct roteador{ //Struct das variaveis A,B...
   int id;
   int n_porta;
   char num_ip[15];
   struct roteador* next;
   struct roteador* under;
   struct enlace* vetor[100];
} roteador;

typedef struct enlace{ //Struct das variaveis A,B...
   int id;
   int id1;
   int custo;
   struct roteador* saida;
   struct roteador* destino;
} enlace;


typedef struct mensage{   //Struct da mensagem para ser enviada
   int id_destino;
   int port_destino;
   char num_ip_destino[15];
   char check_sum[15];
   char message[100];
} msg;

void die(char *s){
   perror(s);
   exit(1);
}

roteador* nodo(roteador* origem){ //endereçamento das novas transacoes
   roteador* aux = (roteador *) malloc(sizeof(roteador)); //aloca novo
   origem->next = aux;
   aux->next = NULL;
   aux->under = origem;
return aux;
}
enlace* nodo1(enlace* origem){ //endereçamento das novas transacoes
   enlace* aux = (enlace *) malloc(sizeof(enlace)); //aloca novo
return aux;
}

roteador* leituraroteador (){ //lê os roteadores
FILE *fp; //Ponteiro do arquivo
char buf[100]; //Buff da linha
char *pstr;        //ponteiro para posição na strin
char delimita[] = " ";
int first = 1, i;
char id[100], porta[100], ip[100];
roteador* p = (roteador *) malloc(sizeof(roteador)); //aloca primeiro nodo
roteador* pinicio;
pinicio = p;
fp = fopen("roteador.config","r");//abre o arquivo e aponta pro ponteiro

   if(fp){//se abriu o arquivo
       while(fgets(buf, 100, fp) != NULL){ // lê a linha
           pstr = strtok(buf, delimita);//procura o delimitador " "
           strcpy(id,pstr); //passa o valor de antes do delimitador para varia
           pstr = strtok(NULL, delimita); //pega apos o delimitador " "
           strcpy(porta,pstr); //passa para a variavel val
           pstr = strtok(NULL, delimita); //pega apos o delimitador " "
           strcpy(ip,pstr); //passa para a variavel val
           if(first){//se não existe primeiro nodo
               p->id = atoi (id);
               p->n_porta= atoi (porta);
               strcpy(p->num_ip,ip); //copia o valor ||
               p->next = NULL; //aponteiramento esquerda
               p->under=NULL;
               for(i=0; i < 100; i++) p->vetor[i] = NULL;
               first=0; //diz que já foi feito o primeiro

           }else{ //se já existe primeiro
               p = nodo(p); //cria e faz aponteiramento
               p->id = atoi (id);
               p->n_porta= atoi (porta);
               strcpy(p->num_ip,ip); //copia o valor ||
               for(i=0; i < 100; i++) p->vetor[i] = NULL;
           }
       }
   }else{ //se não abrir o arquivo
           printf("Arquivo nao pode ser aberto\n");
           return NULL;
   }
   fclose(fp);
   return pinicio; //retorna inicio da lista de roteadores
}

void arestas(roteador* origem, enlace* aresta){ //insere arestas de valor aos roteadores
   int i=0,flagd=0, flagi=0;
   roteador* origem1;
   origem1 = origem;

   for(;origem1!=NULL;origem1=origem1->next){ //procura id nos roteadores
       if(origem1->id==aresta->id) flagi=1;
       if(origem1->id==aresta->id1) flagd=1;
   }
   if(flagd==1 && flagi==1){ //se existe roteadores da aresta
       for(;origem!=NULL;origem=origem->next){
           if(aresta->id == origem->id){
               while(origem->vetor[i]!=NULL && i<50)i++;
               origem->vetor[i] = aresta;
               aresta->saida = origem;
           }if(aresta->id1 == origem->id){
               aresta->destino = origem;
           }
       }
   }else if(flagd){ //nao existe id origem
       printf("ID Origem NAO EXISTE %d \n",aresta->id);
   }else if(flagi){ //nao existe id destino
       printf("ID Destino NAO EXISTE %d \n",aresta->id1);
   }else{ //nao existe os ids
       printf("ID Origem e Destino NÃO EXISTEM \n");
   }
}

void leituraenlace(roteador* origem){ //le arquivo enlace e cria arestas
FILE *fp; //Ponteiro do arquivo
char buf[100]; //Buff da linha
char *pstr;        //ponteiro para posição na strin
char delimita[] = " ";
int first = 1;
char id_i[100], id_d[100], custo[100];
enlace* p = (enlace *) malloc(sizeof(enlace)); //aloca primeiro nodo
fp = fopen("enlaces.config","r");//abre o arquivo e aponta pro ponteiro

   if(fp){//se abriu o arquivo
       while(fgets(buf, 100, fp) != NULL){ // lê a linha
           pstr = strtok(buf, delimita);//procura o delimitador " "
           strcpy(id_i,pstr); //passa o valor de antes do delimitador para varia
           pstr = strtok(NULL, delimita); //pega apos o delimitador " "
           strcpy(id_d,pstr); //passa para a variavel val
           pstr = strtok(NULL, delimita); //pega apos o delimitador " "
           strcpy(custo,pstr); //passa para a variavel val

           if(first){//se não existe primeiro nodo
               p->id = atoi(id_i);
               p->id1 = atoi(id_d);
               p->custo = atoi(custo);
               //função de enlace
               arestas(origem,p);
               first=0; //diz que já foi feito o primeiro

           }else{ //se já existe primeiro
               p = nodo1(p); //cria e faz aponteiramento
               p->id = atoi(id_i);
               p->id1 = atoi(id_d);
               p->custo = atoi(custo);
               //função de enlace
               arestas(origem,p);
           }
       }
   }else{ //se não abrir o arquivo
           printf("Arquivo nao pode ser aberto\n");
           //return NULL;
   }
   fclose(fp);
}

void envio(int id_destino, char message1[], roteador* origem){ //FUNCAO QUE CONCATENA INFORMAÇÕES PARA O ENVIO
   int cont = 0;
   msg* info = (msg *) malloc(sizeof(msg)); //cria a struct de informação

   strcpy(info->message,message1);
   info->id_destino = id_destino;
   for(;origem!=NULL;origem = origem->next){
       if(origem->id == id_destino){
           info->port_destino = origem->n_porta;
           strcpy(info->num_ip_destino,origem->num_ip);
       }
   }
   //info->check_sum = check_sum();
   client(info);
}
void mostra(roteador* origem){
    int x;
   for(;origem!=NULL;origem = origem->next){
       printf("%d\n", origem->id);
       for(x = 0;origem->vetor[x];x++) printf("destino %d\n", origem->vetor[x]->id1);
   }

}

void client(msg* info){ //FUNÇÃO DE ENVIO MENSAGEM
   struct sockaddr_in si_other;
   int s, i, slen=sizeof(si_other);
   char buf[BUFLEN]; //VARIAVEIS DE CONTROLE
   char SERVER[20];
   char message[BUFLEN];
   char aux[BUFLEN];
   int PORT;

   PORT = info->port_destino; //PORTA DESTINO
   strcpy(SERVER,info->num_ip_destino); //Funções para formação da mensagem [DESTINO|MENSAGEM]
   strcpy(message,info->message);
   sprintf(aux,"%d|",info->id_destino);
   strncat(aux,message,100);
   strcpy(message,aux);
   if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
   {
       die("socket");
   }

   memset((char *) &si_other, 0, sizeof(si_other));
   si_other.sin_family = AF_INET;
   si_other.sin_port = htons(PORT);

   if (inet_aton(SERVER , &si_other.sin_addr) == 0)
   {
       fprintf(stderr, "inet_aton() failed\n");
       exit(1);
   }

       //send the message
       if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
       {
           die("sendto()");
       }

       //receive a reply and print it
       //clear the buffer by filling null, it might have previously received data
       memset(buf,'\0', BUFLEN);

       //try to receive some data, this is a blocking call
       if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
       {
           die("recvfrom()");
       }

       printf("\nACK Recebido! Recebeu do Servidor %d\n",info->id_destino);
       //puts(buf);

   close(s);
}


void server(roteador* origem){ //servidor esperando um cliente
   struct sockaddr_in si_me, si_other;
   int s, i, slen = sizeof(si_other) , recv_len;
   char numero[15];//variaveis de controle
   char message[100];//variaveis de controle
   char buf[BUFLEN];
   char check[15];
   int x,y,z,prox,custo=11234567; //variaveis de controle
   roteador* aux_rot;

   PORT = origem->n_porta; //seta porta

   //create a UDP socket
   if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
   {
       die("socket");
   }

   // zero out the structure
   memset((char *) &si_me, 0, sizeof(si_me));

   si_me.sin_family = AF_INET;
   si_me.sin_port = htons(PORT);
   si_me.sin_addr.s_addr = htonl(INADDR_ANY);

   //printf("%d\n",PORT);
   //bind socket to port
   if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
   {
       die("bind");
   }

   //keep listening for data
   while(1)
   {
       //receive a reply and print it
       //clear the buffer by filling null, it might have previously received data
       memset(buf,'\0', BUFLEN);

       //try to receive some data, this is a blocking call
       if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
       {
           die("recvfrom()");
       }

       //now reply the client with the same data
       if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
       {
           die("sendto()");
       }
       
       y=1;
       custo=11234567;
       memset(numero,'\0',15);
       memset(check,'\0',15);
       memset(message,'\0',100);

       for(x=0,z=0;x<120;x++,z++){ //divisao da mensagem em DESTINO | MENSAGEM
           if(buf[x]=='|'){
               x++;
               y=0;
               z=0;
           }
           if(y){
               numero[z]=buf[x];
           }else{
               message[z]=buf[x];
           }
       }

       //printf("%d == %d\n", atoi(numero),origem->id);
       if(atoi(numero)!=origem->id){ //PROCURANDO ENTRE POSSIVEIS CAMINHOS O MENOR
               for(aux_rot = origem;aux_rot->under!=NULL;aux_rot = origem->under);
               envio(atoi(numero),message,aux_rot);
       }else printf("Pacote entregue ao destino");
   }

   close(s);
   pthread_exit(NULL);
}

void Lroteadores(roteador* origem, int rt){//lança threads do roteador com sua lista(tabela) de vizinhos e configuração
   int x =1;
   for(x = 1;origem!=NULL;origem = origem->next,x++){
       //printf("%d %d\n",x,rt );
       if(origem->id==rt){
           pthread_create(&tids[x], NULL, server, origem);
           printf("%d\n", origem->id);
       }
   usleep(1000);
   }
}

int verifica(int destino, roteador* origem){ //PROCURA SE ID DIGITADO ESTA CERTO!
   int flag1 = 0;
   for(;origem!=NULL;origem=origem->next){
       if(destino==origem->id) flag1=1;
   }
   if(!flag1){//FLAG ERRO DESTINHO
       printf("ID errado\n");
       return 0;
   }else return 1;
}

int main(void)
{
char message[100];
roteador* iniroteador;//ponteiro de inicio
roteador* roteador; //ponteiro que recebe retorno da leitura dos roteadores]
int destino=0; //auxiliares
int aux[50];
int router;
memset(aux,(int)-1,sizeof(int)*50);

roteador = leituraroteador();//le roteadores e devolve o inicio deles
leituraenlace(roteador);//cria arestas


//carregados(roteador);
mostra(roteador);
//autos(roteador);
   printf("Qual roteador iniciar ?\n");
   scanf("%d", &router);
   verifica(router,roteador);
   Lroteadores(roteador,router);
   while(destino!=-1){
       iniroteador = roteador;//marca inicio dos roteadores
        //CHAMADA DE MENOR VALOR DO CAMINHO
           //printf("Digite ID origem | ID destino\n");
           scanf("%d", &destino);
           //printf("Enter message : ");
           scanf("%s",message);
           if(verifica(destino,roteador)){      //verifica os roteadores digitados
               envio(destino,message,iniroteador); //chama o envio da mensagem ao primeiro roteador
           }
   }

return 0;
}
