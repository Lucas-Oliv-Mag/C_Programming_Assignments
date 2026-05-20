#ifndef AULA3_SRC // Include guards por habito mesmo.
#define AULA3_SRC



#pragma region READ ME <

/*
__________________________________________________________________

  Atividade da matéria algoritmos e programação N° 03.

  - Aluno: Lucas Oliveira Magalhães.

  - Curso: Eng. Elétrica 1th - Noite.
  
  - Title:     *** Calculadora de Média de alunos. ***
  - Desc:

        Programa ao qual gerencia uma lista de alunos, onde voce 
        pode realizar algumas operacoes, mas principalmente dar
        notas e ver se o usuario passou ou nao, e claro, calcular a media deles.

        Neste fiz mais simples, apenas implementando algoritimos
        de lista linkada junto com os dados para enxugar o código,
        e uma interface de usuario um pouco mais flexivel
        para as brincadeiras gostosas.

        Features implementadas no código:

    
    * Utilizei o VScode, não faço ideia se o Dev C++ tem suporte
        as diretivas de pragma region, se não tiver, é só ignorar!
    *O dedo coçou para implementar um banco de dados em SQlite ;)

__________________________________________________________________


*/
#pragma endregion





#pragma region INCLUDES <

    #include <stdio.h> // Permite manipular o terminal, para exibir mensagens e ler entradas do usuário.
    #include <stdlib.h> // tem funções de alocação dinâmica de memória, como malloc e free, para criar e gerenciar a lista linkada de alunos.
    #include <stdint.h> // usar tipos de dados inteiros com tamanhos específicos, como uint32_t, para armazenar o RA do aluno de forma eficiente e padronizada.
    #include <stdbool.h> // Para usar tipos booleanos
    #include <stddef.h> // define alguns tipos para noix.

#pragma endregion





#pragma region ESTRUTURAS <


    typedef struct Aluno_t {
        uint32_t RA; // Armazena o RA ficticio do aluno, para simular um cadastro mais realista e servir de ID.
        float Notas[4]; // Armazena as 4 notas do aluno em um array de floats.
        struct Aluno_t * Prox_aluno; // Ponteiro para o próximo aluno na lista linkada.
        char nome[100]; // Armazena o nome do aluno em forma de string.
    }Aluno_t;

    typedef struct Lista_alunos {

        Aluno_t * Primeiro_aluno; // Ponteiro para o primeiro aluno da lista linkada.
        Aluno_t * Ultimo_aluno; // Ponteiro para o último aluno da lista linkada, para facilitar a inserção.
        uint32_t Quantidade_alunos; // Armazena a quantidade de alunos cadastrados na lista.

    }Lista_alunos_t; // Declaração implicita.


 


#pragma endregion





#pragma region PROTÓTIPOS <


    #pragma region funcoes branching

        void Menu_Principal();
        void Menu_Cadastro();            
        void Menu_Edicao();           
        void Menu_Consulta();           
        void Menu_Exclusao();            
        void Menu_Exibir_Situacao();    
        void Menu_Alterar_Nota_Corte();  
        void Menu_Excluir_base();      

    #pragma endregion

    #pragma region funcoes auxiliares

        /* macro para limpar o buffer de teclado do scanf caso o usuario cague com a input */
        //#define SCAN_CLEAR() while(getchar() != '\n')
        #define SCAN_CLEAR() { int c; while ((c = getchar()) != '\n' && c != EOF); }

        Aluno_t * Procurar_aluno(uint32_t RA_alvo); // Preocura um aluno na lista pelo RA e retorna o endereço de memória dele.
        bool Excluir_aluno(uint32_t RA_alvo);

    #pragma endregion

 


#pragma endregion





#pragma region VARs GLOBAIS <

    Lista_alunos_t Lista ={

        .Primeiro_aluno = NULL,  // Inicializa o ponteiro para o primeiro aluno como NULL, indicando que a lista está vazia.
        .Ultimo_aluno = NULL,   //  Indica a mesma coisa.
        .Quantidade_alunos = 0 //  nao preciso comentar 

    }; // Variável global para armazenar a lista de alunos cadastrados.

    Aluno_t * Aluno_atual; // Armazena o endereço do aluno atual.

    float Nota_de_corte = 6.5; // é usada para determinar se um aluno passou ou não, e pode ser alterada.

#pragma endregion



/* TODA A LÓGICA DA APLICAÇÃO AGORA ABAIXO! */
int main() {


    printf("\n\nBem-vindo a calculadora de media de alunos!");
    printf("\nAqui voce pode cadastrar alunos, calcular suas medias e verificar suas situacoes!.");
    printf("\n *OBS:Tudo funciona inteiramente pelo terminal, o tratamentos de erros eh MINIMO,\n entao nao abuse do sistema de graca, pois eh certeza que a aplicacao vai crashar...\n");
   
    unsigned int resposta; // Tive alguns problemas com esta variavel, por algum motivo...

    do{

        Menu_Principal(); // Chama a função do menu principal para iniciar a interação com o usuário.
        printf("\nGostaria de realizar outra operacao? (para SIM, digite 1): ");
        SCAN_CLEAR();
        scanf("%u", &resposta); // Lê a resposta do usuário para decidir se ele quer realizar outra operação ou encerrar o programa.
        system("cls"); // aquela organização báaasica.
            
    }while(resposta == 1); // Se a resposta for diferente de 'N', o programa continua perguntando até que o usuário decida encerrar.

    printf("Encerrando o programa, obrigado pela atencao ...\n");
    return 0;

    // Fim de programa.
}





#pragma region IMPLEMENTAÇÕES <

    void Menu_Principal() {

        uint32_t Cmd; // Armazena o comando digitado pelo usuario.

        Menu_principal_init:
        /* Label para o goto, usado para voltar ao menu principal em caso de comando inválido.
         Se usasse recursão do tipo Menu_principal() dentro do default do switch, poderia causar um estouro de stack,
        então nem adianta tentar meus nobres. */

        Cmd = 0;
        
        // Aqui não precisa de legenda, é quase auto explicativo.

        printf("\n *** Digite o comando que voce quer executar: ***");

        printf("\n1 - Cadastrar um novo aluno.");
        printf("\n2 - Editar notas de um aluno.");
        printf("\n3 - consultar status de um aluno");
        printf("\n4 - Excluir um aluno do cadastro.");
        printf("\n5 - Exibir a situacao de todos os alunos cadastrados.");
        printf("\n6 - Alterar a nota de corte (atual: %.2f).", Nota_de_corte);
        printf("\n0 - Sair do programa.\n\n");


        scanf("%u", &Cmd);

        switch (Cmd) // As vias de fato.
        {
            case 1: Menu_Cadastro();            break;
            case 2: Menu_Edicao();              break;
            case 3: Menu_Consulta();            break;
            case 4: Menu_Exclusao();            break;
            case 5: Menu_Exibir_Situacao();     break;
            case 6: Menu_Alterar_Nota_Corte();  break;
            case 0: exit(0); return;            break;
            default:

                printf("\nComando invalido! Digite um comando válido para continuar.");
                SCAN_CLEAR(); // Limpa o buffer do scanf para evitar problemas com entradas inválidas.
                goto Menu_principal_init; // Volta para o menu principal para o usuário tentar novamente.
                return;
                
            break;

            return;
        } 
    }

    void Menu_Cadastro() {
        // Implementação do menu de cadastro, onde o usuário pode inserir os dados de um novo aluno.

        system("cls"); // aquela organização báaasica.
        printf("\n*** Menu para Cadastrar novo aluno! ***\n");

        Aluno_t * Novo;

        Menu_Cadastro_init:

        Novo = (Aluno_t *)malloc(sizeof(Aluno_t)); // reserva a memoria para o novo.
        if(Novo == NULL) { // Ve se está dando para alocar.
            printf("\n--Erro interno ao alocar memoria para o novo aluno!\n");
            return;
        }


        printf("\nDigite o RA do novo aluno: ");
        if(scanf("%u", &Novo->RA) != 1) { // Verifica se oq foi digitado é aceitavel como RA.
            printf("Entrada inválida para o RA.\n");
            free(Novo);
            return;
        }

        if(Procurar_aluno(Novo->RA) != NULL){

            printf("\n---POR FAVOR, DIGITE UM NOVO RA, QUE NAO TENHA SIDO INSERIDO!");
            SCAN_CLEAR();
            free(Novo);
            goto Menu_Cadastro_init;

        }

        printf("\nAgora Digite o nome dele:");
        printf("\n(CUIDADO:Apenas 100 caracteres, e tambeem aperte BACKSPACE (vulgo <-) ou o programa explode):\n\n ->  ");

        SCAN_CLEAR(); // Limpa o buffer do scanf para evitar problemas com entradas inválidas
        if(scanf(" %99[^\n]", Novo->nome) != 1) { // Lê o nome do aluno, permitindo espaços.
            printf("\n\n -- Entrada invalida para o nome do aluno!");
            free(Novo);
            return;
        }

        if(Lista.Primeiro_aluno == NULL && Lista.Ultimo_aluno == NULL) { // Se a lista estiver vazia, o novo aluno se torna o primeiro e o último da lista.
            
            Lista.Primeiro_aluno = Novo;
            Lista.Ultimo_aluno = Novo;

        } else { // Caso contrário, o novo aluno é adicionado ao final da lista.
            
            Lista.Ultimo_aluno->Prox_aluno = Novo; // O próximo do último aluno atual aponta para o novo aluno.
            Lista.Ultimo_aluno = Novo; // Aqui tambem para funcionar.

        }
        
        Lista.Quantidade_alunos++; // Incrementa a quantidade de alunos cadastrados na lista.


        // Limpa a memoria para oficializar.
        Novo->Prox_aluno = NULL; // O próximo do novo aluno é NULL, pois ele será o último da lista se tudo der certo
      
        for(uint8_t i = 0; i < 4; i++) { // Inicializa as notas do novo aluno como -1 que indica que ainda não foram inseridas, para evitar lixo de memória.
            Novo->Notas[i] = -1.0;
        }

        printf("\ninsira as notas deles no menu de editar notas!");
  
        return;
    }

    void Menu_Edicao(){

        system("cls"); 
        printf("\n*** Menu para Editar notas ***\n");

        Menu_Edicao_init:

        printf("\nDigite o RA do aluno que voce quer editar as notas: ");
        printf("\n -> ");

        uint32_t RA_alvo;
        if(scanf("%u", &RA_alvo) != 1) { // Verifica se a entrada para o RA é válida.
            printf("\n --Entrada inválida para o RA, digite novamente.\n");
            goto Menu_Edicao_init; // Volta para o menu de edição para o usuário tentar novamente.
        }

        if(Lista.Quantidade_alunos < 1 || Lista.Primeiro_aluno == NULL){ // Verifica se há alunos cadastrados na lista, se não houver, exibe uma mensagem e retorna ao menu principal.
            printf("\n--Nenhum aluno cadastrado ainda nobreza!\n");
            return;
        }
       
    
        Aluno_t * Cursor = Lista.Primeiro_aluno; // Variável auxiliar para percorrer a lista de alunos.
        
        printf("\n Preocurando pelo aluno com o RA: %u \n",RA_alvo);
    
        for(uint32_t i = 0; i < Lista.Quantidade_alunos; i++){

            
            if(Cursor->RA == RA_alvo){

                printf("\n * Aluno encontrado: *");
                printf("\nNome: %s",Cursor->nome);

                for(uint8_t K = 0; K < 4; K++){

                    if(Cursor->Notas[K] == -1.0){

                        printf("\n -- O aluno possui notas nao registradas, marcadas como -1.0");

                        break;

                    }
                }

                printf("\n -> Notas por semestres atuais: 1th : %2.f, 2th : %2.f, 3th : %2.f, 4th : %2.f \n",Cursor->Notas[0],Cursor->Notas[1],Cursor->Notas[2],Cursor->Notas[3]); 

                float Nova_nota = -1.0;
                
                for(uint8_t L = 0; L < 4; L++){

                    printf("\nDigite a nova nota do %u th Semestre:\n -> ",(L + 1));
                  
                    if(scanf("%f",&Nova_nota) != 1){

                        printf("\n--POR FAVOR, DIGITE UM VALOR NUMERICO VALIDO PARA A NOTA!");
                        L--; // Isso faz com que o loop repita o mesmo passo.
                    }else if(Nova_nota < 0 || Nova_nota > 10.0){

                        printf("\n--POR FAVOR, DIGITE UM VALOR ENTRE 0 ~ 10.0");
                        L--;

                    }else{

                        Cursor->Notas[L] = Nova_nota; // Registra a nova efetivamente.

                    }

                }
            
                
                break; // saimos do loop pois achamos o bonito
            }

            Cursor = Cursor->Prox_aluno;

        }


        printf("\n\n * Notas notas cadastradas com sucesso!");
    }          
   
    void Menu_Consulta(){

        system("cls"); 
        printf("\n*** Menu para Consultar status de um Aluno ***\n");


        uint32_t RA_alvo;
        if(Lista.Quantidade_alunos < 1U || Lista.Primeiro_aluno == NULL){ // Verifica se há alunos cadastrados na lista, se não houver, exibe uma mensagem e retorna ao menu principal.
          
            printf("\n--Nenhum aluno cadastrado ainda nobreza!\n-- Volte apertando qualquer tecla.");
            scanf("%u",&RA_alvo);//
            SCAN_CLEAR(); 
            return;

        }

        
        Menu_Consulta_init:
        printf("\nDigite o RA do aluno que voce quer editar as notas: ");
        printf("\n -> ");

        if(scanf("%u", &RA_alvo) != 1) { // Verifica se a entrada para o RA é válida.
            printf("\n --Entrada inválida para o RA, digite novamente.\n");
            goto Menu_Consulta_init; // Volta para o menu de edição para o usuário tentar novamente.
        }


        Aluno_t * Aluno_alvo = Procurar_aluno(RA_alvo);

        printf("\n Preocurando pelo aluno com o RA: %u \n",RA_alvo);

        printf("\n end:%d",Aluno_alvo);

        if(Aluno_alvo == NULL){

            printf("\n---ERRO AO PREOCURAR PELO ALUNO, VERIFIQUE O RA DIGITADO!");
            SCAN_CLEAR();
            goto Menu_Consulta_init;

        }else{

            printf("\n Aluno encontrado, nome: %s", Aluno_alvo->nome);
            printf("\n -> Notas por semestres atuais: 1th : %2.f, 2th : %2.f, 3th : %2.f, 4th : %2.f \n",Aluno_alvo->Notas[0],Aluno_alvo->Notas[1],Aluno_alvo->Notas[2],Aluno_alvo->Notas[3]); 

        }
        
        float Media = 0; // Acumulador para armazenar a media

        for(uint8_t j = 0; j < 4; j++){ // Calculo da media.

            Media += Aluno_alvo->Notas[j];

        }

        Media /= 4; 

        printf("\nSituacao atual - MEDIA FINAL: %2.f, nota minima para passar: %2.f, ",Media, Nota_de_corte);
        if(Media  >= Nota_de_corte){

            printf("APROVADO!\n\n");

        }else{

            printf("NAO APROVADO!\n\n");
        }
       

    }        
    
    void Menu_Exclusao(){

        system("cls");
        printf("\n *** Menu para Excluir um aluno ***");

        Menu_exclusao_init:
        printf("\n Qual o RA do aluno que sera excluido?\n -> ");
        uint32_t RA_alvo = 0;

        if(scanf("%u",&RA_alvo) != 1){

            printf("\n---POR FAVOR DIGITAR UM RA VALIDO!");
            SCAN_CLEAR();
            goto Menu_exclusao_init;

        }

        Aluno_t * ExAluno = Procurar_aluno(RA_alvo);
        
        if(ExAluno == NULL){ 

            printf("\n---NAO FOI POSSIVEL ENCONTRAR-LO, APERTE QUALQUER TECLA PARA VOLTAR!");
            scanf("%u",&RA_alvo);
            SCAN_CLEAR();
            return;

        }else{

            if(Excluir_aluno(RA_alvo) == true){

                printf("\n * EXCLUSAO FEITA COM SUCESSO!");
                return;

            }else{

                printf("\n * NAO FOI POSSIVEL EXCLUIR O ALUNO!");
                return;

            }

        }
    
    

    }  

    void Menu_Exibir_Situacao(){


        system("cls");
        printf("\n*** Situacao de todos os alunos cadastrados! ***\n");

        if(Lista.Primeiro_aluno == NULL) { // Verifica se a lista está vazia, caso esteja, exibe uma mensagem e retorna ao menu principal.
            printf("\n--Nenhum aluno cadastrado ainda meu nobre!\n");
            return;
        }

        Aluno_t * Cursor = Lista.Primeiro_aluno; // Variável auxiliar para percorrer a lista de alunos.
        
        printf("\nQuantidade de alunos cadastrados: %d", Lista.Quantidade_alunos);
    
        for(uint32_t i = 0; i < Lista.Quantidade_alunos; i++){

            printf("\n - %d, ", Cursor->RA);
            printf("%s, ", Cursor->nome);
            //printf("Notas: [%.2f, %.2f, %.2f, %.2f], ", Cursor->Notas[0], Cursor->Notas[1], Cursor->Notas[2], Cursor->Notas[3]);

            for(uint8_t j = 0; j < 4; j++) { // Verifica se as notas foram inseridas, se alguma for -1, significa que ainda não foram inseridas, e o programa exibe uma mensagem indicando isso.
              
                if(Cursor->Notas[j] == -1.0) {
                    printf("N/A"); // Exibe "N/A" para indicar que as notas ainda não foram inseridas.
                }else{
                    printf("%.2f", Cursor->Notas[j]); // Exibe a nota formatada com 2 casas decimais.
                }

                printf(", "); // Apenas para organizar a exibição das notas.
            }

            if(Cursor->Prox_aluno != NULL) {
                Cursor = Cursor->Prox_aluno; // Avança para o próximo aluno na lista.
            }
        }

        return;
    }

    void Menu_Alterar_Nota_Corte(){

        system("cls");
        printf("\n*** Menu para alterar nota de corte ***\n");
        printf("\n -> NOTA ATUAL: %2.f",Nota_de_corte);
        printf("\n\n Qual seria a nova Nota de corte ?\n -> ");
        
        float nova_nota = -1.0;

        Alterar_nota_corte:

        if(scanf("%f",&nova_nota) != 1){

            printf("\n--POR FAVOR DIGITAR UM NUMERO VALIDO!\n -> ");
            SCAN_CLEAR();
            goto Alterar_nota_corte;

        }

        if(nova_nota < 0.1 || nova_nota > 10.0){

            printf("\n-- POR FAVOR, DIGITAR UM VALOR ENTRE 0 ~ 10.0\n -> ");
            SCAN_CLEAR();
            goto Alterar_nota_corte;

        }

        Nota_de_corte = nova_nota;

        printf("\n\n Sucesso ! Nova nota de corte a todos os alunos: %2.f", Nota_de_corte);

        return;
    }  
 
    
    Aluno_t * Procurar_aluno(uint32_t RA_alvo){

        Aluno_t * Cursor = Lista.Primeiro_aluno;

        if(Cursor == NULL || Lista.Quantidade_alunos == 0){ return NULL;} // Verifica se a lista esta vazia.

        for(uint32_t i = 0; i < Lista.Quantidade_alunos; i++){
            
            if(Cursor->RA == RA_alvo){

                return Cursor;
                
            }else{

                Cursor = Cursor->Prox_aluno; // Avança para o próximo aluno.

            }
        }   

        return NULL; // Não achou.
    }

    bool Excluir_aluno(uint32_t RA_alvo){

        if(Lista.Quantidade_alunos < 1U || Lista.Primeiro_aluno == NULL){

            return false; // Lista vazia
        }

        Aluno_t * Cursor = Lista.Primeiro_aluno;
        Aluno_t * Backup = NULL; // Armazenara o aluno anterior ao alvo para emendar a lista depois.
        Aluno_t * Prox   = NULL; // Guarda o proximo aluno, depois do alvo, para remendar depois no Backup

        // validar se eh o primeiro no depois hein
        if(Cursor->RA == RA_alvo){ // quer dizer que o primeiro elemento que deve ser excluido.

            if(Cursor->Prox_aluno == NULL){

                Lista.Primeiro_aluno = NULL;

            }else{

                Lista.Primeiro_aluno = Cursor->Prox_aluno; // remenda a lista, agora o primeiro elemento eh o segundo da lista.
            }

            free(Cursor);
            Lista.Quantidade_alunos--;
            return true;
        }
        for (uint32_t i = 1; i < Lista.Quantidade_alunos; i++)// ja comeca no 1, pois ja varremos o primeiro no
        {
            
            if(Cursor->Prox_aluno->RA == RA_alvo){

                Backup = Cursor; // Aponta para um no antes
                Cursor = Cursor->Prox_aluno; // aponta para o no que sera excluido

                if(Cursor->Prox_aluno != NULL){ 

                    Prox = Cursor->Prox_aluno; // aponta para o no posterior que sera emendado.

                }else{
                 
                    Prox = NULL; // Fim de linha meu bem!
                }

                Backup->Prox_aluno = Prox;

                Lista.Quantidade_alunos--;

                free(Cursor); // Libera a memoria alocada.

                return true; // lista acabou e nao se achou o aluno!


            }else if(Cursor->Prox_aluno != NULL){

                Cursor = Cursor->Prox_aluno; // Avança na lista linkada.

            }else{

                return false; // lista acabou e nao se achou o aluno!
            }

        }
        
        return false; // lista acabou e nao se achou o aluno!
    }


#pragma endregion










#endif // fim do include guard.