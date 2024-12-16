# Sistema de estações metro
## Conceitos Gerais  

Este documento descreve como implementamos todas as especificações do trabalho em nosso código. Cada ponto será descrito na mesma ordem das exigências.  

---

## 1. Mapear as estações da imagem  
**Como fizemos:**  

As estações são representadas pelas structs `Estacao` e `Linha`.  
- Cada **estação** possui um nome e uma lista de linhas às quais está conectada.  
- Cada **linha** possui um nome e uma lista de estações associadas.  
- A função `InsercaoDadosMetro` é responsável por adicionar as estações e linhas específicas ao programa.  

---

## 2. Gerar relatório com estações vizinhas  
**Como fizemos:**  

A funcionalidade é implementada pela função `gerarRelatorio`.  
- A função gera um relatório detalhado das linhas de trem, destacando as estações e suas conexões.  
- A função evita duplicatas e garante que todas as estações e conexões sejam incluídas no relatório.  
- A recursão é usada para garantir que todas as linhas sejam percorridas.  

---

## 3. O sistema deve permitir a alteração dos dados de qualquer estação  
**Como fizemos:**  

A funcionalidade é implementada pela função `alterarDadosEstacao`.  
- Recebe como parâmetros a estação a ser alterada e o novo nome desejado.  
- Utiliza a função `strcpy` para atualizar o nome da estação.  

---

## 4. Definir a qual linha uma estação pertence e o caminho para ela  
**Como fizemos:**  

- A função `adicionarEstacaoLinhas` permite criar novas estações e definir as rotas que as conectam a outras estações.  
- Cada estação é associada às linhas especificadas, estabelecendo conexões bidirecionais entre estações e linhas.  
- A função `InsercaoDadosMetro` cria as estações iniciais e suas conexões no mapa do metrô de Lisboa.  

---

## 5. As estações podem fechar  
**Como fizemos:**  

A função `excluirEstacao` é usada para remover uma estação de todas as linhas às quais pertence.  
- Percorre todas as linhas do metrô.  
- Remove a estação da lista de estações da linha e ajusta os ponteiros.  
- Libera a memória alocada para a estação.  
- Pode ser utilizada para fechamentos permanentes ou temporários.  

---

## 6. Visualizar rotas entre estações  
**Como fizemos:**  

A funcionalidade utiliza o algoritmo de **Dijkstra**, implementado na função `Dijkstra`.  
- Determina rotas mínimas entre estações considerando tempos de viagem.  
- Sugere caminhos alternativos ou a opção de caminhar, caso as estações sejam vizinhas.  

---

## 7. Mostrar quais estações dão acesso à linha desejada pelo usuário  
**Como fizemos:**  

A função `identificarEstacoesConexao` implementa essa funcionalidade:  
1. Coleta informações do usuário sobre a linha desejada.  
2. Verifica se o grafo formado pelas estações é de Euler usando a função `verificaGrafoEuler`.  
3. Lista as estações conectadas à linha desejada com a função `temConexaoComLinha`.  

---

## 8. Criar nova linha  
**Como fizemos:**  

A função `criarLinha` é responsável por essa funcionalidade:  
- O usuário fornece o nome da nova linha e as estações a serem adicionadas.  
- A função cria uma nova estrutura de linha e associa as estações especificadas.  

---

## 9. Mostrar estações com o maior grau  
**Como fizemos:**  

A função `buscarEstacoesComMaisConexoes` identifica as estações com o maior número de conexões.  
- Percorre todas as linhas e estações.  
- Conta as conexões de cada estação.  
- Atualiza a lista de estações com maior grau de conexões, evitando duplicatas.  
- Imprime o resultado ou indica que nenhuma estação foi encontrada.  

---

## 10. Grafo de Euler  
**Como fizemos:**  

A função `verificaGrafoEuler` verifica se o conjunto de estações forma um subgrafo com características de um **Grafo de Euler**.  
- Avalia se todos os graus das estações são pares, garantindo a existência de um ciclo Euleriano (ciclo que passa por todas as arestas exatamente uma vez).  
