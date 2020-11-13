<p align="center"><b>Universidade Federal do Rio Grande do Sul – UFRGS</b></p>

<big><b>Disciplina: Computação &amp; Música (2020/1) – Tópicos Especiais em Computação XVI</b></big>

<big><b>Professores: Marcelo de Oliveira Johann, Marcelo Soares Pimenta e Rodrigo Schramm</b></big>

<big><p align="center"><b>Relatório do Trabalho Prático</b></p></big>

<big><b>Alunas: Izadora Dourado Berti(275606) e Letícia dos Santos(275604)</b></big>
___
# t2_ComputacaoMusica

**Pré-requisito: biblioteca libsndfile (disponível em http://www.mega-nerd.com/libsndfile/).**

Esse projeto foi escrito utilizando as tarefas do laboratório (disponível em https://github.com/schramm/compmus_2020_1) como modelo.
A biblioteca read_write_wav.h possibilita ler e escrever em um arquivo *.wav.

Vídeo da apresentação em:

Para executar:
```prompt
make
./gerador_musical
```

## PROGRAMA GERADOR MUSICAL
O programa gera áudios de 15 segundos em arquivo *.wav. O áudio pode conter ruído, escala crescente de notas MIDI, notas aleatórias ou imagem esteganografada. As notas utilizam envelope. Dependendo da sua escolha, serão necessárias mais dados. As informações apresentadas a seguir são sobre o código do arquivo gerador_musical.cpp.

### Envelope
Sabendo que TRANSICAO=0.1, existe a seguinte função dentro da classe SineOscillator no para calcular os valores do buffer:

```
void process(float* audio_buffer, int buffer_len){
  if(DEBUG) cout << "sine class -> call process: "<<  buffer_len << " amp: "<<  amp << " freq: "<< freq << "  Fs: " << Fs << endl;

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
GRÁFICO\
Isso se torna um envelope sobre o seno, que produz um som mais agradável.

### Ruído
Utiliza distribuição gausiana de média 0 e variância 1 para determinar a amostra.

### Escala crescente
Cria uma amostra contendo uma escala de notas que inicia no valor MIDI(0 até 127) informado, cada uma com a duração(em segundos) informada.

### Notas aleatórias
Após informar a oitava utilizada(-1 até 9), cria amostra contendo notas aleatórias com a duração(em segundos) informada.

### Imagem esteganografada
De acordo com a Wikipédia, [esteganografia](https://pt.wikipedia.org/wiki/Esteganografia) (do grego "escrita escondida") é o estudo e uso das técnicas para ocultar a existência de uma mensagem dentro de outra, uma forma de segurança por obscurantismo. O código em C disponibilizado [aqui](https://www.seeingwithsound.com/im2sound.htm) foi adaptado para ser utilizado no programa.

A imagem de 64x64 bits com 16 escalas de cinza está em um arquivo *.txt, sendo que, após um processamento do programa, 'a' corresponde a 0, 'b' a 1, a assim sucessivemente até 'p' corresponder a 15 dentro de uma matriz 64x64. Inicialmente, a são criados valores para frequência e fase das ondas seno que serão utilizadas posteriormente. Após, é feito um loop para determinar os valores que serão colocados no buffer que será gravado no arquivo de áudio. Uma coluna da matriz de cinzas é analisada a cada interação, os elementos são utilizados como amplitude do seno e a soma dos senos da coluna resulta em uma onda. A onda passa por um filtro passa-baixa de segunda ordem. Então, o resultado é colocado no buffer. Como a imagem preenche somente 1 segundo, o restante do buffer é preenchido com copias do primeiro segundo.


Documento: https://ufrgscpd-my.sharepoint.com/:w:/g/personal/00275604_ufrgs_br/ERbtLlcDGphJudISohRb3_MBa9o8atdyETxBzuNKU3v1Nw?e=gO3K8y

Apresentação: https://ufrgscpd-my.sharepoint.com/:p:/g/personal/00275604_ufrgs_br/EZwAgF1ln4lLhDtIV7DrD2AB_MiEfXURlHzZiLWpY9Llww?e=iyNYu3

