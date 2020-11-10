#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

#include "read_write_wav.h"

using namespace std;

#define DURACAO_NOTA 0.3
#define TAM_PARTITURA 16
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
            cout << "sine class -> call process: "<<  buffer_len << " amp: "<<  amp << " freq: "<< freq << "  Fs: " << Fs << endl;

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



int main(int c, char** argv)
{
    const float duration = 30.0; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    vector<MusicNote> notes;

    //===============================
    SineOscillator *s[TAM_PARTITURA];
    
    int notas[TAM_PARTITURA]={70,69,70,69,74,72,58,58,    60,74,60,75,65,75,80,0};

    int i,j;
    for(i=0; i<TAM_PARTITURA; i++){
        s[i] = new SineOscillator(notas[i], 1, Fs);
    }
    for(i=0; i<TAM_PARTITURA*4; i++){
        MusicNote m(s[(i%TAM_PARTITURA)], (i*DURACAO_NOTA), ((i+1)*DURACAO_NOTA));
        notes.push_back(m);
    }

    //===============================
    // get max signal duration
    int max_pos = 0;
    for (int k=0; k<notes.size(); k++)
    {
        max_pos = std::max((float)max_pos, (float)round(notes[k].end_time*Fs));
    }
    audio_buffer = new float[max_pos];
    memset(audio_buffer, 0, max_pos);

    cout << "maxPos: " << max_pos <<  endl;
    // write the notes into the audio buffer
    for (int k=0; k<notes.size(); k++)
    {
        int startPos = notes[k].start_time*Fs;
        int endPos = notes[k].end_time*Fs;
        cout << "startPos: " << startPos << endl;
        cout << "endPos: " << endPos << endl;
        cout << "opa" << endl;
        notes[k].sp->process(audio_buffer + startPos, endPos-startPos);
    }

    // ============================
    // save output wave
    string wav_name = "melody.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, max_pos, Fs);
    cout << "done." << endl;
    delete [] audio_buffer;

    return 0;
}



