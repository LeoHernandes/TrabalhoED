# Sobre o projeto
Um estudo sobre árvores AVL e Rubro-Negras aplicadas no armazenamento e busca de palavras feito como trabalho final da disciplina de Estrutura de Dados na UFRGS (2020/2) ministrada pela professora Viviane Moreira.
Todo o projeto foi feito em dupla com [Níkolas Padão](https://github.com/nibst).

## Sobre o método de análise
Para o estudo do desempenho dessa estrutura de dados, foram utilizadas bases de dados com variadas quantidades de tweets (100, 10.000, 100.000, 450.000, 900.000). Foi feita a indexação de todas as palavras diferentes nos dois tipos de árvores e, logo após, foi realizada uma busca de 1000 palavras.
Durante o processamento das informações, foi calculado o custo computacional para se realizar a tarefa, contabilizando a quantidade de comparações que o programa fez para encontrar a posição correta onde deveriam ser armazenadas as palavras e também a quantidade de comparações para encontrar as palavras na estrutura durante as buscas sequenciais. Além disso, foi contado o total de rotações realizadas para manter as árvores balanceadas durante toda a fase de indexação das palavras.
A seguir, apresentam-se os resultados obtidos de cada tipo de árvore e uma comparação do desempenho das duas estruturas de dados.

## Árvore AVL:

![Avl stats](https://user-images.githubusercontent.com/72423032/158038685-ae812fcd-5fdf-4b4b-bf5e-5f39a24b11eb.png)

![Avl busca](https://user-images.githubusercontent.com/72423032/158038724-370fa980-e8c9-429e-a162-0e7f08cbcd42.png)

## Árvore Rubro-Negra

![RB stats](https://user-images.githubusercontent.com/72423032/158038763-cb74c1bd-1b02-429b-9aa8-4a6a5c4c7635.png)

![RB busca](https://user-images.githubusercontent.com/72423032/158038772-d05cb36e-2b29-4644-8cad-b35dd281aeb8.png)

## Rotações e altura das árvores:
Nessas características conseguimos observar um comportamento interessante que mais tarde vai ser essencial para uma possível conclusão.
No gráfico comparativo abaixo sobre as rotações feitas na fase de indexação, percebe-se claramente que em todos os casos a árvore AVL faz mais rotações que a árvore Rubro-Negra:

![rotações](https://user-images.githubusercontent.com/72423032/158038848-742f828e-8069-4b68-860d-ffb467538b18.png)


Isso implica diretamente na altura das árvores, como podemos ver na tabela abaixo:

![altura](https://user-images.githubusercontent.com/72423032/158038875-10218ff1-0773-4612-8d15-f43d76ba480b.png)
