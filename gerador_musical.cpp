#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>


#include <fstream>
#include <random>
#include <string>

#include "read_write_wav.h"
//#include "arti_abc.c"
#define C_ALLOC(number, type) ((type *) calloc((number),sizeof(type)) )
#define TwoPi 2*M_PI
#define D      1   /* Linear|Exponential=0|1 distribution    */
//#define FS 20000   /* Sample  frequency (Hz)                 */
//#define T   15.0   /* Image to sound conversion time (s)     */
//#define N     64   /* Resolution, i.e., # rows and columns   */
#define FL   500   /* Lowest  frequency (Hz) in visual sound */
#define FH  5000   /* Highest frequency (Hz)                 */
//#define FS 20000   /* Sample  frequency (Hz)                 */
unsigned long ir=0L, ia=9301L, ic=49297L, im=233280L;
double rnd(void){ ir = (ir*ia+ic) % im; return ir / (1.0*im); }

using namespace std;

#define DEBUG 0

#define TRANSICAO 0.1

//helper functions
float midi2freq(int m)
{
    return 440*pow(2,(m-69.0f)/12.0f);
}

float freq2midi(float freq)
{
    return 12*log2( freq/440) + 69;
}

class SoundProcessor  
{
    public:

        virtual void process(float*, int)
        {
        }
};

class SineOscillator : public SoundProcessor
{
    public: 

        float freq;
        float amp;
        float Fs;


        SineOscillator(int nota_midi, float _amp, float _Fs)
        {
            freq = midi2freq(nota_midi);
            amp = _amp;
            Fs = _Fs;
        }

        ~SineOscillator()
        {
        }   

        void process(float* audio_buffer, int buffer_len) 
        {
            if(DEBUG) cout << "sine class -> call process: "<<  buffer_len << " amp: "<<  amp << " freq: "<< freq << "  Fs: " << Fs << endl;

            int dividido=buffer_len*TRANSICAO;
            float fator;
            for (int n=0; n<buffer_len; n++){       ////ENVELOPE PRIMITIVO
                if(n<dividido)                      fator=(n / ((float) dividido));//inicio
                else if (n> buffer_len - dividido)  fator=(buffer_len - n) / ((float) dividido);//final                
                else                                fator=1.0;//no meio
                audio_buffer[n] = fator * amp * sin(2*M_PI*freq*(((float)n)/Fs));
            }

        }

};

class MusicNote
{
    public:
        float start_time; // pos in seconds
        float end_time; // pos in seconds
        SoundProcessor* sp;   

    MusicNote(SoundProcessor *o, float s, float e):sp(o), start_time(s), end_time(e)
    {
    }
};

void ruidoORIGINAL(float* audio_buffer, int buffer_len){
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,1.0);

    if(DEBUG) cout << "RUIDO"<< endl;

    for (int n=0; n<buffer_len; n++){
        audio_buffer[n] = distribution(generator);
    }    
    return;
}

class audio
{
private:
    float* buffer;
    int buffer_len;
    float Fs;
    float duration;
public:
    audio(float* audio_buffer, int buffer_len, float Fs, float duration);
    void ruido();
    void escala_crescente();
    void aleatorio();
    void imagem();
    ~audio();
};

audio::audio(float* audio_buffer, int len, float freq_amostragem, float duracao)
{
    buffer=audio_buffer;
    buffer_len=len;
    Fs=freq_amostragem;
    duration=duracao;
    cout << "som criado" << endl;
}

void audio::ruido(){
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,1.0);

    if(DEBUG) cout << "RUIDO"<< endl;

    for (int n=0; n<buffer_len; n++){
        buffer[n] = distribution(generator);
    }    
    return;
}

void audio::escala_crescente(){
    int i, notaMIDIinicial, tam_escala, k, max_pos, startPos, endPos;
    float nota_duration;
    vector<MusicNote> notes;
    SineOscillator *s;

    cout << "Digite a nota MIDI inicial[0 até 127]: ";
    cin >> notaMIDIinicial;
    cout << "Digite a duração em segundos de cada nota: ";
    cin >> nota_duration;

    tam_escala=int(duration/nota_duration);

    for(i=0; i<tam_escala; i++){
        s = new SineOscillator(notaMIDIinicial+i, 1, Fs);
        MusicNote m(s, i*nota_duration, (i+1)*nota_duration);
        notes.push_back(m);
    }

    // write the notes into the audio buffer
    for (k=0; k<notes.size(); k++){
        startPos = notes[k].start_time*Fs;
        endPos = notes[k].end_time*Fs;
        if(DEBUG){
            cout << "startPos: " << startPos << endl;
            cout << "endPos: " << endPos << endl;
        }
        notes[k].sp->process(buffer + startPos, endPos-startPos);
    }
    return;
}

void audio::aleatorio(){
    int i, oitavaMIDIinicial, tam_escala, k, max_pos, startPos, endPos, nota;
    float nota_duration;
    vector<MusicNote> notes;
    SineOscillator *s;

    cout << "Digite a oitava inicial[-1 até 9]: ";
    cin >> oitavaMIDIinicial;
    cout << "Digite a duração em segundos de cada nota: ";
    cin >> nota_duration;

    tam_escala=int(duration/nota_duration);

    for(i=0; i<tam_escala; i++){
        nota=12*(oitavaMIDIinicial+1)+rand()%12;
        s = new SineOscillator(nota, 1, Fs);
        if(DEBUG) cout << nota << endl;
        MusicNote m(s, i*nota_duration, (i+1)*nota_duration);
        notes.push_back(m);
    }

    // write the notes into the audio buffer
    for (k=0; k<notes.size(); k++){
        startPos = notes[k].start_time*Fs;
        endPos = notes[k].end_time*Fs;
        if(DEBUG){
            cout << "startPos: " << startPos << endl;
            cout << "endPos: " << endPos << endl;
        }
        notes[k].sp->process(buffer + startPos, endPos-startPos);
    }
    return;
}

void audio::imagem(){
    int    i, j;
    float duration_imagem=1.0;
    string linha[64];

    cout << "A imagem do arquivo imagem.txt será escondida no áudio." << endl;

    ifstream arq ("imagem.txt");
    if (arq.is_open()){
        for(i=0; getline (arq,linha[i]); i++){ }
        arq.close();
    }else{
        cout << "Erro ao abrir o arquivo" << endl;
    }
    
    long n=linha[0].size();//comprimento/largura da matriz quadrada A
    long k = 0L, ns = (long) (Fs * duration_imagem), m = ns / n;
    double **A, t, dt = 1.0 / Fs, *w, *phi0, s, y, yp, z, tau1, tau2, scale = 0.5/sqrt(n);

    double resultado;

    w    = C_ALLOC(n, double);
    phi0 = C_ALLOC(n, double);
    A    = C_ALLOC(n, double *);
    for (i=0; i<n; i++) A[i] = C_ALLOC(n, double);  /* N x N pixel matrix */

    /* Set hard-coded image */
    //a = 0
    //16 tonalidades de cinza
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            if (linha[i][j]>'a') A[n-i-1][j] = pow(10.0,(linha[i][j]-'a'-15)/10.0); /* 2dB steps */
            else A[n-i-1][j] = 0.0;
        }
    }
   
    //coloca inicialmente, notas aleatórias
    /* Set lin|exp (0|1) frequency distribution and random initial phase */
    for (i=0; i<n; i++) w[i] = TwoPi * FL * pow(1.0* FH/FL,1.0*i/(n-1));//EXPONENCIAL
    
    for (i=0; i<n; i++) phi0[i] = TwoPi * rand();


    tau1 = 0.5 / w[n-1];
    tau2 = 0.25 * tau1*tau1;
    y = z = 0.0; 
    for ( ; k < ns; k++) {
        s = 0.0;
        t = k * dt;
        j = k / m;
        if (j>n-1) j=n-1;
        for (i=0; i<n; i++)
            s += A[i][j] * sin(w[i] * t + phi0[i]);//A IMAGEM DEFINE A AMPLITUDE DO SENO

        if (k < ns/(5*n)) s = (2.0*rnd()-1.0) / scale;  /* "click" */

        //FILTRO PASSA BAIXA DE SEGUNDA ORDEM
        yp = y;
        y = tau1/dt + tau2/(dt*dt);
        y  = (s + y * yp + tau2/dt * z) / (1.0 + y);
        z = (y - yp) / dt;
        resultado=(scale * y); /* y = 2nd order filtered s */
        buffer[k]=resultado;
   }

   free(w);
   free(phi0);
   free(A);

   for(i=k; i<buffer_len; i++){//preenche o resto do audio
       buffer[i]=buffer[i-k];
   }

}

audio::~audio()
{
}


void escala_crescenteORIGINAL(float* audio_buffer, int buffer_len, float Fs, float duration){
    int i, notaMIDIinicial, tam_escala, k, max_pos, startPos, endPos;
    float nota_duration;
    vector<MusicNote> notes;
    SineOscillator *s;

    cout << "Digite a nota MIDI inicial[0 até 127]: ";
    cin >> notaMIDIinicial;
    cout << "Digite a duração em segundos de cada nota: ";
    cin >> nota_duration;

    tam_escala=int(duration/nota_duration);

    for(i=0; i<tam_escala; i++){
        s = new SineOscillator(notaMIDIinicial+i, 1, Fs);
        MusicNote m(s, i*nota_duration, (i+1)*nota_duration);
        notes.push_back(m);
    }

    // write the notes into the audio buffer
    for (k=0; k<notes.size(); k++){
        startPos = notes[k].start_time*Fs;
        endPos = notes[k].end_time*Fs;
        if(DEBUG){
            cout << "startPos: " << startPos << endl;
            cout << "endPos: " << endPos << endl;
        }
        notes[k].sp->process(audio_buffer + startPos, endPos-startPos);
    }
    return;
}

void audio_aleatorioORIGINAL(float* audio_buffer, int buffer_len, float Fs, float duration){

    int i, oitavaMIDIinicial, tam_escala, k, max_pos, startPos, endPos, nota;
    float nota_duration;
    vector<MusicNote> notes;
    SineOscillator *s;

    cout << "Digite a oitava inicial[-1 até 9]: ";
    cin >> oitavaMIDIinicial;
    cout << "Digite a duração em segundos de cada nota: ";
    cin >> nota_duration;

    tam_escala=int(duration/nota_duration);

    for(i=0; i<tam_escala; i++){
        nota=12*(oitavaMIDIinicial+1)+rand()%12;
        s = new SineOscillator(nota, 1, Fs);
        if(DEBUG) cout << nota << endl;
        MusicNote m(s, i*nota_duration, (i+1)*nota_duration);
        notes.push_back(m);
    }

    // write the notes into the audio buffer
    for (k=0; k<notes.size(); k++){
        startPos = notes[k].start_time*Fs;
        endPos = notes[k].end_time*Fs;
        if(DEBUG){
            cout << "startPos: " << startPos << endl;
            cout << "endPos: " << endPos << endl;
        }
        notes[k].sp->process(audio_buffer + startPos, endPos-startPos);
    }
    return;
}



int TESTEimprime_texto(float* buffer, int buffer_len, float Fs, float duration){
    int    i, j, b, d = D;
    float duration_imagem=1.0;

    string linha[64];
    ifstream arq ("imagem.txt");
    if (arq.is_open()){
        for(i=0; getline (arq,linha[i]); i++){ }
        arq.close();
    }

   long n=linha[0].size();//comprimento/largura da matriz quadrada A

   long   k = 0L, ns = (long) (Fs * duration_imagem), m = ns / n;
   double **A, t, dt = 1.0 / Fs, *w, *phi0, s, y, yp, z, tau1, tau2, scale = 0.5/sqrt(n);

    double resultado;

   w    = C_ALLOC(n, double);
   phi0 = C_ALLOC(n, double);
   A    = C_ALLOC(n, double *);
   for (i=0; i<n; i++) A[i] = C_ALLOC(n, double);  /* N x N pixel matrix */

   /* Set hard-coded image */
   //a = 0
   //16 tonalidades de cinza
   for (i=0; i<n; i++) {
      for (j=0; j<n; j++) {
         if (linha[i][j]>'a') A[n-i-1][j] = pow(10.0,(linha[i][j]-'a'-15)/10.0); /* 2dB steps */
         else A[n-i-1][j] = 0.0;
      }
   }
   
    //coloca inicialmente, notas aleatórias
   /* Set lin|exp (0|1) frequency distribution and random initial phase */
   if (d) for (i=0; i<n; i++) w[i] = TwoPi * FL * pow(1.0* FH/FL,1.0*i/(n-1));
   else   for (i=0; i<n; i++) w[i] = TwoPi * FL + TwoPi * (FH-FL)   *i/(n-1) ;
   for (i=0; i<n; i++) phi0[i] = TwoPi * rand();


  tau1 = 0.5 / w[n-1];
  tau2 = 0.25 * tau1*tau1;
  y = z = 0.0; 
   for ( ; k < ns; k++) {
      s = 0.0;
      t = k * dt;
      j = k / m;
      if (j>n-1) j=n-1;
      for (i=0; i<n; i++)
        s += A[i][j] * sin(w[i] * t + phi0[i]);//A IMAGEM DEFINE A AMPLITUDE DO SENO

      if (k < ns/(5*n)) s = (2.0*rnd()-1.0) / scale;  /* "click" */

    //FILTRO PASSA BAIXA DE SEGUNDA ORDEM
      yp = y;
      y = tau1/dt + tau2/(dt*dt);
      y  = (s + y * yp + tau2/dt * z) / (1.0 + y);
      z = (y - yp) / dt;
      resultado=(scale * y); /* y = 2nd order filtered s */
    buffer[k]=resultado;

   }

   printf("%ld\n", ns);

   free(w);
   free(phi0);
   free(A);

   for(i=k; i<buffer_len; i++){
       buffer[i]=buffer[i-k];
   }

   //for(i=0; i<buffer_len; i++) 
   //if ((ceil(ns/2))!=(ns/2)) putc(0,fp);
   //fclose(fp);
   return(0);

}
int main(int c, char** argv)
{
    const float duration = 15.0; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    string wav_name;


    audio_buffer = new float[buffer_len];
    memset(audio_buffer, 0, buffer_len);


    audio som(audio_buffer, buffer_len, Fs, duration);
    //return 0;

    int i,j, escolha;

    cout << "PROGRAMA GERADOR MUSICAL\nO programa gera áudios de " << duration << " segundos em arquivo *.wav" << endl;
    cout << "Digite:\n1 para ruído;\n2 para escala crescente de notas MIDI;\n3 para notas aleatórias.\n4 som ABC\nEscolha: ";

    cin >> escolha;

    switch (escolha){
        case 1: som.ruido();            wav_name = "ruido.wav"; break;
        case 2: som.escala_crescente(); wav_name = "crescente.wav"; break;
        case 3: som.aleatorio();        wav_name = "aleatorio.wav"; break;
        case 4: som.imagem();           wav_name = "mensagem.wav"; break;
        default: return 0;
            break;
    }
    if(DEBUG) for(i=0; i<50; i++)
        printf("%f ", audio_buffer[i]);

    // ============================
    // save output wave
    write_wave_file (wav_name.c_str(), audio_buffer, buffer_len, Fs);
    if(DEBUG) cout << "done." << endl;
    delete [] audio_buffer;

    return 0;
}



