#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#ifdef WIN32
#include <windows.h>
#endif
#define CONF "averba.conf"
#define CONF_QNT 5
#define CONF_C 50
FILE *logger;
struct dirent *listar;
DIR *pro;
int qnt=0;
char *configs[] = {"xml_len=","max_xml=","pro_dir=","cnpj_lg=","senh_lg="};
struct conf 
{
		int  xml_len;
		long max_xml;
		char pro_dir[100];
		char cnpj_lg[50];
		char senh_lg[50];
};
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
struct conf config;

int configurar()	
{
	char *c,*teste;
	FILE *arquivo;
	c = malloc(CONF_C);
	teste = malloc(CONF_C/2);
	
	arquivo = fopen(CONF,"r");
	if(arquivo == NULL)
	{
			printf("[!] Sem arquivo de configuracao\n");
			return 1;
	}
	config.max_xml = 0;
	config.xml_len = 0;
	
	while((fgets(c,50,arquivo))!=NULL)
	{
		memset(teste,0x0,strlen(teste));
		strcpy(teste,c);
		teste[8] = '\0';
		if(strcmp(teste,configs[0])==0)
		{
			config.xml_len = atoi(c+8);
			printf("xml_len=%i\n",config.xml_len);
		}
		if(strcmp(teste,configs[1])==0)
		{
			config.max_xml = atoi(c+8);
			printf("max_xml=%li\n",config.max_xml);
		}
		if(strcmp(teste,configs[2])==0)
		{
			strcpy(config.pro_dir,c+8);
			if(config.pro_dir[strlen(config.pro_dir)-1] == '\n'||config.pro_dir[strlen(config.pro_dir)-1] == '\r')
				config.pro_dir[strlen(config.pro_dir)-1] = '\0';
			printf("pro_dir=%s\n",config.pro_dir);
		}
		if(strcmp(teste,configs[3])==0)
		{
			strcpy(config.cnpj_lg,c+8);
			if(config.cnpj_lg[strlen(config.cnpj_lg)-1] == '\n')
				config.cnpj_lg[strlen(config.cnpj_lg)-1] = '\0';
			printf("cnpj_lg=%s\n",config.cnpj_lg);
		}		
		if(strcmp(teste,configs[4])==0)
		{
			strcpy(config.senh_lg,c+8);
			if(config.senh_lg[strlen(config.senh_lg)-1] == '\n')
				config.senh_lg[strlen(config.senh_lg)-1] = '\0';
			printf("senh_lg=%s\n",config.senh_lg);
		}
	}
	if(config.xml_len==0)
	{
		printf("falta parametro xml_len\n");
		return 1;
	}
	if(config.max_xml==0)
	{
		printf("falta parametro max_xml\n");
		return 1;
	}
	if(strlen(config.pro_dir)==0)
	{
		printf("falta parametro tag pro_dir\n");
		return 1;
	}
	if(strlen(config.cnpj_lg)==0)
	{
		printf("falta parametro tag cnpj_lg\n");
		return 1;
	}
	if(strlen(config.senh_lg)==0)
	{
		printf("falta parametro tag senh_lg\n");
		return 1;
	}
	return 0;
}

int estruturar()
{
	qnt=0;
	pro = opendir(config.pro_dir);
	if(pro==NULL)
	{
	
			printf("[!]diretorio não encontrado!\n");
			return 1;
	}
	printf("Estruturando...\n");
	//inserindo as chaves de xml em uma estrutura
	while((listar = readdir(pro)))
	{
		if(strcmp(listar->d_name,"..")!=0&&strcmp(listar->d_name,".")!=0&&((strlen(listar->d_name))<=config.xml_len))
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

			if(qnt==config.max_xml)
			{
				printf("[!] Limite de pesquisa por ctes\n");
				qnt--;
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
	if(configurar())
		return 1;
	xmls = malloc(sizeof(struct xml)*config.max_xml);
	if(estruturar())
		return 1;
	char *chave = malloc(44);
	char *command = malloc(2000);
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
			if(strcmp(listar->d_name,"..")!=0&&strcmp(listar->d_name,".")!=0&&strlen(listar->d_name)<=config.xml_len)
			{
				//printf("%s\n",listar->d_name);
				
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
					sprintf(command,"php login.php %s/%s %s %s",config.pro_dir,listar->d_name,config.cnpj_lg,config.senh_lg);
					command[200] = '\0';
					logger = fopen("logs.txt","a+");
					if(logger!=NULL)
					{
						fprintf(logger,"%s\n",command);
						fclose(logger);
					}
					err = system(command);
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










