<p align="center"><b>Universidade Federal do Rio Grande do Sul – UFRGS</b></p>

**Disciplina: Computação &amp; Música (2020/1) – Tópicos Especiais em Computação XVI**

**Professores: Marcelo de Oliveira Johann, Marcelo Soares Pimenta e Rodrigo Schramm**

<p align="center"><b>Relatório do Trabalho Prático</b></p>

**Alunas: Izadora Dourado Berti(275606) e Letícia dos Santos(275604)**
___
# t2_ComputacaoMusica

Repositório https://github.com/LeDSantos/t2_ComputacaoMusica/

**Pré-requisito: biblioteca [libsndfile](http://www.mega-nerd.com/libsndfile/).**

Esse projeto foi escrito utilizando as tarefas do [laboratório](https://github.com/schramm/compmus_2020_1) como modelo.
A biblioteca read_write_wav.h possibilita ler e escrever em um arquivo *.wav.

**Vídeo da apresentação em: https://youtu.be/qhsNxg7OAaA**

Apresentação em: [Apresentação Trabalho Prático Computação e Música.pptx](https://github.com/LeDSantos/t2_ComputacaoMusica/blob/main/Apresenta%C3%A7%C3%A3o%20Trabalho%20Pr%C3%A1tico%20Computa%C3%A7%C3%A3o%20e%20M%C3%BAsica.pptx)

Para executar:
```prompt
make
./gerador_musical
```

## PROGRAMA GERADOR MUSICAL
O programa gera áudios de 15 segundos em arquivo *.wav. O áudio pode conter ruído, escala crescente de notas MIDI, notas aleatórias ou imagem esteganografada. As notas utilizam envelope. Dependendo da sua escolha, serão necessárias mais dados. As informações apresentadas a seguir são sobre o código do arquivo [gerador_musical.cpp](gerador_musical.cpp).

### Envelope
Sabendo que TRANSICAO=0.1, existe a seguinte função dentro da classe SineOscillator no para calcular os valores do buffer:

```
void process(float* audio_buffer, int buffer_len){

  int dividido=buffer_len*TRANSICAO;
  float fator;
  for (int n=0; n<buffer_len; n++){     ////ENVELOPE PRIMITIVO
    if(n<dividido)                      fator=(n / ((float) dividido));//inicio
    else if (n> buffer_len - dividido)  fator=(buffer_len - n) / ((float) dividido);//final                
    else                                fator=1.0;//no meio
  audio_buffer[n] = fator * amp * sin(2*M_PI*freq*(((float)n)/Fs));
  }
}
```
Fator se comporta da seguinte forma:\
<img src="https://github.com/LeDSantos/t2_ComputacaoMusica/blob/main/grafico.jpg" alt="fator" width="600"/>

Isso se torna um envelope sobre o seno, que produz um som mais agradável.

### Ruído
Utiliza distribuição gausiana de média 0 e variância 1 para determinar a amostra.

### Escala crescente
Cria uma amostra contendo uma escala de notas que inicia no valor MIDI(0 até 127) informado, cada uma com a duração(em segundos) informada.

### Notas aleatórias
Após informar a oitava utilizada(-1 até 9), cria amostra contendo notas aleatórias com a duração(em segundos) informada.

### Imagem esteganografada
De acordo com a Wikipédia, [esteganografia](https://pt.wikipedia.org/wiki/Esteganografia) (do grego "escrita escondida") é o estudo e uso das técnicas para ocultar a existência de uma mensagem dentro de outra, uma forma de segurança por obscurantismo. O código em C disponibilizado em https://www.seeingwithsound.com/im2sound.htm foi adaptado para ser utilizado no programa. Esse código faz parte do projeto The vOICe, uma tecnologia de substituição sensorial que converte visões visuais puras em ondas sonoras correspondentes enquanto ainda preserva o significado da informação visual total.

A imagem de 64x64 bits com 16 escalas de cinza está em um arquivo *.txt, sendo que, após um processamento do programa, 'a' corresponde a 0, 'b' a 1, a assim sucessivemente até 'p' corresponder a 15 dentro de uma matriz 64x64.

Inicialmente, são criados valores para frequência e fase das ondas seno que serão utilizadas posteriormente. Após, é feito um loop para determinar os valores que serão colocados no buffer que será gravado no arquivo de áudio. Uma coluna da matriz de cinzas é analisada a cada interação, os elementos são utilizados como amplitude do seno e a soma dos senos da coluna resulta em uma onda. A onda passa por um filtro passa-baixa de segunda ordem. Então, o resultado é colocado no buffer. Como a imagem preenche somente 1 segundo, o restante do buffer é preenchido com copias do primeiro segundo.

**Fontes dos áudios**: [fonte_musicas_eruditas.md](fonte_musicas_eruditas.md)
