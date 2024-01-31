#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int Cpf;
    char Nome[50];
    char Sobrenome[50];
    char email[50];
    int telefone;
} hospede;

void cadastro(char arq[]);
void Remover(char arq[], int Cpfy);
int BuscarPosicaoLivre(FILE *file);
hospede *Buscar(char arq[], int Cpfy);
void AtualizarCadastro(char arq[]);
void Imprimir(char arq[]);
void GerarRelatorio(char arq[], char arqt[]);

int main()
{
    char arquivot[] = "relatorio.txt";
    char arquivo[] = "hospedes.dat";
    int op, Cpfx;
    FILE *file = fopen(arquivo, "ab+"); // Abre o arquivo em modo de leitura e escrita, criando-o se não existir

    if (file == NULL)
    {
        printf("ERRO! Não foi possível abrir/criar o arquivo.\n");
        return 1; // Encerra o programa com código de erro
    }

    fclose(file); // Fecha o arquivo imediatamente após verificar a existência

    do
    {
        // Opções de cadastro.
        printf("// ----- // ----- // CADASTRO HÓSPEDE // ----- // ----- // \n");
        printf("[1] - Buscar por cadastro \n");
        printf("[2] - Cadastrar novo \n");
        printf("[3] - Atualizar cadastro \n");
        printf("[4] - Remover cadastro \n");
        printf("[5] - Imprimir todos cadastrados \n");
        printf("[6] - Gerar relatório \n");
        printf("[9] - Finalizar \n");
        printf("Entre com uma opção: \n");

        scanf("%d", &op);

        switch (op)
        {
        case 1:
            printf("Informe seu CPF para busca: ");
            scanf("%d", &Cpfx);
            hospede *resultado = Buscar(arquivo, Cpfx);
            if (resultado != NULL)
            {
                printf("CPF encontrado: %d\n", resultado->Cpf);
                free(resultado);
            }
            else
            {
                printf("CPF não encontrado.\n");
            }
            break;
        case 2:
            cadastro(arquivo);
            break;
        case 3:
            AtualizarCadastro(arquivo);
            break;
        case 4:
            printf("Informe seu CPF para remoção: ");
            scanf("%d", &Cpfx);
            Remover(arquivo, Cpfx);
            break;
        case 5:
            Imprimir(arquivo);
            break;
        case 6:
            GerarRelatorio(arquivo, arquivot);
            break;
        case 9:
            return 0;
            break;
        default:
            printf("Opção inválida.\n");
            break;
        }

    } while (op != 0);

    return 0;
}
hospede *Buscar(char arq[], int Cpfy)
{
    FILE *file = fopen(arq, "rb");
    hospede *hc = malloc(sizeof(hospede)); // Alocar memória para a estrutura encontrada
    int encontrado = 0;

    if (file && hc)
    {
        while (fread(hc, sizeof(hospede), 1, file))
        {
            if (hc->Cpf == Cpfy)
            {
                encontrado = 1;
                break;
            }
        }

        fclose(file);

        if (encontrado)
        {
            // Se encontrado, não precisa alocar uma nova estrutura, basta retornar o ponteiro atual
            return hc;
        }
        else
        {
            free(hc); // Libera a memória alocada se não encontrado
            return NULL;
        }
    }
    else
    {
        if (hc)
        {
            free(hc); // Libera a memória alocada em caso de erro
        }
        return NULL;
    }
}

void cadastro(char arq[])
{
    FILE *file = fopen(arq, "rb+");
    hospede h;

    if (file)
    {
        int posicaoLivre = BuscarPosicaoLivre(file);

        if (posicaoLivre != -1)
        {
            fseek(file, posicaoLivre * sizeof(hospede), SEEK_SET);

            fread(&h, sizeof(hospede), 1, file);

            if (h.Cpf == 0)
            {
                printf("CPF: \n");
                scanf("%d", &h.Cpf);

                hospede *existente = Buscar(arq, h.Cpf);

                if (existente == NULL)
                {
                    printf("Nome: \n");
                    scanf("%s", h.Nome);
                    printf("Sobrenome: \n");
                    scanf("%s", h.Sobrenome);
                    printf("E-mail: \n");
                    scanf("%s", h.email);
                    printf("Telefone: \n");
                    scanf("%d", &h.telefone);

                    fseek(file, posicaoLivre * sizeof(hospede), SEEK_SET);

                    fwrite(&h, sizeof(hospede), 1, file);
                    fflush(file);

                    fclose(file);
                    return;
                }
                else
                {
                    printf("ERRO! CPF já cadastrado.\n");
                }
            }
        }
        else
        {
            fseek(file, 0, SEEK_END);

            printf("CPF: \n");
            scanf("%d", &h.Cpf);

            hospede *existente = Buscar(arq, h.Cpf);

            if (existente == NULL || existente->Cpf == 0)
            {
                printf("Nome: \n");
                scanf("%s", h.Nome);
                printf("Sobrenome: \n");
                scanf("%s", h.Sobrenome);
                printf("E-mail: \n");
                scanf("%s", h.email);
                printf("Telefone: \n");
                scanf("%d", &h.telefone);

                fseek(file, 0, SEEK_END);
                fwrite(&h, sizeof(hospede), 1, file);
                fflush(file);
                fclose(file);
                return;
            }
            else
            {
                printf("ERRO! CPF já cadastrado.\n");
            }
        }

        fclose(file);
    }
    else
    {
        printf("ERRO! Não foi possível abrir o arquivo.\n");
    }
}

void Remover(char arq[], int Cpfy)
{
    FILE *file = fopen(arq, "rb+");
    hospede hc;

    if (file)
    {
        while (fread(&hc, sizeof(hospede), 1, file))
        {
            if (hc.Cpf == Cpfy)
            {
                fseek(file, -sizeof(hospede), SEEK_CUR);
                hc.Cpf = 0;
                fwrite(&hc, sizeof(hospede), 1, file);
                fflush(file); // Certifique-se de que os dados são gravados no arquivo
                printf("Cpf removido \n");
                break;
            }
        }
        printf("Esse CPF nao existe para remoção \n");
        fclose(file);
    }
    else
    {
        printf("ERRO! Não foi possível abrir o arquivo.\n");
    }
}
int BuscarPosicaoLivre(FILE *file)
{
    fseek(file, 0, SEEK_SET);

    hospede h;
    int posicao = 0;

    while (fread(&h, sizeof(hospede), 1, file))
    {
        if (h.Cpf == 0)
        {
            return posicao;
        }

        posicao++;
    }

    return -1;
}

void AtualizarCadastro(char arq[])
{
    FILE *file = fopen(arq, "rb+");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int cpfk, op;
    hospede hx;

    printf("Digite CPF para busca: ");
    scanf("%d", &cpfk);

    while (fread(&hx, sizeof(hospede), 1, file))
    {
        if (hx.Cpf == cpfk)
        {
            printf("CPF: %d\n", hx.Cpf);
            printf("Nome: %s\n", hx.Nome);
            printf("Sobrenome: %s\n", hx.Sobrenome);
            printf("E-mail: %s\n", hx.email);
            printf("Telefone: %d\n", hx.telefone);

            printf("Para CONFIRMAR as informações, digite 1\n");
            printf("Para ATUALIZAR as informações, digite 2\n");
            scanf("%d", &op);

            switch (op)
            {
            case 1:
                fclose(file);
                return;
            case 2:
                printf("Digite os novos valores:\n");
                printf("CPF: ");
                scanf("%d", &hx.Cpf);
                printf("Nome: ");
                scanf("%s", hx.Nome);
                printf("Sobrenome: ");
                scanf("%s", hx.Sobrenome);
                printf("E-mail: ");
                scanf("%s", hx.email);
                printf("Telefone: ");
                scanf("%d", &hx.telefone);

                // Move o ponteiro de volta para a posição do registro
                fseek(file, -sizeof(hospede), SEEK_CUR);

                // Escreve o registro atualizado de volta no arquivo
                fwrite(&hx, sizeof(hospede), 1, file);

                printf("Cadastro atualizado com sucesso.\n");
                fclose(file);
                return;
            default:
                printf("Opção inválida.\n");
                break;
            }
        }
    }

    printf("Não há cadastro com esse CPF nos registros.\n");
    fclose(file);
}
void Imprimir(char arq[])
{
    FILE *file = fopen(arq, "rb");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int cpfk;
    hospede hx;

    printf("Digite CPF para busca: ");
    scanf("%d", &cpfk);

    while (fread(&hx, sizeof(hospede), 1, file))
    {
        if (hx.Cpf == cpfk)
        {
            printf("CPF: %d\n", hx.Cpf);
            printf("Nome: %s\n", hx.Nome);
            printf("Sobrenome: %s\n", hx.Sobrenome);
            printf("E-mail: %s\n", hx.email);
            printf("Telefone: %d\n", hx.telefone);

            fclose(file);
            return;
        }
    }

    printf("Não há cadastro com esse CPF nos registros.\n");
    fclose(file);
}
void GerarRelatorio(char arq[], char arqt[])
{
    FILE *entrada = fopen(arq, "rb");
    FILE *saida = fopen(arqt, "w");

    if (entrada == NULL || saida == NULL)
    {
        perror("Erro ao abrir os arquivos");
        return;
    }

    hospede hx;

    fprintf(saida, "Relatório de Hóspedes:\n\n");

    while (fread(&hx, sizeof(hospede), 1, entrada))
    {
        fprintf(saida, "CPF: %d\n", hx.Cpf);
        fprintf(saida, "Nome: %s\n", hx.Nome);
        fprintf(saida, "Sobrenome: %s\n", hx.Sobrenome);
        fprintf(saida, "E-mail: %s\n", hx.email);
        fprintf(saida, "Telefone: %d\n\n", hx.telefone);
    }

    printf("Relatório gerado com sucesso. Confira o arquivo %s.\n", arqt);

    fclose(entrada);
    fclose(saida);
}