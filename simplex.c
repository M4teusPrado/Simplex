#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int   readMatrix(char *path, int **data);
void  aloca(int **p, int tam);
int   qtdVariaveis(char *path);
int   qtdLinhas(char *path);

void  inicializa_matriz(int lin, int col, float m[lin][col]);
void  monta_matriz(int lin, int col, float m[lin][col], int variaveis, int *p);
void  adiciona_folgas(int lin,int col,float m[lin][col]);
void  exibe_matriz(int lin,int col,float m[lin][col]);
void  adiciona_linha_z(int lin, int col, float m[lin][col], int *p, int var);

bool  valores_negativos(int lin, int col, float m[lin][col]);
int   valor_mais_negativo(int lin, int col, float m[lin][col]);
int   busca_linha_pivo(int lin, int col, int coluna_pivo, float m[lin][col]);
void  escalona_coluna(int lin, int col, int linha_pivo, int coluna_pivo, float m[lin][col]);
float valor_multiplicacao(float dividendo, float divisor);

int main ( void )
{
  char file[100] = "exemplo.txt";
  int *m = NULL, i = 0, tam_aloc, variaveis = 0, colunas = 0, linhas = 0, j = 0, restricoes = 0;
  tam_aloc   = readMatrix(file, &m);
  variaveis  = qtdVariaveis(file);
  restricoes = variaveis + 1;
  colunas    = variaveis * 2 + 2;
  linhas     = variaveis + 1;

  float matriz[linhas + 1][colunas];

//Inicializa e modela matriz
   inicializa_matriz(linhas + 1, colunas, matriz);
   monta_matriz(linhas + 1, colunas, matriz, variaveis, m);
   adiciona_folgas(linhas + 1, colunas, matriz);
   adiciona_linha_z(linhas + 1, colunas, matriz, m, variaveis);

   bool x = false;

int coluna_pivo = -1, linha_pivo = -1;
    
    while(i < 2)
    {

    //Passo 1 - retorna índice do valor mais negativo da coluna z
        coluna_pivo = valor_mais_negativo(linhas, colunas, matriz);
        
    //Passo 2 - Encontra o pivo
        linha_pivo = busca_linha_pivo(linhas, colunas, coluna_pivo, matriz);
    //Passo 3 - Escalonar coluna do pivo 
        escalona_coluna(linhas, colunas, linha_pivo, coluna_pivo, matriz);
     i++;
        
    }       

    exibe_matriz(linhas + 1, colunas, matriz);
}

int readMatrix(char *path, int **data)
{
    FILE *file;
    file = fopen(path, "r");
    int aux = 0, tam_aloc = 0;

    if (file == NULL) {
        fprintf(stderr, "error: while trying to open `%s' for reading\n", path);
        return 0; //
    }
    
    while (fscanf(file, "%d ", &aux) == 1) {
      aloca(data, ++tam_aloc);
      *(*data + tam_aloc - 1) = aux;
    }

    fclose(file);
    return tam_aloc;
}

void aloca(int **p, int tam){

  if( (*p = (int *)realloc( *p ,tam * sizeof(int))) == NULL) {
        printf("Estamos sem memoria...");
        exit(1);
    }
}

int qtdVariaveis(char *path)
{
  FILE *file;
  int cont = 0, i = 0;
  char c, *result, linha[100];

  if ((file = fopen(path, "r")) == NULL)
      exit(1); 

  fread (&c, sizeof(char), 1, file);
  result = fgets(linha, 100, file);

  for(i = 0; i < (strlen(linha)); i++)
    if( *(result + i) == ' ')
      cont++;
    
  fclose(file);
  return cont + 1;
}

void inicializa_matriz(int lin, int col, float m[lin][col]) 
{
  int i = 0, j = 0;
  for(i = 0; i < lin; i++)
    for(j = 0; j < col; j++)
      m[i][j] = 0.0;
}

void monta_matriz(int lin, int col, float m[lin][col], int variaveis, int *p)
{
    int  i = 0, j = 0, k = 0;

    for(i = 0; i < lin - 1; i++)
    {
        for(j = 0; j < col; j++)
        {
            if(j < variaveis) 
            {
                m[i][j] = *(p + k + variaveis);
                k++;
            }
            else if(j == col - 1)
            {
                m[i][j] = *(p + k + variaveis);
                k++;
            }
        }
    }
}

void adiciona_folgas(int lin,int col,float m[lin][col]) 
{
    int  i = 0, j = 0;

    for( i = 0; i < lin - 1; i++) 
        for( j = 0; j < col; j++)
            if(j - i == 2)
                m[i][j] = 1.0;
}

void exibe_matriz(int lin,int col,float m[lin][col])
{
    int  i = 0, j = 0;

    for( i = 0; i < lin; i++)
    {
        for( j = 0; j < col; j++) 
        {
            printf("%.2f\t", m[i][j]);
        }
        printf("\n");        
    }
        
}

void adiciona_linha_z(int lin, int col, float m[lin][col], int *p, int var)
{
    int k = 0, j = 0;
    
    while(j < var) 
    {
        if(*(p + k) > 0)
            m[lin - 1][j++] = *(p + k) * -1;
        else
            m[lin - 1][j++] = *(p + k);
        k++;
    }
}

bool  valores_negativos(int lin, int col, float m[lin][col])
{
    for(int j = 0; j < col ; j++) 
        if(m[lin][j] < 0) return true;
    return false;
}

int  valor_mais_negativo(int lin, int col, float m[lin][col]) 
{
    float aux = m[lin][0];
    int indice = -1;

    for( int i = 0; i < col; i++)
    {
        if(aux >= m[lin][i] )
        {   
            indice = i;
            aux = m[lin][i];
        }
    }
    return indice;
}

int  busca_linha_pivo(int lin, int col, int coluna_pivo, float m[lin][col])
{
    int i = 0, aux = 0, flag = 0;
    float menor_resultado = 0.0;

    for(int i = 0; i < lin; i++)
    {
        if(m[i][coluna_pivo] > 0 && flag == 0)  //Primeira interação maior que zero 
        {
            aux = i;
            flag++;
            menor_resultado = m[i][col - 1] / m[i][coluna_pivo];
        }
        else if(m[i][coluna_pivo] > 0 )
        {
            if(menor_resultado > (m[i][col - 1] / m[i][coluna_pivo]))
            {
                menor_resultado = m[i][col - 1] / m[i][coluna_pivo];
                aux = i;
            }
        }
    }

    return aux;
}

void escalona_coluna(int lin, int col, int linha_pivo, int coluna_pivo, float m[lin][col])
{
    
    float x = 0, pivo = m[linha_pivo][coluna_pivo];
    int k = 0, flag = 0, j = 0;

    x = valor_multiplicacao(1,pivo);
    for(j = 0; j < col; j++)
        m[linha_pivo][j] = m[linha_pivo][j] * x;
    
    for(int i = 0; i < lin + 1; i++) {
        for(j = 0; j < col; j++){
            if(j == coluna_pivo && m[i][j] != 0 && i != linha_pivo) {
                if(flag == 0) {
                    x = m[i][j]  *  (-1);
                    flag++;
                }
                     
                for(int k = 0; k < col; k++)
                {
                    //printf("%f + %f * %f  = %f\n",m[i][k] , x , m[linha_pivo][k], (m[i][k] + x * m[linha_pivo][k]));
                    m[i][k] = m[i][k] + x * m[linha_pivo][k];                    
                }   
            }
        }
        
        flag = 0;
    }
}

float valor_multiplicacao(float dividendo, float divisor)
{
    return dividendo/divisor;
}