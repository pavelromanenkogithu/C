#pragma once
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <span>

class AudioMixer {
public:
    struct Sample {
        std::vector<float> data;     // mono sample
        uint32_t loopStart = 0;
        uint32_t loopEnd = 0;
        bool loop = false;
        bool bidirectional = false;
    };

    struct Voice {
        bool active = false;
        const Sample* sample = nullptr;

        double position = 0.0;
        double increment = 0.0;

        float volume = 1.0f;  // 0..1
        float pan = 0.5f;     // 0 = left, 1 = right

        bool reverse = false;
    };

public:
    explicit AudioMixer(uint32_t sampleRate, size_t maxVoices = 64)
        : m_sampleRate(sampleRate),
          m_voices(maxVoices)
    {}

    // =========================
    // Voice Control
    // =========================

    int play(const Sample& sample,
             float frequency,
             float volume = 1.0f,
             float pan = 0.5f)
    {
        for (size_t i = 0; i < m_voices.size(); ++i) {
            if (!m_voices[i].active) {
                auto& v = m_voices[i];
                v.active = true;
                v.sample = &sample;
                v.position = 0.0;
                v.increment = frequency / m_sampleRate;
                v.volume = volume;
                v.pan = pan;
                v.reverse = false;
                return static_cast<int>(i);
            }
        }
        return -1; // no free voice
    }

    void stop(int voiceId) {
        if (validVoice(voiceId))
            m_voices[voiceId].active = false;
    }

    void setVolume(int voiceId, float volume) {
        if (validVoice(voiceId))
            m_voices[voiceId].volume = volume;
    }

    void setPan(int voiceId, float pan) {
        if (validVoice(voiceId))
            m_voices[voiceId].pan = pan;
    }

    void setFrequency(int voiceId, float frequency) {
        if (validVoice(voiceId))
            m_voices[voiceId].increment = frequency / m_sampleRate;
    }

    // =========================
    // Rendering
    // =========================

    void render(float* outLeft,
                float* outRight,
                size_t frames)
    {
        std::fill(outLeft,  outLeft  + frames, 0.0f);
        std::fill(outRight, outRight + frames, 0.0f);

        for (auto& v : m_voices) {
            if (!v.active || !v.sample)
                continue;

            mixVoice(v, outLeft, outRight, frames);
        }

        applyLimiter(outLeft, outRight, frames);
    }

private:
    void mixVoice(Voice& v,
                  float* outL,
                  float* outR,
                  size_t frames)
    {
        const auto& s = *v.sample;

        for (size_t i = 0; i < frames; ++i) {

            if (!v.active)
                return;

            size_t idx = static_cast<size_t>(v.position);

            if (idx + 1 >= s.data.size()) {
                if (s.loop) {
                    handleLoop(v, s);
                    idx = static_cast<size_t>(v.position);
                } else {
                    v.active = false;
                    return;
                }
            }

            float frac = static_cast<float>(v.position - idx);

            // Linear interpolation
            float a = s.data[idx];
            float b = s.data[idx + 1];
            float sample = a + (b - a) * frac;

            float leftGain  = v.volume * (1.0f - v.pan);
            float rightGain = v.volume * v.pan;

            outL[i] += sample * leftGain;
            outR[i] += sample * rightGain;

            v.position += v.reverse ? -v.increment : v.increment;
        }
    }

    void handleLoop(Voice& v, const Sample& s)
    {
        if (!s.loop) {
            v.active = false;
            return;
        }

        if (s.bidirectional) {
            v.reverse = !v.reverse;
            v.position = std::clamp(v.position,
                                    static_cast<double>(s.loopStart),
                                    static_cast<double>(s.loopEnd - 1));
        } else {
            v.position = s.loopStart;
        }
    }

    void applyLimiter(float* left,
                      float* right,
                      size_t frames)
    {
        for (size_t i = 0; i < frames; ++i) {
            left[i]  = std::tanh(left[i]);
            right[i] = std::tanh(right[i]);
        }
    }

    bool validVoice(int id) const {
        return id >= 0 && static_cast<size_t>(id) < m_voices.size();
    }

private:
    uint32_t m_sampleRate;
    std::vector<Voice> m_voices;
};
