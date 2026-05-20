
#pragma region README
/*
    Olá Professor, Prazer, sou o Lucas Oliveira, do 1° Semestre de Eng. Elétrica e
    venho te entregar esta atividade solicitada no dia 16/02/26, ao qual o senhor
    solicitou um algoritimo que calculasse a média de valores,
    como eu já estava estudando afundo C, resolvi utilizar meu acervo, assim
    demorou um pouco para te entregar.

    ---

     Desde já agradeço a atenção, e enfatizo a minha admiração pela sua disciplina,
    atualmente sou Tec. Mecatrônico, e venho me dedicando a uma transição de carreira
    para Dev. de sistemas embarcados, tenho e venho tendo algumas experiências 
    com a linguagem C em low-level/bare-metal.

    ---
     Infelizmente me ausentarei dia 02/03, que seria a primeira aula no Lab de TI, 
    por questões interpessoais.
    ____________________________________________________________________________________
*/
#pragma endregion

#pragma region INCLUDES

    #include <stdio.h>  // Para os Prints e Scans da vida.
    #include <stddef.h> // Para os assets de NULL.
    #include <stdint.h> // Tipos de inteiros.
    #include <stdlib.h> // Manipular memoria com malloc e free.
    #include <stdbool.h> // Para retorno de funções.

#pragma endregion

#pragma region  META PROGRAMACAO

    // Protótipos das funções que calculam a media.
    int          int_Avarage_array   (unsigned int Lenght, int * Array);
    float        float_Avarage_array (unsigned int Lenght, float * Array);
    unsigned int uint_Avarage_array  (unsigned int Lenght, unsigned int* Array);
    double       double_Avarage_array(unsigned int Lenght, double * Array);

    // Macro auxiliar, basicamente retira o primeiro argumento de uma expressão com varios argumentos.
    #define GET_FIRST(first, ...) first

    /* Macro de dispatch, o _Generic reconhece qual o tipo dos dados, e então encaminha para a função correta, e então retorna o valor */
    #define Avarage(...) _Generic((GET_FIRST(__VA_ARGS__)), \
        int:          int_Avarage_array(sizeof((int[]){__VA_ARGS__})/sizeof(int), (int[]){__VA_ARGS__}), \
        unsigned int: uint_Avarage_array(sizeof((unsigned int[]){__VA_ARGS__})/sizeof(unsigned int), (unsigned int[]){__VA_ARGS__}), \
        float:        float_Avarage_array(sizeof((float[]){__VA_ARGS__})/sizeof(float), (float[]){__VA_ARGS__}), \
        double:       double_Avarage_array(sizeof((double[]){__VA_ARGS__})/sizeof(double), (double[]){__VA_ARGS__}) \
    )

    /* Permite a reutilizacao da macro para calcular a media de arrays em tempo de execução.*/
    #define AvarageArray(ptr, len) _Generic((ptr), \
        int*:          int_Avarage_array(len, ptr), \
        float*:        float_Avarage_array(len, ptr),\
        double*:       double_Avarage_array(len, ptr) \
    )

    // Imprimie no consolearrays prontos, você passa o tamanho. 
    #define PrintArray(len, arr) _Generic((arr), \
        int*:   int_print(len, arr),             \
        float*: float_print(len, arr)            \
    )

    /* macro para limpar o buffer do scanf caso o usuario cague com o programa */
    #define SCAN_CLEAR() while(getchar() != '\n')

    // Protótipo das perguntas, se não o código não seria legivel.
    bool PERGUNTA1();
    unsigned int PERGUNTA2();
    unsigned int PERGUNTA3();


    typedef enum Q3_Resposta{Error = 0,Inteiro = 1,Uinteiro = 2,PontoFlutuante = 3,PontoFlutuanteDuplo = 4}Data_t;


    #define RETRYS_SETPOINT 3


#pragma endregion


#pragma region EXEMPLO AULA1

int main(){
   
    struct{ // Armazenara as respostas e algumas configurações.

        unsigned int lenght;
        
        unsigned int Offset;

        Data_t Type;

        char * Cmd;

        union{
            int IN;
            unsigned int UI;
            float  FT;
            double DB; 
        }Result;

    }Resposta;
    
    printf("INICIO DO PROGRAMA - AULA #01 - EXERCICIO");

    int IMedia = Avarage(1,2,3,4,5,6,7,8,9,10);  // Exemplo com sintaxe bonita ;)

    printf("\n\rInit> Valor de media de 1 a 10 eh: %d",IMedia);

    float FMedia = Avarage(1.32f, 12.4f, 5.98f, 6.0f); // Mesma sintaxe, dados de tipos diferentes.

    printf("\n\r Valor de media de 1.32, 12.4, 5.98 e 6.0 eh: %f",FMedia);

    // Modo interativo, pois o programa é sociavel.


    INICIO_DAS_PERGUNTAS:


    if(!PERGUNTA1()){ goto FIM_DO_PROGRAMA;}

    Resposta.lenght = PERGUNTA2();
    if(Resposta.lenght == 0){ printf("\n\rTamanho do conjunto invalido!"); goto FIM_DO_PROGRAMA;}

    Resposta.Type = PERGUNTA3();

    
    switch (Resposta.Type)
    {

    case Inteiro:              Resposta.Offset = sizeof(int);            Resposta.Cmd = "%d";         break;
    case Uinteiro:             Resposta.Offset = sizeof(unsigned int);   Resposta.Cmd = "%u";         break;
    case PontoFlutuante:       Resposta.Offset = sizeof(float);          Resposta.Cmd = "%f";         break;
    case PontoFlutuanteDuplo:  Resposta.Offset = sizeof(double);         Resposta.Cmd = "%lf";        break;
    default:
        printf("\n\rTipo de dado nao reconhecido!");
        goto FIM_DO_PROGRAMA;
    }

    void * Ptr_buffer = malloc(Resposta.Offset * Resposta.lenght); // Aloca espaco para o buffer.
    if(Ptr_buffer == NULL){ printf("Erro na alocacao do buffer"); goto FIM_DO_PROGRAMA;} // Asset de memoria.

    for(unsigned int index = 0; index < Resposta.lenght; index ++){// Recolhe o conjunto que o usuario recolher.
        
        scanf(Resposta.Cmd, Ptr_buffer + (Resposta.Offset * index)); // Calcula onde salvar na memoria.

    }

    printf("\n\rResultado da media: ");
    switch(Resposta.Type)   // Dispatcher par acalcular a media
    {
        case Inteiro:             

            Resposta.Result.IN = int_Avarage_array(Resposta.lenght,(int*)Ptr_buffer); 
            printf(Resposta.Cmd,Resposta.Result.IN);                  
        
        break;
        case Uinteiro:            
            
           Resposta.Result.UI  =   uint_Avarage_array(Resposta.lenght,  (unsigned int*)Ptr_buffer);    
           printf(Resposta.Cmd,Resposta.Result.UI);                  
        
        
        break;
        case PontoFlutuante:      
            
            Resposta.Result.FT =  float_Avarage_array(Resposta.lenght, (float*)Ptr_buffer);    
            printf(Resposta.Cmd,Resposta.Result.FT);                  
                
        
        break;
        case PontoFlutuanteDuplo:
        
            Resposta.Result.DB =   double_Avarage_array(Resposta.lenght, (double *)Ptr_buffer);    
            printf(Resposta.Cmd,Resposta.Result.DB);                  
   
                    
        break;

    }

    free(Ptr_buffer); // Libera o buffer.

    goto INICIO_DAS_PERGUNTAS;
    
    FIM_DO_PROGRAMA:

        printf("\n\r Saindo...\n");
        system("pause");
        exit(1);

    return 1;
}

#pragma endregion




#pragma region ESCOPO FUNCOES





    #pragma region func medias
    // FUNÇÕES DE CALCULO DAS MEDIAS.
    /*
    *  @brief Retorna a media do array passado como argumento.
    *  @param Lenght Tamanho do buffer em posicoes.
    *  @param Array Ponteiro para o array.
    * 
    * @return valor de media.
    */
    int int_Avarage_array(unsigned int Lenght, int * Array)
    {
        if(Array == NULL || Lenght == 0){return 0;} // Asset, impede de executar lixo.

        int Acumulator = 0U;

        for(uint32_t i = 0U; i < Lenght; i++){

            Acumulator += Array[i];

        }

        Acumulator /= Lenght;

        return Acumulator;
    }


    /*
    *  @brief Retorna a media do array passado como argumento.
    *  @param Lenght Tamanho do buffer em posicoes.
    *  @param Array Ponteiro para o array.
    * 
    * @return valor de media.
    */
    float float_Avarage_array(unsigned int Lenght, float * Array){

        if(Array == NULL || Lenght == 0){return 0;} // Asset, impede de executar lixo.

        float Acumulator = 0U;
        
        for(uint32_t i = 0U; i < Lenght; i++){

            Acumulator += Array[i];

        }

        Acumulator /= Lenght;

        return Acumulator;
    }


    /*
    *  @brief Retorna a media do array passado como argumento.
    *  @param Lenght Tamanho do buffer em posicoes.
    *  @param Array Ponteiro para o array.
    * 
    * @return valor de media.
    */
    unsigned int uint_Avarage_array(unsigned int Lenght, unsigned int * Array)
    {

        if(Array == NULL || Lenght == 0){return 0;} // Asset, impede de executar lixo.

        unsigned int Acumulator = 0U;
        
        for(uint32_t i = 0U; i < Lenght; i++){

            Acumulator += Array[i];

        }

        Acumulator /= Lenght;

        return Acumulator;
    }


    /*
    *  @brief Retorna a media do array passado como argumento.
    *  @param Lenght Tamanho do buffer em posicoes.
    *  @param Array Ponteiro para o array.
    * 
    * @return valor de media.
    */
    double double_Avarage_array(unsigned int Lenght, double * Array)
    {

        if(Array == NULL || Lenght == 0){return 0;} // Asset, impede de executar lixo.

        double Acumulator = 0U;
        
        for(uint32_t i = 0U; i < Lenght; i++){

            Acumulator += Array[i];

        }

        Acumulator /= Lenght;

        return Acumulator;
    }



    // FUNÇÔES PARA IMPRIMIR NO CONSOLE



    // ESCOPO DAS PERGUNTAS

    #pragma endregion 

    #pragma region perguntas
   bool PERGUNTA1(){

        char opcao;
        bool Error_flag;

        do {
            Error_flag = false;

            printf("\n\r Voce gostaria de calcular a media de algum outro conjunto numerico? (Y/N)");
            scanf(" %c", &opcao);

            if (opcao == 'Y' || opcao == 'y') { 

                return true;
                
            } else if (opcao == 'N' || opcao == 'n') {

                return false;

            } else {
                printf("Opcao invalida! Digite Y ou N.\n");
                Error_flag = true;
            }
        } while (Error_flag != false);

    }



    unsigned int PERGUNTA2(){

        unsigned int Lenght;
        
        printf("\n\r Qual seria o tamanho do conjunto numerico?");

        if(scanf(" %u", &Lenght) != 1){

            printf("\n\rEi, isto nao eh um numero valido!\n");
        
            // LIMPEZA DE BUFFER: Remove a letra que ficou presa
            SCAN_CLEAR();
            return false;

        } else {
            printf("\n\r - >Tamanho do conjunto: %u\n", Lenght);
            return Lenght;
        }



    }


    unsigned int PERGUNTA3(){

        char opcao;
        void * P_buffer;

        int InterCount = 0;

        do{    
       
            printf("\n\r Qual o tipo do conjunto numerico? \n\r 1. Inteiro \n\r 2. Inteiro sem sinal \n\r 3. Ponto flutuante \n\r 4. Ponto flutuante duplo");
            scanf(" %c", &opcao);

            opcao -= '0'; // Extrai valor decimal.
            
            if(opcao > 4){

                printf("\n\rPor favor, selecione um numero entre 1 ~ 4!");
                InterCount ++;    

            }else{
                
                printf("\n\r - > \n\r"); // Apenas para embelezar.
                return opcao;
            }
          
        }while(InterCount < RETRYS_SETPOINT);

        return 0;

    }

    #pragma endregion // fim das perguntas



#pragma endregion // fim do escopo das funcoes.