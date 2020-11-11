#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

#include <random>
#include <string>

#include "read_write_wav.h"

using namespace std;

#define DEBUG 1

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
//


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

void ruido(float* audio_buffer, int buffer_len){
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,1.0);

    if(DEBUG) cout << "RUIDO"<< endl;

    for (int n=0; n<buffer_len; n++){
        audio_buffer[n] = distribution(generator);
    }    
    return;
}

void escala_crescente(float* audio_buffer, int buffer_len, float Fs, float duration){
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

    /*max_pos = 0;
    for (k=0; k<notes.size(); k++){
        max_pos = std::max((float)max_pos, (float)round(notes[k].end_time*Fs));
    }

    if(DEBUG) cout << "maxPos: " << max_pos <<  endl;
    */
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

void audio_aleatorio(float* audio_buffer, int buffer_len, float Fs, float duration){

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

int main(int c, char** argv)
{
    const float duration = 15.0; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    float *audio_buffer2;
    string wav_name;

    audio_buffer = new float[buffer_len];
    memset(audio_buffer, 0, buffer_len);

    int i,j, escolha;

    cout << "PROGRAMA GERADOR MUSICAL\nO programa gera áudios de " << duration << " segundos em arquivo *.wav" << endl;
    cout << "Digite:\n1 para ruído;\n2 para escala crescente de notas MIDI;\n3 para notas aleatórias.\nEscolha: ";

    cin >> escolha;

    switch (escolha){
        case 1: ruido(audio_buffer, buffer_len); wav_name = "ruido.wav"; break;
        case 2: escala_crescente(audio_buffer, buffer_len, Fs, duration); wav_name = "crescente.wav"; break;
        case 3: audio_aleatorio(audio_buffer, buffer_len, Fs, duration); wav_name = "aleatorio.wav"; break;
    /*    case 4: 
            escala_crescente(audio_buffer, buffer_len, Fs, duration);
            audio_buffer2 = new float[buffer_len];
            memset(audio_buffer2, 0, buffer_len);
            audio_aleatorio(audio_buffer2, buffer_len, Fs, duration);

            for(i=0; i<buffer_len; i++)
                audio_buffer[i]+=audio_buffer2[i];
                audio_buffer[i]/=2;
            wav_name = "aleatorio_crescente.wav";
            break;*/ //ideia legal, mas sai um som horrivel
        default: return 0;
            break;
    }

    // ============================
    // save output wave
    write_wave_file (wav_name.c_str(), audio_buffer, buffer_len, Fs);
    if(DEBUG) cout << "done." << endl;
    delete [] audio_buffer;

    return 0;
}



