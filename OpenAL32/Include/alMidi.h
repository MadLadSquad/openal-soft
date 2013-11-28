#ifndef ALMIDI_H
#define ALMIDI_H

#include "alMain.h"
#include "evtqueue.h"

#ifdef __cplusplus
extern "C" {
#endif

struct MidiSynthVtable;

typedef struct MidiSynth {
    EvtQueue EventQueue;

    ALuint64 LastEvtTime;
    ALuint64 NextEvtTime;
    ALdouble SamplesSinceLast;
    ALdouble SamplesToNext;

    ALuint SampleRate;
    ALdouble SamplesPerTick;

    volatile ALenum State;

    char *FontName;

    const struct MidiSynthVtable *vtbl;
} MidiSynth;


struct MidiSynthVtable {
    void (*const Destruct)(MidiSynth *self);

    void (*const setState)(MidiSynth *self, ALenum state);
    void (*const update)(MidiSynth *self, ALCdevice *device);
    void (*const process)(MidiSynth *self, ALuint samples, ALfloat (*restrict DryBuffer)[BUFFERSIZE]);

    void (*const Delete)(MidiSynth *self);
};

#define DEFINE_MIDISYNTH_VTABLE(T)                                            \
DECLARE_THUNK(T, MidiSynth, void, Destruct)                                   \
DECLARE_THUNK1(T, MidiSynth, void, setState, ALenum)                          \
DECLARE_THUNK1(T, MidiSynth, void, update, ALCdevice*)                        \
DECLARE_THUNK2(T, MidiSynth, void, process, ALuint, ALfloatBUFFERSIZE*restrict) \
DECLARE_THUNK(T, MidiSynth, void, Delete)                                     \
                                                                              \
static const struct MidiSynthVtable T##_MidiSynth_vtable = {                  \
    T##_MidiSynth_Destruct,                                                   \
                                                                              \
    T##_MidiSynth_setState,                                                   \
    T##_MidiSynth_update,                                                     \
    T##_MidiSynth_process,                                                    \
                                                                              \
    T##_MidiSynth_Delete,                                                     \
}


MidiSynth *SynthCreate(ALCdevice *device);

#ifdef __cplusplus
}
#endif

#endif /* ALMIDI_H */
