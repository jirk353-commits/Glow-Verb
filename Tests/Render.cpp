// Offline renderer for tone analysis: GVRender <type 0-8 | p:presetIndex> <sampleRate> <out.wav> [input.wav]
// Without an input file it renders the impulse response (100% wet).
#include "PluginProcessor.h"
#include <juce_audio_formats/juce_audio_formats.h>
#include <cstdio>

using namespace juce;

static void setP (GlowVerbProcessor& p, const char* id, float v)
{
    auto* prm = p.apvts.getParameter (id);
    prm->setValueNotifyingHost (prm->convertTo0to1 (v));
}

int main (int argc, char** argv)
{
    if (argc < 4) { std::printf ("usage: GVRender <type|p:N> <fs> <out.wav> [in.wav]\n"); return 1; }
    ScopedJuceInitialiser_GUI init;
    const String sel (argv[1]);
    const double fs = String (argv[2]).getDoubleValue();
    const File outFile (File::getCurrentWorkingDirectory().getChildFile (argv[3]));

    AudioBuffer<float> input;
    if (argc > 4)
    {
        AudioFormatManager fm; fm.registerBasicFormats();
        std::unique_ptr<AudioFormatReader> r (fm.createReaderFor (File::getCurrentWorkingDirectory().getChildFile (argv[4])));
        if (r == nullptr) { std::printf ("cannot read input\n"); return 1; }
        input.setSize (2, (int) r->lengthInSamples + (int) (fs * 4));
        input.clear();
        r->read (&input, 0, (int) r->lengthInSamples, 0, true, true);
    }
    else
    {
        input.setSize (2, (int) (fs * 8));
        input.clear();
        input.setSample (0, 0, 1.0f); input.setSample (1, 0, 1.0f);
    }

    GlowVerbProcessor p;
    p.setPlayConfigDetails (2, 2, fs, 512);
    p.prepareToPlay (fs, 512);
    if (sel.startsWith ("p:")) p.setCurrentProgram (sel.substring (2).getIntValue());
    else p.applyEnvironment (sel.getIntValue());
    if (argc <= 4) setP (p, IDs::mix, 100.0f);
    p.prepareToPlay (fs, 512);

    const int total = input.getNumSamples();
    AudioBuffer<float> work (4, 512);
    MidiBuffer midi;
    AudioBuffer<float> out (2, total);
    for (int pos = 0; pos < total; pos += 512)
    {
        const int n = jmin (512, total - pos);
        AudioBuffer<float> b (work.getArrayOfWritePointers(), 4, 0, n);
        b.clear();
        for (int c = 0; c < 2; ++c) b.copyFrom (c, 0, input, c, pos, n);
        p.processBlock (b, midi);
        for (int c = 0; c < 2; ++c) out.copyFrom (c, pos, b, c, 0, n);
    }

    outFile.deleteFile();
    WavAudioFormat wav;
    std::unique_ptr<AudioFormatWriter> w (wav.createWriterFor (new FileOutputStream (outFile), fs, 2, 32, {}, 0));
    if (w == nullptr) return 1;
    w->writeFromAudioSampleBuffer (out, 0, total);
    std::printf ("wrote %s\n", outFile.getFullPathName().toRawUTF8());
    return 0;
}
