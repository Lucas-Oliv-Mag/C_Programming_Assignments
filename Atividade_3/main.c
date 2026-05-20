
// includes nescessarios
#pragma region Dependencias 

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <stddef.h>
    #include <time.h>

    //#include <locale.h> // Biblioteca necessária para configurar os caracteres especiais de idioma

    #include "sqlite3.h" // Biblioteca do banco de dados SQLite3


#pragma endregion






// strings para manipulação SQL.
#pragma region macros

    sqlite3 *db; // handler do database

    const char *db_name = "logins.db";
        

#pragma endregion





// declaração das funções.
#pragma region protótipos

    bool mk_database(const char* db_str, sqlite3** db_ptr); // Se conecta no banco de dados, ou cria  caso ele não exista.

    bool mk_tables_sql(const char* db_str ,sqlite3** new_db); // cria as tabelas do banco de dados.

    bool mk_insert_sql(sqlite3 ** db_to_insert, uint32_t id, char * name, char * password); // Insere um novo usuario, retorna true se tudo deu certo, e false caso tenha dado errado.

    bool verify_login(sqlite3 **db_ptr, char * user_name, char * password); //Lê o usuario na memoria, retorna true se o login bateu, e false caso tnão.

#pragma endregion







// regras de negócios 
#pragma region inicio

    int main() 
    {

        //setlocale(LC_ALL, "PORTUGUESE"); // muda o idioma para o atual do SO.

        bool db_status = false;

        srand(time(NULL)); // inicializa a seed para o rand().

        do{ 

            db_status = mk_database(db_name, &db);
            
        }while(db_status = false); // inicializa o db de qualquer jeito.

        unsigned int i = 1; // contador de id para os logins, para facilitar a inserção.

        
        printf("\n>> Seja bem vindo ao sistema de login!");
        int exit;


        do{

            printf("\n>> Digite a opcao que deseja: \n - 1. Cadastrar novo usuario\n - 2. Fazer login\n - 3. Sair\n");

            int option = 0;

            scanf("%d", &option);

            exit = -1;
            
            char new_name[100];
            char new_password[100];


            switch (option)
            {
                case 1:
                    
                    printf("\n>> Digite o nome do novo usuario: ");

                    scanf("%s", &new_name);

                    printf("\n>> Digite a senha do novo usuario: ");
                    scanf("%s", &new_password);

                    mk_insert_sql(&db, (unsigned int)rand(), new_name, new_password);

                break;

                case 2:

                    printf("\n>> Digite o nome do usuario: ");

                    scanf("%s", &new_name);
                    
                    printf("\n>> Digite a senha do usuario: ");

                    scanf("%s", &new_password);

                    verify_login(&db, new_name,new_password);


                break;

                case 3:
                    exit = 1;
                break;

                default:
                    printf("\n>> Opcao invalida, por favor, tente novamente.");
                break;
            }

        }while(exit != 1);
        

        printf("\n>> Saindo do sistema... Ate mais!");

        sqlite3_close(db);    //Fecha a conexão com o banco

        system("pause");


      return 1;
    }

#pragma endregion








#pragma region desenvolvimento

    bool mk_database(const char* db_str, sqlite3** db_ptr){

        char * err_msg = NULL;
        char rt = '0';
        int rc = 0;

        rc = sqlite3_open_v2(db_str, db_ptr, SQLITE_OPEN_READWRITE, NULL);

        if (rc == SQLITE_CANTOPEN) {
                
            printf("\n>> O banco de dados não existe...\n");
            printf("\nDeseja criar um novo banco de dados para salvar os logins ? Digite Y caso queira.\n");

            scanf("%c", &rt);

            if(rt == 'Y' || rt == 'y'){

                rc = sqlite3_open(db_str, db_ptr);

                if(rc != SQLITE_OK){ 

                    printf("\n>> Não foi possível criar o banco de dados. Erro (%d): %s\n", rc, sqlite3_errstr(rc));
                    sqlite3_close(*db_ptr);
                    return false;

                }
                else{  

                    return mk_tables_sql(db_str , db_ptr);

                }

                
            }else{ printf("\n >> Tudo bem então..."); return false; }
                
        }else if (rc == SQLITE_OK) {

            printf("\n>> Banco de dados encontrado!\n");
            return true;

        }

    }

    bool mk_insert_sql(sqlite3 ** db_to_insert, uint32_t id, char * name, char * password){

        char * er_msg = NULL;
        int ret = 0;
        char sql_string[300]; // grande o suficiente para caber o nome.

        if(strlen(name) > 100){ printf("\n>> Nome muito grande, por favor, abreviar!"); return false; }

        snprintf(sql_string, sizeof(sql_string), "INSERT INTO Logins (Id, Name, Password) VALUES ( %u, '%s', '%s');", id, name, password);

        ret = sqlite3_exec(*db_to_insert,sql_string, NULL, NULL, &er_msg);

        if (ret != SQLITE_OK) {
            fprintf(stderr, "\n>> Erro ao inserir novo login: %s \n", er_msg);
            sqlite3_free(er_msg);
        }else{
            printf("\n >> Novo usuario %s inserido com sucesso!", name);
            return true;
        }
        return false;
    }

 
    bool verify_login(sqlite3 **db_ptr, char * user_name, char * password){

        if(strlen(user_name) > 100){ printf("\n>> Nome muito grande nem entram no bano de dados!"); return false; }

        char sql[160];
        char **resultados;
        int linhas, colunas;
        char *err_msg = NULL;

        snprintf(sql, sizeof(sql), "SELECT Id, Name, Password FROM Logins WHERE Name = '%s';", user_name);

        // Aloca e preenche a matriz 'resultados' de uma vez só
        int rc = sqlite3_get_table(*db_ptr, sql, &resultados, &linhas, &colunas, &err_msg);
        
        if (rc == SQLITE_OK && linhas > 0) {
            // resultados[0] até resultados[colunas-1] são os CABEÇALHOS
            // resultados[colunas] em diante são os DADOS reais
            printf("ID: %s | Nome: %s\n", resultados[colunas], resultados[colunas + 1]);

            if(strcmp(resultados[colunas + 2], password) == 0){
                printf("\n>> Login bem sucedido! Bem vindo, %s!", user_name);
                sqlite3_free_table(resultados);
                return true;
            }else{
                printf("\n>> Senha incorreta para o usuário %s.", user_name);
                sqlite3_free_table(resultados);
                return false;
            }
        } else {

            printf("Nenhum registro encontrado ou erro: %s\n", err_msg ? err_msg : "Nenhum");
            
            sqlite3_free(err_msg);

            return false;
        }

        // OBRIGATÓRIO: Libera a matriz inteira com uma única chamada
        sqlite3_free_table(resultados);
        return false;
    }

    bool mk_tables_sql(const char* db_str ,sqlite3** new_db){    // cria as tabelas do banco de dados.

        char* err_msg = NULL;
        const char * sql_cr_tables = "CREATE TABLE \"Logins\" (\"Id\" INTEGER NOT NULL UNIQUE,\"Name\" TEXT NOT NULL, \"Password\" TEXT NOT NULL,PRIMARY KEY(\"Id\") ); ";
        
        int rc = sqlite3_exec(*new_db, sql_cr_tables, 0, 0, &err_msg);
        
        if (rc != SQLITE_OK ) {
            fprintf(stderr, "\n >>Erro ao gerar tabelas: %s .", err_msg);
            sqlite3_free(err_msg);
            return false;
        }
        
        printf("\n>> Banco de dados formatado...");

        return true;
    }



#pragma endregion

