#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#ifdef WIN32
#include <windows.h>
#endif
#define MAX_XMLS_QNT 100000
#define PRO_DIR "ct-es" 
struct dirent *listar;
DIR *pro;
int qnt=0;
struct xml
{
		char chave[45];
		char uf[3]; 
		char ano_mes[5];
		char cnpj[15];
		char modelo[3];
		char serie[4];
		char cte[10];
		char forma;
		char codigo[9];
		char dv;
};

struct xml *xmls;

int estruturar()
{
	pro = opendir(PRO_DIR);
	if(pro==NULL)
	{
			printf("[!]diretorio não encontrado!\n");
			return 1;
	}
	//inserindo as chaves de xml em uma estrutura
	while((listar = readdir(pro)))
	{
		if(strcmp(listar->d_name,"..")!=0&&strcmp(listar->d_name,".")!=0&&strlen(listar->d_name)==56)
		{
			strcpy(xmls[qnt].chave,(listar->d_name));
			xmls[qnt].chave[44] = '\0';
			
			strcpy(xmls[qnt].uf, xmls[qnt].chave);
			xmls[qnt].uf[2] = '\0';
			
			strcpy(xmls[qnt].ano_mes, xmls[qnt].chave+2);
			xmls[qnt].ano_mes[4] = '\0';
			
			strcpy(xmls[qnt].cnpj, xmls[qnt].chave+6);
			xmls[qnt].cnpj[14] = '\0';
			
			strcpy(xmls[qnt].modelo, xmls[qnt].chave+20);
			xmls[qnt].modelo[2] = '\0';
			
			strcpy(xmls[qnt].serie, xmls[qnt].chave+22);
			xmls[qnt].serie[3] = '\0';
			
			strcpy(xmls[qnt].cte, xmls[qnt].chave+25);
			xmls[qnt].cte[9] = '\0';
			
			xmls[qnt].forma = xmls[qnt].chave[34];
			
			strcpy(xmls[qnt].codigo, xmls[qnt].chave+35);
			xmls[qnt].codigo[8] = '\0';
			
			xmls[qnt].dv = xmls[qnt].chave[43];
			
			if(qnt==MAX_XMLS_QNT)
			{
					printf("[!] Limite de pesquisa por ctes\n");
					return 1;
			}
			qnt++;
		}
	}
	qnt--;
	return 0;
}

int main()
{
	int cont,saida=0,err=0;
	setlocale(LC_ALL,"Portuguese");
	xmls = malloc(sizeof(xmls)*MAX_XMLS_QNT);
	if(estruturar())
		return 1;
	char *chave = malloc(44);
	char *command = malloc(75);
	while(1)
	{
		#ifdef __linux__
		sleep(1);
		#endif
		#ifdef WIN32
		Sleep(1000);
		#endif
		
		printf("Verificando...\n");
		seekdir(pro,0);
		while((listar = readdir(pro)))		
		{
			if(strcmp(listar->d_name,"..")!=0&&strcmp(listar->d_name,".")!=0)
			{
				strcpy(chave,listar->d_name);
				chave[44] = '\0';
				for(cont=0;cont<=qnt;cont++)
				{
					if(strcmp(chave,xmls[cont].chave)==0)
					{
						saida=1;
						break;
					}
				}
				if(saida==0)
				{
					printf("Xml novo encontrado: %s\n",chave);
					printf("\n=================================================================\n");
					printf("Chave:   %s\n",xmls[qnt].chave);
					printf("UF:      %s\n",xmls[qnt].uf);
					printf("AnoMes:  %s\n",xmls[qnt].ano_mes);
					printf("CNPJ:    %s\n",xmls[qnt].cnpj);
					printf("Modelo:  %s\n",xmls[qnt].modelo);
					printf("Serie:   %s\n",xmls[qnt].serie);
					printf("CT-e:    %s\n",xmls[qnt].cte);
					printf("Forma    %c\n",xmls[qnt].forma);
					printf("Codigo:  %s\n",xmls[qnt].codigo);
					printf("Digito:  %c\n",xmls[qnt].dv);
					printf("\n=================================================================\n");	
					#ifdef __linux__
					sleep(1);
					#endif
					#ifdef WIN32
					Sleep(1000);
					#endif			
					sprintf(command,"%s/%s",PRO_DIR,listar->d_name);
					command[70] = '\0';
					err = execl("php","login.php",PRO_DIR);
					if(err!=0)
					{
							printf("\n[!] algum erro ocorreu\n");
					}
					else
					{
							qnt=0;
							if(estruturar())
								return 1;
							break;
					}
				}
				else
				{
					saida=0;
				}
				
			}
		}
	}
}










