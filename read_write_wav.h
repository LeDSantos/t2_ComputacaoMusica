#include <sndfile.hh>
#include <cstring>

int read_wave_file (const char* fname, float*& audio_buffer, int* buffer_len, int* sample_rate);
void write_wave_file (const char* fname, float* audio_buffer, int buffer_len, int sample_rate);


int read_wave_file (const char * fname, float*& buffer, int* buffer_len, int* sample_rate )
{
    SNDFILE *sf;
    SF_INFO info;
    info.format = 0;
    sf = sf_open(fname,SFM_READ,&info);
    if (sf == NULL)
    {
        printf("Failed to open the file.\n");
        exit(-1);
    }

    printf ("Opened file '%s'\n", fname) ;
    printf ("    Sample rate : %d\n", info.samplerate ) ;
    printf ("    Channels    : %d\n", info.channels ) ;

    sample_rate[0] = info.samplerate;
    buffer_len[0] = info.channels*info.frames;

    buffer = (float *) malloc(buffer_len[0]*sizeof(float));
    sf_read_float(sf, buffer, buffer_len[0]);
    sf_close(sf);

    printf("wave file loaded.\n");
    return info.channels;
} /* read_file */


void write_wave_file (const char * fname, float* buffer, int buffer_size, int sample_rate)
{
// TODO this function will only record mono channels!
    if (buffer==NULL) return;

    SF_INFO sfinfo ;
    sfinfo.channels = 1;
    sfinfo.samplerate = sample_rate;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE * outfile = sf_open(fname, SFM_WRITE, &sfinfo);
    sf_count_t count = sf_write_float(outfile, &buffer[0], buffer_size) ;
    (void) count;
    sf_write_sync(outfile);
    sf_close(outfile);

    printf("output wave file saved: %s.\n", fname);
}


