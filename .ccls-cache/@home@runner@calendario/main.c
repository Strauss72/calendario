#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// struct registro
struct data {
  int dia, mes, ano;
};
typedef struct data Data;
struct contato {
  int id;
  char nome[255];
  char apelido[255];
  char sexo;
  char rua[255];
  int numero;
  char complemento[255];
  char bairro[255];
  char cidade[255];
  char uf[255];
  int cep;
  int tel_residencia;
  int tel_comercial;
  int tel_celular;
  Data aniversario;
  Data primeiro_encontro;
  char local_primeiro_encontro[255];
  char tipo_relacao[10];
  union {
    char local_trab[100];
    char local_com[100];
  };
  char observacoes[255];
  bool excluir;
};
typedef struct contato Contato;
struct node {
  Contato *info;
  struct node *prox;
};
typedef struct node Node;

// funcoes
void display_menu(void);
void display_menu_2(void);
void printprint(void);

FILE *openFile(const char *file, const char *mode);
Node *arqBinarioLista(Node *lst, FILE *fin);
void geraArquivo(Node *lst);
void geraArquivoBackUp(Node *lst);

void exibeDadosContatos(Node *lst);

Node *incluirContato(Node *lst);

Node *excluirContato(Node *lst);

int consultaNome(Node *lst, char *nomeInput);
void consultaMesAniv(Node *lst, int mesInput);
void consultaTipoRelacao(Node *lst, char *tipoRelacaoInput);
void consultaTipoRelacaoEMesAniv(Node *lst, char *tipoRelacaoInput,
                                 int mesInput);
void consultaAniversariantes(Node *lst, int dia, int mes, int ano);

Node *excluiNome(Node *lst, char *nome);
Node *excluiCodigo(Node *lst, int codigo);

Node *alteraContato(Node *lst);
Node *altera(Node *lst);
int retornaCodigo(Node *lst);
void exibe3em3(Contato *lst);
void exibeDadosContato(Node *lst, int dia, int mes, int ano);
void exibeAniversariantesMes(Node *lst, int mes);
void exibeDadosParentes(Node *lst);

Data converteDataNascimento(char *data);
/*-----------------------------------------------------------------------------*/
int main(void) {
  printprint();
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("Dia (hoje): %d/%02d/%02d\n", tm.tm_mday, tm.tm_mon + 1,
         tm.tm_year + 1900);
  FILE *arqBinario;
  Node *lst = NULL;
  int choice, choice2;
  int fim = 0;
  arqBinario = openFile("calendario.dat", "rb");
  if (arqBinario != NULL) {
    lst = arqBinarioLista(lst, arqBinario);
    fclose(arqBinario);
    // checar aniversariante
    consultaAniversariantes(lst, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
  }

  while (fim == 0) {
    display_menu();
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      lst = incluirContato(lst);
      printf("\nContato Inserido :)\n");
      break;
    case 2:
      display_menu_2();
      scanf("%d", &choice2);
      char nomeInput[100], tipoRelacaoInput[10];
      int mesInput;
      switch (choice2) {
      case 1:
        printf("Insira nome: ");
        scanf(" %[^\n]", nomeInput);
        consultaNome(lst, nomeInput);
        break;
      case 2:
        printf("Insira mes: ");
        scanf("%d", &mesInput);
        consultaMesAniv(lst, mesInput);
        break;
      case 3:
        printf("Insira tipo relacao: ");
        scanf(" %[^\n]", tipoRelacaoInput);
        consultaTipoRelacao(lst, tipoRelacaoInput);
        break;
      case 4:
        printf("Insira tipo relacao: ");
        scanf(" %[^\n]", tipoRelacaoInput);
        printf("Insira mes: ");
        scanf("%d", &mesInput);
        consultaTipoRelacaoEMesAniv(lst, tipoRelacaoInput, mesInput);
        break;
      default:
        printf("ERROR - ENTER A VALID OPTION");
        break;
      }
      break;
    case 3:
      printf("FUNCAO_OP_3");
      lst = alteraContato(lst);
      break;
    case 4:
      printf("FUNCAO_OP_4");
      lst = excluirContato(lst);
      break;
    case 5: // listar
      printf("Listando ...\n");
      exibeDadosContato(lst, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
      break;
    case 6:
      printf("FUNCAO_OP_6");
      geraArquivoBackUp(lst);
      break;
    case 7:
      geraArquivo(lst);
      printf("\nFinalizando programa ...");
      fim = 1;
      break;
    default:
      printf("ERROR - ENTER A VALID OPTION");
      break;
    }
    printf("\n---------------------------------------------\n\n");
  }
  return 0;
}
/*-----------------------------------------------------------------------------*/
FILE *openFile(const char *file, const char *mode) { // funcao fopen melhorada
  FILE *arq;
  arq = fopen(file, mode);
  if (arq == NULL)
    return NULL;
  return arq;
}
Node *arqBinarioLista(Node *lst, FILE *fin) { // file -> struct node
  Node *tmp = (Node *)malloc(sizeof(Node));
  tmp->info = (Contato *)malloc(sizeof(Contato));
  if (!tmp)
    exit(1);
  if (!tmp->info)
    exit(1);
  tmp->prox = NULL;
  Node *novo = NULL;
  Node *current = NULL;
  Node *head = lst;

  // FILE READING ...
  while (fread(tmp->info, sizeof(Contato), 1, fin)) {
    // CRIACAO DO NOVO BLOCK
    novo = (Node *)malloc(sizeof(Node));
    if (!novo)
      exit(1);
    novo->info = (Contato *)malloc(sizeof(Contato));
    if (!novo->info)
      exit(1);
    novo->prox = NULL;

    // PASSAGEM DE DADOS
    // codigo
    novo->info->id = tmp->info->id;
    // nome
    strcpy(novo->info->nome, tmp->info->nome);
    // apelido
    strcpy(novo->info->apelido, tmp->info->apelido);
    // sexo
    novo->info->sexo = tmp->info->sexo;
    // endereco
    strcpy(novo->info->apelido, tmp->info->apelido);
    novo->info->numero = tmp->info->numero;
    strcpy(novo->info->complemento, tmp->info->complemento);
    strcpy(novo->info->bairro, tmp->info->bairro);
    strcpy(novo->info->cidade, tmp->info->cidade);
    strcpy(novo->info->uf, tmp->info->uf);
    novo->info->cep = tmp->info->cep;
    // telefone
    novo->info->tel_residencia = tmp->info->tel_residencia;
    novo->info->tel_comercial = tmp->info->tel_comercial;
    novo->info->tel_celular = tmp->info->tel_celular;
    // data_aniversario
    novo->info->aniversario.dia = tmp->info->aniversario.dia;
    novo->info->aniversario.mes = tmp->info->aniversario.mes;
    novo->info->aniversario.ano = tmp->info->aniversario.ano;
    // primeiro_encontro
    novo->info->primeiro_encontro.dia = tmp->info->primeiro_encontro.dia;
    novo->info->primeiro_encontro.mes = tmp->info->primeiro_encontro.mes;
    novo->info->primeiro_encontro.ano = tmp->info->primeiro_encontro.ano;
    // local_primeiro_encontro
    strcpy(novo->info->local_primeiro_encontro,
           tmp->info->local_primeiro_encontro);
    // tipo de relacao
    strcpy(novo->info->tipo_relacao, tmp->info->tipo_relacao);
    if (strcmp(novo->info->tipo_relacao, "T"))
      strcpy(novo->info->local_com, tmp->info->local_trab);
    else if (strcmp(novo->info->tipo_relacao, "C"))
      strcpy(novo->info->local_com, tmp->info->local_com);
    // observacoes
    strcpy(novo->info->observacoes, tmp->info->observacoes);

    novo->info->excluir = false;

    // BLOCK-CHAIN
    if (head == NULL) {
      current = head = novo;
    } else {
      current = current->prox = novo;
    }
  }
  return head;
}
Node *incluirContato(Node *lst) {
  char nome[255];
  char tmp[255];

  Node *novo = (Node *)malloc(sizeof(Node));
  novo->info = (Contato *)malloc(sizeof(Contato));
  if (!novo)
    exit(1);
  if (!novo->info)
    exit(1);
  novo->info->id = retornaCodigo(lst);
  printf("Forneca as seguintes informacoes do contato (id:%d): \n",
         novo->info->id);
  printf("Nome: ");
  scanf(" %[^\n]", novo->info->nome);
  printf("Apelido: ");
  scanf(" %[^\n]", novo->info->apelido);
  printf("Sexo: ");
  scanf(" %c", &novo->info->sexo);
  printf("\nendereco:\n-rua: ");
  scanf(" %[^\n]", novo->info->rua);
  printf("-numero: ");
  scanf("%d", &novo->info->numero);
  printf("-complemento: ");
  scanf(" %[^\n]", novo->info->complemento);
  printf("-bairro: ");
  scanf(" %[^\n]", novo->info->bairro);
  printf("-cidade: ");
  scanf(" %[^\n]", novo->info->cidade);
  printf("-uf: ");
  scanf(" %[^\n]", novo->info->uf);
  printf("-cep: ");
  scanf("%d", &novo->info->cep);
  printf("telefone residencial: ");
  scanf("%d", &novo->info->tel_residencia);
  printf("telefone comercial: ");
  scanf("%d", &novo->info->tel_comercial);
  printf("telefone celular: ");
  scanf("%d", &novo->info->tel_celular);
  printf("aniversario: ");
  scanf(" %d/%d/%d", &novo->info->aniversario.dia, &novo->info->aniversario.mes,
        &novo->info->aniversario.ano);
  printf("data primeiro encontro: ");
  scanf(" %d/%d/%d", &novo->info->primeiro_encontro.dia,
        &novo->info->primeiro_encontro.mes, &novo->info->primeiro_encontro.ano);
  printf("local primeiro encontro: ");
  scanf(" %[^\n]", novo->info->local_primeiro_encontro);
  printf("tipo de relacao: ");
  scanf(" %[^\n]", novo->info->tipo_relacao);
  if (strcmp(novo->info->tipo_relacao, "T") == 0) {
    printf("local de trabalho : ");
    scanf(" %[^\n]", novo->info->local_trab);
  } else if (strcmp(novo->info->tipo_relacao, "C") == 0) {
    printf("local de comercio : ");
    scanf(" %[^\n]", novo->info->local_com);
  }
  printf("observacoes: ");
  scanf(" %[^\n]", novo->info->observacoes);

  novo->info->excluir = false;

  novo->prox = lst;
  return novo;
}
// void exibeDadosContato(Node *lst) {
//   printf("INFORMACOES DOS CONTATOS LISTADOS\n");
//   Node *p = lst;
//   while (p != NULL) {
//     printf("id:%d | nome: %s | telefone celular: %d |
//     %d/%d/%d\n",p->info->id, p->info->nome,
//            p->info->tel_celular, p->info->aniversario.dia,
//            p->info->aniversario.mes, p->info->aniversario.ano);
//     p = p->prox;
//   }
// }
void exibeDadosContato(Node *lst, int dia, int mes, int ano) {
  printf("\nINFORMACOES DOS CONTATOS LISTADOS\n");
  exibeDadosContatos(lst);
  printf("\nINFORMACOES DOS PARENTES\n");
  exibeDadosParentes(lst);
  printf("\nINFORMACOES DOS ANIVERSARIANTES DO DIA\n");
  consultaAniversariantes(lst, dia, mes, ano);
  printf("\nINFORMACOES DOS ANIVERSARIANTES DO MES\n");
  exibeAniversariantesMes(lst, mes);
}
void exibeDadosContatos(Node *lst) {
  Node *p = lst;
  while (p != NULL) {
    exibe3em3(p->info);
    p = p->prox;
  }
}

void exibe3em3(Contato *lst) {
  printf("id: %d | nome: %s | apelido: %s \n", lst->id, lst->nome,
         lst->apelido);
  printf("sexo: %c | rua: %s | numero: %d\n", lst->sexo, lst->rua, lst->numero);
  printf("complemento: %s | bairro: %s | cidade: %s\n", lst->complemento,
         lst->bairro, lst->cidade);
  printf("uf: %s | cep: %d | telefone residencia: %d\n", lst->uf, lst->cep,
         lst->tel_residencia);
  printf(
      "telefone comercial: %d | telefone celular: %d | aniversario: %d/%d/%d\n",
      lst->tel_comercial, lst->tel_celular, lst->aniversario.dia,
      lst->aniversario.mes, lst->aniversario.ano);
  printf("data do primeiro encontro: %d/%d/%d | local do primeiro encontro: %s "
         "| tipo de relacao: %s\n",
         lst->primeiro_encontro.dia, lst->primeiro_encontro.mes,
         lst->primeiro_encontro.ano, lst->local_primeiro_encontro,
         lst->tipo_relacao);
  if (strcmp(lst->tipo_relacao, "Trabalho") == 0)
    printf("local do trabalho: %s | ", lst->local_trab);
  else if (strcmp(lst->tipo_relacao, "C") == 0)
    printf("local do comercio: %s | ", lst->local_com);
  printf("observacoes: %s | excluir: %d\n", lst->observacoes, lst->excluir);
}
void exibeDadosParentes(Node *lst) {
  Node *p = lst;
  while (p != NULL) {
    if (strcmp(p->info->tipo_relacao, "P") == 0)
      exibe3em3(p->info);
    p = p->prox;
  }
}

void exibeAniversariantesMes(Node *lst, int mes) {
  Node *p = lst;
  while (p != NULL) {
    if (p->info->aniversario.mes == mes)
      exibe3em3(p->info);
    p = p->prox;
  }
}

void consultaAniversariantes(Node *lst, int dia, int mes, int ano) {
  Node *p = lst;
  while (p != NULL) {
    if (p->info->aniversario.mes == mes && p->info->aniversario.dia == dia)
      printf("Hoje, %s %s esta fazendo %d anos. Feliz aniversario!\n",
             p->info->nome, p->info->apelido, ano - p->info->aniversario.ano);
    p = p->prox;
  }
}
// corrigir -> passar para 4 bin e corrigir o open
void geraArquivoBackUp(Node *lst) {

  FILE *arqSaidaA = openFile("calendarioA.dat", "wb");
  FILE *arqSaidaP = openFile("calendarioP.dat", "wb");
  FILE *arqSaidaC = openFile("calendarioC.dat", "wb");
  FILE *arqSaidaT = openFile("calendarioT.dat", "wb");

  Node *p = lst;
  while (p != NULL) {
    if (p->info->excluir == false) {
      if (strcmp(p->info->tipo_relacao, "A") == 0)
        fwrite(p->info, sizeof(Contato), 1, arqSaidaA);
      else if (strcmp(p->info->tipo_relacao, "P") == 0)
        fwrite(p->info, sizeof(Contato), 1, arqSaidaP);
      else if ((strcmp(p->info->tipo_relacao, "C") == 0))
        fwrite(p->info, sizeof(Contato), 1, arqSaidaC);
      else if (strcmp(p->info->tipo_relacao, "Trabalho") == 0)
        fwrite(p->info, sizeof(Contato), 1, arqSaidaT);
      else
        printf("ERROR");
    }
    p = p->prox;
  }
  fclose(arqSaidaA);
  fclose(arqSaidaP);
  fclose(arqSaidaC);
  fclose(arqSaidaT);
}
void geraArquivo(Node *lst) {
  FILE *arqSaida;
  arqSaida = openFile("calendario.dat", "wb");

  Node *p = lst;
  while (p != NULL) {
    if (p->info->excluir == false)
      fwrite(p->info, sizeof(Contato), 1, arqSaida);
    p = p->prox;
  }

  Node *tmp;
  while (lst != NULL) {
    tmp = lst;
    lst = lst->prox;
    free(tmp->info);
    free(tmp);
  }
  fclose(arqSaida);
}
//
int consultaNome(Node *lst, char *nomeInput) {
  Node *p = lst;
  int c = 0;
  while (p != NULL) {
    if (strcmp(p->info->nome, nomeInput) == 0) {
      printf("nome : %s\n", p->info->nome);
      c++;
    }
    // exibe rows ...
    p = p->prox;
  }
  if (c == 0) {
    printf("nao existe nenhum contato no sistema com essas definicoes");
    return -1;
  } else if (c < 2)
    return 0;
  else
    return 1;
}
void consultaMesAniv(Node *lst, int mesInput) {
  Node *p = lst;
  int c = 0;
  while (p != NULL) {
    if (p->info->aniversario.mes == mesInput) {
      printf("nome : %s\n", p->info->nome);
      c = 1;
    }
    // exibe rows ...
    p = p->prox;
  }
  if (c == 0)
    printf("nao existe nenhum contato no sistema com essas definicoes");
}
void consultaTipoRelacao(Node *lst, char *tipoRelacaoInput) {
  Node *p = lst;
  int c = 0;
  while (p != NULL) {
    if (strcmp(p->info->tipo_relacao, tipoRelacaoInput) == 0) {
      printf("nome : %s\n", p->info->nome);
      c = 1;
    }
    // exibe rows ...
    p = p->prox;
  }
  if (c == 0)
    printf("nao existe nenhum contato no sistema com essas definicoes");
}
void consultaTipoRelacaoEMesAniv(Node *lst, char *tipoRelacaoInput,
                                 int mesInput) {
  Node *p = lst;
  int c = 0;
  while (p != NULL) {
    if (strcmp(p->info->tipo_relacao, tipoRelacaoInput) == 0 &&
        p->info->aniversario.mes == mesInput) {
      printf("nome : %s\n", p->info->nome);
      c = 1;
    }
    p = p->prox;
  }
  if (c == 0)
    printf("nao existe nenhum contato no sistema com essas definicoes");
}
//
Node *alteraNome(Node *lst, char *nome) {
  Node *p = lst;
  while (p != NULL) {
    if (strcmp(p->info->nome, nome) == 0) {
      altera(p);
      return lst;
    }
    p = p->prox;
  }
  printf("ERROR - NAO TEM NOME");
  return lst;
}
Node *alteraCodigo(Node *lst, int codigo) {
  Node *p = lst;
  while (p != NULL) {
    if (p->info->id == codigo) {
      altera(p);
      return lst;
    }
    p = p->prox;
  }
  printf("ERROR - NAO TEM CODIGO");
  return lst;
}
Node *alteraContato(Node *lst) {
  char nomeInput[100];
  printf("digite nome : ");
  scanf(" %[^\n]", nomeInput);
  int resultado = consultaNome(lst, nomeInput);
  if (resultado == 1) {
    printf("Digite codigo: ");
    int codigo;
    scanf("%d", &codigo); // deleto por codigo
    lst = alteraCodigo(lst, codigo);
  } else if (resultado == 0) { // deleto por nome
    lst = alteraNome(lst, nomeInput);
    printf("...alterando");
    return lst;
  }
  return lst; // nao deleto por nao existir nome
}
Node *altera(Node *lst) {
  Node *p = lst;
  char tmp[255];
  printf("informacoes: (para nao alterar digite '-')\n");
  
  printf("nome atual: %s | nome alteracao: ", p->info->nome);
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  
  printf("Apelido: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->apelido, tmp);
  
  printf("Sexo: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) p->info->sexo = tmp[0];
  
  printf("\nendereco:\n-rua: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->rua, tmp);
  
  printf("-numero: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) p->info->numero = atoi(tmp);
  
  printf("-complemento: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  
  printf("-bairro: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  
  printf("-cidade: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  
  printf("-uf: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  
  printf("-cep: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) p->info->cep = atoi(tmp);
  
  printf("telefone residencial: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) p->info->tel_residencia = atoi(tmp);
  
  printf("telefone comercial: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) p->info->tel_comercial = atoi(tmp);
  
  printf("telefone celular: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) p->info->tel_celular = atoi(tmp);
  
  printf("aniversario: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) {
    p->info->aniversario = converteDataNascimento(tmp);
    }
  
  printf("data primeiro encontro: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) {
    p->info->primeiro_encontro = converteDataNascimento(tmp);
    }
  
  printf("local primeiro encontro: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  
  printf("tipo de relacao: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  
  if (strcmp(p->info->tipo_relacao, "T") == 0) {
    printf("local de trabalho : ");
    scanf(" %[^\n]", tmp);
    if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  } else if (strcmp(p->info->tipo_relacao, "C") == 0) {
    printf("local de comercio : ");
    scanf(" %[^\n]", tmp);
    if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  }
  printf("observacoes: ");
  scanf(" %[^\n]", tmp);
  if (strcmp(tmp, "-") != 0) strcpy(p->info->nome, tmp);
  
  return lst;
}
//
Node *excluiNome(Node *lst, char *nome) {
  Node *p = lst;
  while (p != NULL) {
    if (strcmp(p->info->nome, nome) == 0) {
      p->info->excluir = true;
      return lst;
    }
    p = p->prox;
  }
  printf("ERROR - NAO TEM NOME");
  return lst;
}
Node *excluiCodigo(Node *lst, int codigo) {
  Node *p = lst;
  while (p != NULL) {
    if (p->info->id == codigo) {
      p->info->excluir = true;
      return lst;
    }
    p = p->prox;
  }
  printf("ERROR - NAO TEM CODIGO");
  return lst;
}
Node *excluirContato(Node *lst) {
  char nomeInput[100];
  printf("digite nome : ");
  scanf(" %[^\n]", nomeInput);
  int resultado = consultaNome(lst, nomeInput);

  if (resultado == 1) {
    printf("Digite codigo: ");
    int codigo;
    scanf("%d", &codigo); // deleto por codigo
    lst = excluiCodigo(lst, codigo);
  } else if (resultado == 0) { // deleto por nome
    lst = excluiNome(lst, nomeInput);
    printf("...deletado");
    return lst;
  }
  return lst; // nao deleto por nao existir nome
}
//
void display_menu(void) {
  printf("\nMENU:\n");
  printf("1 - Inserir\n");
  printf("2 - Consultar\n");
  printf("3 - Alterar\n");
  printf("4 - Exclusao\n");
  printf("5 - Listar\n");
  printf("6 - Back-up\n");
  printf("7 - Finalizar\n");
  printf("Insira sua opcao : ");
}
void display_menu_2(void) {
  printf("\nCONSULTA\n");
  printf("1 - Pesquisar por nome\n");
  printf("2 - Pesquisar por mes de aniversario\n");
  printf("3 - Pesquisar por tipo de relacao\n");
  printf("4 - Pesquisar por mes de aniversario e tipo de relacao\n");
  printf("Insira sua opcao : ");
}
void printprint(void) {
  printf("\t\tCALENDARIO\n");
  printf("---------------------------\n");
  printf("DOM\tSEG\tTER\tQUA\tQUI\tSEX\tSAB\n");
  printf("---------------------------\n");
  printf("0\t0\t0\t1\t2\t3\t4\n");
  printf("5\t6\t7\t8\t9\t10\t11\n");
  printf("12\t13\t14\t15\t16\t17\t18\n");
  printf("19\t20\t21\t22\t23\t24\t25\n");
  printf("26\t27\t27\t28\t29\t30\t1\n");
  printf("---------------------------\n\n");
}

int retornaCodigo(Node *lst) {
  int maior_codigo = 0; // maior codigo
  Node *p = lst;
  while (p != NULL) {
    if (p->info->id > maior_codigo)
      maior_codigo = p->info->id;
    p = p->prox;
  }
  maior_codigo++;
  return maior_codigo;
}

void converteDataNascimento(char *data, Data * d) {;
  char dia[3],mes[3],ano[5];
  dia[0] = *(data);
  dia[1] = *(data+1);
  dia[2] = '\0';
  //printf("%s\n",dia);
  mes[0] = *(data+3);
  mes[1] = *(data+4);
  mes[2] = '\0';
  //printf("%s\n",mes);
  ano[0] = *(data+6);
  ano[1] = *(data+7);
  ano[2] = *(data+8);
  ano[3] = *(data+9);
  ano[4] = '\0';
  //printf("%s",ano);
  d.dia = atoi(dia);
  d.mes = atoi(mes);
  d.ano = atoi(ano);
  return d;
}