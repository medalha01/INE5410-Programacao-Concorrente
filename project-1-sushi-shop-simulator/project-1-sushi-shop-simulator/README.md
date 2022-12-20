# Trabalho 1 - Programação Concorrente - ine5410 (2022.2) - Sushi Shop Simulator

## Visão geral

O Trabalho 1 (T1) da disciplina de Programação Concorrente consiste em construir um simulador de um restaurante de comida japonesa. O simulador deve implementar o comportamento de clientes que vão a um Sushi Shop para comer. Ao entrarem no restaurante, os clientes se sentam ao redor de uma mesa que possui uma esteira rolante. Os sushi chefs fazem os sushis e os colocam na esteira. Quando um cliente deseja comer um sushi que está passando na esteira próximo a ele, ele pega o alimento e come. Após comer todos os sushis que desejava, o cliente vai embora do restaurante.

Temos na imagem abaixo um esquemático dos atores e componentes importantes da simulação:

![sushi shop overview](/docs/images/overview.png)

## Estrutura do projeto

Você recebeu um esqueleto de código com funcionalidades essenciais da simulação, incluindo as principais abstrações:

- *conveyor_belt.c* representando a esteira de comida japonesa e os assentos onde ficarão os clientes;
- *customer.c* representando os clientes do Sushi Shop;
- *globals.c* contendo definições de variáveis globais da simulação;
- *hostess.c* representando um funcionário da recepção do restauraute, que irá guiar os novos clientes para a esteira;
- *menu.h* com as definições dos pratos disponíveis;
- *queue.c* representando uma fila de clientes;
- *sushi_chef.c* representando os cozinheiros do restaurante;
- *virtual_clock.c* contendo um contador de tempo para a simulação.

O código para cada abstração é dado com algumas partes críticas faltando. Você deve garantir o funcionamento correto esperado para cada elemento da simulação: Customer, Sushi Chef, Hostess, etc. Cuidados devem ser tomados para que não ocorram deadlocks, condições de corrida, nem outros problemas de concorrência. Alguns comentários foram deixados pelo código-fonte para auxiliar, entretanto vocês podem utilizar as estratégias que melhor desejarem para completar a simulação --- só não é permitido remover as funções dadas!

## Compilando o código

O código pode ser compilado utilizando o comando `make build`.

## Executando a simulação

Basta executar o binário gerado na pasta `/bin`, passando os parâmetros de simulação desejados.
Para mais detalhes, execute:

```shell
./bin/assignment-1 -h
```

### Utilizando Docker

Caso você encontre problemas para compilar a aplicação (geralmente usuários de Windows encontram problemas com dependências de sistema), você pode usar o ambiente docker fornecido. Execute `make run-docker` no terminal para iniciar uma sessão interativa em um container Linux com gcc e todas dependências necessárias já instaladas.

OBS: você precisa ter a ferramenta `docker` instalada na sua máquina. Caso não possua o comando `make`, você pode executar o ambiente docker utilizando os dois comandos abaixo:

```shell
docker build . -t gcc-sandbox
```

```shell
docker run -it gcc-sandbox
```


## Comportamentos esperados na simulação

### Conveyor Belt (Esteira)

1. A esteira de Sushi (`conveyor_belt`) possui dois arrays de inteiros representando os slots de comida e os assentos.
2. Periodicamente, os slots de comida da esteira movem-se (ver arquivo `conveyor_belt.c`).
3. A posição dos assentos permanece fixa durante a simulação.
4. Todos os assentos e slots de comida iniciam-se vazios quando o restaurante abre.

### Sushi Chef

1. Quando o restaurente abre, um único Sushi Chef (`sushi_chef`) é criado e deve sentar-se á esquerda da esteira.
3. O Sushi Chef só começa a cozinhar após posicionar-se na esteira.
4. O Sushi Chef irá cozinhar itens do menu aleatóriamente e deverá adicioná-los apenas aos slots livres à sua frente.
5. Quando o restaurante fechar, o Sushi Chef deve parar de cozinhar imediatamente.

### Hostess

1. O Hostess (`hostess`) deverá observar os assentos vagos na esteira e guiar os clientes (`customer`) da fila (`queue`) conforme eles chegam (ordem FIFO). A abstração dada para a fila possui um método já implementado para gerar clientes aleatóriamente durante a simulação.
2. Guiar um cliente para a esteira significa posicioná-lo em um assento da esteira global e removê-lo da fila de entrada.
3. Quando o restaurante fechar, o Hostess deve imediatamente parar de guiar novos clientes para a esteira e finalizar (desalocar) todos os clientes que restarem na fila.

### Customer

1. Os clientes (`customer`) possuem uma lista com as quantidades de cada item do menu que desejam consumir.
2. Os clientes só comem após estarem posicionados na esteira.
3. Os clientes só podem pegar comida nos slots à sua frente ou opcionalmente nos slots adjacentes (i, i-1 ou i+1).
4. Os clientes só devem consumir um item da esteira caso ele esteje em sua "lista de desejos".
5. Os clientes só podem remover e consumir um único item por vez.
6. Após consumir todos os itens desejados, o cliente deve retirar-se da esteira imediatamente.

### Relógio Virtual

1. O relógio da simulação (`virtual_clock.c`) já possui grande parte da lógica implementada, e deve ser usado basicamente para informar o tempo de fechamento do restaurante para o Hostess e Sushi Chef (você deve pensar em uma estratégia para que isso seja feito).

### Variáveis Globais

1. Na simulação são definidas um `conveyor_belt_t`, `queue_t` e `virtual_clock_t` globais que devem ser usados.
2. Devem ser criadas variáveis globais adicionais para armazenar os seguintes dados sobre a simulação:
    - Quantidade de clientes que sentaram-se na esteira e consumiram todos os itens desejados
    - Quantidades produzidas de cada item do menu
    - Quantidades consumidas de cada item do menu
