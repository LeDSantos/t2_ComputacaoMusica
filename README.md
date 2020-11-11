# t2_ComputacaoMusica
Trabalho prático de Computação &amp; Música (2020/1) - Tópicos Especiais em Computação XVI, UFRGS.

**Pré-requisito: biblioteca libsndfile (disponível em http://www.mega-nerd.com/libsndfile/).**

Esse projeto foi escrito utilizando as tarefas do laboratório (disponível em https://github.com/schramm/compmus_2020_1) como modelo.
A biblioteca read_write_wav.h possibilita ler e escrever em um arquivo *.wav.

Para executar:
```prompt
make
./gerador_musical
```

## PROGRAMA GERADOR MUSICAL
O programa gera áudios de 15 segundos em arquivo *.wav. O áudio pode conter ruído, escala crescente de notas MIDI ou notas aleatórias. Dependendo da sua escolha, serão necessárias mais informações.

Tive uma ideia de outro coisa q a gente pode fazer: gerar audios, tipo com ruido em distribuição gaussiana ou com notas musicais aleatórias(dá de fazer isso com o código do lab de C++) e coisas desse tipo, colocar no sonic visualizer e analisar\
a parte prática do trabalho seria gerar os áudios por programação mesmo\
dava até de ler informações de um arquivo, tipo pega uma sequencia de números e aquilo vira uma música\
Título: geração de aúdio "por meio de algoritmo" e análise\
esse por meio de algoritmo tem um nome mais chique, agora não tô lembrando\
analise do audio gerado vesus uma gravação: o gerado vai estar com menos ruido\
depois entre os gerados: ruído, escala crescente de notas, aleatório e os nossos trabalhos artisticos\
dá de pegar gravação de flauta e piano por exemplo: a flauta faz um seno mais puro\
as gravações dá de pegar de musica clássica q tá em domínio público\
e colocamos em anexo o código q gerou cada audio

Documento: https://ufrgscpd-my.sharepoint.com/:w:/g/personal/00275604_ufrgs_br/ERbtLlcDGphJudISohRb3_MBa9o8atdyETxBzuNKU3v1Nw?e=gO3K8y

Apresentação: https://ufrgscpd-my.sharepoint.com/:p:/g/personal/00275604_ufrgs_br/EZwAgF1ln4lLhDtIV7DrD2AB_MiEfXURlHzZiLWpY9Llww?e=iyNYu3

